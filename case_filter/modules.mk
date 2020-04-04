mod_case_filter.la: mod_case_filter.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_case_filter.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_case_filter.la
