/*
it's an internal header file, included by other stl headers;
you should not attempt to use it directly
*/
#ifndef __SGI_STL_INTERNAL_PAIR_H_
#define __SGI_STL_INTERNAL_PAIR_H_
#include "stl_config.h"

__STL_BEGIN_NAMESPACE

template <class T1, class T2>
struct pair {
    using first_type = T1;
    using second_type = T2;

    T1 first;
    T2 second;

    pair() : first(T1()), second(T2()) {}
    pair(const T1& t1, const T2& t2) : first(t1), second(t2) {}

#ifdef __STL_MEMBER_TEMPLATES
    template <class U1, class U2>
    pair(const pair<U1,U2>& p) : first(p.first), second(p.second) {}
#endif
};

template <class T1, class T2>
inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return x.first < y.first ||
        (!(y.first < x.first) && x.second < y.second);
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER
template <class T1, class T2>
inline bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return !(x == y);
}

template <class T1, class T2>
inline bool operator>(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return y < x;
}



#endif




__STL_END_NAMESPACE


#endif
