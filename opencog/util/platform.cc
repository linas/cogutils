/*
 * opencog/util/platform.cc
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * Copyright (C) 2008 by OpenCog Foundation
 * All Rights Reserved
 *
 * Written by Gustavo Gama <moshe@metacog.org>
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

#include "platform.h"
#include <stdlib.h>

// ======================================================
#ifdef WIN32_NOT_UNIX

#include <sys/timeb.h>
#include <winsock2.h>
#include <process.h>
#include <math.h>
#include <io.h>

int round(float x)
{
    return ((x -(int)(x)) < 0.5 ) ? (int)x : (int)x + 1;
}

int gettimeofday(struct timeval* tp, void* tzp)
{
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    tp->tv_sec = (long) timebuffer.time;
    tp->tv_usec = timebuffer.millitm * 1000;
    /* 0 indicates that the call succeeded. */
    return 0;
}

void usleep(unsigned useconds)
{
    // Sleep is in milliseconds
    // If 0 is passed to Sleep()
    // It skips rest of thread scheduled time
    // This is the best achievable with Millisecond
    // resolution
    Sleep((int)(useconds / 1000));
}

unsigned sleep(unsigned seconds)
{
    Sleep(seconds * 1000);
    return 0;
}

unsigned long long atoll(const char *str)
{
    unsigned long long la = 0;
    sscanf(str, "%Lu", &la);
    return la;
}

#endif // WIN32_NOT_UNIX

// ==========================================================

#include <stdlib.h>
#include <unistd.h>   // for sbrk(), sysconf()

// Return memory usage per sbrk system call.
size_t opencog::getMemUsage()
{
    static void *old_sbrk = 0;
    void *p = sbrk(0);
    if (old_sbrk == 0 || old_sbrk > p)
    {
        old_sbrk = p;
        return 0;
    }
    size_t diff = (size_t)p - (size_t)old_sbrk;
    return diff;
}

#ifdef __APPLE__
#include <sys/sysctl.h>
#include <sys/types.h>
#include <pthread.h>

uint64_t opencog::getTotalRAM()
{
   int mib[2];
   uint64_t physmem;
   size_t len;

   mib[0] = CTL_HW;
   mib[1] = HW_MEMSIZE;
   len = sizeof(physmem);
   sysctl(mib, 2, &physmem, &len, NULL, 0);
   return physmem;
}

uint64_t opencog::getFreeRAM()
{
    return getTotalRAM() - getMemUsage();
}

void opencog::set_thread_name(const char* name)
{
    pthread_setname_np(name);
}

#else // __APPLE__

// If not Apple, then Linux.
#include <sys/sysinfo.h>
#include <sys/prctl.h>

uint64_t opencog::getTotalRAM()
{
    // return getpagesize() * get_phys_pages();
    return getpagesize() * sysconf(_SC_PHYS_PAGES);
}

uint64_t opencog::getFreeRAM()
{
    // return getpagesize() * get_avphys_pages();
    return getpagesize() * sysconf(_SC_AVPHYS_PAGES);
}

void opencog::set_thread_name(const char* name)
{
    prctl(PR_SET_NAME, name, 0, 0, 0);
}
#endif // __APPLE__
