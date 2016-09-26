
SUBDIRS = r7db
#SUBDIRS = r7db r7core r7web

CLEANDIRS = $(SUBDIRS:%=clean-%)

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

clean: $(CLEANDIRS)
$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: all clean
