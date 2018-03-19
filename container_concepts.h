#ifndef __STL_CONTAINER_CONCEPTS_H
#define __STL_CONTAINER_CONCEPTS_H
#include "stl_config.h"
#include "concept_checks.h"

#ifdef __STL_USE_CONCEPT_CHECKS

// This File covers the following concepts:
//      _Container
//      _ForwardContainer
//      _ReversibleContainer
//      _const_ReversibleContainer
//      _RandomAccessContainer

struct _ERROR_IN_STL_CONTAINER {
    template <class _Container>
    static void __begin_iterator_accessor_requirement_violation(_Container __c) {
        __c.begin();
    }

    template <class _Container>
    static void __const_begin_iterator_accessor_requirement_violation(const _Container& __c) {
        __c.begin();
    }

    template <class _Container>
    static void __end_iterator_accessor_requirement_violation(_Container __c) {
        __c.end();
    }

    template <class _Container>
    static void __const_end_iterator_accessor_requirement_violation(const _Container& __c) {
        __c.end();
    }

    template <class _Container>
    static void __rbegin_iterator_accessor_requirement_violation(_Container __c) {
        __c.rbegin();
    }

    template <class _Container>
    static void __const_rbegin_iterator_accessor_requirement_violation(const _Container& __c) {
        __c.rbegin();
    }

    template <class _Container>
    static void __rend_iterator_accessor_requirement_violation(_Container __c) {
        __c.rend();
    }

    template <class _Container>
    static void __const_rend_iterator_accessor_requirement_violation(const _Container& __c) {
        __c.rend();
    }

    template <class _Container>
    static void __size_function_must_be_const(const _Container& __c) {
        __c.size();
    }

    template <class _Container>
    static void __size_function_requirement_violation(_Container& __c) {
        __c.size();
        __size_function_must_be_const(__c);
    }

    template <class _Container>
    static void __max_size_function_must_be_const(const _Container& __c) {
        __c.max_size();
    }

    template <class _Container>
    static void __max_size_function_requirement_violation(_Container& __c) {
        __c.max_size();
        __max_size_function_must_be_const(__c);
    }

    template <class _Container>
    static void __empty_function_must_be_const(const _Container& __c) {
        __c.empty();
    }

    template <class _Container>
    static void __empty_function_requirement_violation(_Container& __c) {
        __c.empty();
        __empty_function_must_be_const(__c);
    }

    template <class _Container>
    static void __swap_function_requirement_violation(_Container& __c) {
        __c.swap(__c);
    }
};

__STL_TYPEDEF_REQUIREMENT(iterator);
__STL_TYPEDEF_REQUIREMENT(const_iterator);

template <class _Container>
struct _Container_concept_specification {
    static void _Container_requirement_violation(_Container __c) {
        _Assignable_concept_specification<_Container>::_Assignable_requirement_violation(__c);
        __value_type__typedef_requirement_violation<_Container>();
        __difference_type__typedef_requirement_violation<_Container>();
        __size_type__typedef_requirement_violation<_Container>();
        __reference__typedef_requirement_violation<_Container>();
        __const_reference__typedef_requirement_violation<_Container>();
        __pointer__typedef_requirement_violation<_Container>();
        __const_pointer__typedef_requirement_violation<_Container>();
        __iterator__typedef_requirement_violation<_Container>();
        __const_iterator__typedef_requirement_violation<_Container>();

        _ERROR_IN_STL_CONTAINER::__const_begin_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__const_end_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__begin_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__end_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__size_function_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__max_size_function_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__empty_function_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__swap_function_requirement_violation(__c);

        typedef typename _Container::iterator iter;
        typedef typename _Container::const_iterator const_iter;
        _InputIterator_concept_specification<const_iter>::_InputIterator_requirement_violation(const_iter());
        _InputIterator_concept_specification<iter>::_InputIterator_requirement_violation(iter());
    }
};

template <class _ForwardContainer>
struct _ForwardContainer_concept_specification {
    static void _ForwardContainer_requirement_violation(_ForwardContainer __c) {
        _Container_concept_specification<_ForwardContainer>::_Container_requirement_violation(__c);

        typedef typename _ForwardContainer::iterator iter;
        typedef typename _ForwardContainer::const_iterator const_iter;
        _ForwardIterator_concept_specification<const_iter>::_ForwardIterator_requirement_violation(const_iter());
        _Mutable_ForwardIterator_concept_specification<iter>::_Mutable_ForwardIterator_requirement_violation(iter());
    }
};

__STL_TYPEDEF_REQUIREMENT(reverse_iterator);
__STL_TYPEDEF_REQUIREMENT(const_reverse_iterator);

template <class _ReversibleContainer>
struct _ReversibleContainer_concept_specification {
    static void _ReversibleContainer_requirement_violation(_ReversibleContainer __c) {
        _ForwardContainer_concept_specification<_ReversibleContainer>::_ForwardContainer_requirement_violation(__c);

        __reverse_iterator__typedef_requirement_violation<_ReversibleContainer>();
        __const_reverse_iterator__typedef_requirement_violation<_ReversibleContainer>();

        _ERROR_IN_STL_CONTAINER::__const_rbegin_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__const_rend_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__rbegin_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__rend_iterator_accessor_requirement_violation(__c);

        typedef typename _ReversibleContainer::iterator iter;
        typedef typename _ReversibleContainer::const_iterator const_iter;
        _BidirectionalIterator_concept_specification<const_iter>::_BidirectionalIterator_requirement_violation(const_iter());
        _Mutable_BidirectionalIterator_concept_specification<iter>::_Mutable_BidirectionalIterator_requirement_violation(iter());
    }
};

template <class _ReversibleContainer>
struct _const_ReversibleContainer_concept_specification {
    static void _const_ReversibleContainer_requirement_violation(_ReversibleContainer __c) {
        _Container_concept_specification<_ReversibleContainer>::_Container_requirement_violation(__c);

        __reverse_iterator__typedef_requirement_violation<_ReversibleContainer>();
        __const_reverse_iterator__typedef_requirement_violation<_ReversibleContainer>();

        _ERROR_IN_STL_CONTAINER::__const_rbegin_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__const_rend_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__rbegin_iterator_accessor_requirement_violation(__c);
        _ERROR_IN_STL_CONTAINER::__rend_iterator_accessor_requirement_violation(__c);

        typedef typename _ReversibleContainer::iterator iter;
        typedef typename _ReversibleContainer::const_iterator const_iter;

        _BidirectionalIterator_concept_specification<const_iter>::_BidirectionalIterator_requirement_violation(const_iter());
    }
};

template <class _RandomAccessContainer>
struct _RandomAccessContainer_concept_specification {
    static void _RandomAccessContainer_requirement_violation(_RandomAccessContainer __c) {
        _ReversibleContainer_concept_specification<_RandomAccessContainer>::_ReversibleContainer_requirement_violation(__c);

        typedef typename _RandomAccessContainer::value_type __T;
        typedef typename _RandomAccessContainer::difference_type _Dist;
        typedef typename _Mutable_trait<_T>::_Type Type;
        typedef          Type*                     _TypePtr;
        typedef typename _Mutable_trait<_Dist>::_Type Dist;
        _STL_ERROR::__element_access_operator_requirement_violation(__c, _TypePtr(), Dist());

        typedef typename _RandomAccessContainer::iterator iter;
        typedef typename _RandomAccessContainer::const_iterator const_iter;
        _RandomAccessIterator_concept_specification<const_iter>::_RandomAccessIterator_requirement_violation(const_iter());
        _Mutable_RandomAccessIterator_concept_specification<iter>::_Mutable_RandomAccessIterator_requirement_violation(iter());
    }
};

#endif /*__STL_USE_CONCEPT_CHECKS*/
#endif /*__STL_CONTAINER_CONCEPTS_H*/