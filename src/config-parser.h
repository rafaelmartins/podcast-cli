/**
 * podcast-cli: A simple podcast client for the command-line interface.
 * Copyright (C) 2011 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <glib.h>

typedef struct{
    gchar* download_command;
    gchar* player_command;
    gchar* media_directory;
} main_config_t;

typedef struct{
    gchar* id;
    gchar* feed_url;
} podcast_t;

typedef struct{
    main_config_t* main_config;
    GArray* podcasts;
} config_t;

config_t* load_config(GError** err);

#endif /* CONFIG_PARSER_H */
