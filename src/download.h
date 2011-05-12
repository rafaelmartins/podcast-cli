/**
 * podcast-cli: A simple podcast client for the command-line interface.
 * Copyright (C) 2011 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <glib.h>
#include "config-parser.h"

enum download_err {
    DOWNLOAD_ERR_MKDIR,
    DOWNLOAD_ERR_SYSTEM,
    DOWNLOAD_ERR_RENAME,
};

static GQuark download_quark(void);
gboolean download_file(gchar* url, podcast_t* podcast, config_t* config, GError** err);

#endif /* DOWNLOAD_H */
