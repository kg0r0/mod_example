mod_example.la: mod_example.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_example.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_example.la
