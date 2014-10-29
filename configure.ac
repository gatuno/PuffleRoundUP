AC_INIT([Puffle Round UP], [0.9.2], [], [puffle-roundup])

AC_REVISION([Revision 1])

AC_CONFIG_AUX_DIR([build-aux])

AC_CONFIG_SRCDIR([src/roundup.c])

# For debian systems, /usr as default
AC_PREFIX_DEFAULT([/usr])

# We need Gcc
AC_PROG_CC

# and automake
AM_INIT_AUTOMAKE([-Wall -Werror])

# Translate this program
AM_GNU_GETTEXT_VERSION([0.18.1])
AM_GNU_GETTEXT([external])

AM_PROG_CC_C_O

# Check for pkg-config
PKG_PROG_PKG_CONFIG

SDL_VERSION=1.2.14
SDL_IMAGE_VERSION=1.2.10
SDL_TTF_VERSION=2.0.11

AC_MSG_CHECKING([if you have SDL installed on your system])
PKG_CHECK_EXISTS([sdl >= $SDL_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([SDL not found in your system])])
PKG_CHECK_MODULES(SDL, [sdl >= $SDL_VERSION], [], [])

AC_MSG_CHECKING([if you have SDL_image installed on your system])
PKG_CHECK_EXISTS([SDL_image >= $SDL_IMAGE_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([SDL_image not found in your system])])
PKG_CHECK_MODULES(SDL_image, [SDL_image >= $SDL_IMAGE_VERSION], [], [])

AC_MSG_CHECKING([if you have SDL_ttf installed on your system])
PKG_CHECK_EXISTS([SDL_image >= $SDL_IMAGE_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([SDL_ttf not found in your system])])
PKG_CHECK_MODULES(SDL_ttf, [SDL_ttf >= $SDL_TTF_VERSION], [], [])

# SDL_Mixer doesn't have pkg-config
SDL_mixer_LIBS=""
AC_ARG_VAR(SDL_mixer_LIBS, [linker flags for SDL_mixer])
# AC_MSG_CHECKING([if you have SDL_mixer installed on your system])
AC_CHECK_HEADERS(SDL/SDL_mixer.h, [], [AC_MSG_FAILURE([SDL_mixer not found in your system])])
AC_CHECK_LIB(SDL_mixer, main, [SDL_mixer_LIBS="-lSDL_mixer"], [AC_MSG_FAILURE([SDL_mixer not found in your system])])

AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_HOST

AC_CONFIG_FILES([
                 Makefile
                 src/Makefile
                 data/Makefile
                 po/Makefile.in
])

AC_OUTPUT
