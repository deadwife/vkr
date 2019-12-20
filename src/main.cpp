#include "head/explorer.h"
#include "head/utils.h"

int main() {
    // main setup
    unordered_set<uword> Q = {};
    Explorer explorer(2, 4);

    // parallel setup 
    size_t CHILD_NUM = 15;

    // prime generator setup
    uword q, lower = 1000, upper = lower * 8.333; // Eq = 10**11
    size_t PRIMES_NUM = 3;

    // children setup
    for(size_t i = 0; i < CHILD_NUM; ++i) {
        if(!fork()) {
            sleep(i + 1);
            srand(time(NULL));
            break;
        }
    }

    // exploration
    while(true) {
        q = 1; // 10**13 ~ 3 sec => 10**11 ~ 5 min
        for(size_t i = 0; i < PRIMES_NUM; ++i) {
            uword qq = Explorer::generate_q(lower, upper, Q);
            q *= qq;
        }
        cout << "q: " << q << endl << flush;
        if(explorer.explore(q)) {
            explorer.get_info(q);
            return 0;
        }
        else Q.insert(q);
    }
    return 0;
}