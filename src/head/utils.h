#pragma once

#include "main.h"


unordered_set<uword> initQ();


template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i != v.size() - 1)
            os << "\n";
    }
    return os;
}

template <typename T>
ofstream& operator<<(ofstream& os, const vector<T>& v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i != v.size() - 1)
            os << "\n";
    }
    return os;
}

template <typename T>
ostream& operator<<(ostream& os, const valarray<T>& v)
{
    os << "[";
    for (int i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i != v.size() - 1)
            os << " ";
    }
    os << "]";
    return os;
}

template <typename T>
ofstream& operator<<(ofstream& os, const valarray<T>& v)
{
    os << "[";
    for (int i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i != v.size() - 1)
            os << " ";
    }
    os << "]";
    return os;
}

template <typename T>
ostream& operator<<(ostream& os, const unordered_set<T>& s)
{
    for (auto it = s.begin(); it != s.end(); ++it)
        os << *it << endl;
    return os;
}

template <typename T>
ofstream& operator<<(ofstream& os, const unordered_set<T>& s)
{
    for (auto it = s.begin(); it != s.end(); ++it)
        os << *it << endl;
    return os;
}