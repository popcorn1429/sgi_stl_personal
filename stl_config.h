#ifndef __SGI_STL_CONFIG_H_
#define __SGI_STL_CONFIG_H_

//Flags:
// __STL_NO_BOOL: 
//          defined if the compiler doesn't have bool as a builtin type.
// __STL_HAS_WCHAR_T:
//          defined if the compiler has wchar_t as a builtin type.
// __STL_NO_DRAND48:
//          defined if the compiler doesn't have the drand48 function.
// __STL_STATIC_TEMPLATE_MEMBER_BUG:
//          defined if the compiler can't handle static members of template classes.
// __STL_STATIC_CONST_INIT_BUG:
//          defined if the compiler can't handle a constant-initializer 
//          in the declaration of a static const data member of integer type.
// __STL_CLASS_PARTIAL_SPECIALIZATION:
//          defined if the compiler supports partial specialization of template classes.
// __STL_PARTIAL_SPECIALIZATION_SYNTAX:
//          defined if the compiler supports partial specialization syntax for full
//          specialization of class templates.
// __STL_FUNCTION_TMPL_PARTIAL_ORDER:
//          defined if the compiler supports partial ordering of function templates.
// __STL_MEMBER_TEMPLATES:
//          defined if the compiler supports template member functions of classes.
// __STL_MEMBER_TEMPLATE_CLASSES:
//          defined if the compiler supports nested classes that are member templates of other classes.
// __STL_TEMPLATE_FRIENDS:
//          defined if the compiler supports templatized friend declarations.
// __STL_EXPLICIT_FUNCTION_TMPL_ARGS:
//          defined if the compiler supports calling a function template
//          by providing its template arguments explicitly.
// __STL_LIMITED_DEFAULT_TEMPLATES:
//          defined if the compiler is unable to handle default template parameters
//          that depend on previous template parameters.
// __STL_NON_TYPE_TMPL_PARAM_BUG:
//          defined if the compiler has trouble with function
//          template argument deduction for non-type template parameters.
// __SGI_STL_NO_ARROW_OPERATOR:
//          defined if the compiler is unable to support 
//          the -> operator for iterators.
// __STL_DEFAULT_CONSTRUCTOR_BUG:
//          defined if T() does not work properly when T is a builtin type.
// __STL_USE_EXCEPTIONS:
//          defined if the compiler (in the current compilation mode) supports exceptions.
// __STL_USE_NAMESPACES:
//          defined if the compiler has the necessary support for namespaces.
// __STL_NO_EXCEPTION_HEADER:
//          defined if the compiler does not have a standard-conforming header <exception>.
// __STL_NO_BAD_ALLOC:
//          defined if the compiler does not have a <new> header, 
//          or if <new> doesn't contain a bad_alloc class. 
//          If a bal_alloc class exists, it's assumed to be in namespace std.
// __STL_SGI_THREADS:
//          defined if this is being compiled for an SGI IRIX system in 
//          multithreaded mode, using native SGI threads instead of pthreads.
// __STL_WIN32THREADS:
//          defined if this is being compiled on a WIN32 compiler in multithreaded mode.
// __STL_PTHREADS:
//          defined if we should use portable pthreads synchronization.
// __STL_UITHREADS:
//          defined if we should use UI/solaris/UnixWare threads synchronization.
//          UIthreads are similar to pthreads, but are based on an earlier version
//          of the Posix threads standard.
// __STL_LONG_LONG:
//          if the compiler has long long and unsigned long long types.
// __STL_THREADS:
//          defined if thread safety is needed.
// __STL_VOLATILE:
//          defined to be "volatile" if threads are being used, and the empty string otherwise.
// __STL_USE_CONCEPT_CHECKS:
//          defined to enable some extra compile-time error checking
//          to make sure that user-defined template arguments satisfy
//          all of the appropriate requirments. This may result in more
//          comprehensible error messages. It incurs no runtime overhead.
//          This feature requires member templates and partial specialization.
// __STL_NO_USING_CLAUSE_IN_CLASS:
//          The compiler does not handle "using" clauses inside of class definitions.
// __STL_NO_FRIEND_TEMPLATE_CLASS:
//          The compiler does not handle friend declarations where the friend is a template class.
// __STL_NO_FUNCTION_PTR_IN_CLASS_TEMPLATE:
//          The compiler does not support the use of 
//          a function pointer type as the argument for a template.
// __STL_MEMBER_TEMPLATE_KEYWORD:
//          standard C++ requires the template keyword in a few new places.
//          This flag is set for compilers that support this usage.

// User-settable macros that control compilation:
// __STL_USE_SGI_ALLOCATIONS: 
//          if defined, then the STL will use older SGI-style allocations,
//          instead of standard-conforming allocators, even if the compiler 
//          supports all of the language features needed for standard-conforming allocators.
// __STL_NO_NAMESPACES:
//          if defined, don't put the library in namespace std, even if the compiler supports namespaces.
// __STL_NO_RELOPS_NAMESPACE:
//          if defined, don't put the relational operator templates ( > , <= , >= , != ) in namespace std::rel_ops,
//          even if the compiler supports namespaces and partial ordering of function templates.
// __STL_ASSERTIONS: 
//          if defined, then enable runtime checking through the __stl_assert macro.
// _PTHREADS: 
//          if defined, use Posix threads for multithreading support.
// _UITHREADS:
//          if defined, use SCO/Solaris/UI threads for multithreading support.
// _NOTHREADS: 
//          if defined, don't use any multithreading support.
// _STL_NO_CONCEPT_CHECKS:
//          if defined, disables the error checking that we get from __STL_USE_CONCEPT_CHECKS.
// __STL_USE_NEW_IOSTREAMS:
//          if defined, then the STL will use new, standard-conforming iostreams .
//          If not defined, the STL will use old cfront-style iostreams.


// Other macros defined by this file:
// bool, true, and false, if __STL_NO_BOOL is defined.
// typename, as a null macro if it's not already a keyword.
// explicit, as a null macro if it's not already a keyword.
// namespace-related macros: __STD, __STL_BEGIN_NAMESPACE, etc.
// exception-related macros: __STL_TRY, __STL_UNWIND, etc.
// __stl_assert, either as a test or as a null macro, depending on wether or not __STL_ASSERTIONS is defined.


#if defined(_PTHREADS) && !defined(_NOTHREADS)
#   define __STL_PTHREADS
#endif

#if defined(_UITHREADS) && !defined(_PTHREADS) && !defined(_NOTHREADS)
#   define __STL_UITHREADS
#endif

#if defined(__sgi) && !defined(__GNUC__)
#   include <standards.h>
#   if !defined(_BOOL)
#      define __STL_NO_BOOL
#   endif
#   if defined(_MIPS_SIM) && _MIPS_SIM == _ABIO32
#      define __STL_STATIC_CONST_INIT_BUG
#   endif
#   if defined(_WCHAR_T_IS_KEYWORD)
#      define __STL_HAS_WCHAR_T
#   endif
#   if !defined(_TYPENAME_IS_KEYWORD)
#      define __STL_NEED_TYPENAME
#   endif
#   if defined(_PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES)
#      define __STL_CLASS_PARTIAL_SPECIALIZATION
#   endif
#   if (_COMPILER_VERSION >= 730) && defined(_MIPS_SIM) && _MIPS_SIM != _ABIO32
#      define __STL_FUNCTION_TMPL_PARTIAL_ORDER
#   endif
#   if defined(_MEMBER_TEMPLATES)
#      define __STL_MEMBER_TEMPLATES
#      define __STL_TEMPLATE_FRIENDS
#      define __STL_MEMBER_TEMPLATE_CLASSES
#   endif
#   if defined(_MEMBER_TEMPLATE_KEYWORD)
#      define __STL_MEMBER_TEMPLATE_KEYWORD
#   endif
#   if defined(_STANDARD_C_PLUS_PLUS)
#      define __STL_EXPLICIT_FUNCTION_TMPL_ARGS
#   endif
#   if (_COMPILER_VERSION >= 730) && defined(_MIPS_SIM) && _MIPS_SIM != _ABIO32
#      define __STL_MEMBER_TEMPLATE_KEYWORD
#   endif
#   if _COMPILER_VERSION < 720 || (defined(_MIPS_SIM) && _MIPS_SIM == _ABIO32)
#      define __STL_DEFAULT_CONSTRUCTOR_BUG
#   endif
#   if !defined(_EXPLICIT_IS_KEYWORD)
#      define __STL_NEED_EXPLICIT
#   endif
#   if defined(__EXCEPTIONS)
#      define __STL_USE_EXCEPTIONS
#   endif
#   if (COMPILER_VERSION >= 721) && defined(_NAMESPACES)
#      define __STL_HAS_NAMESPACES
#   endif
#   if (COMPILER_VERSION < 721) || \
    !defined(__STL_HAS_NAMESPACES) || defined(__STL_NO_NAMESPACES)
#      define __STL_NO_EXCEPTION_HEADER
#   endif
#   if _COMPILER_VERSION < 730 || !defined(_STANDARD_C_PLUS_PLUS) || \
    !defined(_NAMESPACES)
#      define __STL_NO_BAD_ALLOC
#   endif
#   if !defined(_NOTHREADS) && !defined(__STL_PTHREADS)
#      define __STL_SGI_THREADS
#   endif
#   if defined(_LONGLONG) && defined(_SGIAPI) && _SGIAPI
#      define _STL_LONG_LONG
#   endif
#   if _COMPILER_VERSION >= 730 && defined(_STANDARD_C_PLUS_PLUS)
#      define __STL_USE_NEW_IOSTREAMS
#   endif
#   if _COMPILER_VERSION >= 730 && defined(_STANDARD_C_PLUS_PLUS)
#      define __STL_CAN_THROW_RANGE_ERRORS
#   endif
#   if _COMPILER_VERSION >= 730 && defined(_STANDARD_C_PLUS_PLUS)
#      define __SGI_STL_USE_AUTO_PTR_CONVERSIONS
#   endif
#endif

#if defined(__USLC__)
#   define __STL_HAS_WCHAR_T
#   define __STL_CLASS_PARTIAL_SPECIALIZATION
#   define __STL_PARTIAL_SPECIALIZATION_SYNTAX
#   define __STL_FUNCTION_TMPL_PARTIAL_ORDER
#   define __STL_MEMBER_TEMPLATES
#   define __STL_MEMBER_TEMPLATE_CLASSES
#   define __STL_USE_EXCEPTIONS
#   define __STL_HAS_NAMESPACES
#   define __STL_USE_NAMESPACES
#   define __STL_LONG_LONG
#   if defined(_REENTRANT)
#      define _UITHREADS
#      define __STL_UITHREADS
// use the following defines instead if th UI threads defines 
// when you want to use POSIX threads
//#       define _PTHREADS
//#       define __STL_PTHREADS
#   endif
#endif

#if defined(__GNUC__)
#   if __GNUC__ == 2 && __GNUC_MINOR__ <= 7
#      define __STL_STATIC_TEMPLATE_MEMBER_BUG
#   endif
#   if __GNUC__ < 2
#      define __STL_NEED_TYPENAME
#      define __STL_NEED_EXPLICIT
#   endif
#   if __GNUC__ == 2 && __GNUC_MINOR__ <= 8
#      define __STL_NO_EXCEPTION_HEADER
#      define __STL_NO_BAD_ALLOC
#   endif
#   if __GNUC__ == 2 && __GNUC_MINOR__ >= 8
#      define __STL_CLASS_PARTIAL_SPECIALIZATION
#      define __STL_FUNCTION_TMPL_PARTIAL_ORDER
#      define __STL_EXPLICIT_FUNCTION_TMPL_ARGS
#      define __STL_MEMBER_TEMPLATES
#      define __STL_CAN_THROW_RANGE_ERRORS
#      if __GNUC_MINOR__ >= 9
#         define __STL_MEMBER_TEMPLATE_CLASSES
#         define __STL_TEMPLATE_FRIENDS
#         define __SGI_STL_USE_AUTO_PTR_CONVERSIONS
#         define __STL_HAS_NAMESPACES
#      endif
#   endif
#   define __STL_DEFAULT_CONSTRUCTOR_BUG
#   if defined(__EXCEPTIONS)
#      define __STL_USE_EXCEPTIONS
#   endif
#   if defined(_REENTRANT)
#      define __STL_PTHREADS
#   endif
#   if (__GNUC__ < 2) || (__GNUC__ == 2 && __GNUC_MINOR__ < 95)
#      define __STL_NO_FUNCTION_PTR_IN_CLASS_TEMPLATE
#   endif
#endif

#if defined(__SUNPRO_CC)
#   define __STL_NO_BOOL
#   define __STL_NEED_TYPENAME
#   define __STL_NEED_EXPLICIT
#   define __STL_USE_EXCEPTIONS
#   if defined(_REENTRANT)
#      define __STL_PTHREADS
#   endif
#   define __SGI_STL_NO_ARROW_OPERATOR
#   define __STL_PARTIAL_SPECIALIZATION_SYNTAX
#   define __STL_NO_EXCEPTION_HEADER
#   define __STL_NO_BAD_ALLOC
#endif

#if defined(__COMO__)
#   define __STL_MEMBER_TEMPLATES
#   define __STL_MEMBER_TEMPLATE_CLASSES
#   define __STL_TEMPLATE_FRIENDS
#   define __STL_CLASS_PARTIAL_SPECIALIZATION
#   define __STL_USE_EXCEPTIONS
#   define __STL_HAS_NAMESPACES
#endif

#if defined(__ICL)
#   define __STL_LONG_LONG
#   define __STL_MEMBER_TEMPLATES
#   define __STL_MEMBER_TEMPLATE_CLASSES
#   define __STL_TEMPLATE_FRIENDS
#   define __STL_FUNCTION_TMPL_PARTIAL_ORDER
#   define __STL_CLASS_PARTIAL_SPECIALIZATION
#   define __STL_NO_DRAND48
#   define __STL_HAS_NAMESPACES
#   define __STL_USE_EXCEPTIONS
#   define __STL_MEMBER_TEMPLATE_KEYWORD
#   if defined(_CPPUNWIND)
#      define __STL_USE_EXCEPTIONS
#   endif
#   if defined(_MT)
#      define __STL_WIN32THREADS
#   endif
#endif

#if defined(__MINGW32__)
#   define __STL_NO_DRAND48
#   if defined(_MT)
#      define __STL_WIN32THREADS
#   endif
#endif

#if defined(__CYGWIN__)
#   define __STL_NO_DRAND48
#endif

#if defined(_MSC_VER) && !defined(__ICL) && !defined(__MWERKS__)
#   define __STL_NO_DRAND48
#   define __STL_STATIC_CONST_INIT_BUG
#   define __STL_NEED_TYPENAME
#   define __STL_NO_USING_CLAUSE_IN_CLASS
#   define __STL_NO_FRIEND_TEMPLATE_CLASS
#   if _MSC_VER < 1100
#      define __STL_NEED_EXPLICIT
#      define __STL_NO_BOOL
#      define __STL_NO_BAD_ALLOC
#   endif
#   if _MSC_VER > 1000
#      include <yvals.h>
#      define __STL_DONT_USE_BOOL_TYPEDEF
#   endif
#   define __STL_NON_TYPE_TMPL_PARAM_BUG
#   define __SGI_STL_NO_ARROW_OPERATOR
#   define __STL_DEFAULT_CONSTRUCTOR_BUG
#   if defined(_CPPUNWIND)
#      define __STL_USE_EXCEPTIONS
#   endif
#   if defined(_MT)
#      define __STL_WIN32THREADS
#   endif
#   if _MSC_VER >= 1200
#      define __STL_PARTIAL_SPECIALIZATION_SYNTAX
#      define __STL_HAS_NAMESPACES
#      define __STL_CAN_THROW_RANGE_ERRORS
#      define NOMINMAX
#      undef  min
#      undef  max
//disable warning 'initializers put in unrecognized initialization area'
#      pragma warning (disable : 4075)
//disable warning 'empty controlled statement found'
#      pragma warning (disable : 4390)
//disable warning 'debug symbol greater than 255 chars'
#      pragma warning (disable : 4786)
#   endif
#   if _MSC_VER < 1100
#      define __STL_NO_EXCEPTION_HEADER
#      define __STL_NO_BAD_ALLOC
#   endif
//because of a microsoft front end bug, 
//we must not provide a namespace qualifier when declaring a friend function
#   define __STD_QUALIFIER
#endif

#if defined(__BORLANDC__)
#   define __STL_NO_BAD_ALLOC
#   define __STL_NO_DRAND48
#   define __STL_DEFAULT_CONSTRUCTOR_BUG
#   if __BORLANDC__ >= 0x540
#      define __STL_CLASS_PARTIAL_SPECIALIZATION
#      define __STL_FUNCTION_TMPL_PARTIAL_ORDER
#      define __STL_EXPLICIT_FUNCTION_TMPL_ARGS
#      define __STL_MEMBER_TEMPLATES
#      define __STL_TEMPLATE_FRIENDS
#   else
#      define __STL_NEED_TYPENAME
#      define __STL_LIMITED_DEFAULT_TEMPLATES
#      define __SGI_STL_NO_ARROW_OPERATOR
#      define __STL_NON_TYPE_TMPL_PARAM_BUG
#   endif
#   if defined(_CPPUNWIND)
#      define __STL_USE_EXCEPTIONS
#   endif
#   if defined(__MT__)
#      define __STL_WIN32THREADS
#   endif
#endif

#if defined(__STL_NO_BOOL) && !defined(__STL_DONT_USE_BOOL_TYPEDEF)
    typedef int bool;
#   define true 1
#   define false 0
#endif

#if defined(__STL_NEED_TYPENAME)
#   define typename
#endif

#if defined(__STL_LIMITED_DEFAULT_TEMPLATES)
#   define __STL_DEPENDENT_DEFAULT_TMPL(_Tp)
#else
#   define __STL_DEPENDENT_DEFAULT_TMPL(_Tp) = _Tp
#endif

#if defined(__STL_MEMBER_TEMPLATE_KEYWORD)
#   define __STL_TEMPLATE template
#else
#   define __STL_TEMPLATE
#endif

#if defined(__STL_NEED_EXPLICIT)
#   define explicit
#endif

#if defined(__STL_EXPLICIT_FUNCTION_TMPL_ARGS)
#   define __STL_NULL_TMPL_ARGS <>
#else
#   define __STL_NULL_TMPL_ARGS
#endif

#if defined(__STL_CLASS_PARTIAL_SPECIALIZATION) \
    || defined(__STL_PARTIAL_SPECIALIZATION_SYNTAX)
#   define __STL_TEMPLATE_NULL template<>
#else
#   define __STL_TEMPLATE_NULL
#endif

#if defined(__STL_CLASS_PARTIAL_SPECIALIZATION) && \
    defined(__STL_MEMBER_TEMPLATES) && \
    defined(__STL_MEMBER_TEMPLATE_CLASSES) && \
    !defined(__STL_NO_BOOL) && \
    !defined(__STL_NON_TYPE_TMPL_PARAM_BUG) && \
    !defined(__STL_LIMITED_DEFAULT_TEMPLATES) && \
    !defined(__STL_USE_SGI_ALLOCATORS)
#   define __STL_USE_STD_ALLOCATORS
#endif

#if !defined(__STL_DEFAULT_ALLOCATOR)
#   if defined(__STL_USE_STD_ALLOCATORS)
#      define __STL_DEFAULT_ALLOCATOR(T) allocator< T >
#   else
#      define __STL_DEFAULT_ALLOCATOR(T) alloc
#   endif
#endif

#if defined(__STL_HAS_NAMESPACES) && !defined(__STL_NO_NAMESPACES)
#   define __STL_USE_NAMESPACES
#   define __STD std
#   define __STL_BEGIN_NAMESPACE namespace std {
#   define __STL_END_NAMESPACE }
#   if defined(__STL_FUNCTION_TMPL_PARTIAL_ORDER) && \
    !defined(__STL_NO_RELOPS_NAMESPACE)
#      define __STL_USE_NAMESPACE_FOR_RELOPS
#      define __STL_BEGIN_RELOPS_NAMESPACE namespace std { namespace rel_ops {
#      define __STL_END_RELOPS_NAMESPACE } }
#      define __STD_RELOPS std::rel_ops
#   else
#      define __STL_USE_NAMESPACE_FOR_RELOPS
#      define __STL_BEGIN_RELOPS_NAMESPACE namespace std {
#      define __STL_END_RELOPS_NAMESPACE }
#      define __STD_RELOPS std
#   endif
#else
#   define __STD
#   define __STL_BEGIN_NAMESPACE
#   define __STL_END_NAMESPACE
#	define __STL_USE_NAMESPACE_FOR_RELOPS
#	define __STL_BEGIN_RELOPS_NAMESPACE
#	define __STL_END_RELOPS_NAMESPACE
#   define __STD_RELOPS
#	undef  __STL_USE_NAMESPACES
#endif

#if defined(__STL_USE_NAMESPACES) && !defined(__STD_QUALIFIER)
#	define __STD_QUALIFIER std::
#else
#	define __STD_QUALIFIER
#endif

#if defined(__STL_USE_EXCEPTIONS)
#	define __STL_TRY try
#	define __STL_CATCH_ALL catch(...)
#	define __STL_THROW(x) throw x
#	define __STL_RETHROW throw
#	define __STL_NOTHROW throw()
#	define __STL_UNWIND(action) catch(...) {action; throw;}
#else
#	define __STL_TRY
#	define __STL_CATCH_ALL if (false)
#	define __STL_THROW(x)
#	define __STL_RETHROW
#	define __STL_NOTHROW
#	define __STL_UNWIND(action)
#endif

#if defined(__STL_ASSERTIONS)
#   include <stdio.h>
#	define __stl_assert(expr) \
		if (!(expr)) {fprintf(stderr, "%s:%d STL assertion failure: %s\n", \
                             __FILE__, __LINE__, # expr); abort(); }
#else
#	define __stl_assert(expr)
#endif

#if defined(__STL_WIN32THREADS) || defined(__STL_SGI_THREADS) \
    || defined(__STL_PTHREADS)  || defined(__STL_UITHREADS)
#	define __STL_THREADS
#	define __STL_VOLATILE volatile
#else
#	define __STL_VOLATILE
#endif

#if defined(__STL_CLASS_PARTIAL_SPECIALIZATION) \
    && defined(__STL_MEMBER_TEMPLATES) \
    && !defined(__STL_NO_CONCEPT_CHECKS)
#	define __STL_USE_CONCEPT_CHECKS
#endif

#endif
