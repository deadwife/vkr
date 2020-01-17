#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <valarray>
#include <unordered_set>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <iomanip>

using namespace std;

typedef vector<valarray<size_t>> umat;
typedef valarray<size_t> uvec;
typedef unsigned long long uword;
typedef vector<vector<vector<bool>>> bcube;
typedef vector<vector<bool>> bmat;
typedef vector<bool> bvec;