#ifndef __SGI_STL_INTERNAL_ALGOBASE_H
#define __SGI_STL_INTERNAL_ALGOBASE_H

#include "stl_config.h"
#include "stl_relops.h"
#include "stl_pair.h"
#include "type_traits.h"

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <new.h>

#ifdef __STL_USE_NEW_IOSTREAMS
#include <iosfwd>
#else
#include <iostream>
#endif /*__STL_USE_NEW_IOSTREAMS*/

#include "stl_iterator_base.h" //including "concept_checks.h"
#include "stl_iterator.h"

__STL_BEGIN_NAMESPACE

//swap and iter_swap
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*) {
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b) {
    __STL_REQUIRES(_ForwardIter1, _Mutable_ForwardIterator);
    __STL_REQUIRES(_ForwardIter2, _Mutable_ForwardIterator);
    __STL_CONVERTIBLE(typename iterator_traits<_ForwardIter1>::value_type, typename iterator_traits<_ForwardIter2>::value_type);
    __STL_CONVERTIBLE(typename iterator_traits<_ForwardIter2>::value_type, typename iterator_traits<_ForwardIter1>::value_type);
    __iter_swap(__a, __b, __VALUE_TYPE(__a));
}

template <class _Tp>
inline void swap(_Tp& __a, _Tp& __b) {
    __STL_REQUIRES(_Tp, _Assignable);
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

//min and max
#if !defined(__BORLANDC__) || __BORLANDC__ >= 0x540 /* C++ builder 4.0 */
#undef min
#undef max

template <class _Tp>
inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
    __STL_REQUIRES(_Tp, _LessThanComparable);
    return __b < __a ? __b : __a;
}

template <class _Tp>
inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
    __STL_REQUIRES(_Tp, _LessThanComparable);
    return __a < __b ? __b : __a;
}
#endif

template <class _Tp, class _Compare>
inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp) {
    return __comp(__b, __a) ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp) {
    return __comp(__a, __b) ? __b : __a;
}

// copy
// All of these auxiliary functions serve two purposes.
// 1. Replace calls to copy with memmove whenever possible.
//    Memmove, not memcpy, because the input and output ranges 
//    are permitted to overlap.
// 2. If we're using random access iterators, when write the
//    loops as a for loop with an explicit count.
template <class _InputIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_InputIter __first, _InputIter __last, _OutputIter __result, input_iterator_tag, _Distance*) {
    for (; __first != __last; ++__result, ++__first) {
        *__result = *__first;
    }
    return __result;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, random_access_iterator_tag, _Distance*) {
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _Tp>
inline _Tp* __copy_trivial(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    memmove(__result, __first, sizeof(_Tp)* (__last - __first));
    return __result + (__last - __first);
}

#if defined(__STL_FUNCTION_TMPL_PARTIAL_ORDER)
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux2(_InputIter __first, _InputIter __last, _OutputIter __result, __false_type) {
    return __copy(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux2(_InputIter __first, _InputIter __last, _OutputIter __result, __true_type) {
    return __copy(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}

#ifndef __USLC__

template <class _Tp>
inline _Tp* __copy_aux2(_Tp* __first, _Tp* __last, _Tp* __result, __true_type) {
    return __copy_trivial(__first, __last, __result);
}

#endif /*__USLC__*/

template <class _Tp>
inline _Tp* __copy_aux2(const _Tp* __first, const _Tp* __last, _Tp* __result, __true_type) {
    return __copy_trivial(__first, __last, __result);
}

template <class _InputIter, class _OutputIter, class _Tp>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, _Tp*) {
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _Trivial;
    return __copy_aux2(__first, __last, __result, _Trivial());
}

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result) {
    __STL_REQUIRES(_InputIter, _InputIterator);
    __STL_REQUIRES(_OutputIter, _OutputIterator);
    return __copy_aux(__first, __last, __result, __VALUE_TYPE(__first));
}

//Hack for compilers that don't have partial ordering of function templates
// but do have partial specialization of class templates.
#elif defined(__STL_CLASS_PARTIAL_SPECIALIZATION)

template <class _InputIter, class _OutputIter, class _BoolType>
struct __copy_dispatch {
    static _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result) {
        typedef typename iterator_traits<_InputIter>::iterator_category _Category;
        typedef typename iterator_traits<_InputIter>::difference_type _Distance;
        return __copy(__first, __last, __result, _Category(), (_Distance*)0);
    }
};

template <class _Tp>
struct __copy_dispatch<_Tp*, _Tp*, __true_type> {
    static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
        return __copy_trivial(__first, __last, __result);
    }
};

template <class _Tp>
struct __copy_dispatch<const _Tp*, _Tp*, __true_type> {
    static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
        return __copy_trivial(__first, __last, __result);
    }
};

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result) {
    __STL_REQUIRES(_InputIter, _InputIterator);
    __STL_REQUIRES(_OutputIter, _OutputIterator);
    typedef typename iterator_traits<_InputIter>::value_type _Tp;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _Trivial;
    return __copy_dispatch<_InputIter, _OutputIter, _Trivial>::copy(__first, __last, __result);
}

//Fallback for compilers with neither partial ordering nor partial
// specialization. Define the faster version for basic builtin types.

#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result) {
    return __copy(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}

#define __SGI_STL_DECLARE_COPY_TRIVIAL(_Tp)                                  \
    inline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) { \
        memmove(__result, __first, sizeof(_Tp)* (__last - __first));         \
        return __result + (__last - __first);                                \
    }

__SGI_STL_DECLARE_COPY_TRIVIAL(char)
__SGI_STL_DECLARE_COPY_TRIVIAL(signed char)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned char)
__SGI_STL_DECLARE_COPY_TRIVIAL(short)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned short)
__SGI_STL_DECLARE_COPY_TRIVIAL(int)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned int)
__SGI_STL_DECLARE_COPY_TRIVIAL(long)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned long)
#ifdef __STL_HAS_WCHAR_T
__SGI_STL_DECLARE_COPY_TRIVIAL(wchar_t)
#endif /*__STL_HAS_WCHAR_T*/

#ifdef _STL_LONG_LONG
__SGI_STL_DECLARE_COPY_TRIVIAL(long long)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned long long)
#endif /*_STL_LONG_LONG*/

__SGI_STL_DECLARE_COPY_TRIVIAL(float)
__SGI_STL_DECLARE_COPY_TRIVIAL(double)
__SGI_STL_DECLARE_COPY_TRIVIAL(long double)

#undef __SGI_STL_DECLARE_COPY_TRIVIAL
#endif /*__STL_FUNCTION_TMPL_PARTIAL_ORDER*/

//copy_backward
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
inline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, bidirectional_iterator_tag, _Distance*) {
    while (__first != __last)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, random_access_iterator_tag, _Distance*) {
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
// This dispatch class is a workaround for compilers
// that don't have partial ordering of function templates.
// All we're doing is creating a specialization so that
// we can turn a call to copy_backward into a memmove
// whenever possible.
template <class _BidirectionalIter1, class _BidirectionalIter2, class _BoolType>
struct __copy_backward_dispatch {
    typedef typename iterator_traits<_BidirectionalIter1>::iterator_category _Cat;
    typedef typename iterator_traits<_BidirectionalIter1>::difference_type _Distance;
    static _BidirectionalIter2 copy(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result) {
        return __copy_backward(__first, __last, __result, _Cat(), (_Distance*)0);
    }
};

template <class _Tp>
struct __copy_backward_dispatch<_Tp*, _Tp*, __true_type> {
    static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
        const ptrdiff_t _Num = __last - __first;
        memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
        return __result - _Num;
    }
};

template <class _Tp>
struct __copy_backward_dispatch<const _Tp*, _Tp*, __true_type> {
    static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
        return __copy_backward_dispatch<_Tp*, _Tp*, __true_type>::copy(__first, __last, __result);
    }
};

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result) {
    __STL_REQUIRES(_BidirectionalIter1, _BidirectionalIterator);
    __STL_REQUIRES(_BidirectionalIter2, _Mutable_BidirectionalIterator);
    __STL_CONVERTIBLE(typename iterator_traits<_BidirectionalIter1>::value_type, typename iterator_traits<_BidirectionalIter2>::value_type);
    typedef typename __type_traits<typename iterator_traits<_BidirectionalIter2>::value_type>::has_trivial_assignment_operator _Trivial;

    return __copy_backward_dispatch<_BidirectionalIter1, _BidirectionalIter2, _Trivial>::copy(__first, __last, __result);
}

#else /*__STL_CLASS_PARTIAL_SPECIALIZATION*/

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result) {
    return __copy_backward(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}

#endif/*__STL_CLASS_PARTIAL_SPECIALIZATION*/

//copy_n (not part of the C++ standard)
template <class _InputIter, class _Size, class _OutputIter>
pair<_InputIter, _OutputIter> __copy_n(_InputIter __first, _Size __count, _OutputIter __result, input_iterator_tag) {
    for (; __count > 0; --__count) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return pair<_InputIter, _OutputIter>(__first, __result);
}

template <class _RandomAccessIter, class _Size, class _OutputIter>
inline pair<_RandomAccessIter, _OutputIter> __copy_n(_RandomAccessIter __first, _Size __count, _OutputIter __result, random_access_iterator_tag) {
    _RandomAccessIter __last = __first + __count;
    return pair<_RandomAccessIter, _OutputIter>(__last, copy(__first, __last, __result));
}

template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter> __copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
    return __copy_n(__first, __count, __result, __ITERATOR_CATEGORY(__first));
}

template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter> copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
    __STL_REQUIRES(_InputIter, _InputIterator);
    __STL_REQUIRES(_OutputIter, _OutputIterator);
    return __copy_n(__first, __count, __result);
}

//fill and fill_n



__STL_END_NAMESPACE

#endif /*__SGI_STL_INTERNAL_ALGOBASE_H*/