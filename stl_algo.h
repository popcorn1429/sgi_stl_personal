#ifndef __SGI_STL_INTERNAL_ALGO_H
#define __SGI_STL_INTERNAL_ALGO_H

#include "stl_config.h"
#include "stl_heap.h"
#include "stl_tempbuf.h" //_Temporary_buffer

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

//adjacent_find : search from first to last, if *iter == *(iter+1), return iter
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
template <typename ForwardIter1, typename ForwardIter2>
ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2) {
    __STL_REQUIRES(ForwardIter1, _ForwardIterator);
    __STL_REQUIRES(ForwardIter2, _ForwardIterator);
    __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<ForwardIter1>::value_type, typename iterator_traits<ForwardIter2>::value_type);
    //if empty
    if (first1 == last1 || first2 == last2)
        return first1;

    //if the pattern is length 1
    ForwardIter2 tmp(first2);
    ++tmp;
    if (tmp == last2) {
        return find(first1, last1, *first2);
    }

    //general case
    ForwardIter2 p1(first2), p;
    ++p1;
    ForwardIter1 current(first1);
    while (first1 != last1) {
        first1 = find(first1, last1, *first2);
        if (first1 == last1)
            return last1; //can't find

        p = p1;
        current = first1;
        if (++current == last1)
            return last1;

        while (*current == *p) {
            if (++p == last2)
                return first1; // got it!
            if (++current == last1)
                return last1;
        }
        ++first1; //continue ...
    }
    return first1;
}

template <typename ForwardIter1, typename ForwardIter2, typename BinaryPred>
ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2, BinaryPred pred) {
    __STL_REQUIRES(ForwardIter1, _ForwardIterator);
    __STL_REQUIRES(ForwardIter2, _ForwardIterator);
    __STL_BINARY_FUNCTION_CHECK(BinaryPred, bool, typename iterator_traits<ForwardIter1>::value_type, typename iterator_traits<ForwardIter2>::value_type);

    //if empty
    if (first1 == last1 || first2 == last2)
        return first1;

    //if the pattern is length 1
    ForwardIter2 tmp(first2);
    ++tmp;
    if (tmp == last2) {
        while (first1 != last1 && !pred(*first1, *first2))
            ++first1;
        return first1;
    }

    //general case
    ForwardIter2 p1(first2), p;
    ++p1;
    ForwardIter1 current(first1);
    while (first1 != last1) {
        while (first1 != last1) {
            if (pred(*first1, *first2))
                break;
            ++first1;
        }

        if (first1 == last1)
            return last1; //can't find

        p = p1;
        current = first1;
        if (++current == last1)
            return last1;

        while (pred(*current, *p)) {
            if (++p == last2)
                return first1; // got it!
            if (++current == last1)
                return last1;
        }
        ++first1; //continue ...
    }
    return first1;
}

//search_n : search for __count consecutive copies of __val.
template <typename ForwardIter, typename Integer, typename T>
ForwardIter search_n(ForwardIter first, ForwardIter last, Integer __count, const T& __val) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES(typename iterator_traits<ForwardIter>::value_type, _EqualityComparable);
    __STL_REQUIRES(T, _EqualityComparable);

    if (__count <= 0)
        return first;
    else {
        first = find(first, last, __val);
        while (first != last) {
            Integer n = __count - 1;
            ForwardIter i = first;
            ++i;
            while (i != last && n != 0 && *i == __val) {
                ++i;
                --n;
            }

            if (n == 0)
                return first;
            else
                first = find(i, last, __val);
        }
        return last;
    }
}

template <typename ForwardIter, typename Integer, typename T, typename BinaryPred>
ForwardIter search_n(ForwardIter first, ForwardIter last, Integer __count, const T& __val, BinaryPred pred) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_BINARY_FUNCTION_CHECK(BinaryPred, bool, typename iterator_traits<ForwardIter>::value_type, T);

    if (__count <= 0)
        return first;
    else {
        while (first != last && !pred(*first, __val)) {
            ++first;
        }

        while (first != last) {
            Integer n = __count - 1;
            ForwardIter i = first;
            ++i;
            while (i != last && n != 0 && pred(*i, __val)) {
                ++i;
                --n;
            }

            if (n == 0)
                return first;
            else {
                while (i != last && !pred(*i, __val)) {
                    ++i;
                }
                first = i;
            }
        }
        return last;
    }
}

//swap_ranges 
template <typename ForwardIter1, typename ForwardIter2>
ForwardIter2 swap_ranges(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2) {
    __STL_REQUIRES(ForwardIter1, _Mutable_ForwardIterator);
    __STL_REQUIRES(ForwardIter2, _Mutable_ForwardIterator);
    __STL_CONVERTIBLE(typename iterator_traits<ForwardIter1>::value_type, typename iterator_traits<ForwardIter2>::value_type);
    __STL_CONVERTIBLE(typename iterator_traits<ForwardIter2>::value_type, typename iterator_traits<ForwardIter1>::value_type);
    for (; first1 != last1; ++first1, ++first2) {
        iter_swap(first1, first2);
    }
    return first2;
}

//transform 
template <typename InputIter, typename OutputIter, typename UnaryOper>
OutputIter transform(InputIter first, InputIter last, OutputIter result, UnaryOper oper) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    for (; first != last; ++first, ++result)
        *result = oper(*first);
    return result;
}

template <typename InputIter1, typename InputIter2, typename OutputIter, typename BinaryOper>
OutputIter transform(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, BinaryOper oper) {
    __STL_REQUIRES(InputIter1, _InputIterator);
    __STL_REQUIRES(InputIter2, _InputIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    for (; first1 != last1; ++first1, ++first2, ++result)
        *result = oper(*first1, *first2);
    return result;
}

//replace, replace_if, replace_copy, replace_copy_if
template <typename ForwardIter, typename T>
void replace(ForwardIter first, ForwardIter last, const T& old_value, const T& new_value) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<ForwardIter>::value_type, T);
    __STL_CONVERTIBLE(T, typename iterator_traits<ForwardIter>::value_type);
    for (; first != last; ++first) {
        if (*first == old_value)
            *first = new_value;
    }
}

template <typename ForwardIter, typename Predicate, typename T>
void replace_if(ForwardIter first, ForwardIter last, Predicate pred, const T& new_value) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_CONVERTIBLE(T, typename iterator_traits<ForwardIter>::value_type);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<ForwardIter>::value_type);
    for (; first != last; ++first) {
        if (pred(*first))
            *first = new_value;
    }
}

template <typename InputIter, typename OutputIter, typename T>
OutputIter replace_copy(InputIter first, InputIter last, OutputIter result, const T& old_value, const T& new_value) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<InputIter>::value_type, T);
    for (; first != last; ++first, ++result) {
        *result = (*first == old_value) ? new_value : *first;
    }
    return result;
}

template <typename InputIter, typename OutputIter, typename Predicate, typename T>
OutputIter replace_copy_if(InputIter first, InputIter last, OutputIter result, Predicate pred, const T& new_value) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<InputIter>::value_type);
    for (; first != last; ++first, ++result) {
        *result = pred(*first) ? new_value : *first;
    }
    return result;
}

//generate, generate_n
template <typename ForwardIter, typename Generator>
void generate(typename first, typename last, Generator gen) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_GENERATOR_CHECK(Generator, typename iterator_traits<ForwardIter>::value_type);
    for (; first != last; ++first)
        *first = gen();
}

template <typename OutputIter, typename Size, typename Generator>
OutputIter generate_n(OutputIter first, Size n, Generator gen) {
    __STL_REQUIRES(OutputIter, _OutputIterator);
    for (; n > 0; --n, ++first)
        *first = gen();
    return first;
}

//remove, remove_if, remove_copy, remove_copy_if
template <typename InputIter, typename OutputIter, typename T>
OutputIter remove_copy(InputIter first, InputIter last, OutputIter result, const T& value) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<InputIter>::value_type, T);
    for (; first != last; ++first) {
        if (!(*first == value)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}

template <typename InputIter, typename OutputIter, typename Predicate>
OutputIter remove_copy_if(InputIter first, InputIter last, OutputIter result, Predicate pred) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<InputIter>::value_type);
    for (; first != last; ++first) {
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}

template <typename ForwardIter, typename T>
ForwardIter remove(ForwardIter first, ForwardIter last, const T& value) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<ForwardIter>::value_type, T);
    __STL_CONVERTIBLE(T, typename iterator_traits<ForwardIter>::value_type);

    first = find(first, last, value);
    ForwardIter i = first;
    return first == last ? first : remove_copy(++i, last, first, value);
}

template <typename ForwardIter, typename Predicate>
ForwardIter remove_if(ForwardIter first, ForwardIter last, Predicate pred) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<ForwardIter>::value_type);
    
    first = find_if(first, last, pred);
    ForwardIter i = first;
    return first == last ? first : remove_copy_if(++i, last, first, pred);
}

//unique, unique_copy
template <typename InputIter, typename OutputIter, typename T>
OutputIter __unique_copy(InputIter first, InputIter last, OutputIter result, T* p) {
    T value = *first;
    *result = value;
    while (++first != last) {
        if (!(value == *first)) {
            value = *first;
            *++result = value;
        }
    }
    return ++result;  //return the position after last value
}

template <typename InputIter, typename OutputIter>
inline OutputIter __unique_copy(InputIter first, InputIter last, OutputIter result, output_iterator_tag) {
    return __unique_copy(first, last, result, __VALUE_TYPE(first));
}

template <typename InputIter, typename ForwardIter>
ForwardIter __unique_copy(InputIter first, InputIter last, ForwardIter result, forward_iterator_tag) {
    *result = *first;
    while (++first != last) {
        if (!(*result == *first)) {
            *++result = *first;
        }
    }
    return ++result;
}

template <typename InputIter, typename OutputIter>
inline OutputIter unique_copy(InputIter first, InputIter last, OutputIter result) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    __STL_REQUIRES(typename iterator_traits<InputIter>::value_type, _EqualityComparable);
    if (first == last) return result;
    return __unique_copy(first, last, result, __ITERATOR_CATEGORY(result));
}

template <typename InputIter, typename OutputIter, typename BinaryPredicate, typename T>
OutputIter __unique_copy(InputIter first, InputIter last, OutputIter result, BinaryPredicate pred, T*) {
    __STL_BINARY_FUNCTION_CHECK(BinaryPredicate, bool, T, T);
    T value = *first;
    *result = value;
    while (++first != last) {
        if (!pred(value, *first)) {
            value = *first;
            *++result = value;
        }
    }
    return ++result;
}

template <typename InputIter, typename OutputIter, typename BinaryPredicate>
inline OutputIter __unique_copy(InputIter first, InputIter last, OutputIter result, BinaryPredicate pred, output_iterator_tag) {
    return __unique_copy(first, last, result, pred, __VALUE_TYPE(first));
}

template <typename InputIter, typename ForwardIter, typename BinaryPredicate>
ForwardIter __unique_copy(InputIter first, InputIter last, ForwardIter result, BinaryPredicate pred, forward_iterator_tag) {
    __STL_BINARY_FUNCTION_CHECK(BinaryPredicate, bool, typename iterator_traits<ForwardIter>::value_type, typename iterator_traits<InputIter>::value_type);
    *result = *first;
    while (++first != last) {
        if (!pred(*result, *first)) {
            *++result = *first;
        }
    }
    return ++result;
}

template <typename InputIter, typename OutputIter, typename BinaryPredicate>
inline OutputIter unique_copy(InputIter first, InputIter last, OutputIter result, BinaryPredicate pred) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    if (first == last) return result;
    return __unique_copy(first, last, result, pred, __ITERATOR_CATEGORY(result));
}

template <typename ForwardIter>
ForwardIter unique(ForwardIter first, ForwardIter last) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_REQUIRES(typename iterator_traits<ForwardIter>::value_type, _EqualityComparable);
    first = adjacent_find(first, last);
    return unique_copy(first, last, first);
}

template <typename ForwardIter, typename BinaryPredicate>
ForwardIter unique(ForwardIter first, ForwardIter last, BinaryPredicate pred) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_BINARY_FUNCTION_CHECK(BinaryPredicate, bool, typename iterator_traits<ForwardIter>::value_type, typename iterator_traits<ForwardIter>::value_type);
    first = adjacent_find(first, last, pred);
    return unique_copy(first, last, first, pred);
}

//reverse and reverse_copy, and their auxiliary functions.
template <typename BidirectionalIter>
void __reverse(BidirectionalIter first, BidirectionalIter last, bidirectional_iterator_tag) {
    while (true) {
        if (first == last || first == --last) //here : last - 1
            return;
        else
            iter_swap(first++, last);
    }
}

template <typename RandomAccessIter>
void __reverse(RandomAccessIter first, RandomAccessIter last, random_access_iterator_tag) {
    while (first < last)
        iter_swap(first++, --last);
}

template <typename BidirectionalIter>
inline void reverse(BidirectionalIter first, BidirectionalIter last) {
    __STL_REQUIRES(BidirectionalIter, _Mutable_BidirectionalIterator);
    __reverse(first, last, __ITERATOR_CATEGORY(first));
}

template <typename BidirectionalIter, typename OutputIter>
OutputIter reverse_copy(BidirectionalIter first, BidirectionalIter last, OutputIter result) {
    __STL_REQUIRES(BidirectionalIter, _BidirectionalIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    while (first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}

//rotate and rotate_copy
template <typename EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}

template <typename ForwardIter, typename Distance>
ForwardIter __rotate(ForwardIter first, ForwardIter middle, ForwardIter last, Distance*, forward_iterator_tag) {
    if (first == middle)
        return last;
    if (last == middle)
        return first;

    ForwardIter first2 = middle;
    do {
        swap(*first++, *first2++);
        if (first == middle)
            middle = first2;
    } while (first2 != last);

    ForwardIter new_middle = first;
    first2 = middle;
    while (first2 != last) {
        swap(*first++, *first2++);
        if (first == middle)
            middle = first2;
        else if (first2 == last)
            first2 = middle;
    }
    return new_middle;
}

template <typename BidirectionalIter, typename Distance>
BidirectionalIter __rotate(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, Distance*, bidirectional_iterator_tag) {
    __STL_REQUIRES(BidirectionalIter, _Mutable_BidirectionalIterator);
    if (first == middle)
        return last;
    if (last == middle)
        return first;

    __reverse(first, middle, bidirectional_iterator_tag());
    __reverse(middle, last, bidirectional_iterator_tag());
    while (first != middle && middle != last) {
        swap(*first++, *--last);
    }

    if (first == middle) {
        __reverse(middle, last, bidirectional_iterator_tag());
        return last;
    }
    else {
        __reverse(first, middle, bidirectional_iterator_tag());
        return first;
    }
}

template <typename RandomAccessIter, typename Distance, typename T>
RandomAccessIter __rotate(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Distance*, T*) {
    __STL_REQUIRES(RandomAccessIter, _Mutable_RandomAccessIterator);
    Distance n = last - first;
    Distance k = middle - first;
    Distance l = n - k;
    RandomAccessIter result = first + (last - middle);

    if (k == 0)
        return last;
    else if (k == l) {
        swap_ranges(first, middle, middle);
        return result;
    }

    Distance d = __gcd(n, k);
    for (Distance i = 0; i < d; ++i) {
        T tmp = *first;
        RandomAccessIter p = first;

        if (k < l) {
            for (Distance j = 0; j < l / d; ++j) {
                if (p > first + l) {
                    *p = *(p - l);
                    p -= l;
                }
                
                *p = *(p + k);
                p += k;
            }
        }
        else {
            for (Distance j = 0; j < k / d - 1; ++j) {
                if (p < last - k) {
                    *p = *(p + k);
                    p += k;
                }

                *p = *(p - l);
                p -= l;
            }
        }

        *p = tmp;
        ++first;
    }

    return result;
}

template <typename ForwardIter>
inline ForwardIter rotate(ForwardIter first, ForwardIter middle, ForwardIter last) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    return __rotate(first, middle, last, __DISTANCE_TYPE(first), __ITERATOR_CATEGORY(first));
}

template <typename ForwardIter, typename OutputIter>
OutputIter rotate_copy(ForwardIter first, ForwardIter middle, ForwardIter last, OutputIter result) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    return copy(first, middle, copy(middle, last, result));
}

//Return a random number in the range [0, n). This function encapsulates
// wether we're using rand (part of the standard C library) or lrand48
// (not standard, but a much better choice whenever it's available).

template <typename Distance>
inline Distance __random_number(Distance n) {
#ifdef __STL_NO_DRAND48
    return rand() & n;
#else 
    return lrand48() % n;
#endif
}

//random_shuffle
template <typename RandomAccessIter>
inline void random_shuffle(RandomAccessIter first, RandomAccessIter last) {
    __STL_REQUIRES(RandomAccessIter, _Mutable_RandomAccessIterator);
    if (first == last) return;

    for (RandomAccessIter i = first + 1; i != last; ++i) {
        iter_swap(i, first + __random_number((i - first) + 1));
    }
}

template <typename RandomAccessIter, typename RandomNumberGenerator>
void random_shuffle(RandomAccessIter first, RandomAccessIter last, RandomNumberGenerator& rand) {
    __STL_REQUIRES(RandomAccessIter, _Mutable_RandomAccessIterator);
    if (first == last) return;

    for (RandomAccessIter i = first + 1; i != last; ++i) {
        iter_swap(i, first + rand((i - first) + 1));
    }
}

//random_sample and random_sample_n (these're not part of the C++ standard)
template <typename ForwardIter, typename OutputIter, typename Distance>
OutputIter random_sample_n(ForwardIter first, ForwardIter last, OutputIter out, const Distance n) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    Distance remaining = 0;
    distance(first, last, remaining);
    Distance m = min(n, remaining);

    while (m > 0) {
        if (__random_number(remaining) < m) {
            *out = *first;
            ++out;
            --m;
        }
        --remaining;
        ++first;
    }
    return out;
}

template <typename ForwardIter, typename OutputIter, typename Distance, typename RandomNumberGenerator>
OutputIter random_sample_n(ForwardIter first, ForwardIter last, OutputIter out, const Distance n, RandomNumberGenerator& rand) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES(OutputIter, _OutputIterator);
    __STL_UNARY_FUNCTION_CHECK(RandomNumberGenerator, Distance, Distance);
    Distance remaining = 0;
    distance(first, last, remaining);
    Distance m = min(n, remaining);

    while (m > 0) {
        if (rand(remaining) < m) {
            *out = *first;
            ++out;
            --m;
        }
        --remaining;
        ++first;
    }
    return out;
}

template <typename InputIter, typename RandomAccessIter, typename Distance>
RandomAccessIter __random_sample(InputIter first, InputIter last, RandomAccessIter out, const Distance n) {
    Distance m = 0;
    Distance t = n;
    for (; first != last && m < n; ++m; ++first)
        out[m] = *first;

    while (first != last) {
        ++t;
        Distance M = __random_number(t);
        if (M < n)
            out[M] = *first;
        ++first;
    }
    return out + m;
}

template <typename InputIter, typename RandomAccessIter, typename RandomNumberGenerator, typename Distance>
RandomAccessIter __random_sample(InputIter first, InputIter last, RandomAccessIter out, RandomNumberGenerator& rand, const Distance n) {
    __STL_UNARY_FUNCTION_CHECK(RandomNumberGenerator, Distance, Distance);
    Distance m = 0;
    Distance t = n;
    for (; first != last && m < n; ++m, ++first)
        out[m] = *first;

    while (first != last) {
        ++t;
        Distance M = rand(t);
        if (M < n)
            out[M] = *first;
        ++first;
    }
    return out + m;
}

template <typename InputIter, typename RandomAccessIter>
inline RandomAccessIter random_sample(InputIter first, InputIter last, RandomAccessIter out_first, RandomAccessIter out_last) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(RandomAccessIter, _Mutable_RandomAccessIterator);
    return __random_sample(first, last, out_first, out_last - out_first);
}

template <typename InputIter, typename RandomAccessIter, typename RandomNumberGenerator>
inline RandomAccessIter random_sample(InputIter first, InputIter last, RandomAccessIter out_first, RandomAccessIter out_last, RandomNumberGenerator& rand) {
    __STL_REQUIRES(InputIter, _InputIterator);
    __STL_REQUIRES(RandomAccessIter, _Mutable_RandomAccessIterator);
    return __random_sample(first, last, out_first, rand, out_last - out_first);
}

//partition, stable_partition, and their auxiliary functions.
template <typename ForwardIter, typename Predicate>
ForwardIter __partition(ForwardIter first, ForwardIter last, Predicate pred, forward_iterator_tag) {
    if (first == last) return first;

    while (pred(*first)) {
        if (++first == last) return first;
    }
    
    ForwardIter next = first;

    while (++next != last) {
        if (pred(*next)) {
            swap(*first, *next);
            ++first;
        }
    }

    return first;
}

template <typename BidirectionalIter, typename Predicate>
BidirectionalIter __partition(BidirectionalIter first, BidirectionalIter last, Predicate pred, bidirectional_iterator_tag) {
    while (true) {
        while (true) {
            if (first == last)
                return first;
            else if (pred(*first))
                ++first;
            else 
                break;
        }
        --last;
        while (true) {
            if (first == last)
                return first;
            else if (!pred(*last))
                --last;
            else
                break;
        }
        iter_swap(first, last);
        ++first;
    }
}

template <typename ForwardIter, typename Predicate>
inline ForwardIter partition(ForwardIter first, ForwardIter last, Predicate pred) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<ForwardIter>::value_type);
    return __partition(first, last, pred, __ITERATOR_CATEGORY(first));
}

template <typename ForwardIter, class Predicate, typename Distance>
ForwardIter __inplace_stable_partition(ForwardIter first, ForwardIter last, Predicate pred, Distance len) {
    if (len == 1)
        return pred(*first) ? last : first;

    ForwardIter middle = first;
    advance(middle, len / 2);
    return rotate(__inplace_stable_partition(first, middle, pred, len / 2), middle, __inplace_stable_partition(middle, last, pred, len - len / 2));
}

template <typename ForwardIter, typename Pointer, typename Predicate, typename Distance>
ForwardIter __stable_partition_adaptive(ForwardIter first, ForwardIter last, Predicate pred, Distance len, Pointer buffer, Distance buffer_size) {
    if (len <= buffer_size) {
        ForwardIter result1 = first;
        Pointer result2 = buffer;
        for (; first != last; ++first) {
            if (pred(*first)) {
                *result1 = *first;
                ++result1;
            }
            else {
                *result2 = *first;
                ++result2;
            }
        }

        copy(buffer, result2, result1);
        return result1;
    }
    else {
        ForwardIter middle = first;
        advance(middle, len / 2);
        return rotate(__stable_partition_adaptive(first, middle, pred, len / 2, buffer, buffer_size), middle, __stable_partition_adaptive(middle, last, pred, len - len / 2, buffer, buffer_size));
    }
}

template <typename ForwardIter, typename Predicate, typename T, typename Distance>
inline ForwardIter __stable_partition_aux(ForwardIter first, ForwardIter last, Predicate pred, T*, Distance*) {
    _Temporary_buffer<ForwardIter, T> buf(first, last);
    if (buf.size() > 0)
        return __stable_partition_adaptive(first, last, pred, Distance(buf.requested_size()), buf.begin(), buf.size());
    else
        return __inplace_stable_partition(first, last, pred, Distance(buf.requested_size()));
}

template <typename ForwardIter, typename Predicate>
inline ForwardIter stable_partition(ForwardIter first, ForwardIter last, Predicate pred) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_UNARY_FUNCTION_CHECK(Predicate, bool, typename iterator_traits<ForwardIter>::value_type);
    if (first == last)
        return first;
    else
        return __stable_partition_aux(first, last, pred, __VALUE_TYPE(first), __DISTANCE_TYPE(first));
}

template <typename RandomAccessIter, typename T>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot) {
    while (true) {
        while (*first < pivot)
            ++first;
        --last;
        while (pivot < *last)
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

template <typename RandomAccessIter, typename T, typename Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp) {
    while (true) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

//sort() and its auxiliary functions.
const int __stl_threshold = 16;

template <typename RandomAccessIter, typename T>
void __unguarded_linear_insert(RandomAccessIter last, T val) {
    RandomAccessIter next = last;
    --next;
    while (val < *next) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template <typename RandomAccessIter, typename T, typename Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp) {
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template <typename RandomAccessIter, typename T>
inline void __linear_insert(RandomAccessIter first, RandomAccessIter last, T*) {
    T val = *last;
    if (val < *first) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val);
}

template <typename RandomAccessIter, typename T, typename Compare>
inline void __linear_insert(RandomAccessIter first, RandomAccessIter last, T*, Compare comp) {
    T val = *last;
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

template <typename RandomAccessIter>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last) {
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, __VALUE_TYPE(first));
}

template <typename RandomAccessIter, typename Compare>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, __VALUE_TYPE(first), comp);
}

template <typename RandomAccessIter, typename T>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*) {
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __unguarded_linear_insert(i, T(*i));
}

template <typename RandomAccessIter>
inline void __unguarded_insertion_sort(RandomAccessIter first, RandomAccessIter last) {
    __unguarded_insertion_sort_aux(first, last, __VALUE_TYPE(first));
}

template <typename RandomAccessIter, typename T, typename Compare>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp) {
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}

template <typename RandomAccessIter, typename Compare>
inline void __unguarded_insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    __unguarded_insertion_sort_aux(first, last, __VALUE_TYPE(first), comp);
}

template <typename RandomAccessIter>
void __final_insertion_sort(RandomAccessIter first, RandomAccessIter last) {
    if (last - first > __stl_threshold) {
        __insertion_sort(first, first + __stl_threshold);
        __unguarded_insertion_sort(first + __stl_threshold, last);
    }
    else
        __insertion_sort(first, last);
}

template <typename RandomAccessIter, typename Compare>
void __final_insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    if (last - first > __stl_threshold) {
        __insertion_sort(first, first + __stl_threshold, comp);
        __unguarded_insertion_sort(first + __stl_threshold, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}

template <typename Size>
inline Size __lg(Size n) {
    Size k = 0;
    for (; n != 1; n >>= 1;) ++k;
    return k;
}

template <typename RandomAccessIter, typename T, typename Size>
void __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit) {
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last);
            return;
        }
        --depth_limit;
        RandomAccessIter cut = __unguarded_partition(first, last, T(__median(*first, *(first + (last - first)/2), *(last-1))));
        __introsort_loop(cut, last, (T*)0, depth_limit);
        last = cut;
    }
}

template <typename RandomAccessIter, typename T, typename Size, typename Compare>
void __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit, Compare comp) {
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);   //!!!!where is the implementation of partial_sort :-(
            return;
        }
        --depth_limit;
        RandomAccessIter cut = __unguarded_partition(first, last, T(__median(*first, *(first + (last - first) / 2), *(last - 1))), comp);
        __introsort_loop(cut, last, (T*)0, depth_limit, comp);
        last = cut;
    }
}

template <typename RandomAccessIter>
inline void sort(RandomAccessIter first, RandomAccessIter last) {
    __STL_REQUIRES(RandomAccessIter, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<RandomAccessIter>::value_type, _LessThanComparable);
    if (first != last) {
        __introsort_loop(first, last, __VALUE_TYPE(first), __lg(last - first) * 2);
        __final_insertion_sort(first, last);
    }
}

template <typename RandomAccessIter, typename Compare>
inline void sort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    __STL_REQUIRES(RandomAccessIter, _Mutable_RandomAccessIterator);
    __STL_BINARY_FUNCTION_CHECK(Compare, bool, typename iterator_traits<RandomAccessIter>::value_type, typename iterator_traits<RandomAccessIter>::value_type);
    if (first != last) {
        __introsort_loop(first, last, __VALUE_TYPE(first), __lg(last - first) * 2, comp);
        __final_insertion_sort(first, last, comp);
    }
}

//Binary search (lower_bound, upper_bound, equal_range, binary_search).
template <typename ForwardIter, typename T, typename Distance>
ForwardIter __lower_bound(ForwardIter first, ForwardIter last, const T& val, Distance*) {
    //the range [first, last) is an ordered sequence, and *first is the most smallest element.
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (*middle < val) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else {
            len = half;
        }
    }
    return first;
}

template <typename ForwardIter, typename T>
inline ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& val) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(T, typename iterator_traits<ForwardIter>::value_type); //is this realy nessesary ???
    __STL_REQUIRES(T, _LessThanComparable);
    return __lower_bound(first, last, val, __DISTANCE_TYPE(first));
}

template <typename ForwardIter, typename T, typename Compare, typename Distance>
ForwardIter __lower_bound(ForwardIter first, ForwardIter last, const T& val, Compare comp, Distance*) {
    //the range [first, last) is an ordered sequence, and *first is the most smallest element.
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else {
            len = half;
        }
    }
    return first;
}

template <typename ForwardIter, typename T, typename Compare>
inline ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& val, Compare comp) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(T, typename iterator_traits<ForwardIter>::value_type);  //why? I don't think it's nessesary~ 
    __STL_BINARY_FUNCTION_CHECK(Compare, bool, T, T);
    return __lower_bound(first, last, val, comp, __DISTANCE_TYPE(first));
}

template <typename ForwardIter, typename T, typename Distance>
ForwardIter __upper_bound(ForwardIter first, ForwardIter last, const T& val, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (val < *middle)
            len = half;
        else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

template <typename ForwardIter, typename T>
inline ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& val) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(T, typename iterator_traits<ForwardIter>::value_type);
    __STL_REQUIRES(T, _LessThanComparable);
    return __upper_bound(first, last, val, __DISTANCE_TYPE(first));
}

template <typename ForwardIter, typename T, typename Compare, typename Distance>
ForwardIter __upper_bound(ForwardIter first, ForwardIter last, const T& val, Compare comp, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(val, *middle))
            len = half;
        else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

template <typename ForwardIter, typename T, typename Compare>
inline ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& val, Compare comp) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(T, typename iterator_traits<ForwardIter>::value_type);
    __STL_BINARY_FUNCTION_CHECK(Compare, bool, T, T);
    return __upper_bound(first, last, val, comp, __DISTANCE_TYPE(first));
}

template <typename ForwardIter, typename T, typename Distance>
pair<ForwardIter, ForwardIter> __equal_range(ForwardIter first, ForwardIter last, const T& val, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle, left, right;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (*middle < val) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else if (val < *middle)
            len = half;
        else {
            left = lower_bound(first, middle, val);
            advance(first, len);   //not last, because len may be changed before(this is in while)
            right = upper_bound(++middle, first, val);
            return pair<ForwardIter, ForwardIter>(left, right);
        }
    }
    return pair<ForwardIter, ForwardIter>(first, first);
}

template <typename ForwardIter, typename T>
inline pair<ForwardIter, ForwardIter> equal_range(ForwardIter first, ForwardIter last, const T& val) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(T, typename iterator_traits<ForwardIter>::value_type);
    __STL_REQUIRES(T, _LessThanComparable);
    return __equal_range(first, last, val, __DISTANCE_TYPE(first));
}

template <typename ForwardIter, typename T, typename Compare, typename Distance>
pair<ForwardIter, ForwardIter> __equal_range(ForwardIter first, ForwardIter last, const T& val, Compare comp, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle, left, right;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else if (comp(val, *middle))
            len = half;
        else {
            left = lower_bound(first, middle, val, comp);
            advance(first, len);   //not last, because len may be changed before(this is in while)
            right = upper_bound(++middle, first, val, comp);
            return pair<ForwardIter, ForwardIter>(left, right);
        }
    }
    return pair<ForwardIter, ForwardIter>(first, first);
}

template <typename ForwardIter, typename T, typename Compare>
inline pair<ForwardIter, ForwardIter> equal_range(ForwardIter first, ForwardIter last, const T& val, Compare comp) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(T, typename iterator_traits<ForwardIter>::value_type);
    __STL_BINARY_FUNCTION_CHECK(Compare, bool, T, T);
    return __equal_range(first, last, val, comp, __DISTANCE_TYPE(first));
}

template <typename ForwardIter, typename T>
bool binary_search(ForwardIter first, ForwardIter last, const T& val) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(T, typename iterator_traits<ForwardIter>::value_type);
    __STL_REQUIRES(T, _LessThanComparable);
    ForwardIter i = lower_bound(first, last, val);
    return i != last && !(val < *i);
}

template <typename ForwardIter, typename T, typename Compare>
bool binary_search(ForwardIter first, ForwardIter last, const T& val, Compare comp) {
    __STL_REQUIRES(ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(T, typename iterator_traits<ForwardIter>::value_type);
    __STL_BINARY_FUNCTION_CHECK(Compare, bool, T, T);
    ForwardIter i = lower_bound(first, last, val, comp);
    return i != last && !comp(val, *i);
}

//inplace_merge and its auxiliary functions.
template <typename BidirectionalIter, typename Distance>
void __merge_without_buffer(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, Distance len1, Distance len2) {
    if (len1 == 0 || len2 == 0)
        return;
    if (len1 + len2 == 2) {
        if (*middle < *first)
            iter_swap(first, middle);

        return;
    }

    BidirectionalIter first_cut = first;
    BidirectionalIter second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {
        len11 = len1 / 2;
        advance(first_cut, len11);
        second_cut = lower_bound(middle, last, *first_cut);
        distance(middle, second_cut, len22);
    }
    else {
        len22 = len2 / 2;
        advance(second_cut, len22);
        first_cut = upper_bound(first, middle, *second_cut);
        distance(first, first_cut, len11);
    }

    BidirectionalIter new_middle = rotate(first_cut, middle, second_cut);
    __merge_without_buffer(first, first_cut, new_middle, len11, len22);
    __merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22);
}

template <typename BidirectionalIter, typename Distance, typename Compare>
void __merge_without_buffer(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, Distance len1, Distance len2, Compare comp) {
    if (len1 == 0 || len2 == 0)
        return;
    if (len1 + len2 == 2) {
        if (comp(*middle, *first))
            iter_swap(first, middle);
        return;
    }

    BidirectionalIter first_cut = first;
    BidirectionalIter second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {
        len11 = len1 / 2;
        advance(first_cut, len11);
        second_cut = lower_bound(middle, last, *first_cut, comp);
        distance(middle, second_cut, len22);
    }
    else {
        len22 = len2 / 2;
        advance(second_cut, len22);
        first_cut = upper_bound(first, middle, *second_cut, comp);
        distance(first, first_cut, len11);
    }

    BidirectionalIter new_middle = rotate(first_cut, middle, second_cut);
    __merge_without_buffer(first, first_cut, new_middle, len11, len22, comp);
    __merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22, comp);
}

template <typename BidirectionalIter1, typename BidirectionalIter2, typename Distance>
BidirectionalIter1 __rotate_adaptive(BidirectionalIter1 first, BidirectionalIter1 middle, BidirectionalIter1 last, Distance len1, Distance len2, BidirectionalIter2 buffer, Distance buffer_size) {
    BidirectionalIter2 buffer_end;
    if (len1 > len2 && len2 <= buffer_size) {
        buffer_end = copy(middle, last, buffer);
        copy_backward(first, middle, last);
        return copy(buffer, buffer_end, first);
    }
    else if (len1 <= buffer_size) {
        buffer_end = copy(first, middle, buffer);
        copy(middle, last, first);
        return copy_backward(buffer, buffer_end, last);
    }
    else
        return rotate(first, middle, last);
}

template <typename BidirectionalIter1, typename BidirectionalIter2, typename BidirectionalIter3>
BidirectionalIter3 __merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1, BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter3 result) {
    if (first1 == last1)
        return copy_backward(first2, last2, result);
    if (first2 == last2)
        return copy_backward(first1, last1, result);

    --last1;
    --last2;
    while (true) {
        if (*last2 < *last1) {
            *--result = *last1;
            if (first1 == last1)
                return copy_backward(first2, ++last2, result);
            --last1;
        }
        else {
            *--result = *last2;
            if (first2 == last2)
                return copy_backward(first1, ++last1, result);
            --last2;
        }
    }
}

template <typename BidirectionalIter1, typename BidirectionalIter2, typename BidirectionalIter3, typename Compare>
BidirectionalIter3 __merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1, BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter3 result, Compare comp) {
    if (first1 == last1)
        return copy_backward(first2, last2, result);
    if (first2 == last2)
        return copy_backward(first1, last1, result);

    --last1;
    --last2;
    while (true) {
        if (comp(*last2, *last1)) {
            *--result = *last1;
            if (first1 == last1)
                return copy_backward(first2, ++last2, result);
            --last1;
        }
        else {
            *--result = *last2;
            if (first2 == last2)
                return copy_backward(first1, ++last1, result);
            --last2;
        }
    }
}

template <typename BidirectionalIter, typename Distance, typename Pointer>
void __merge_adaptive(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, Distance len1, Distance len2, Pointer buffer, Distance buffer_size) {
    if (len1 <= len2 && len1 <= buffer_size) {
        Pointer buffer_end = copy(first, middle, buffer);
        merge(buffer, buffer_end, middle, last, first);
    }
    else if (len2 <= buffer_size) {
        Pointer buffer_end = copy(middle, last, buffer);
        __merge_backward(first, middle, buffer, buffer_end, last);
    }
    else {
        BidirectionalIter first_cut = first;
        BidirectionalIter second_cut = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if (len1 > len2) {
            len11 = len1 / 2;
            advance(first_cut, len11);
            second_cut = lower_bound(middle, last, *first_cut);
            distance(middle, second_cut, len22);
        }
        else {
            len22 = len2 / 2;
            advance(second_cut, len22);
            first_cut = upper_bound(first, middle, *second_cut);
            distance(first, first_cut, len11);
        }

        BidirectionalIter new_middle = __rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
        __merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
        __merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size);
    }
}

template <typename BidirectionalIter, typename Distance, typename Pointer, typename Compare>
void __merge_adaptive(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, Distance len1, Distance len2, Pointer buffer, Distance buffer_size, Compare comp) {
    if (len1 <= len2 && len1 <= buffer_size) {
        Pointer buffer_end = copy(first, middle, buffer);
        merge(buffer, buffer_end, middle, last, first, comp);
    }
    else if (len2 <= buffer_size) {
        Pointer buffer_end = copy(middle, last, buffer);
        __merge_backward(first, middle, buffer, buffer_end, last, comp);
    }
    else {
        BidirectionalIter first_cut = first;
        BidirectionalIter second_cut = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if (len1 > len2) {
            len11 = len1 / 2;
            advance(first_cut, len11);
            second_cut = lower_bound(middle, last, *first_cut, comp);
            distance(middle, second_cut, len22);
        }
        else {
            len22 = len2 / 2;
            advance(second_cut, len22);
            first_cut = upper_bound(first, middle, *second_cut, comp);
            distance(first, first_cut, len11);
        }

        BidirectionalIter new_middle = __rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
        __merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size, comp);
        __merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size, comp);
    }
}

template <typename BidirectionalIter, typename T, typename Distance>
inline void __inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, T*, Distance*) {
    Distance len1 = 0;
    distance(first, middle, len1);
    Distance len2 = 0;
    distance(middle, last, len2);

    _Temporary_buffer<BidirectionalIter, T> buf(first, last);
    if (buf.begin() == 0)
        __merge_without_buffer(first, middle, last, len1, len2);
    else
        __merge_adaptive(first, middle, last, len1, len2, buf.begin(), Distance(buf.size()));
}

template <typename BidirectionalIter, typename T, typename Distance, typename Compare>
inline void __inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, T*, Distance*, Compare comp) {
    Distance len1 = 0;
    distance(first, middle, len1);
    Distance len2 = 0;
    distance(middle, last, len2);

    _Temporary_buffer<BidirectionalIter, T> buf(first, last);
    if (buf.begin() == 0)
        __merge_without_buffer(first, middle, last, len1, len2, comp);
    else
        __merge_adaptive(first, middle, last, len1, len2, buf.begin(), Distance(buf.size()), comp);
}

template <typename BidirectionalIter>
inline void inplace_merge(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last) {
    __STL_REQUIRES(BidirectionalIter, _Mutable_BidirectionalIterator);
    __STL_REQUIRES(typename iterator_traits<BidirectionalIter>::value_type, _LessThanComparable);

    if (first == middle || middle == last)
        return;
    __inplace_merge_aux(first, middle, last, __VALUE_TYPE(first), __DISTANCE_TYPE(first));
}

template <typename BidirectionalIter, typename Compare>
inline void inplace_merge(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, Compare comp) {
    __STL_REQUIRES(BidirectionalIter, _Mutable_BidirectionalIterator);
    __STL_BINARY_FUNCTION_CHECK(Compare, bool, typename iterator_traits<BidirectionalIter>::value_type, typename iterator_traits<BidirectionalIter>::value_type);

    if (first == middle || middle == last)
        return;
    __inplace_merge_aux(first, middle, last, __VALUE_TYPE(first), __DISTANCE_TYPE(first), comp);
}

//stable_sort() and its auxiliary functions.
template <typename RandomAccessIter>
void __inplace_stable_sort(RandomAccessIter first, RandomAccessIter last) {
    if (last - first < 15) {
        __insertion_sort(first, last);
        return;
    }

    RandomAccessIter middle = first + (last - first) / 2;
    __inplace_stable_sort(first, middle);
    __inplace_stable_sort(middle, last);
    __merge_without_buffer(first, middle, last, middle - first, last - middle);
}

template <typename RandomAccessIter, typename Compare>
void __inplace_stable_sort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    if (last - first < 15) {
        __insertion_sort(first, last, comp);
        return;
    }

    RandomAccessIter middle = first + (last - first) / 2;
    __inplace_stable_sort(first, middle, comp);
    __inplace_stable_sort(middle, last, comp);
    __merge_without_buffer(first, middle, last, middle - first, last - middle, comp);
}

template <typename RandomAccessIter1, typename RandomAccessIter2, typename Distance>
void __merge_sort_loop(RandomAccessIter1 first, RandomAccessIter1 last, RandomAccessIter2 result, Distance step_size) {
    Distance two_step = 2 * step_size;
    while (last - first >= two_step) {
        result = merge(first, first + step_size, first + step_size, first + two_step, result);
        first += two_step;
    }

    step_size = min(Distance(last - first), step_size);
    merge(first, first + step_size, first + step_size, last, result);
}

template <typename RandomAccessIter1, typename RandomAccessIter2, typename Distance, typename Compare>
void __merge_sort_loop(RandomAccessIter1 first, RandomAccessIter1 last, RandomAccessIter2 result, Distance step_size, Compare comp) {
    Distance two_step = 2 * step_size;
    while (last - first >= two_step) {
        result = merge(first, first + step_size, first + step_size, first + two_step, result, comp);
        first += two_step;
    }

    step_size = min(Distance(last - first), step_size);
    merge(first, first + step_size, first + step_size, last, result, comp);
}

const int __stl_chunk_size = 7;

template <typename RandomAccessIter, typename Distance>
void __chunk_insertion_sort(RandomAccessIter first, RandomAccessIter last, Distance chunk_size) {
    while (last - first >= chunk_size) {
        __insertion_sort(first, first + chunk_size);
        first += chunk_size;
    }
    __insertion_sort(first, last);
}

template <typename RandomAccessIter, typename Distance, typename Compare>
void __chunk_insertion_sort(RandomAccessIter first, RandomAccessIter last, Distance chunk_size, Compare comp) {
    while (last - first >= chunk_size) {
        __insertion_sort(first, first + chunk_size, comp);
        first += chunk_size;
    }
    __insertion_sort(first, last, comp);
}

template <typename RandomAccessIter, typename Pointer, typename Distance>
void __merge_sort_with_buffer(RandomAccessIter first, RandomAccessIter last, Pointer buffer, Distance*) {
    Distance len = last - first;
    Pointer buffer_last = buffer + len;

    Distance step_size = __stl_chunk_size;
    __chunk_insertion_sort(first, last, step_size);

    while (step_size < len) {
        __merge_sort_loop(first, last, buffer, step_size);
        step_size *= 2;
        __merge_sort_loop(buffer, buffer_last, first, step_size);
        step_size *= 2;
    }
}

template <typename RandomAccessIter, typename Pointer, typename Distance, typename Compare>
void __merge_sort_with_buffer(RandomAccessIter first, RandomAccessIter last, Pointer buffer, Distance*, Compare comp) {
    Distance len = last - first;
    Pointer buffer_last = buffer + len;

    Distance step_size = __stl_chunk_size;
    __chunk_insertion_sort(first, last, step_size, comp);

    while (step_size < len) {
        __merge_sort_loop(first, last, buffer, step_size, comp);
        step_size *= 2;
        __merge_sort_loop(buffer, buffer_last, first, step_size, comp);
        step_size *= 2;
    }
}

template <typename RandomAccessIter, typename Pointer, typename Distance>
void __stable_sort_adaptive(RandomAccessIter first, RandomAccessIter last, Pointer buffer, Distance buffer_size) {
    Distance len = (last - first + 1) / 2;
    RandomAccessIter middle = first + len;
    if (len > buffer_size) {
        __stable_sort_adaptive(first, middle, buffer, buffer_size);
        __stable_sort_adaptive(middle, last, buffer, buffer_size);
    }
    else {
        __merge_sort_with_buffer(first, middle, buffer, (Distance*)0);
        __merge_sort_with_buffer(middle, last, buffer, (Distance*)0);
    }
    __merge_adaptive(first, middle, last, Distance(middle - first), Distance(last - middle), buffer, buffer_size);
}

template <typename RandomAccessIter, typename Pointer, typename Distance, typename Compare>
void __stable_sort_adaptive(RandomAccessIter first, RandomAccessIter last, Pointer buffer, Distance buffer_size, Compare comp) {
    Distance len = (last - first + 1) / 2;
    RandomAccessIter middle = first + len;
    if (len > buffer_size) {
        __stable_sort_adaptive(first, middle, buffer, buffer_size, comp);
        __stable_sort_adaptive(middle, last, buffer, buffer_size, comp);
    }
    else {
        __merge_sort_with_buffer(first, middle, buffer, (Distance*)0, comp);
        __merge_sort_with_buffer(middle, last, buffer, (Distance*)0, comp);
    }
    __merge_adaptive(first, middle, last, Distance(middle - first), Distance(last - middle), buffer, buffer_size, comp);
}

template <typename RandomAccessIter, typename T, 



__STL_END_NAMESPACE

#endif /*__SGI_STL_INTERNAL_ALGO_H*/