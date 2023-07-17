#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT()

AC_LANG(C++)

AC_CHECK_LIB(SDL2, SDL_Init, [HAVE_SDL2=TRUE] ,[HAVE_SDL2=FALSE])
if test "$HAVE_SDL2" == TRUE; then
  AC_MSG_RESULT([SDL2 library found.])
else
  AC_MSG_RESULT([SDL2 library not found. Please install SDl2 library.])
fi

AC_OUTPUT(src/Makevars)
