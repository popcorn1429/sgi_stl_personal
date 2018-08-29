#ifndef __SGI_STL_CHAR_TRAITS_H
#define __SGI_STL_CHAR_TRAITS_H

#include "stl_config.h"
#include <cstring>  //memcpy memmove memcmp ...
#include <cwchar>   //wchar_t
#include <cwctype>  //wint_t

#if defined(__STL_USE_NEW_IOSTREAMS) && !defined(__SGI_STL_IOSFWD)
#include <iosfwd>
#endif /* use new iostreams */

__STL_BEGIN_NAMESPACE

// Class __char_traits_base.
template <typename CharT, typename IntT>
class __char_traits_base {
public:
    typedef CharT char_type;
    typedef IntT  int_type;
#ifdef __STL_USE_NEW_IOSTREAMS
    typedef streamoff  off_type;
    typedef streampos  pos_type;
    typedef mbstate_t  state_type;
#endif /* __STL_USE_NEW_IOSTREAMS */

    static void assign(char_type& c1, const char_type& c2) { c1 = c2; }
    static bool eq(const char_type& c1, const char_type& c2) { return c1 == c2; }
    static bool lt(const char_type& c1, const char_type& c2) { return c1 < c2; }

    static int compare(const char_type* s1, const char_type* s2, size_t n) {
        for (size_t i = 0; i < n; ++i)
            if (!eq(s1[i], s2[i]))
                return s1[i] < s2[i] ? -1 : 1;

        return 0;
    }

    static size_t length(const char_type* s) {
        const char_type nullchar = char_type();
        size_t i = 0;
        for (; !eq(s[i], nullchar); ++i) {}
        return i;
    }

    static const char_type* find(const char_type* s, size_t n, const char_type& c) {
        for (; n > 0; ++s, --n)
            if (eq(*s, c))
                return s;
        return nullptr;
    }

    static char_type* move(char_type* dst, const char_type* src, size_t n) {
        memmove(dst, src, n * sizeof(char_type));
        return dst;
    }

    static char_type* copy(char_type* dst, const char_type* src, size_t n) {
        memcpy(dst, src, n * sizeof(char_type));
        return dst;
    }

    static char_type* assign(char_type* s, size_t n, char_type c) {
        for (size_t i = 0; i < n; ++i)
            s[i] = c;
        return s;
    }

    static int_type eof() {
        return static_cast<int_type>(-1);
    }

    static bool eq_int_type(const int_type& c1, const int_type& c2) {
        return c1 == c2;
    }

    static int_type not_eof(const int_type& c) {
        return !eq_int_type(c, eof()) ? c : 0;
    }

    static char_type to_char_type(const int_type& c) {
        return static_cast<char_type>(c);
    }

    static int_type to_int_type(const char_type& c) {
        return static_cast<int_type>(c);
    }
};

// Generic char_traits class.
// Note that this class is provided only as a base for 
// explicit specialization; it's unlikely to be useful
// as is for any particular user-defined type.
// In particular, it will NOT work for a non-POD type.
template <typename charT>
class char_traits : public __char_traits_base<charT, charT> {};

// Specialization for char.
__STL_TEMPLATE_NULL
class char_traits<char> : public __char_traits_base<char, int> {
public:
    static char_type to_char_type(const int_type& c) {
        return static_cast<char_type>(static_cast<unsigned char>(c));
    }

    static int_type to_int_type(const char_type& c) {
        return static_cast<unsigned char>(c);  //unsigned char will convert to int automatically
    }

    static int compare(const char_type* s1, const char_type* s2, size_t n) {
        return memcmp(s1, s2, n);
    }

    static size_t length(const char_type* s) {
        return strlen(s);
    }

    static void assign(char_type& c1, const char_type& c2) { c1 = c2; }

    static char_type* assign(char_type* s, size_t n, char_type c) {
        memset(s, c, n);
        return s;
    }
};

// Specialization for wchar_t
__STL_TEMPLATE_NULL
class char_traits<wchar_t> : public __char_traits_base<wchar_t, wint_t> {};




__STL_END_NAMESPACE

#endif /*__SGI_STL_CHAR_TRAITS_H*/
