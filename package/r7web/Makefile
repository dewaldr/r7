PKGNAME 	= r7web
PKGVERSION 	= 1.0-1

#-----------
SRCDIR		= ../../web
INSTDIR		= /usr/share/robot7
INSTUSR		= root
INSTGRP		= root

#-----------
DEBFILE = $(PKGNAME)-$(PKGVERSION).deb
TMPDIR = /tmp/$(USER)
PKGDIR = $(TMPDIR)/$(PKGNAME)
INSTCMD = `find $(SRCDIR) ! -regex ".*[/]\.svn[/]?.*" ! -regex "*~" -type f -print0 -print \
	| sed 's_\(.*\)$(SRCDIR)_install -D \1 $(PKGDIR)$(INSTDIR)_'`

all: prepare package

prepare:
	mkdir -p $(PKGDIR)/DEBIAN
	eval "$(INSTCMD)"
	cp DEBIAN/* $(PKGDIR)/DEBIAN/

package:
	dpkg-deb --build $(PKGDIR) $(DEBFILE)

clean:
	$(RM) -r $(TMPDIR)
	$(RM) $(DEBFILE)
