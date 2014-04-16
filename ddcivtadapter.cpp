/////////////////////////////////////////////////////////////////////////////
//
// @file ddcivtadapter.cpp @brief Digital Devices Common Interface plugin for VDR.
//
// Copyright (c) 2013 - 2014 by Jasmin Jessich.  All Rights Reserved.
//
// Contributor(s):
//
// License: GPLv2
//
// This file is part of vdr_plugin_ddci2.
//
// vdr_plugin_ddci2 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// vdr_plugin_ddci2 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with vdr_plugin_ddci2.  If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////

#include "ddcivtadapter.h"
#include "ddcivtcamslot.h"
#include "logging.h"

#include <vdr/device.h>

//------------------------------------------------------------------------

void DdCiVtAdapter::CleanUp()
{
	LOG_FUNCTION_ENTER;

	LOG_FUNCTION_EXIT;
}

//------------------------------------------------------------------------

DdCiVtAdapter::DdCiVtAdapter( DdCiCamCtrl &theCamCtrl )
: camCtrl( theCamCtrl )
{
	LOG_FUNCTION_ENTER;

	// SetDescription( "DDCI adapter on device %d (%s)", device->DeviceNumber(), *caDevName );

	for (int i = 0; i < MAXDEVICES; i++) {
		/* allCamSlots[ i ] = */
		// Note: the new CAM slot is stored within the base class
		new DdCiVtCamSlot( *this, theCamCtrl );
	}

	LOG_FUNCTION_EXIT;
}

//------------------------------------------------------------------------

DdCiVtAdapter::~DdCiVtAdapter()
{
	LOG_FUNCTION_ENTER;

	// Cancel( 3 );
	CleanUp();

	/** the CAM slots in array camSlots get deleted by the adapter base class, but this
	 * may take some time, so we inform the slots about the deletion of the adapter and
	 * that further calls to adapter functions may fail.
	 */
	int iter = 0;
	while (DdCiVtCamSlot *s = static_cast <DdCiVtCamSlot *>( cCiAdapter::ItCamSlot( iter ) )) {
		s->ShutDown();
	}

	LOG_FUNCTION_EXIT;
}

//------------------------------------------------------------------------

void DdCiVtAdapter::Action()
{
	LOG_FUNCTION_ENTER;

	L_ERROR_STR( "Thread is NOT used, here is something really bad!!!" );

	LOG_FUNCTION_EXIT;
}

//------------------------------------------------------------------------

int DdCiVtAdapter::Read( uint8_t *Buffer, int MaxLength )
{
	return 0;
}

//------------------------------------------------------------------------

void DdCiVtAdapter::Write( const uint8_t *Buffer, int Length )
{
}

//------------------------------------------------------------------------

bool DdCiVtAdapter::Reset( int Slot )
{
	return true;
}

//------------------------------------------------------------------------

eModuleStatus DdCiVtAdapter::ModuleStatus( int Slot )
{
	return msNone;
}

//------------------------------------------------------------------------

bool DdCiVtAdapter::Assign( cDevice *Device, bool Query )
{
	return true;
}
