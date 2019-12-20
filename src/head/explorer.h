#pragma once

#include "main.h"

class Explorer
{
public:
    Explorer(size_t n, size_t k);

    static uword generate_q(const uword& lower, const uword& upper,const unordered_set<uword>& Q);

    bool explore(const uword& q);
    void get_info(const uword& q);

private:
    const size_t n, k;
    const uword linear_function_count, number_of_sets;
    const umat L;
    uvec result;
    umat Lcoins;
    clock_t time_taken;

    // lind members, for n=2, k=4 only
    bvec isBad;
    bmat badMat;
    bcube badCube;
    vector<size_t> cur_sets;
    //

    const umat initL();

    uvec toKary(const uword& num, const size_t& count, uvec* const_count);

    bool lind(const uword& set, const size_t& cur_coins);
    void collect_info();
};