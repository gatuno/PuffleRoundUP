# Automake file for Puffle Round UP

gamedatadir = $(pkgdatadir)/data

bin_PROGRAMS = puffle-roundup
puffle_roundup_SOURCES = roundup.c cp-button.c draw-text.c gettext.h

if MINGW32
puffle_roundup_SOURCES += strsep.c
endif

EXTRA_DIST = cp-button.h draw-text.h

if MINGW32
roundup_ico.o: roundup.rc ../data/roundup.ico
	$(WINDRES) $(srcdir)/roundup.rc -O coff -o roundup_ico.o

mingw_ldadd = roundup_ico.o

else # NOT MINGW
mingw_ldadd =
endif

puffle_roundup_CPPFLAGS = -DGAMEDATA_DIR=\"$(gamedatadir)/\" -DLOCALEDIR=\"$(localedir)\" $(AM_CPPFLAGS)
puffle_roundup_CFLAGS = $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS) $(SDL_ttf_LIBS) $(AM_CFLAGS)
puffle_roundup_LDADD = $(SDL_LIBS) $(SDL_image_LIBS) $(SDL_mixer_LIBS) $(SDL_ttf_LIBS)  $(mingw_ldadd)
LDADD = $(LIBINTL)
