#ifndef __CONCEPT_CHECKS_H
#define __CONCEPT_CHECKS_H

#ifndef __STL_USE_CONCEPT_CHECKS
//If the compiler lack the features that are necessary for
//concept checks, we define the concept check macros to do nothing.
#define __STL_REQUIRES(__type_var, __concept) do {} while (0)
#define __STL_CLASS_REQUIRES(__type_var, __concept) \
    static int __##__type_var##_##__concept
#define __STL_CONVERTIBLE(__type_x, __type_y) do {} while(0)
#define __STL_REQUIRES_SAME_TYPE(__type_x, __type_y) do {} while(0)
#define __STL_CLASS_REQUIRES_SAME_TYPE(__type_x, __type_y) \
    static int __##__type_x##__type_y##_require_same_type
#define __STL_GENERATOR_CHECK(__func, __ret) do {} while(0)
#define __STL_CLASS_GENERATOR_CHECK(__func, __ret) \
    static int __##__func##__ret##_generator_check
#define __STL_UNARY_FUNCTION_CHECK(__func, __ret, __arg) do {} while(0)
#define __STL_CLASS_UNARY_FUNCTION_CHECK(__func, __ret, __arg) \
    static int __##__func##__ret##__arg##_unary_function_check
#define __STL_BINARY_FUNCTION_CHECK(__func, __ret, __first, __second) do {} while(0)
#define __STL_CLASS_BINARY_FUNCTION_CHECK(__func, __ret, __first, __second) \
    static int __##__func##__ret##__first##__second##_binary_function_check
#define __STL_REQUIRES_BINARY_OP(__opname, __ret, __first, __second) do {} while(0)
#define __STL_CLASS_REQUIRES_BINARY_OP(__opname, __ret, __first, __second) \
    static int __##__opname##__ret##__first##__second##_REQUIRES_binary_op
#else
/*
This macro tests whether the template argument "__type_var" satisfies the 
requirements of "__concept".
Here is a list of concepts that we know how to check:
    _Allocator
    _Assignable
    _DefaultConstructible
    _EqualityComparable
    _LessThanComparable
    _TrivialIterator
    _InputIterator
    _OutputIterator
    _ForwardIterator
    _BidirectionalIterator
    _RandomAccessIterator
    _Mutable_TrivialIterator
    _Mutable_ForwardIterator
    _Mutable_BidirectionalIterator
    _Mutable_RandomAccessIterator
*/
#define __STL_REQUIRES(__type_var, __concept) \
    do { \
        void (*__x)(__type_var) = __concept##_concept_specification<__type_var>::__concept##_requirement_violation; \
        __x = __x; \
    } while (0)

//Use this macro to check whether type x is convertible to type y
#define __STL_CONVERTIBLE(__type_x, __type_y) \
    do { \
        void (*__x)(__type_x, __type_y) = _STL_CONVERT_ERROR<__type_x, __type_y>::__type_X_is_not_convertible_to_type_Y; \
        __x = __x; \
    } while (0)

//Use this macro to test whether two template arguments are the same type
#define __STL_REQUIRES_SAME_TYPE(__type_x, __type_y) \
    do { \
        void (*__x)(__type_x, __type_y) = _STL_SAME_TYPE_ERROR<__type_x, __type_y>::__type_X_not_same_as_type_Y; \
        __x = __x; \
    } while (0)

//function object checks
#define __STL_GENERATOR_CHECK(__func, __ret) \
    do { \
        __ret (*__x) (__func&) = _STL_GENERATOR_ERROR<__func, __ret>::__generator_requirement_violation; \
        __x = __x; \
    } while (0)

#define __STL_UNARY_FUNCTION_CHECK(__func, __ret, __arg) \
    do { \
        __ret (*__x) (__func&, const __arg&) = _STL_UNARY_FUNCTION_ERROR<__func, __ret, __arg>::__unary_function_requirement_violation; \
        __x = __x; \
    } while (0)

#define __STL_BINARY_FUNCTION_CHECK(__func, __ret, __first, __second) \
    do { \
        __ret (*__x)(__func&, const __first&, const __second) = _STL_BINARY_FUNCTION_ERROR<__func, __ret, __first, __second>::__binary_function_requirement_violation; \
        __x = __x; \
    } while (0)

#define __STL_REQUIRES_BINARY_OP(__opname, __ret, __first, __second) \
    do { \
        __ret (*__x)(__first&, __second&) = \
            _STL_BINARY##__opname##_ERROR<__ret, __first, __second>::__binary_operator_requirement_violation; \
        __ret (*__y)(const __first&, const __second&) = \
            _STL_BINARY##__opname##_ERROR<__ret, __first, __second>::__const_binary_operator_requirement_violation; \
        __y = __y; \
        __x = __x; \
    } while (0)

#ifdef __STL_NO_FUNCTION_PTR_IN_CLASS_TEMPLATE

#define __STL_CLASS_REQUIRES(__type_var, __concept)
#define __STL_CLASS_REQUIRES_SAME_TYPE(__type_x, __type_y)
#define __STL_CLASS_GENERATOR_CHECK(__func, __ret)
#define __STL_CLASS_UNARY_FUNCTION_CHECK(__func, __ret, __arg)
#define __STL_CLASS_BINARY_FUNCTION_CHECK(__func, __ret, __first, __second)
#define __STL_CLASS_REQUIRES_BINARY_OP(__opname, __ret, __first, __second)

#else
//

#define __STL_CLASS_REQUIRES(__type_var, __concept) \
    typedef void (*__func##__type_var##__concept)(__type_var); \
    template <__func##__type_var##__concept T> \
    struct __dummy_struct_##__type_var##__concept {} ; \


#endif



#endif


#endif
