#ifndef __SGI_STL_INTERNAL_NUMERIC_H
#define __SGI_STL_INTERNAL_NUMERIC_H

#include "stl_config.h"
#include "concept_checks.h"
#include "stl_iterator_base.h"

__STL_BEGIN_NAMESPACE

template <typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T initial) {
    __STL_REQUIRES(InputIterator, _InputIterator);
    for (; first != last; ++first)
        initial = initial + *first;

    return initial;
}

template <typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T initial, BinaryOperation binary_op) {
    __STL_REQUIRES(InputIterator, _InputIterator);
    for (; first != last; ++first)
        initial = binary_op(initial, *first);

    return initial;
}

template <typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T initial) {
    __STL_REQUIRES(InputIterator1, _InputIterator);
    __STL_REQUIRES(InputIterator2, _InputIterator);
    for (; first1 != last1; ++first1, ++first2)
        initial = initial + (*first1 * *first2);

    return initial;
}

template <typename InputIterator1, typename InputIterator2, typename T, typename BinaryOperation1, typename BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T initial, BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
    __STL_REQUIRES(InputIterator1, _InputIterator);
    __STL_REQUIRES(InputIterator2, _InputIterator);
    for (; first1 != last1; ++first1, ++first2) {
        initial = binary_op1(initial, binary_op2(*first1, *first2));
    }
    return initial;
}

template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*) {
    T val = *first;
    while (++first != last) {
        val = val + *first;
        *++result = val;
    }
    return ++result;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
    __STL_REQUIRES(InputIterator, _InputIterator);
    __STL_REQUIRES(OutputIterator, _OutputIterator);
    if (first == last) return result;
    *result = *first;
    return __partial_sum(first, last, result, __VALUE_TYPE(first));
}

template <typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op) {
    T val = *first;
    while (++first != last) {
        val = binary_op(val, *first);
        *++result = val;
    }
    return ++result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    __STL_REQUIRES(InputIterator, _InputIterator);
    __STL_REQUIRES(OutputIterator, _OutputIterator);
    if (first == last) return result;
    *result = *first;
    return __partial_sum(first, last, result, __VALUE_TYPE(first), binary_op);
}

template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*) {
    T val = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = tmp - val;
        val = tmp;
    }
    return ++result;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
    __STL_REQUIRES(InputIterator, _InputIterator);
    __STL_REQUIRES(OutputIterator, _OutputIterator);
    if (first == last) return result;
    *result = *first;
    return __adjacent_difference(first, last, result, __VALUE_TYPE(first));
}

template <typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op) {
    T val = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = binary_op(tmp, val);
        val = tmp;
    }
    return ++result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    __STL_REQUIRES(InputIterator, _InputIterator);
    __STL_REQUIRES(OutputIterator, _OutputIterator);
    if (first == last) return result;
    *result = *first;
    return __adjacent_difference(first, last, result, __VALUE_TYPE(first), binary_op);
}

// Returns x**n, where n >= 0.
// Note that "multiplication" is required to be associative,
// but not necessarily commutative.
template <typename T, typename Integer, typename MonoidOperation>
T __power(T x, Integer n, MonoidOperation oper) {
    if (0 == n)
        return identity_element(oper);
    else {
        while ((n & 1) == 0) {
            n >>= 1;
            x = oper(x, x);
        }

        T result = x;
        n >>= 1;
        while (n != 0) {
            x = oper(x, x);
            if ((n & 1) != 0)
                result = oper(result, x);
            n >>= 1;
        }
        return result;
    }
}

template <typename T, typename Integer>
inline T __power(T x, Integer n) {
    return __power(x, n, multiplies<T>());
}

// Alias for the internal name __power.Note that power is an extension,
// not part of the C++ standard.
template <typename T, typename Integer, typename MonoidOperation>
inline T power(T x, Integer n, MonoidOperation oper) {
    return __power(x, n, oper);
}

template <typename T, typename Integer>
inline T power(T x, Integer n) {
    return __power(x, n);
}

//iota is not part of the C++ standard.
template <typename ForwardIter, typename T>
void iota(ForwardIter first, ForwardIter last, T value) {
    __STL_REQUIRES(ForwardIter, _Mutable_ForwardIterator);
    __STL_CONVERTIBLE(T, typename iterator_traits<ForwardIter>::value_type);
    while (first != last)
        *first++ = value++;
}

__STL_END_NAMESPACE

#endif /*__SGI_STL_INTERNAL_NUMERIC_H*/
