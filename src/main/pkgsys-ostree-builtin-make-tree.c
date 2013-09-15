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

#include <ostree.h>

#include "pkgsys-ostree-builtins.h"

#include "libgsystem.h"

static GOptionEntry options[] = {
  { NULL }
};

gboolean
pkgsys_ostree_builtin_make_tree (int argc, char **argv, GFile *repo_path, GCancellable *cancellable, GError **error)
{
  GOptionContext *context;
  gboolean ret = FALSE;
  const char *ref;
  gs_unref_object OstreeRepo *repo = NULL;
  gs_free char *resolved_rev = NULL;

  context = g_option_context_new ("REF [PACKAGENAME...] - Construct an OSTree commit from packages");
  g_option_context_add_main_entries (context, options, NULL);

  if (!g_option_context_parse (context, &argc, &argv, error))
    goto out;

  repo = ostree_repo_new (repo_path);
  /* if (!ostree_repo_check (repo, error)) */
  /*   goto out; */

  /* if (argc <= 2) */
  /*   { */
  /*     g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED, */
  /*                  "A REF argument and at least one package is required"); */
  /*     goto out; */
  /*   } */
  /* ref = argv[1]; */
 
  ret = TRUE;
 out:
  if (context)
    g_option_context_free (context);
  return ret;
}
