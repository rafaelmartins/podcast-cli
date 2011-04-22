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
#include <stdlib.h>
#include "config-parser.h"


static GQuark config_quark(void){
    return g_quark_from_static_string("config-parser");
}

config_t* load_config(GError** err){
    
    GKeyFile *k = g_key_file_new();
    
    // load config file
    gchar* config_file = g_build_filename(g_get_home_dir(), ".podcastrc", NULL);
    if(!g_key_file_load_from_file(k, config_file, G_KEY_FILE_NONE, err)){
        g_free(config_file);
        return NULL;
    }
    g_free(config_file);
    
    // initialize config structures
    config_t* config = (config_t*) malloc(sizeof(config_t));
    if(config == NULL){
        g_set_error(err, config_quark(), CONFIG_ERR_MEMORY_ALLOC,
            "malloc: Failed to alloc memory for read the config_t* structure.\n");
        return NULL;
    }
    config->main_config = (main_config_t*) malloc(sizeof(main_config_t));
    if(config->main_config == NULL){
        g_set_error(err, config_quark(), CONFIG_ERR_MEMORY_ALLOC,
            "malloc: Failed to alloc memory for read the main_config_t* structure.\n");
        return NULL;
    }
    
    // storage variables
    gsize num_keys;
    gchar **keys;
    gchar *tmp = NULL;
    
    // load main config parameters
    if(!g_key_file_has_group(k, "main")){
        g_set_error(err, config_quark(), CONFIG_ERR_NO_MAIN,
            "g_key_file_has_group: Your configuration file should have a [main] section.\n");
        return NULL;
    }
    keys = g_key_file_get_keys(k, "main", &num_keys, err);
    for(int i=0; i<num_keys; i++){
        tmp = g_key_file_get_value(k, "main", keys[i], err);
        if(*err != NULL){
            return NULL;
        }
        if(g_strcmp0(keys[i], "download_command") == 0){
            config->main_config->download_command = g_strdup(tmp);
            g_free(tmp);
        }
        else if(g_strcmp0(keys[i], "player_command") == 0){
            config->main_config->player_command = g_strdup(tmp);
            g_free(tmp);
        }
        else if(g_strcmp0(keys[i], "media_directory") == 0){
            config->main_config->media_directory = g_strdup(tmp);
            g_free(tmp);
        }
        else{
            g_set_error(err, config_quark(), CONFIG_INVALID_PARAM,
                "Invalid configuration parameter: %s.\n", keys[i]);
            g_free(tmp);
            return NULL;
        }
    }
    g_strfreev(keys);
    
    // load feed urls
    if(!g_key_file_has_group(k, "podcast")){
        g_set_error(err, config_quark(), CONFIG_ERR_NO_PODCAST,
            "g_key_file_has_group: Your configuration file should have a [podcast] section.\n");
        return NULL;
    }
    config->podcasts = NULL;
    keys = g_key_file_get_keys(k, "podcast", &num_keys, err);
    for(config->pod_len = 0; config->pod_len < num_keys; config->pod_len++){
        tmp = g_key_file_get_value(k, "podcast", keys[config->pod_len], err);
        if(*err != NULL){
            return NULL;
        }
        config->podcasts = (podcast_t**) realloc(
            config->podcasts, (config->pod_len + 1) * sizeof(podcast_t*)
        );
        if(config->podcasts == NULL){
            g_set_error(err, config_quark(), CONFIG_ERR_MEMORY_ALLOC,
                "malloc: Failed to alloc memory for read the podcast_t** structure.\n");
            return NULL;
        }
        config->podcasts[config->pod_len] = (podcast_t*) malloc(sizeof(podcast_t));
        if(config->podcasts[config->pod_len] == NULL){
            g_set_error(err, config_quark(), CONFIG_ERR_MEMORY_ALLOC,
                "malloc: Failed to alloc memory for read the podcast_t* structure.\n");
            return NULL;
        }
        config->podcasts[config->pod_len]->id = g_strdup(keys[config->pod_len]);
        config->podcasts[config->pod_len]->feed_url = g_strdup(tmp);
        g_free(tmp);
    }
    g_strfreev(keys);
        
    g_key_file_free(k);
    return config;
}

void free_config(config_t* config){
    g_free(config->main_config->download_command);
    g_free(config->main_config->media_directory);
    g_free(config->main_config->player_command);
    free(config->main_config);
    for(int i=0; i<config->pod_len; i++){
        g_free(config->podcasts[i]->id);
        g_free(config->podcasts[i]->feed_url);
        free(config->podcasts[i]);
    }
    free(config->podcasts);
    free(config);
}
