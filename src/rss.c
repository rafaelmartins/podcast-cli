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

#include <glib.h>
#include <mrss.h>
#include <stdlib.h>
#include "rss.h"

static GQuark rss_quark(void){
    return g_quark_from_static_string("rss");
}

rss_t* load_rss(gchar* feed_url, GError** err){
    
    mrss_t *feed;
    mrss_error_t ret;
    mrss_item_t *item;
    CURLcode code;
    
    ret = mrss_parse_url_with_options_and_error(feed_url, &feed, NULL, &code);
    if(ret){
        if(ret == MRSS_ERR_DOWNLOAD){
            g_set_error(err, rss_quark(), RSS_ERR_CURL, "mrss_curl_strerror: %s\n",
                mrss_curl_strerror(code));
        }
        else{
            g_set_error(err, rss_quark(), RSS_ERR_MRSS, "mrss_strerror: %s\n",
                mrss_strerror(ret));
        }
        return NULL;
    }
    
    rss_t* rss = (rss_t*) malloc(sizeof(rss_t));
    if(rss == NULL){
        g_set_error(err, rss_quark(), RSS_ERR_MEMORY_ALLOC,
            "malloc: Failed to alloc memory for read the rss_t* structure.\n");
        return NULL;
    }
    
    rss->chapters = NULL;
    for(item = feed->item, rss->chap_len = 0; item != NULL; item = item->next, rss->chap_len++){
        rss->chapters = (chapter_t**) realloc(
            rss->chapters, (rss->chap_len + 1) * sizeof(chapter_t*)
        );
        if(rss->chapters == NULL){
            g_set_error(err, rss_quark(), RSS_ERR_MEMORY_ALLOC,
                "malloc: Failed to alloc memory for read the chapter_t** structure.\n");
            return NULL;
        }
        rss->chapters[rss->chap_len] = (chapter_t*) malloc(sizeof(chapter_t));
        if(rss->chapters[rss->chap_len] == NULL){
            g_set_error(err, rss_quark(), RSS_ERR_MEMORY_ALLOC,
                "malloc: Failed to alloc memory for read the chapter_t* structure.\n");
            return NULL;
        }
                
        rss->chapters[rss->chap_len]->url = g_strdup(item->enclosure_url);
        rss->chapters[rss->chap_len]->type = g_strdup(item->enclosure_type);
        rss->chapters[rss->chap_len]->size = item->enclosure_length;
    }
    
    mrss_free(feed);
    
    return rss;
}

void free_rss(rss_t* rss){
    for(int i=0; i<rss->chap_len; i++){
        g_free(rss->chapters[i]->url);
        g_free(rss->chapters[i]->type);
        free(rss->chapters[i]);
    }
    free(rss->chapters);
    free(rss);
}

