#ifndef _SGI_STL_INTERNAL_HEAP_H_
#define _SGI_STL_INTERNAL_HEAP_H_

#include "stl_config.h"
#include "concept_checks.h"
#include "stl_iterator_base.h"

__STL_BEGIN_NAMESPACE
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1209
#endif

// Heap-manipulation functions: push_heap, pop_heap, make_heap, sort_heap
// push_heap : parent-node should always larger then its children-node
template <class _RandomAccessIterator, class _Dist, class _Tp>
void __push_heap(_RandomAccessIterator __first, _Dist __holeIndex, _Dist __topIndex, _Tp __val) {
    _Dist __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && *(__first + __parent) < __val){
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template <class _RandomAccessIterator, class _Dist, class _Tp>
inline void __push_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Dist*, _Tp*) {
    __push_heap(__first, _Dist((__last - __first) - 1), _Dist(0), _Tp(*(__last - 1)));
}

template <class _RandomAccessIterator>
inline void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type, _LessThanComparable);
    __push_heap_aux(__first, __last, __DISTANCE_TYPE(__first), __VALUE_TYPE(__first));
}

template <class _RandomAccessIterator, class _Dist, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Dist __holeIndex, _Dist __topIndex, _Tp __val, _Compare __comp) {
    _Dist __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template <class _RandomAccessIterator, class _Compare, class _Dist, class _Tp>
inline void __push_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Dist*, _Tp*) {
    __push_heap(__first, _Dist((__last - __first) - 1), _Dist(0), _Tp(*(__last - 1)), __comp);
}

template <class _RandomAccessIterator, class _Compare>
inline void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __push_heap_aux(__first, __last, __comp, __DISTANCE_TYPE(__first), __VALUE_TYPE(__first));
}

template <class _RandomAccessIterator, class _Dist, class _Tp>
void __adjust_heap(_RandomAccessIterator __first, _Dist __holeIndex, _Dist __len, _Tp __val) {
    _Dist __topIndex = __holeIndex;
    _Dist __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (*(__first + __secondChild) < *(__first + (__secondChild - 1)))
            --__secondChild;

        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * __holeIndex + 2;
    }
    
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }

    __push_heap(__first, __holeIndex, __topIndex, __val);
}

template <class _RandomAccessIterator, class _Tp, class _Dist>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Dist*) {
    *__result = *__first;
    __adjust_heap(__first, _Dist(0), _Dist(__last - __first), __val);
}

template <class _RandomAccessIterator, class _Tp>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*) {
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __DISTANCE_TYPE(__first));
}

template <class _RandomAccessIterator>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type, _LessThanComparable);
    __pop_heap_aux(__first, __last, __VALUE_TYPE(__first));
}

template <class _RandomAccessIterator, class _Dist, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Dist __holeIndex, _Dist __len, _Tp __val, _Compare __comp) {
    _Dist __topIndex = __holeIndex;
    _Dist __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            --__secondChild;

        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * __holeIndex + 2;
    }

    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild-1));
        __holeIndex = __secondChild - 1;
    }

    __push_heap(__first, __holeIndex, __topIndex, __val, comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Dist>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Dist*) {
    *__result = *__first;
    __adjust_heap(__first, _Dist(0), _Dist(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp) {
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __DISTANCE_TYPE(__first));
}

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __fist, _RandomAccessIterator __last, _Compare __comp) {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __pop_heap_aux(__fist, __last, __VALUE_TYPE(__fist), __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Dist>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Dist*) {
    if (__last - __first < 2) return;

    _Dist __len = __last - __first;
    _Dist __parentIndex = (__len - 2) / 2;

    while (true) {
        __adjust_heap(__first, __parentIndex, __len, _Tp(*(__first + __parentIndex)));

        if (0 == __parentIndex) return;

        --__parentIndex;
    }
}

template <class _RandomAccessIterator>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type, _LessThanComparable);
    __make_heap(__first, __last, __VALUE_TYPE(__first), __DISTANCE_TYPE(__first));
}

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Dist>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Dist*) {
    if (__last - __first < 2) return;

    _Dist __len = __last - __first;
    _Dist __parentIndex = (__len - 2) / 2;

    while (true) {
        __adjust_heap(__first, __parentIndex, __len, _Tp(*(__first + __parentIndex)), __comp);

        if (0 == __parentIndex) return;

        --__parentIndex;
    }
}

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __make_heap(__first, __last, __comp, __VALUE_TYPE(__first), __DISTANCE_TYPE(__first));
}

template <class _RandomAccessIterator>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type, _LessThanComparable);
    while (__last - __first > 1)
        pop_heap(__first, __last--);
}

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1209
#endif

__STL_END_NAMESPACE

#endif /*_SGI_STL_INTERNAL_HEAP_H_*/