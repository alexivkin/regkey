#if !defined(_SysRegKeyService_h__)
#define _SysRegKeyService_h__

//////////////////////////////////////////////////////////////////////////////////

#include "SysRegKey.h"

//////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////
//
//			CServiceRegKey Class - Derived from CSysRegKey
//
//////////////////////////////////////////////////////////////////////////////////
//
//		Written by  Jay Wheeler
//					EarthWalk Software
//					March, 1998.
//
//////////////////////////////////////////////////////////////////////////////////
//
//		CSysRegKeyService is copyright © 1998, 1999, 2000. EarthWalk Software.
//
//		This library is free software; you can redistribute it and/or
//		modify it under the terms of the GNU Lesser General Public
//		License as published by the Free Software Foundation; either
//		version 2.1 of the License, or (at your option) any later version.
//
//		This library is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//		Lesser General Public License for more details.
//
//		You should have received a copy of the GNU Lesser General Public
//		License along with this library; if not, write to the Free Software
//		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//////////////////////////////////////////////////////////////////////////////////

class CServiceRegKey : public CSysRegKey
{

public:
	CServiceRegKey(HKEY p_hKeyParent=NULL,
				CString p_KeyName="SYSTEM\\CurrentControlSet\\Services\\",
				CString p_ServiceName="",
				REGSAM p_samDesired=KEY_ALL_ACCESS);
	virtual ~CServiceRegKey();

	//////////////////////////////////////////////////////////////////////////////
	
public:
	bool			ReLoadData(void);
	bool			StoreData(void);

	//////////////////////////////////////////////////////////////////////////////
	
public:
	CString			g_RunApp;
	CString			g_Param;

protected:
};

//////////////////////////////////////////////////////////////////////////////////
#endif
