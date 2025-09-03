/*
 * opencog/util/platform.h
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Moshe Looks <moshe@metacog.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _OPENCOG_PLATFORM_H
#define _OPENCOG_PLATFORM_H

#ifdef WIN32

#pragma warning(disable:4290)

#define strcasecmp _stricmp
#define snprintf _snprintf

#endif // WIN32

#include <stdio.h>
#include <string.h>
#include <string>
#include <stdint.h>

#ifdef WIN32_NOT_UNIX

#define M_PI 3.14159265358979323846

struct timezone {};

int                round(float x);
int                gettimeofday(struct timeval* tp, void* tzp);
void               usleep(unsigned useconds);
unsigned long long atoll(const char *str);
unsigned int       sleep(unsigned seconds);

#endif // ~WIN32_NOT_UNIX

namespace opencog
{
/** \addtogroup grp_cogutil
 *  @{
 */

//! Return the total amount of heap allocated (according to sbrk, on unix).
size_t getMemUsage();

//! Return the total number of bytes of physical RAM installed.
uint64_t getTotalRAM();

//! Return the total number of free bytes available in RAM (excluding OS caches)
uint64_t getFreeRAM();

void set_thread_name(const char* name);

/** @}*/
} // namespace opencog

#endif // _OPENCOG_PLATFORM_H
