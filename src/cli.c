/**
 * podcast-cli: A simple podcast client for the command-line interface.
 * Copyright (C) 2011 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>

#include "cli.h"
#include "config-parser.h"
#include "download.h"
#include "rss.h"

config_t* conf = NULL;
rss_t* rss = NULL;
podcast_t* podcast = NULL;

// Global vars for options
gboolean list_podcasts = FALSE;
gchar* list_chapters = NULL;
gchar* get_latest = NULL;

static GOptionEntry entries[] = {
    {"list-podcasts", 0, 0, G_OPTION_ARG_NONE, &list_podcasts, "List available podcasts", NULL},
    {"list-chapters", 'l', 0, G_OPTION_ARG_STRING, &list_chapters, "List chapters available for a podcast", "PODCAST_ID"},
    {"get-latest", 0, 0, G_OPTION_ARG_STRING, &get_latest, "Fetch latest chapter available for a podcast", "PODCAST_ID"},
    /*{ "max-size", 'm', 0, G_OPTION_ARG_INT, &max_size, "Test up to 2^M items", "M" },
    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Be verbose", NULL },
    { "beep", 'b', 0, G_OPTION_ARG_NONE, &beep, "Beep when done", NULL },
    { "rand", 0, 0, G_OPTION_ARG_NONE, &rand, "Randomize the data", NULL },*/
    {NULL}
};


void init_conf(GError** err){
    if(conf == NULL){
        conf = load_config(err);
        if(conf == NULL && *err != NULL){
            g_print("%s\n", (*err)->message);
            exit(EXIT_FAILURE);
        }
    }
}

void init_rss(gchar* podcast_id, GError** err){
    init_conf(err);
    int i;
    for(i=0; i<conf->pod_len; i++){
        if(g_strcmp0(podcast_id, conf->podcasts[i]->id) == 0){
            break;
        }
    }
    if(i >= conf->pod_len){
        g_print("Invalid podcast ID: %s\n", podcast_id);
        exit(EXIT_FAILURE);
    }
    if(rss != NULL){
        free_rss(rss);
    }
    podcast = conf->podcasts[i];
    rss = load_rss(conf->podcasts[i]->feed_url, err);
    if(rss == NULL && *err != NULL){
        g_print("%s\n", (*err)->message);
        exit(EXIT_FAILURE);
    }
}

void free_all(void){
    if(conf != NULL){
        free_config(conf);
    }
    if(rss != NULL){
        free_rss(rss);
    }
}


int cli_main(int argc, char *argv[], GError** err){
    GOptionContext *context = g_option_context_new("- Podcast CLI client.");
    g_option_context_add_main_entries (context, entries, NULL);
    if(!g_option_context_parse (context, &argc, &argv, err)){
        g_print("option parsing failed: %s\n", (*err)->message);
        return EXIT_FAILURE;
    }
    
    if(list_podcasts){
        init_conf(err);
        g_print("Available podcasts:\n\n");
        for(int i=0; i<conf->pod_len; i++){
            g_print("    %s: %s\n", conf->podcasts[i]->id, conf->podcasts[i]->feed_url);
        }
        g_print("\n");
    }
    else if(list_chapters != NULL){
        init_rss(list_chapters, err);
        if(rss->chap_len == 0){
            g_print("No chapters available for this podcast.\n");
        }
        else{
            g_print("Available chapters (%s):\n\n", list_chapters);
            for(int i=0; i<rss->chap_len; i++){
                g_print("    %s (%s, %i)\n",
                    rss->chapters[i]->title,
                    rss->chapters[i]->type,
                    rss->chapters[i]->size);
            }
            g_print("\n");
        }
    }
    else if(get_latest != NULL){
        init_rss(get_latest, err);
        if(rss->chap_len == 0){
            g_print("No chapters available for this podcast.\n");
        }
        else{
            g_print("Downloading (%s): %s\n\n", get_latest, rss->chapters[0]->title);
            if(!download_file(rss->chapters[0]->url, podcast, conf, err)){
                g_print("%s\n", (*err)->message);
                free_all();
                exit(EXIT_FAILURE);
            }
        }
    }
    
    free_all();
    return EXIT_SUCCESS;
}

