PKGNAME = r7db
PKGVERSION = 1.0-1

#-----------
DEBFILE = "$(PKGNAME)-$(PKGVERSION).deb"
TMPDIR  = "/tmp/$(USER)"
PKGDIR  = "$(TMPDIR)/$(PKGNAME)"

all: prepare package

prepare:
	mkdir -p $(PKGDIR)/DEBIAN
	mkdir -p $(PKGDIR)/usr/share/robot7/schema

	# Set ownership/permissions; use install
	cp DEBIAN/* $(PKGDIR)/DEBIAN/
	cp ../../db/sqlite/r7db-sqlite.schema $(PKGDIR)/usr/share/robot7/schema/
	cp ../../db/sqlite/r7db-sqlite.data $(PKGDIR)/usr/share/robot7/schema/

package:
	dpkg-deb --build $(PKGDIR) $(DEBFILE)

clean:
	rm -rf $(TMPDIR)
	rm -f $(DEBFILE)

