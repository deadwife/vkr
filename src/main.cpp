#include "head/explorer.h"
#include "head/utils.h"


int main() {
    // main setup
    //
    // Q пока не общий!
    unordered_set<uword> Q = initQ();
    cout << "Q size: " << Q.size() * 8 << " bytes" << endl << flush;
    ofstream qfile("../qfile.txt", ios::app);
    ofstream outfile("../outfile.txt", ios::app);
    Explorer explorer(2, 4);

    if(explorer.explore_parallel(15)) {
            explorer.get_info(1, outfile);
            return 0;
    }
    cout << "Nothing found!\n" << flush;
    return 0;

    // testzone
    srand(time(nullptr));

    // while(true) {
    //     uvec f((size_t)0, 16);
    //     for (size_t i = 0; i < f.size(); i++)
    //     {
    //         f[i] = rand() % 4;
    //     }
    //     cout << explorer.calc_dif(f).sum() << "\t" << explorer.calc_r_dif(f, 7).sum() << endl << flush;
    // }
    // return 0;
    


    // logfile setup
    time_t cur_time = time(NULL);
    tm* timeinfo = localtime(&cur_time);
    ostringstream oss;
    oss << put_time(timeinfo, "%F, %R");
    ofstream logfile("../logs/" + oss.str() + ".txt", ios::app);

    // prime generator setup
    // 1000, 1000 * 8,333 ~ 1-10 min, Eq ~ 10**11
    uword q, lower = 1, upper = lower * 4096;
    #define PRIMES_NUM 1

    // parallel setup 
    #define CHILD_NUM 15

    // children setup
    for(size_t i = 0; i < CHILD_NUM; ++i) {
        if(!fork()) {
            sleep(i + 1);
            srand(time(NULL));
            break;
        }
    }

    // exploration
    // попробовать реализовать "спуск" по невязке
    // мутации
    while(true) {
        q = 1; // 10**13 ~ 3 sec => 10**11 ~ 5 min
        for(size_t i = 0; i < PRIMES_NUM; ++i) {
            uword qq = Explorer::generate_q(lower, upper, Q);
            q *= qq;
        }
        Q.insert(q);

        // output + log
        cout << "q: " << q << endl << flush;
        logfile << "q: " << q << endl << flush;

        // таких пока нет (меньше 15)
        if(explorer.explore_descent(q, 26)) {
            explorer.get_info(q, outfile);
            return 0;
        }
        // если q неудачный, добавляем его в список неудачников
        qfile << q << endl << flush;
    }
    return 0;
}