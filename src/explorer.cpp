#include "head/utils.h"
#include "head/explorer.h"


// конструктор
Explorer::Explorer(size_t n, size_t k) :
    n(n),
    k(k),
    linear_function_count(uword(pow(k, n + 1))),
    number_of_sets(uword(pow(k, n))),
    Lcoins(linear_function_count),
    time_taken(0.0),
    result(number_of_sets),
    badMat(number_of_sets, bvec(number_of_sets, false)),
    badCube(number_of_sets, bmat(number_of_sets, bvec(number_of_sets, false))),
    cur_sets(0),
    L(initL()) 
{
    // true ~ linear dependency
    for (size_t i = 0; i < number_of_sets; ++i)
        for (size_t j = 0; j < number_of_sets; ++j) {
            bool stop = false;
            uvec iset, jset;
            for(size_t a = 1; a < k; ++a) {
                for(size_t b = 1; b < k; ++b) {
                    iset = toKary(i, n + 1, nullptr);
                    jset = toKary(j, n + 1, nullptr);
                    iset[n] = 1; jset[n] = 1;
                    uvec setsum = (a * iset + b * jset) % k; // num * set?
                    if(setsum.max() == 0) {
                        // above means linear dependency
                        badMat[i][j] = true;
                        stop = true;
                        break;
                    }
                }
                if(stop) break;
            }
        }

    // true ~ linear dependency
    for(size_t i = 0; i < number_of_sets; ++i)
        for(size_t j = 0; j < number_of_sets; ++j)
            for(size_t s = 0; s < number_of_sets; ++s) {
                bool stop = false;
                uvec iset, jset, sset;
                for(size_t a = 0; a < k; ++a) {
                    for(size_t b = 0; b < k; ++b) {
                        for(size_t c = 1; c < k; ++c) {
                            iset = toKary(i, n + 1, nullptr);
                            jset = toKary(j, n + 1, nullptr);
                            sset = toKary(s, n + 1, nullptr);
                            iset[n] = 1; jset[n] = 1; sset[n] = 1;
                            uvec setsum = (a * iset + b * jset + c * sset) % k;
                            if(setsum.max() == 0) {
                                // above means linear dependency
                                badCube[i][j][s] = true;
                                stop = true;
                                break;
                            }
                        }
                        if(stop) break;
                    }
                    if(stop) break;
                }
            }
};

// генератор случайного простого числа на [lower, upper)
uword Explorer::generate_q(const uword& lower, const uword& upper, const unordered_set<uword>& Q) {
    bvec prime(upper, true);
    prime[0] = false; prime[1] = false;
    for(uword p = 2; p * p < upper; ++p) {  
        if (prime[p]) { 
            for(uword i = p * p; i < upper; i += p) prime[i] = false; 
        }
    }
    vector<uword> res = {};
    for(size_t i = lower; i < upper; ++i) {
        if(prime[i]) res.push_back(i);
    }

    // берем случайное простое число из найденных
    // игнорируем те, что уже были (находятся в Q)
    uword r;
    do {
        r = rand() % res.size();
    } while(Q.find(res[r]) != Q.end());
    return res[r];
}

// инициализация линейных функций
const umat Explorer::initL() {
    size_t counter = 0;
    umat L(linear_function_count, uvec(number_of_sets));
    uvec ki(n + 1), kj(n + 1);
    for(size_t i = 0; i < linear_function_count; ++i) {
        for(size_t j = 0; j < number_of_sets; ++j) {
            counter = 1;
            ki = toKary(i, n + 1, nullptr);
            kj = toKary(j, n + 1, nullptr);
            kj[n] = 1;
            L[i][j] = (ki * kj).sum() % k;
        }
    }
    return L;
}

// перевод в k-ичную систему счисления
uvec Explorer::toKary(const uword& num, const size_t& count, uvec* const_count) {
    uword current_num = num;
    uvec kary_representation((size_t)0, count);
    if(const_count) {
        size_t temp;
        for(size_t i = 0; i < count; ++i) {
            temp = current_num % k;
            kary_representation[i] = temp;
            (*const_count)[temp]++;
            current_num /= k;
        }
    } else {
        for(size_t i = 0; i < count; ++i) {
            kary_representation[i] = (size_t)(current_num % k);
            current_num /= k;
        }
    }
    return kary_representation;
}

// базовый алгоритм поиска универсальной k-значной функции
bool Explorer::explore(const uword& qstart) {
    const uword qmax = pow(2, 32) - 1;
    const size_t min_const_count = n + 1;
    const size_t min_coins_count = n + 1;
    clock_t start_time = clock();

    for(uword q = qstart; q < qmax; q += qstart) {
        uvec const_count((size_t)0, k);
        uvec f = toKary(q, number_of_sets, &const_count);
        if(const_count.min() < min_const_count) continue;

        bool success = true;
        for(uword linf = 0; linf < linear_function_count; ++linf) {
            size_t coins = 0;
            cur_sets.clear();
            for(uword set = 0; set < number_of_sets; ++set)
                if(L[linf][set] == f[set])
                    if(lind(set, coins)) ++coins;
            if(coins < min_coins_count) {
                success = false;
                break;
            }
        }
        if(success) {
            result = f;
            time_taken = double((clock() - start_time) / CLOCKS_PER_SEC);
            return true;
        }
    }
    time_taken = double((clock() - start_time) / CLOCKS_PER_SEC);
    cout << time_taken / 60.f << " min" << endl << flush;
    return false;
}

// алгоритм поиска универсальной k-значной функции
// основан на мутациях
// не тестировался, не закончен
// можно проверить гипотезу "непрерывности" на старых работах
bool Explorer::explore_descent(const uword& qstart, const size_t& threshold) {
    const uword qmax = pow(2, 32) - 1;
    const size_t min_const_count = n + 1;
    const size_t min_coins_count = n + 1;
    clock_t start_time = clock();
    uword q = qstart;
    
    while(q < qmax) {
        uvec const_count((size_t)0, k);
        uvec f = toKary(q, number_of_sets, &const_count);
        if(const_count.min() < min_const_count) {
            q += qstart;
            continue;
        }

        uvec difference = calc_dif(f);
        uvec quad = difference * difference;
        size_t difsum = quad.sum();
        if(difsum == 0) {
            result = f;
            time_taken = double((clock() - start_time) / CLOCKS_PER_SEC);
            return true;
        } else if(difsum < threshold) {
            // возможно, здесь нужно отправлять совсем в другое плавание
            // можно отправлять в метод, основанный только на мутациях
            if(mutate(f, difsum))
            {
                result = f;
                time_taken = double((clock() - start_time) / CLOCKS_PER_SEC);
                return true;
            }
        }
        q += qstart;
    }
    time_taken = double((clock() - start_time) / CLOCKS_PER_SEC);
    cout << time_taken / 60.f << " min" << endl << flush;
    return false;
}

// распарралеливание полного перебора
bool Explorer::explore_parallel(const size_t& child_num) {
    const uword imax = pow(2, 26);
    const size_t min_const_count = n + 1;
    const size_t min_coins_count = n + 1;
    clock_t start_time = clock();
    size_t difmin = 17;

    pid_t ppid = getpid();
    pid_t pid = 0;
    // children setup
    for(size_t i = 0; i < child_num; ++i) {
        if(!fork()) {
            sleep(i + 0.1f);
            pid = i + 1;
            break;
        }
    }
    
    for(uword i = pid; i < imax + 1; i += child_num + 1) {
        // clock_t istart_time = clock();
        uvec const_count((size_t)0, k);
        uvec f = toKary(i, number_of_sets, &const_count);
        if(const_count.min() < min_const_count) continue;
        uvec difference = calc_dif(f);
        size_t difsum = difference.sum();
        if(difsum == 0) {
            result = f;
            time_taken = double((clock() - start_time) / CLOCKS_PER_SEC);
            return true;
        } else if(difsum < difmin) {
            cout << "difmin is now " << difsum << endl << flush;
            difmin = difsum;
        }
        // cout << "itertime: " << (clock() - istart_time) / double(CLOCKS_PER_SEC) << endl << flush; ~4*10^-5
    }
    if(ppid == getpid()) {
        for(size_t i = 0; i < child_num; ++i) {
            cout << "Waiting for child number " << i + 1 << "...\n" << flush;
            wait(nullptr);
        }
    } else exit(0);
    return false;
}

// in progress...
// мутации - малое изменение вектора при малой невязке
// чем меньше невязка, тем больше раз надо пытаться изменить значения
// что-то вроде (порог - невязка) раз
// чем больше невязка, тем больше значений мутируют
//
// адаптивный спуск - анализ вектора невязки
bool Explorer::mutate(uvec& func, const size_t& difval) {
    if(difval == 0) return true;
    
    for (uword i = 0; i < number_of_sets; ++i) {
        for (uword j = i + 1; j < number_of_sets; ++j) {
            uvec f = func;
            for (size_t u = 0; u < 3; ++u) {
                f[i] = (f[i] + 1) % k;
                for (size_t v = 0; v < 3; ++v) {
                    f[j] = (f[j] + 1) % k;
                    uvec quad = calc_dif(f) * calc_dif(f);
                    size_t s = quad.sum();
                    if(s < difval) {
                        cout << s << " is less than " << difval << endl << quad << flush;
                        if(mutate(f, s)) return true;
                    }
                }
            }
        }
    }
    return false;
}

// проверка на линейную независимость векторов
// использует эвристики, свойственные для n=2, k=4
// не годится в общем случае
bool Explorer::lind(const uword& set, const size_t& cur_coins) {
    switch(cur_coins) {
        case 0: {
            cur_sets.push_back(set);
            return true;
        }
        case 1: {
            if(badMat[cur_sets[0]][set]) return false;
            cur_sets.push_back(set);
            return true; 
        }
        case 2: {
            if(badCube[cur_sets[0]][cur_sets[1]][set]) return false;
            cur_sets.push_back(set);
            return true;
        }
        case 3: return false;
        default: throw "error in lind";
    }
}

// сбор информации о наборах для линейных функций
void Explorer::collect_info() {
    for(uword linf = 0; linf < linear_function_count; ++linf) {
        size_t coins = 0;
        cur_sets.clear();
        for(uword set = 0; set < number_of_sets; ++set) {
            if(L[linf][set] == result[set] && lind(set, coins)) {
                ++coins;
            }
        }
        Lcoins[linf] = uvec(cur_sets.data(), cur_sets.size());
    }
}

// вывод информации после поиска
void Explorer::get_info(const uword& q, ofstream& outfile) {
    collect_info();
    cout << "\nF:\t";
    outfile << "\nF:\t";
    cout << result << endl << flush;
    outfile << result << endl << flush;
    cout << "\nq:\t";
    outfile << "\nq:\t";
    cout << q << endl << flush;
    outfile << q << endl << flush;
    cout << "\nCoincidences for linear functions:\n";
    outfile << "\nCoincidences for linear functions:\n";
    cout << Lcoins << endl << flush;
    outfile << Lcoins << endl << flush;
    cout << "\nTime taken:\t~ " << time_taken << " hours\n" << flush;
    outfile << "\nTime taken:\t~ " << time_taken << " hours\n" << flush;
}

// вспомогательная функция-сеттер результата
void Explorer::t_set_res(const uvec& res) {
    result = res;
}

// считает невязку по вектору значений функции f
uvec Explorer::calc_dif(const uvec& f) {
    uvec difference(3, linear_function_count);
    for(uword linf = 0; linf < linear_function_count; ++linf) {
        size_t coins = 0;
        cur_sets.clear();
        for(uword set = 0; set < number_of_sets; ++set) {
            if(L[linf][set] == f[set]) {
                if(lind(set, coins)) {
                    --difference[linf];
                    ++coins;
                }
            }
        }
    }
    return difference;
}

// считает невязку по вектору значений функции f
// предъявляет наборы случайно
// -> то же самое
uvec Explorer::calc_r_dif(const uvec& f, const uword& step) {
    uvec difference(3, linear_function_count);
    for(uword linf = 0; linf < linear_function_count; ++linf) {
        size_t coins = 0;
        cur_sets.clear();
        // случайное предъявление наборов
        uword rset = rand() % number_of_sets;
        for(uword set = 0; set < number_of_sets; ++set) {
            if(L[linf][rset] == f[rset]) {
                if(lind(rset, coins)) {
                    --difference[linf];
                    ++coins;
                }
            }
            rset = (rset + step) % number_of_sets;
        }
    }
    return difference;
}