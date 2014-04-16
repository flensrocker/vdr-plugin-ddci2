/////////////////////////////////////////////////////////////////////////////
//
// @file ddcivtadapter.h @brief Digital Devices Common Interface plugin for VDR.
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

#ifndef __DDCIVTADAPTER_H
#define __DDCIVTADAPTER_H

#include <vdr/ci.h>
#include <vdr/device.h>

// forward declarations
class DdCiVtCamSlot;
class DdCiCamCtrl;

/**
 * This class implements the virtual CAM interface to VDR.
 * It simulates a CAM Adapter with a slot for each device. This is required,
 * because the VDR expects for each device a dedicated CAM slot.
 */
class DdCiVtAdapter: public cCiAdapter
{
private:
	DdCiCamCtrl &camCtrl; //< the DD CI CAM control instance
	// DdCiVtCamSlot *allCamSlots[ MAXDEVICES ];

	void CleanUp();

protected:
	/* see file ci.h in the VDR include directory for the description of
	 * the following functions
	 */
	virtual void Action();
	virtual int Read( uint8_t *Buffer, int MaxLength );
	virtual void Write( const uint8_t *Buffer, int Length );
	virtual bool Reset( int Slot );
	virtual eModuleStatus ModuleStatus( int Slot );
	virtual bool Assign( cDevice *Device, bool Query = false );

public:
	/**
	 * Constructor.
	 * Creates for each VDR device a CAM slot.
	 * @param theCamCtrl the CAM control instance
	 **/
	DdCiVtAdapter( DdCiCamCtrl &theCamCtrl );

	/// Destructor.
	virtual ~DdCiVtAdapter();
};

#endif //__DDCIVTADAPTER_H
