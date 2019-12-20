#pragma once

#include "main.h"

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
