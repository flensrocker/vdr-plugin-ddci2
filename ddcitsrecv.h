/////////////////////////////////////////////////////////////////////////////
//
// @file ddcitsrecv.h @brief Digital Devices Common Interface plugin for VDR.
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

#ifndef __DDCITSRECV_H
#define __DDCITSRECV_H

#include "ddcitsrecvdeliver.h"

#include <vdr/thread.h>
#include <vdr/remux.h>   // TS_SIZE, TS_SYNC_BYTE
#include <vdr/ringbuffer.h>

// forward declarations
class DdCiAdapter;

/**
 * This class implements the physical interface to the CAM TS device.
 * It implements a receive buffer and a receiver thread the TS data
 * independent and with big junks.
 */
class DdCiTsRecv: public cThread
{
private:
	DdCiAdapter &adapter;  //< the associated CI adapter
	int fd;                //< .../frontendX/ciX device read file handle
	cString ciDevName;     //< .../frontendX/ciX device path
	cRingBufferLinear rb;  //< the CAM read buffer
	int pkgCntR;           //< package read counter
	int pkgCntW;           //< package write counter
	bool clear;            //< true, when the buffer shall be cleared
	cMutex mtxClear;       //< clearing is not thread save
	DdCiTsRecvDeliver tsdeliver; //< TS Data deliver thread

	static const int BUF_NUM = 2000;
	static const int BUF_MARGIN = TS_SIZE;

	// cRingBufferLinear requires one margin and 1 byte for internal reasons
	static const int BUF_SIZE = (BUF_MARGIN * (BUF_NUM + 1)) + 1;

	void CleanUp();

public:
	/**
	 * Constructor.
	 * Creates a new CAM TS receiver object.
	 * @param adapter the CAM adapter this slot is associated
	 * @param ci_fdr open file handle for the .../frontendX/ciX device
	 * @param devNameCi the name of the device (.../frontendX/ciX)
	 **/
	DdCiTsRecv( DdCiAdapter &the_adapter, int ci_fdr, cString &devNameCi );

	/// Destructor.
	virtual ~DdCiTsRecv();

	bool Start();
	void Cancel( int waitSec = 0 );

	void ClrBuffer();

	void Deliver();

	/**
	 * Waits for data present from the CAM and tries to deliver it.
	 **/
	virtual void Action();
};

#endif //__DDCITSRECV_H
