/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright (C) 2013 Colin Walters <walters@verbum.org>
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
 */

#pragma once

#include <gio/gio.h>

typedef enum {
  PKGSYS_OSTREE_BUILTIN_FLAG_NONE = 0,
  PKGSYS_OSTREE_BUILTIN_FLAG_NO_REPO = 1,
} PkgsysOstreeBuiltinFlags;

typedef struct {
  const char *name;
  gboolean (*fn) (int argc, char **argv, GFile *repo_path, GCancellable *cancellable, GError **error);
  int flags; /* FedoraOstreeBuiltinFlags */
} PkgsysOstreeCommand;

int pkgsys_ostree_main (int    argc, char **argv, PkgsysOstreeCommand  *commands);

int pkgsys_ostree_run (int argc, char **argv, PkgsysOstreeCommand *commands, GError **error);

int pkgsys_ostree_usage (char **argv, PkgsysOstreeCommand *commands, gboolean is_error);
