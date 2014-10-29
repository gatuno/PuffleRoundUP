gamedatadir = $(pkgdatadir)/data

nobase_dist_gamedata_DATA = images/fondo.png \
	images/intro.png \
	images/puffle-intro.png \
	images/title-cage.png \
	images/puffle-azul.png \
	images/puffle-rosa.png \
	images/puffle-verde.png \
	images/puffle-negro.png \
	images/puffle-morado.png \
	images/puffle-rojo.png \
	images/puffle-amarillo.png \
	images/puffle-blanco.png \
	images/puffle-naranja.png \
	images/puffle-cafe.png \
	images/puffle-pen.png \
	images/wall.png \
	images/reloj.png \
	images/boton-close-over.png \
	images/boton-close-up.png \
	images/boton-close-down.png \
	ccfacefront.ttf

# Instalar los archivos .desktop e iconos
applicationsdir = $(datadir)/applications
applications_DATA = desktop/roundup.desktop

icons16dir = $(datadir)/icons/hicolor/16x16/apps
icons16_DATA = desktop/16x16/roundup.png

icons24dir = $(datadir)/icons/hicolor/24x24/apps
icons24_DATA = desktop/24x24/roundup.png

icons32dir = $(datadir)/icons/hicolor/32x32/apps
icons32_DATA = desktop/32x32/roundup.png

icons48dir = $(datadir)/icons/hicolor/48x48/apps
icons48_DATA = desktop/48x48/roundup.png

icons64dir = $(datadir)/icons/hicolor/64x64/apps
icons64_DATA = desktop/64x64/roundup.png

icons128dir = $(datadir)/icons/hicolor/128x128/apps
icons128_DATA = desktop/128x128/roundup.png

icons256dir = $(datadir)/icons/hicolor/256x256/apps
icons256_DATA = desktop/256x256/roundup.png

# touch the toplevel icon directory as required by the icon theme spec
install-data-hook:
	touch $(DESTDIR)$(datadir)/icons/hicolor

EXTRA_DIST = \
	desktop/roundup.desktop	\
	desktop/16x16/roundup.png		\
	desktop/24x24/roundup.png		\
	desktop/32x32/roundup.png		\
	desktop/48x48/roundup.png		\
	desktop/64x64/roundup.png		\
	desktop/128x128/roundup.png		\
	desktop/256x256/roundup.png
