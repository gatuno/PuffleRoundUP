# Automake file for Puffle Round UP

gamedatadir = $(pkgdatadir)/data

bin_PROGRAMS = puffle-roundup
puffle_roundup_SOURCES = roundup.c
#EXTRA_DIST =

puffle_roundup_CPPFLAGS = -DGAMEDATA_DIR=\"$(gamedatadir)/\" -DLOCALEDIR=\"$(localedir)\" $(AM_CPPFLAGS)
puffle_roundup_CFLAGS = $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS) $(SDL_ttf_LIBS) $(AM_CFLAGS)
puffle_roundup_LDADD = $(SDL_LIBS) $(SDL_image_LIBS) $(SDL_mixer_LIBS) $(SDL_ttf_LIBS)

