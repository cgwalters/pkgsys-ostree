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

#pragma once

#include "pkgsys-ostree-backend.h"

G_BEGIN_DECLS

#define PKGSYS_OSTREE_TYPE_BACKEND_YUM (pkgsys_ostree_backend_yum_get_type ())
#define PKGSYS_OSTREE_BACKEND_YUM(inst) (G_TYPE_CHECK_INSTANCE_CAST ((inst), PKGSYS_OSTREE_TYPE_BACKEND_YUM, PkgsysOstreeBackendYum))
#define PKGSYS_OSTREE_IS_BACKEND_YUM(inst) (G_TYPE_CHECK_INSTANCE_TYPE ((inst), PKGSYS_OSTREE_TYPE_BACKEND_YUM))

typedef struct _PkgsysOstreeBackendYum PkgsysOstreeBackendYum;

GType pkgsys_ostree_backend_yum_get_type (void) G_GNUC_CONST;

PkgsysOstreeBackendYum * pkgsys_ostree_backend_yum_new (void);

G_END_DECLS

