/////////////////////////////////////////////////////////////////////////////
//
// @file ddcivtcamslot.h @brief Digital Devices Common Interface plugin for VDR.
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

#ifndef __DDCIVTCAMSLOT_H
#define __DDCIVTCAMSLOT_H

#include "ddcirecvbuf.h"

#include <vdr/ci.h>

// forward declarations
class DdCiVtAdapter;
class DdCiCamCtrl;

/**
 * This class implements the logical interface to one slot of the CAM device.
 */
class DdCiVtCamSlot: public cCamSlot
{
	DdCiCamCtrl &camCtrl; //< the DD CI CAM control instance
	DdCiRecvBuf rBuffer;  //< the receive buffer
	bool delivered;       //< true, if Decrypt did deliver data at last call
	bool active;          //< true, if this slot does decrypting
	bool shutDown;        //< true, if shut down is in progress

	void StopIt();

public:
	/**
	 * Constructor.
	 * Creates a new virtual CAM slot for the given virtual adapter.
	 * The adapter will take care of deleting the CAM slot, so the caller must
	 * not delete it!
	 * @param adapter the virtual CAM adapter this slot is associated
	 * @param theCamCtrl the CAM control instance
	 **/
	DdCiVtCamSlot( DdCiVtAdapter &adapter, DdCiCamCtrl &theCamCtrl );

	/// Destructor.
	virtual ~DdCiVtCamSlot();


	/* see file ci.h in the VDR include directory for the description of
	 * the following functions
	 */

	/*
	virtual eModuleStatus ModuleStatus(void);
	virtual const char *GetCamName(void);
	virtual bool Ready(void);
	virtual bool HasMMI(void);
	virtual bool HasUserIO(void);
	virtual bool EnterMenu(void);
	virtual cCiMenu *GetMenu(void);
	virtual cCiEnquiry *GetEnquiry(void);
	virtual bool ProvidesCa(const int *CaSystemIds);
	virtual void AddPid(int ProgramNumber, int Pid, int StreamType);
	virtual void SetPid(int Pid, bool Active);
	virtual void AddChannel(const cChannel *Channel);
	virtual bool CanDecrypt(const cChannel *Channel);
	virtual bool IsDecrypting(void);
	*/

	virtual bool Reset(void);
	virtual void StartDecrypting(void);
	virtual void StopDecrypting(void);

	/**
	 * For a detailed description have a look to file ci.h in the VDR include
	 * directory.
	 * This function will copy the given TS packet(s) to the CAM TS send
	 * buffer and return the next decrypted TS packet from the CAM TS
	 * receive buffer.
	 *
	 * @param Data the TS packet(s) to decrypt
	 * @param Count the number of bytes in Data (should be a multiple of
	 *        TS_SIZE). On function return it is set to the number of bytes
	 *        consumed from Data. 0 in case the CAM send buffer is full.
	 * @return A pointer to the first TS packet in the CAM receive buffer, or
	 *        0, if the CAM receive buffer buffer is empty.
	 **/
	virtual uchar *Decrypt(uchar *Data, int &Count);

	/**
	 * Deliver the received CAM TS Data to the CAM slot.
	 * data is always only one TS data packet of size TS_SIZE and it is
	 * guaranteed, that the first byte of data is a TS_SYNC_BYTE.
	 * @param data the received data to
	 * @return 0 .. data delivered
	 *        -1 .. receiver buffer full
	 *        -2 .. no appropriate target found, discard data
	 */
	int DataRecv( uchar *data );

	/**
	 * Due to the SW structure of the VDR base class, the destructor execution
	 * order is a little bit "crazy". To avoid possible problems, this function
	 * gets executed, when the associated adapter is deleted.
	 */
	void ShutDown();
};

#endif //__DDCIVTCAMSLOT_H