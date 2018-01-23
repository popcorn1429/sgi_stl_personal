/*
it's an internal header file, included by other stl headers;
you should not attempt to use it directly
*/

#ifndef __SGI_STL_INTERNAL_RELOPS_H_
#define __SGI_STL_INTERNAL_RELOPS_H_

#include "stl_config.h"

__STL_BEGIN_RELOPS_NAMESPACE

template <class T>
inline bool operator!=(const T& x, const T& y) {
    return !(x == y);
}

template <class T>
inline bool operator>(const T& x, const T& y) {
    return y < x;
}

template <class T>
inline bool operator<=(const T& x, const T& y) {
    return !(y < x);
}

template <class T>
inline bool operator>=(const T& x, const T& y) {
    return !(x < y);
}

__STL_END_RELOPS_NAMESPACE

#endif
