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

#include "pkgsys-ostree-backend-yum.h"
#include "libgsystem.h"

#include <string.h>

struct _PkgsysOstreeBackendYum
{
  GObject       parent_instance;
};

typedef GObjectClass PkgsysOstreeBackendYumClass;

static void pkgsys_ostree_backend_yum_backend_iface_init (PkgsysOstreeBackendInterface *iface);
G_DEFINE_TYPE_WITH_CODE (PkgsysOstreeBackendYum, pkgsys_ostree_backend_yum, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (PKGSYS_OSTREE_TYPE_BACKEND, pkgsys_ostree_backend_yum_backend_iface_init));

static gboolean
pkgsys_ostree_backend_yum_fs_tree_from_packages (PkgsysOstreeBackend   *backend,
                                                 char                 **package_names,
                                                 GCancellable          *cancellable,
                                                 GError               **error)
{
  gboolean ret = FALSE;
  PkgsysOstreeBackendYum *self = PKGSYS_OSTREE_BACKEND_YUM (backend);
  
  ret = TRUE;
 out:
  return ret;
}

static void
pkgsys_ostree_backend_yum_finalize (GObject *object)
{
  PkgsysOstreeBackendYum *self = PKGSYS_OSTREE_BACKEND_YUM (object);

  G_OBJECT_CLASS (pkgsys_ostree_backend_yum_parent_class)->finalize (object);
}

void
pkgsys_ostree_backend_yum_init (PkgsysOstreeBackendYum *self)
{
}

static void
pkgsys_ostree_backend_yum_backend_iface_init (PkgsysOstreeBackendInterface *iface)
{
  iface->fs_tree_from_packages = pkgsys_ostree_backend_yum_fs_tree_from_packages;
}

void
pkgsys_ostree_backend_yum_class_init (PkgsysOstreeBackendYumClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->finalize = pkgsys_ostree_backend_yum_finalize;
}

PkgsysOstreeBackendYum *
pkgsys_ostree_backend_yum_new (void)
{
  return (PkgsysOstreeBackendYum*)g_object_new (PKGSYS_OSTREE_TYPE_BACKEND_YUM, NULL);
}
