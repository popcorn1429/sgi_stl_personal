#ifndef __SGI_STL_INTERNAL_ALGO_H
#define __SGI_STL_INTERNAL_ALGO_H

#include "stl_config.h"
#include "stl_heap.h"
__STL_BEGIN_NAMESPACE
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32) 
#pragma set woff 1209
#endif

// __median (not part of the C++ standard.)
template <class T>
inline const T& __median(const T& a, const T& b, const T& c) {
    __STL_REQUIRES(T, _LessThanComparable);
    if (a < b) {
        if (b < c)
            return b;
        else if (a < c)
            return c;
        else
            return a;
    }
    else if (a < c)
        return a;
    else if (b < c)
        return c;
    else
        return b;
}

template <class T, class Compare>
inline const T& __median(const T& a, const T& b, const T& c, Compare comp) {
    __STL_BINARY_FUNCTION_CHECK(Compare, bool, T, T);
    if (comp(a, b)) {
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    }
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

//for_each. Apply a function to every element of a range.
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function func) {
    __STL_REQUIRES(InputIter, _InputIterator);
    for (; first != last; ++first) {
        func(*first);
    }

    return func;
}

//find and find_if
template <class InputIter, class T>
inline InputIter find(InputIter first, InputIter last, const T& val, input_iterator_tag) {
    while (first != last && !(*first == val)) {
        ++first;
    }
    return first;
}

template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last, Predicate pred, input_iterator_tag) {
    while (first != last && !pred(*first)) {
        ++first;
    }
    return first;
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
template <class RandomAccessIter, class T>
RandomAccessIter find(RandomAccessIter first, RandomAccessIter last, const T& val, random_access_iterator_tag) {
    typename iterator_traits<RandomAccessIter>::difference_type trip_count = (last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
    }

    //Note that ,case blocks don't have [break].
    switch (last - first) {
    case 3:
        if (*first == val) return first;
        ++first;
    case 2:
        if (*first == val) return first;
        ++first;
    case 1:
        if (*first == val) return first;
        ++first;
    case 0:
    default:
        return last;
    }
}

template <class RandomAccessIter, class Predicate>
RandomAccessIter find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred, random_access_iterator_tag) {
    typename iterator_traits<RandomAccessIter>::difference_type trip_count = (last - first) >> 2;

    for (; trip_count > 0; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }

    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
        ++first;
    case 0:
    default:
        return last;
    }
}

#endif /*__STL_CLASS_PARTIAL_SPECIALIZATION*/

template <class InputIter, class T>
inline InputIter find(InputIter first, InputIter last, const T& val) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<InputIter>::value_type, T);
    return find(first, last, val, __ITERATOR_CATEGORY(first));
}

template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last, Predicate pred) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<InputIter>::value_type);
    return find_if(first, last, pred, __ITERATOR_CATEGORY(first));
}

//adjacent_find
template <class ForwardIter>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES(typename iterator_traits<ForwardIter>::value_type, _EqualityComparable);
    if (first == last)
        return last;
    ForwardIter next = first;
    while (++next != last) {
        if (*first == *next)
            return first;
        first = next;
    }

    return last;
}

template <class ForwardIter, class BinaryPredicate>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last, BinaryPredicate binary_pred) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_BINARY_FUNCTION_CHECK(BinaryPredicate, bool, typename iterator_traits<ForwardIter>::value_type, typename iterator_traits<ForwardIter>::value_type);
    if (first == last)
        return last;

    ForwardIter next = first;
    while (++next != last) {
        if (binary_pred(*first, *next))
            return first;
        first = next;
    }
    return last;
}

//count and count_if.
//There are two version of each, one whose return type is void
// and one (present only if we have partial specialization)
// whose return type is iterator_traits<InputIter>::difference_type.
// The C++ standard only has the latter version, but the former,
// which was present in the HP STL, is retained for backward compatibility.
template <class InputIter, class T, class Size>
void count(InputIter first, InputIter last, const T& val, Size& n) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(typename iterator_traits<InputIter>::value_type, _EqualityComparable);
    __STL_REQUIRES(T, _EqualityComparable);
    for (; first != last; ++first) {
        if (*first == val)
            ++n;
    }
}

template <class InputIter, class Predicate, class Size>
void count_if(InputIter first, InputIter last, Predicate pred, Size& n) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<InputIter>::value_type);
    for (; first != last; ++first) {
        if (pred(*first))
            ++n;
    }
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class InputIter, class T>
typename iterator_traits<InputIter>::difference_type count(InputIter first, InputIter last, const T& val) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(typename iterator_traits<InputIter>::value_type, _EqualityComparable);
    __STL_REQUIRES(T, _EqualityComparable);
    typename iterator_traits<InputIter>::difference_type n = 0;
    for (; first != last; ++first) {
        if (*first == val)
            ++n;
    }
    return n;
}

template <class InputIter, class Predicate>
typename iterator_traits<InputIter>::difference_type count_if(InputIter first, InputIter last, Predicate pred) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<InputIter>::value_type);
    typename iterator_traits<InputIter>::difference_type n = 0;
    for (; first != last; ++first) {
        if (pred(*first))
            ++n;
    }
    return n;
}

#endif /*__STL_CLASS_PARTIAL_SPECIALIZATION*/

//search



__STL_END_NAMESPACE

#endif /*__SGI_STL_INTERNAL_ALGO_H*/