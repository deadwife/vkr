#include "head/utils.h"

unordered_set<uword> initQ() {
    unordered_set<uword> Q;
    uword q;
    ifstream qfile;
    qfile.open("../qfile.txt");
    while(qfile >> q) {
        Q.insert(q);
    }
    return Q;
}
