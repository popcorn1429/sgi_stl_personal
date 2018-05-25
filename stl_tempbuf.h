#ifndef __SGI_STL_INTERNAL_TEMPBUF_H
#define __SGI_STL_INTERNAL_TEMPBUF_H
#include "stl_config.h"
#include "stl_pair.h" //pair
#include "type_traits.h" //__true_type  __false_type  __type_traits
#include "stl_iterator_base.h" //distance
#include "stl_construct.h" //destroy  uninitialized_fill_n
__STL_BEGIN_NAMESPACE
template <typename T>
pair<T*, ptrdiff_t> __get_temporary_buffer(ptrdiff_t len, T*) {
    if (len > ptrdiff_t(INT_MAX / sizeof(T)))
        len = INT_MAX / sizeof(T);

    while (len > 0) {
        T* tmp = (T*)malloc((size_t)len*sizeof(T));
        if (tmp != 0)
            return pair<T*, ptrdiff_t>(tmp, len);

        len /= 2;
    }

    return pair<T*, ptrdiff_t>((T*)0, 0);
}

#ifdef __STL_EXPLICIT_FUNCTION_TMPL_ARGS
template <typename T>
inline pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
    return __get_temporary_buffer(len, (T*)0);
}
#endif /* __STL_EXPLICIT_FUNCTION_TMPL_ARGS */

//This overload is not required by the standard, it's an extension.
//It is supported for backward compatibility with the HP STL,
//and because not all compilers support the language feature (explicit
// function template arguments) that is required for the standard 
// version of get_temporary_buffer.
template <typename T>
inline pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*) {
    return __get_temporary_buffer(len, (T*)0);
}

template <typename T>
void return_temporary_buffer(T* p) {
    free(p);
}

template <typename ForwardIterator, typename T>
class _Temporary_buffer {
private:
    ptrdiff_t _M_original_len;
    ptrdiff_t _M_len;
    T*        _M_buffer;

    void _M_allocate_buffer() {
        _M_original_len = _M_len;
        _M_buffer = 0;
        if (_M_len > (ptrdiff_t)(INT_MAX / sizeof(T)))
            _M_len = INT_MAX / sizeof(T);

        while (_M_len > 0) {
            _M_buffer = (T*)malloc(_M_len*sizeof(T));
            if (_M_buffer != 0)
                break;
            _M_len /= 2;
        }
    }

    void _M_initialize_buffer(const T&, __true_type) {}
    void _M_initialize_buffer(const T& val, __false_type) {
        uninitialized_fill_n(_M_buffer, _M_len, val);
    }

public:
    ptrdiff_t size() const { return _M_len; }
    ptrdiff_t requested_size() const { return _M_original_len; }
    T* begin() { return _M_buffer; }
    T* end() { return _M_buffer + _M_len; }

    _Temporary_buffer(ForwardIterator first, ForwardIterator last) {
#if defined(__sgi) && !defined(__GNUC__) && _COMPILER_VERSION < 730
        typedef typename __type_traits<T>::is_POD_type _Trivial;
#else
        typedef typename __type_traits<T>::has_trivial_default_constructor _Trivial;
#endif
        __STL_TRY{
            _M_len = 0;
            distance(first, last, _M_len);
            _M_allocate_buffer();
            if (_M_len > 0)
                _M_initialize_buffer(*first, _Trivial());
        }
        __STL_UNWIND(free(_M_buffer); _M_buffer = 0);
    }

    ~_Temporary_buffer() {
        destroy(_M_buffer, _M_buffer + _M_len);
        free(_M_buffer);
    }

private:
    //disable copy-constructor
    _Temporary_buffer(const _Temporary_buffer&) {}
    //disable assignment operator
    void operator=(const _Temporary_buffer&) {}
};

//Class temporary_buffer is not part of the standard, it's an extension.
template <typename ForwardIter, typename T
#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
    = typename iterator_traits<ForwardIter>::value_type
#endif
                                             >
struct temporary_buffer : public _Temporary_buffer<ForwardIter, T> {
    temporary_buffer(ForwardIter first, ForwardIter last) : _Temporary_buffer<ForwardIter, T>(first, last) {}
    ~temporary_buffer() {}
};

__STL_END_NAMESPACE


#endif /* __SGI_STL_INTERNAL_TEMPBUF_H */