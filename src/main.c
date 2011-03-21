#include <stdio.h>
#include <glib.h>
#include "config-parser.h"


int main(int argc, char** argv){
    GError* err = NULL;
    config_t* conf = load_config(&err);
    if(conf == NULL && err != NULL){
        printf("%s\n", err->message);
    }
    return 0;
}
