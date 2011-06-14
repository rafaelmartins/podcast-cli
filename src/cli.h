/**
 * podcast-cli: A simple podcast client for the command-line interface.
 * Copyright (C) 2011 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2.
 * See the file COPYING.
 */

#ifndef CLI_H
#define CLI_H

#include <glib.h>

int cli_main(int argc, char *argv[], GError** err);

#endif /* CLI_H */
