mod_basefun.la: mod_basefun.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_basefun.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_basefun.la
