#ifndef __SGI_STL_INTERNAL_CONSTRUCT_H
#define __SGI_STL_INTERNAL_CONSTRUCT_H

#include "stl_config.h"
#include "type_traits.h"
#include <new.h>

__STL_BEGIN_NAMESPACE
// construct and destroy.
// These functions are not part of the C++ standard,
// and are provided for backward compatibility with the 
// HP STL.
// We also provide internal names _Construct and _Destroy
// that can be used within the library, so that standard-
// conforming pieces don't have to rely on non-standard
// extensions.

// Internal names

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) {
    new ((void*)__p) _T1(__val);
}

template <class _T1>
inline void _Construct(_T1* __p) {
    new ((void*)__p) _T1();
}

template <class _T>
inline void _Destroy(_T* __p) {
    __p->~T();
}

template <class _ForwardIter>
void __destroy_aux(_ForwardIter __first, _ForwardIter __last, __false_type) {
    for (; __first != __last; ++__first)
        destroy(&*__first);
}

template <class _ForwardIter>
inline void __destroy_aux(_ForwardIter, _ForwardIter, __true_type) {}

template <class _ForwardIter, class _T>
inline void __destroy(_ForwardIter __first, _ForwardIter __last, _T*) {
    typedef typename __type_traits<_T>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _ForwardIter>
inline void _Destroy(_ForwardIter __first, _ForwardIter __last) {
    __destroy(__first, __last, __VALUE_TYPE(__first));
}

inline void _Destroy(char*, char*) {}
inline void _Destroy(int*, int*) {}
inline void _Destroy(long*, long*) {}
inline void _Destroy(float*, float*) {}
inline void _Destroy(double*, double*) {}
#ifdef __STL_HAS_WCHAR_T
inline void _Destroy(wchar_t*, wchar_t*) {}
#endif /*__STL_HAS_WCHAR_T*/

//-------------------------------------------------------------
// Old names for the HP STL.

template <class _T1, class _T2>
inline void construct(_T1* __p, const _T2& __val) {
    _Construct(__p, __val);
}

template <class _T>
inline void construct(_T* __p) {
    _Construct(__p);
}

template <class _T>
inline void destroy(_T* __p) {
    _Destroy(__p);
}

template <class _ForwardIter>
inline void destroy(_ForwardIter __first, _ForwardIter __last) {
    _Destroy(__first, __last);
}

__STL_END_NAMESPACE

#endif /*__SGI_STL_INTERNAL_CONSTRUCT_H*/