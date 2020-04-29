#include "head/explorer.h"
#include "head/utils.h"


int main() {
    ofstream outfile("../outfile.txt", ios::app);
    Explorer explorer(2, 4);

    if(explorer.explore_parallel(31)) {
            explorer.get_info(1, outfile);
            return 0;
    }
    cout << "Nothing found!\n" << flush;
    return 0;
}