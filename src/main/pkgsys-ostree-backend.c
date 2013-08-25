/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright (C) 2013 Colin Walters <walters@verbum.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the licence or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"
#include "pkgsys-ostree-backend.h"
#include "libgsystem.h"

G_DEFINE_INTERFACE (PkgsysOstreeBackend, pkgsys_ostree_backend, G_TYPE_OBJECT)

static void
pkgsys_ostree_backend_default_init (PkgsysOstreeBackendInterface *iface)
{
}

gboolean
pkgsys_ostree_backend_fs_tree_from_packages (PkgsysOstreeBackend  *self,
                                             char                **package_names,
                                             GCancellable         *cancellable,
                                             GError              **error)
{
  g_return_val_if_fail (PKGSYS_OSTREE_IS_BACKEND (self), FALSE);

  return PKGSYS_OSTREE_BACKEND_GET_IFACE (self)->fs_tree_from_packages (self, package_names, 
                                                                        cancellable, error);
}
