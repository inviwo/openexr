///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2005-2012, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//
//	class Semaphore -- implementation for platforms
//	that support Posix threads and Posix semaphores
//
//-----------------------------------------------------------------------------

#include "IlmBaseConfig.h"

#if defined (HAVE_PTHREAD) && defined(HAVE_POSIX_SEMAPHORES)

#include "IlmThreadSemaphore.h"
#include "Iex.h"
#include <assert.h>
#include <errno.h>

ILMTHREAD_INTERNAL_NAMESPACE_SOURCE_ENTER


Semaphore::Semaphore (unsigned int value)
{
    if (::sem_init (&_semaphore, 0, value))
	IEX_NAMESPACE::throwErrnoExc ("Cannot initialize semaphore (%T).");
}


Semaphore::~Semaphore ()
{
#ifdef NDEBUG
    ::sem_destroy (&_semaphore);
#else
    int error = ::sem_destroy (&_semaphore);
    assert (error == 0);
#endif
}


void
Semaphore::wait ()
{
    while( ::sem_wait( &_semaphore ) == -1 && errno == EINTR )
    {
    }
}


bool
Semaphore::tryWait ()
{
    return sem_trywait (&_semaphore) == 0;
}


void
Semaphore::post ()
{
    if (::sem_post (&_semaphore))
        IEX_NAMESPACE::throwErrnoExc ("Post operation on semaphore failed (%T).");
}


int
Semaphore::value () const
{
    int value;

    if (::sem_getvalue (&_semaphore, &value))
        IEX_NAMESPACE::throwErrnoExc ("Cannot read semaphore value (%T).");

    return value;
}


ILMTHREAD_INTERNAL_NAMESPACE_SOURCE_EXIT

#endif
