#pragma once

#include "main.h"

class Explorer
{
public:
    Explorer(size_t n, size_t k);

    static uword generate_q(const uword& lower, const uword& upper,const unordered_set<uword>& Q);

    bool explore(const uword& q);
    bool explore_parallel(const size_t& child_num);
    bool explore_descent(const uword& q, const size_t& threshold);
    void get_info(const uword& q, ofstream& outfile);
    uvec calc_dif(const uvec& f);
    uvec calc_r_dif(const uvec& f, const uword& step);
    void t_set_res(const uvec& res);

private:
    const size_t n, k;
    const uword linear_function_count, number_of_sets;
    const umat L;
    uvec result;
    umat Lcoins;
    clock_t time_taken;

    // lind members, for n=2, k=4 only
    bmat badMat;
    bcube badCube;
    vector<size_t> cur_sets;
    //

    const umat initL();

    uvec toKary(const uword& num, const size_t& count, uvec* const_count);

    bool lind(const uword& set, const size_t& cur_coins);
    
    bool mutate(uvec& f, const size_t& difval);
    void collect_info();
};