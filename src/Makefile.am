# Automake file for Puffle Round UP

gamedatadir = $(pkgdatadir)/data

bin_PROGRAMS = puffle-roundup
puffle_roundup_SOURCES = roundup.c \
	cp-button.c cp-button.h \
	draw-text.c draw-text.h \
	path.c path.h \
	gettext.h

if MINGW32
puffle_roundup_SOURCES += strsep.c strsep.h
endif

if MACOSX
puffle_roundup_SOURCES += SDLMain.m SDLMain.h
endif

EXTRA_DIST = roundup.rc SDLMain.m SDLMain.h

if MINGW32
roundup_ico.o: roundup.rc ../data/roundup.ico
	$(WINDRES) $(srcdir)/roundup.rc -O coff -o roundup_ico.o

mingw_ldadd = roundup_ico.o

else # NOT MINGW
mingw_ldadd =
endif

puffle_roundup_CPPFLAGS = -DGAMEDATA_DIR=\"$(gamedatadir)/\" -DLOCALEDIR=\"$(localedir)\" $(AM_CPPFLAGS)
puffle_roundup_CFLAGS = $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS) $(SDL_ttf_CFLAGS) $(AM_CFLAGS)
if MACOSX
# En MAC OS X, hay que ligar/compilar contra los frameworks
puffle_roundup_LDFLAGS = -Wl,-rpath,@loader_path/../Frameworks $(AM_LDFLAGS)
else
puffle_roundup_LDADD = $(SDL_LIBS) $(SDL_image_LIBS) $(SDL_mixer_LIBS) $(SDL_ttf_LIBS) -lm $(mingw_ldadd)
endif
LDADD = $(LIBINTL)

#------------------ Packaging rules for Mac OSX ------------------------

bundle_root = $(top_builddir)/etc/macfiles
bundle_name = $(bundle_root)/PuffleRoundUP.app
# Special target to copy all needed game files to PuffleRoundUP.app/Contents/Resources.
# For Mac OS X bundle.
.PHONY: bundle
bundle:
	-rm -rf $(bundle_root)
	mkdir -p $(bundle_name)/Contents/MacOS
	mkdir -p $(bundle_name)/Contents/Resources/data/
	echo "APPL????" > $(bundle_name)/Contents/PkgInfo
	cp $(top_builddir)/etc/Info.plist $(bundle_name)/Contents/
	cp $(top_builddir)/etc/roundup.icns $(bundle_name)/Contents/Resources/
	mkdir -p $(bundle_name)/Contents/Resources/data/images
	cp -R $(top_builddir)/data/images/* $(bundle_name)/Contents/Resources/data/images
	mkdir -p $(bundle_name)/Contents/Resources/data/music
	cp $(top_builddir)/data/music/* $(bundle_name)/Contents/Resources/data/music
	mkdir -p $(bundle_name)/Contents/Resources/data/sounds
	cp $(top_builddir)/data/sounds/* $(bundle_name)/Contents/Resources/data/sounds
	cp $(top_builddir)/data/*.ttf $(bundle_name)/Contents/Resources/data
	for cat in $(ALL_LINGUAS); do \
	  dir=$(bundle_name)/Contents/Resources/locale/$$cat/LC_MESSAGES; \
	  mkdir -p $$dir; \
	  echo "installing $$cat as $$dir/puffle-roundup.mo"; \
	  cp $(top_builddir)/po/$$cat.gmo $$dir/puffle-roundup.mo; \
	done

# Special target to copy the binary to PuffleRoundUP.app/Contents/MacOS.
.PHONY: bundle-fw
bundle-fw: bundle
	cp puffle-roundup $(bundle_name)/Contents/MacOS/puffle-roundup
	mkdir  $(bundle_name)/Contents/Frameworks
	for i in SDL SDL_mixer SDL_image SDL_ttf; do \
	  cp -a /Library/Frameworks/$${i}.framework $(bundle_name)/Contents/Frameworks/; \
	done
	strip $(bundle_name)/Contents/MacOS/puffle-roundup

# Special target to make a disk image of the bundled app
.PHONY: bundle-dmg
bundle-dmg: bundle-fw
	ln -s /Applications $(bundle_root)/Applications
	hdiutil create  -ov -srcfolder $(bundle_root) -volname PuffleRoundUP -imagekey zlib-level=6 $(top_builddir)/etc/roundup.dmg

