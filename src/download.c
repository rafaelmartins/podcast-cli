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

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <glib.h>
#include <glib/gstdio.h>
#include "config-parser.h"
#include "download.h"


static GQuark download_quark(void){
    return g_quark_from_static_string("download");
}

gboolean download_file(gchar* url, podcast_t* podcast, config_t* config, GError** err){
    
    gchar* url_basename = g_path_get_basename(url);
    
    gchar* dest_dir = g_build_filename(config->main_config->media_directory,
        podcast->id, NULL);
    
    if(!g_file_test(dest_dir, G_FILE_TEST_IS_DIR)){
        if(g_mkdir_with_parents(dest_dir, 0755) == -1){
            g_set_error(err, download_quark(), DOWNLOAD_ERR_MKDIR,
                "g_mkdir_with_parents (%d): failed to create dir: %s\n", errno,
                dest_dir);
            return FALSE;
        }
    }
    
    gchar* dest_file = g_build_filename(dest_dir, url_basename, NULL);
    
    gchar* temp_file = g_strconcat(dest_file, ".part", NULL);
    
    gchar* command_line = g_strdup_printf(config->main_config->download_command,
        temp_file, url);
    
    int ret = system(command_line);
    if(ret == -1){
        g_set_error(err, download_quark(), DOWNLOAD_ERR_SYSTEM,
            "system (%d): failed to run command: %s\n", errno, command_line);
        return FALSE;
    }
    else if(ret != EXIT_SUCCESS){
        exit(ret);
    }
    else{
        if(g_rename(temp_file, dest_file) == -1){
            g_set_error(err, download_quark(), DOWNLOAD_ERR_RENAME,
                "g_rename (%d): failed to move downloaded file: %s\n", errno,
                temp_file);
            return FALSE;
        }
    }
    
    g_free(command_line);
    g_free(temp_file);
    g_free(dest_file);
    g_free(dest_dir);
    g_free(url_basename);
    
    return TRUE;
}
