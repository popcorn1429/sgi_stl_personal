#ifndef __SGI_STL_INTERNAL_CTRAITS_FUNCTIONS_H
#define __SGI_STL_INTERNAL_CTRAITS_FUNCTIONS_H

// this file contains a few small adapters that allow 
// a character traits class to be used as a function object.
#include "stl_config.h"
#include "stl_function.h"
#include "char_traits.h"

__STL_BEGIN_NAMESPACE

template <typename Traits>
struct _Eq_traits : public binary_function<typename Traits::char_type, typename Traits::char_type, bool> {
    bool operator() (const typename Traits::char_type& x, const typename Traits::char_type& y) const {
        return Traits::eq(x, y);
    }
};

template <typename Traits>
struct _Eq_int_traits : public binary_function<typename Traits::char_type, typename Traits::int_type, bool> {
    bool operator() (const typename Traits::char_type& x, const typename Traits::int_type& y) const {
        return Traits::eq_int_type(Traits::to_int_type(x), y);
    }
};

template <typename Traits>
struct _Lt_traits : public binary_function<typename Traits::char_type, typename Traits::char_type, bool> {
    bool operator() (const typename Traits::char_type& x, const typename Traits::char_type& y) const {
        return Traits::lt(x, y);
    }
};

__STL_END_NAMESPACE

#endif /*__SGI_STL_INTERNAL_CTRAITS_FUNCTIONS_H*/
