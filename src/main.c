#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "cli.h"


int main(int argc, char** argv){
    GError* err = NULL;
    
    return cli_main(argc, argv, &err);
}
