/*********************************************************\
 *  File: SemaphoreLinux.cpp                             *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/System/SemaphoreLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SemaphoreLinux::SemaphoreLinux(uint32 nValue, uint32 nMaxValue) :
	m_nMaxValue(nMaxValue)
{
	m_bCreated = sem_init(&m_hSemaphore, 0, nValue) == 0;
	
	// [DEBUG]
#ifdef DEBUG
	if (!m_bCreated)
		fprintf(stderr, "Could not create semaphore\n error code: %d(%s)", errno, strerror(errno));
#endif
}

/**
*  @brief
*    Destructor
*/
SemaphoreLinux::~SemaphoreLinux()
{
	// Destroy system semaphore
	if (m_bCreated)
		sem_destroy(&m_hSemaphore);
}


//[-------------------------------------------------------]
//[ Private virtual SemaphoreImpl functions               ]
//[-------------------------------------------------------]
bool SemaphoreLinux::Lock()
{
	// Lock semaphore
	return (m_bCreated && sem_wait(&m_hSemaphore) == 0);
}

bool SemaphoreLinux::TryLock(uint64 nTimeout)
{
	// Setup control structure
	struct timespec timeout;
	timeout.tv_sec  = nTimeout/1000;
	timeout.tv_nsec = (nTimeout-timeout.tv_sec)*1000;

	// Lock semaphore
	return (m_bCreated && sem_timedwait(&m_hSemaphore, &timeout) == 0);
}

bool SemaphoreLinux::Unlock()
{
	// Get the current value of the semaphore
	int32 sem_val;
	int result = sem_getvalue(&m_hSemaphore, &sem_val);

	// Is the value -1 or greater or equal to the maximum value?
	// Note: If the value is greater or equal to the maximum value, then no unlock is needed because
	// the semaphore is already unlocked.
	if (result == 0 && sem_val >= 0 && m_nMaxValue > static_cast<uint32>(sem_val)) {
		// Release semaphore
		if (m_bCreated && sem_post(&m_hSemaphore) == 0)
			return true; // Success
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
