mod_portal.la: mod_portal.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_portal.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_portal.la
