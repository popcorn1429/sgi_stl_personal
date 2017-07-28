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
    static __dummy_struct_##__type_var##__concept<__concept##_concept_specification<__type_var>::__concept##_requirement_violation> __dummy_ptr_##__type_var##__concept

#define __STL_CLASS_REQUIRES_SAME_TYPE(__type_x, __type_y) \
    typedef void (* __func_##__type_x##__type_y##same_type)(__type_x, __type_y); \
    template <__func_##__type_x##__type_y##same_type T> \
    struct __dummy_struct_##__type_x##__type_y##_same_type {}; \
    static __dummy_struct_##__type_x##__type_y##_same_type<_STL_SAME_TYPE_ERROR<__type_x, __type_y>::__type_X_not_same_as_type_Y> __dummy_ptr_##__type_x##__type_y##_same_type 

#define __STL_CLASS_GENERATOR_CHECK(__func, __ret) \
    typedef __ret (* __f_##__func##__ret##_generator)(__func&); \
    template <__f_##__func##__ret##_generator T> \
    struct __dummy_struct_##__func##__ret##_generator {}; \
    static __dummy_struct_##__func##__ret##_generator<_STL_GENERATOR_ERROR<__func, __ret>::__generator_requirement_violation> __dummy_ptr_##__func##__ret##_generator

#define __STL_CLASS_UNARY_FUNCTION_CHECK(__func, __ret, __arg) \
    typedef __ret (* __f_##__func##__ret##__arg##_unary_check)(__func&, const __arg&); \
    template <__f_##__func##__ret##__arg##_unary_check T> \
    struct __dummy_struct_##__func##__ret##__arg##_unary_check {}; \
    static __dummy_struct_##__func##__ret##__arg##_unary_check<_STL_UNARY_FUNCTION_ERROR<__func, __ret, __arg>::__unary_function_requirement_violation> __dummy_ptr_##__func##__ret##__arg##_unary_check

#define __STL_CLASS_BINARY_FUNCTION_CHECK(__func, __ret, __first, __second) \
    typedef __ret (* __f_##__func##__ret##__first##__second##_binary_check)(__func&, const __first&, const __second&); \
    template <__f_##__func##__ret##__first##__second##_binary_check T> \
    struct __dummy_struct_##__func##__ret##__first##__second##_binary_check {}; \
    static __dummy_struct_##__func##__ret##__first##__second##_binary_check<_STL_BINARY_FUNCTION_ERROR<__func, __ret, __first, __second>::__binary_function_requirement_violation> __dummy_ptr_##__func##__ret##__first##__second##_binary_check

#define __STL_CLASS_REQUIRES_BINARY_OP(__opname, __ret, __first, __second) \
    typedef __ret (* __f_##__opname##__ret##__first##__second##_binary_op)(const __first& , const __second&); \
    template <__f_##__opname##__ret##__first##__second##_binary_op T> \
    struct __dummy_struct_##__opname##__ret##__first##__second##_binary_op {}; \
    static __dummy_struct_##__opname##__ret##__first##__second##_binary_op<_STL_BINARY##__opname##_ERROR<__ret, __first, __second>::__binary_operator_requirement_violation> __dummy_ptr_##__opname##__ret##__first##__second##_binary_op

#endif

/*
helper class for finding non-const version of a type.
Need to have something to assign to etc,
when testing constant iterators.
*/
template <typename T>
struct _Mutable_trait {
    typedef T _Type;
};

template <typename T>
struct _Mutable_trait<const T> {
    typedef T _Type;
};

/*
helper function for avoiding compiler warnings about unused variables
*/
template <typename T>
void __sink_unused_warning(T) {}

template <typename X, typename Y>
struct _STL_CONVERT_ERROR {
    static void __type_X_is_not_convertible_to_type_Y(X __x, Y) {
        Y __y = __x;
        __sink_unused_warning(__y);
    }
};

template <typename T> 
struct __check_equal {};

template <typename X, typename Y>
struct _STL_SAME_TYPE_ERROR {
    static void __type_X_not_same_as_type_Y(X , Y) {
        __check_equal<X> t = __check_equal<Y>();
    }
};

//Some Function Object Checks

template <typename __func, typename __ret>
struct _STL_GENERATOR_ERROR {
    static __ret __generator_requirement_violation(__func& __f) {
        return __f();
    }
};

template <typename __func>
struct _STL_GENERATOR_ERROR<__func, void> {
    static void __generator_requirement_violation(__func& __f) {
        __f();
    }
};

template <typename __func, typename __ret, typename __arg>
struct _STL_UNARY_FUNCTION_ERROR {
    static __ret __unary_function_requirement_violation(__func& __f, const __arg& __argument) {
        return __f(__argument);
    }
};

template <typename __func, typename __arg>
struct _STL_UNARY_FUNCTION_ERROR<__func, void, __arg> {
    static void __unary_function_requirement_violation(__func& __f, const __arg& __argument) {
        __f(__argument);
    }
};

template <typename __func, typename __ret, typename __first, typename __second>
struct _STL_BINARY_FUNCTION_ERROR {
    static __ret __binary_function_requirement_violation(__func& __f, const __first& __first_arg, const __second& __second_arg) {
        return __f(__first_arg, __second_arg);
    }
};

template <typename __func, typename __first, typename __second>
struct _STL_BINARY_FUNCTION_ERROR<__func, void, __first, __second> {
    static void __binary_function_requirement_violation(__func& __f, const __first& __first_arg, const __second& __second_arg) {
        __f(__first_arg, __second_arg);
    }
};

#define __STL_DEFINE_BINARY_OP_CHECK(_OP, _NAME) \
    template <typename __ret, typename __first, typename __second> \
struct _STL_BINARY##_NAME##_ERROR { \
    static __ret __const_binary_operator_requirement_violation()
}


#endif


#endif
