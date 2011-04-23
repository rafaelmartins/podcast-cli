#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "config-parser.h"
#include "rss.h"


int main(int argc, char** argv){
    GError* err = NULL;
    
    config_t* conf = load_config(&err);
    if(conf == NULL && err != NULL){
        printf("%s\n", err->message);
        return EXIT_SUCCESS;
    }
    
    rss_t* rss = load_rss(conf->podcasts[1]->feed_url, &err);
    if(rss == NULL && err != NULL){
        printf("%s\n", err->message);
        return EXIT_SUCCESS;
    }
    
    free_rss(rss);
    free_config(conf);
    
    return EXIT_SUCCESS;
}
