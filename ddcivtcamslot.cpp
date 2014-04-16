/////////////////////////////////////////////////////////////////////////////
//
// @file ddcivtcamslot.cpp @brief Digital Devices Common Interface plugin for VDR.
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

#include "ddcivtcamslot.h"
#include "ddcivtadapter.h"
#include "logging.h"

//------------------------------------------------------------------------

void DdCiVtCamSlot::StopIt()
{
	active = false;
	rBuffer.Clear();
	delivered = false;
}

//------------------------------------------------------------------------

DdCiVtCamSlot::DdCiVtCamSlot( DdCiVtAdapter &adapter, DdCiCamCtrl &theCamCtrl )
: cCamSlot( &adapter, true )
, camCtrl( theCamCtrl )
, delivered( false )
, active( false )
, shutDown( false )
{
	LOG_FUNCTION_ENTER;
	LOG_FUNCTION_EXIT;
}

//------------------------------------------------------------------------

DdCiVtCamSlot::~DdCiVtCamSlot()
{
	LOG_FUNCTION_ENTER;
	LOG_FUNCTION_EXIT;
}

//------------------------------------------------------------------------

bool DdCiVtCamSlot::Reset()
{
	LOG_FUNCTION_ENTER;

	L_FUNC_NAME();

	bool ret = true;
	if (!shutDown) {
		ret = cCamSlot::Reset();
		if (ret)
			StopIt();
	}

	LOG_FUNCTION_EXIT;

	return ret;
}

//------------------------------------------------------------------------

void DdCiVtCamSlot::StartDecrypting()
{
	LOG_FUNCTION_ENTER;

	L_FUNC_NAME();

	if (!shutDown) {
		StopIt();
		active = true;
		cCamSlot::StartDecrypting();
	}

	LOG_FUNCTION_EXIT;
}

//------------------------------------------------------------------------

void DdCiVtCamSlot::StopDecrypting()
{
	LOG_FUNCTION_ENTER;

	L_FUNC_NAME();

	if (!shutDown) {
		cCamSlot::StopDecrypting();
		StopIt();
	}

	LOG_FUNCTION_EXIT;
}

//------------------------------------------------------------------------

uchar *DdCiVtCamSlot::Decrypt( uchar *Data, int &Count )
{
	if (!active || shutDown) {
		L_ERR_LINE( "Decrypt in deactivated/shutdown state ?!?" );
		Count = 0;
		return 0;
	}

	/*
	 *  WRITE
	 */

	/* It would be possible to store more of the given data, but this did
	 * not work during my tests. So we need to write frame by frame to the
	 * send buffer.
	 */
	// int cnt = Count - (Count % TS_SIZE);  // we write only whole TS frames
	int cnt = TS_SIZE;
	// int stored = ciSend.Write( Data, cnt );
	// Count = stored;
	Count = cnt;

	/*
	 * READ
	 */

	/* Decrypt is called for each frame and we need to return the decoded
	 * frame. But there is no "I_have_the_frame_consumed" function, so the
	 * only chance we have is to delete now the last sent frame from the
	 * buffer.
	 */
	if (delivered) {
		rBuffer.Del( TS_SIZE );
		delivered = false;
	}

	cnt = 0;
	uchar *data = rBuffer.Get( cnt );
	if (!data || (cnt < TS_SIZE)) {
		data = 0;
	}
	else
		delivered = true;

	return data;
}

//------------------------------------------------------------------------

int DdCiVtCamSlot::DataRecv( uchar *data )
{
	if (!active || shutDown) {
		return 0;
	}

	int written = -1;     // default, try again

	int free = rBuffer.Free();
	if (free >= TS_SIZE) {
		free = TS_SIZE;
		written = rBuffer.Put( data, free );
		if (written != free)
			L_ERR_LINE( "Couldn't write previously checked free data ?!?" );
		written = 0;
	}

	return written;
}

//------------------------------------------------------------------------

void DdCiVtCamSlot::ShutDown()
{
	shutDown = true;
}
