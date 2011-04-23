bin_PROGRAMS = podcast-cli

podcast_cli_SOURCES = \
	config-parser.c \
	config-parser.h \
	rss.c \
	rss.h \
	main.c

podcast_cli_CFLAGS = \
	$(AM_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(MRSS_CFLAGS)

podcast_cli_LDADD = \
	$(AM_LDADD) \
	$(GLIB_LIBS) \
	$(MRSS_LIBS)
