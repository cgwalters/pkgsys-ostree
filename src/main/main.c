/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright (C) 2011,2013 Colin Walters <walters@verbum.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Colin Walters <walters@verbum.org>
 */

#include "config.h"

#include <gio/gio.h>

#include <errno.h>
#include <string.h>

#include "pkgsys-ostree-main.h"
#include "pkgsys-ostree-builtins.h"

static PkgsysOstreeCommand commands[] = {
  { "make-tree", pkgsys_ostree_builtin_make_tree, 0 },
  { NULL }
};

int
main (int    argc,
      char **argv)
{
  GError *error = NULL;
  int ret;

  ret = pkgsys_ostree_run (argc, argv, commands, &error);
  if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED))
    pkgsys_ostree_usage (argv, commands, TRUE);

  if (error != NULL)
    {
      g_message ("%s", error->message);
      g_error_free (error);
    }

  return ret;
}
