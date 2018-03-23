#ifndef __SGI_STL_INTERNAL_ITERATOR_H
#define __SGI_STL_INTERNAL_ITERATOR_H

#include "type_traits.h"

__STL_BEGIN_NAMESPACE
template <class _Container>
class back_insert_iterator {
protected:
    _Container* container;
public:
    typedef _Container            container_type;
    typedef output_iterator_tag   iterator_category;
    typedef void                  value_type;
    typedef void                  difference_type;
    typedef void                  pointer;
    typedef void                  reference;

    explicit back_insert_iterator(_Container& __c) : container(&__c) {}

    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __val) {
        container->push_back(__val);
        return *this;
    }

    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
    back_insert_iterator<_Container>& operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION
template <class _Container>
inline output_iterator_tag iterator_category(const back_insert_iterator<_Container>&) {
    return output_iterator_tag();
}
#endif /*__STL_CLASS_PARTIAL_SPECIALIZATION*/

template <class _Container>
inline back_insert_iterator<_Container> back_inserter(_Container& __c) {
    return back_insert_iterator<_Container>(__c);
}

template <class _Container>
class front_insert_iterator {
protected:
    _Container* container;
public:
    typedef _Container             container_type;
    typedef output_iterator_tag    iterator_category;
    typedef void                   value_type;
    typedef void                   difference_type;
    typedef void                   pointer;
    typedef void                   reference;

    explicit front_insert_iterator(_Container& __c) : container(&__c) {}
    
    front_insert_iterator<_Container>& operator=(const typename _Container::value_type& __val) {
        container->push_front(__val);
        return *this;
    }

    front_insert_iterator<_Container>& operator*() { return *this; }
    front_insert_iterator<_Container>& operator++() { return *this; }
    front_insert_iterator<_Container>& operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION
template <class _Container>
inline output_iterator_tag iterator_category(const front_insert_iterator<_Container>&) {
    return output_iterator_tag();
}
#endif /*__STL_CLASS_PARTIAL_SPECIALIZATION*/

template <class _Container>
inline front_insert_iterator<_Container> front_inserter(_Container& _c) {
    return front_insert_iterator<_Container>(_c);
}

template <class _Container>
class insert_iterator {
protected:
    _Container* container;
    typename _Container::iterator iter;
public:
    typedef _Container           container_type;
    typedef output_iterator_tag  iterator_category;
    typedef void                 value_type;
    typedef void                 difference_type;
    typedef void                 pointer;
    typedef void                 reference;

    insert_iterator(_Container& __c, typename _Container::iterator __i) : container(&__c), iter(__i) {}

    insert_iterator<_Container>& operator=(const typename _Container::value_type& __val) {
        iter = container->insert(iter, __val);
        ++iter;
        return *this;
    }

    insert_iterator<_Container>& operator*() { return *this; }
    insert_iterator<_Container>& operator++() { return *this; }
    insert_iterator<_Container>& operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION
template <_Container>
inline output_iterator_tag iterator_category(const insert_iterator<_Container>&) {
    return output_iterator_tag();
}
#endif /*__STL_CLASS_PARTIAL_SPECIALIZATION*/

template <class _Container, class _Iterator>
inline insert_iterator<_Container> inserter(_Container& __c, _Iterator __i) {
    typedef typename _Container::iterator _Iter_type;
    return insert_iterator<_Container>(__c, _Iter_type(__i));
}

#ifndef __STL_LIMITED_DEFAULT_TEMPLATES
template <class _BidirectionalIter, class _T, class _Reference = _T&, class _Distance = ptrdiff_t>
#else
template <class _BidirectionalIter, class _T, class _Reference, class _Distance>
#endif /*__STL_LIMITED_DEFAULT_TEMPLATES*/
class reverse_bidirectional_iterator {
    typedef reverse_bidirectional_iterator<_BidirectionalIter, _T, _Reference, _Distance> _Self;
protected:
    _BidirectionalIter current;
public:
    typedef bidirectional_iterator_tag iterator_category;
    typedef _T                         value_type;
    typedef _Distance                  difference_type;
    typedef _T*                        pointer;
    typedef _Reference                 reference;

    reverse_bidirectional_iterator() {}
    explicit reverse_bidirectional_iterator(_BidirectionalIter __i) : current(__i) {}

    _BidirectionalIter base() const { return current; }
    _Reference operator*() const {
        _BidirectionalIter __tmp = current;
        return *--__tmp;
    }

#ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const { return &(operator*()); }
#endif

    _Self& operator++() {
        --current;
        return *this;
    }

    _Self operator++(int) {
        _Self __tmp = *this;
        --current;
        return __tmp;
    }

    _Self& operator--() {
        ++current;
        return *this;
    }
    _Self operator--(int) {
        _Self __tmp = *this;
        ++current;
        return __tmp;
    }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION
template <class _BidirectionalIter, class _T, class _Reference, class _Distance>
inline bidirectional_iterator_tag iterator_category(const reverse_bidirectional_iterator<_BidirectionalIter, _T, _Reference, _Distance>&) {
    return bidirectional_iterator_tag();
}

template <class _BidirectionalIter, class _T, class _Reference, class _Distance>
inline _T* value_type(const reverse_bidirectional_iterator<_BidirectionalIter, _T, _Reference, _Distance>&) {
    return (_T*)0;
}

template <class _BidirectionalIter, class _T, class _Reference, class _Distance>
inline _Distance* distance_type(const reverse_bidirectional_iterator<_BidirectionalIter, _T, _Reference, _Distance>&) {
    return (_Distance*)0;
}
#endif /*__STL_CLASS_PARTIAL_SPECIALIZATION*/

template <class _BidirectionalIter, class _T, class _Reference, class _Distance>
inline bool operator==(const reverse_bidirectional_iterator<_BidirectionalIter, _T, _Reference, _Distance>& __x, const reverse_bidirectional_iterator<_BidirectionalIter, _T, _Reference, _Distance>& __y) {
    return __x.base() == __y.base();
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER
template <class _BidirectionalIter, class _T, class _Reference, class _Distance>
inline bool operator!=(const reverse_bidirectional_iterator<_BidirectionalIter, _T, _Reference, _Distance>& __x, const reverse_bidirectional_iterator<_BidirectionalIter, _T, _Reference, _Distance>& __y) {
    return !(__x == __y);
}
#endif /*__STL_FUNCTION_TMPL_PARTIAL_ORDER*/

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
// This is the new version of reverse_iterator, as defined in the 
//  draft C++ standard. It relies on the iterator_traits template,
//  which in turn relies on partial specialization. The class 
//  reverse_bidirectional_iterator is no longer part of the draft
//  standard, but it is retained for backward compatibility.

template <class _Iterator>
class reverse_iterator {
protected:
    _Iterator current;
public:
    typedef typename iterator_traits<_Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<_Iterator>::value_type        value_type;
    typedef typename iterator_traits<_Iterator>::difference_type   difference_type;
    typedef typename iterator_traits<_Iterator>::pointer           pointer;
    typedef typename iterator_traits<_Iterator>::reference         reference;

    typedef _Iterator iterator_type;
    typedef reverse_iterator<_Iterator> _Self;

public:
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type __i) : current(__i) {}
    reverse_iterator(const _Self& __r) : current(__r.current) {}
#ifdef __STL_MEMBER_TEMPLATES
    template <class _Iter>
    reverse_iterator(const reverse_iterator<_Iter>& __r) : current(__r.base()) {}
#endif /*__STL_MEMBER_TEMPLATES*/

    iterator_type base() const { return current; }
    reference operator*() const {
        iterator_type __tmp = current;
        return *--__tmp;
    }

#ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const { return &(operator*()); }
#endif /*__SGI_STL_NO_ARROW_OPERATOR*/

    _Self& operator++() {
        --current;
        return *this;
    }

    _Self operator++(int) {
        _Self __tmp = *this;
        --current;
        return __tmp;
    }
};


#endif /*__STL_CLASS_PARTIAL_SPECIALIZATION*/


__STL_END_NAMESPACE

#endif /*__SGI_STL_INTERNAL_ITERATOR_H*/