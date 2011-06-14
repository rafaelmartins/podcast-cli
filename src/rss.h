/**
 * podcast-cli: A simple podcast client for the command-line interface.
 * Copyright (C) 2011 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef RSS_H
#define RSS_H

#include <glib.h>

typedef struct {
    gchar* title;
    gchar* url;
    gchar* type;
    gint size;
} chapter_t;

typedef struct {
    chapter_t** chapters;
    gsize chap_len;
} rss_t;

enum rss_err {
    RSS_ERR_CURL,
    RSS_ERR_MRSS,
    RSS_ERR_MEMORY_ALLOC,
};

static GQuark rss_quark(void);
rss_t* load_rss(gchar* feed_url, GError** err);
void free_rss(rss_t* rss);

#endif /* RSS_H */
