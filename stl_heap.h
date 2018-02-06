#ifndef _SGI_STL_INTERNAL_HEAP_H_
#define _SGI_STL_INTERNAL_HEAP_H_

#include "stl_config.h"

__STL_BEGIN_NAMESPACE
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1209
#endif

// Heap-manipulation functions: push_heap, pop_heap, make_heap, sort_heap
template <class _RandomAccessIterator, class _Dist, class _Tp>
void __push_heap(_RandomAccessIterator __first, _Dist __holeIndex, _Dist __topIndex, _Tp __value) {
    _Dist __parent = (__holeIndex - 1) / 2;

}


__STL_END_NAMESPACE

#endif /*_SGI_STL_INTERNAL_HEAP_H_*/