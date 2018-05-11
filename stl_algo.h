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

__STL_END_NAMESPACE

#endif /*__SGI_STL_INTERNAL_ALGO_H*/