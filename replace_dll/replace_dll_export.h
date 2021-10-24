
#ifndef REPLACE_DLL_EXPORT_H
#define REPLACE_DLL_EXPORT_H

#ifdef REPLACE_DLL_STATIC_DEFINE
#  define REPLACE_DLL_EXPORT
#  define REPLACE_DLL_NO_EXPORT
#else
#  ifndef REPLACE_DLL_EXPORT
#    ifdef replace_dll_EXPORTS
        /* We are building this library */
#      define REPLACE_DLL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define REPLACE_DLL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef REPLACE_DLL_NO_EXPORT
#    define REPLACE_DLL_NO_EXPORT 
#  endif
#endif

#ifndef REPLACE_DLL_DEPRECATED
#  define REPLACE_DLL_DEPRECATED __declspec(deprecated)
#endif

#ifndef REPLACE_DLL_DEPRECATED_EXPORT
#  define REPLACE_DLL_DEPRECATED_EXPORT REPLACE_DLL_EXPORT REPLACE_DLL_DEPRECATED
#endif

#ifndef REPLACE_DLL_DEPRECATED_NO_EXPORT
#  define REPLACE_DLL_DEPRECATED_NO_EXPORT REPLACE_DLL_NO_EXPORT REPLACE_DLL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef REPLACE_DLL_NO_DEPRECATED
#    define REPLACE_DLL_NO_DEPRECATED
#  endif
#endif

#endif /* REPLACE_DLL_EXPORT_H */
