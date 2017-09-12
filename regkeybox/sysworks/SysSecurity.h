#if !defined(_SysSecurity_h__)
#define _SysSecurity_h__

//////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////
//
//		CSysSecurity Class
//
//////////////////////////////////////////////////////////////////////////////////
//
//		Written by  Jay Wheeler
//					EarthWalk Software
//
//		Version 1 - October, 2000.
//
//////////////////////////////////////////////////////////////////////////////////
//
//		CSysSecurity is copyright © 2000. EarthWalk Software.
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

#define MAX_SID_BUFFER		4096

//////////////////////////////////////////////////////////////////////////////////

#define SYSSECURITY_NOERROR			0
#define SYSSECURITY_GET_USERNAME	SYSSECURITY_NOERROR + 1
#define SYSSECURITY_LOOKUP_ACCOUNT	SYSSECURITY_GET_USERNAME + 1
#define SYSSECURITY_INVALID_SID		SYSSECURITY_LOOKUP_ACCOUNT + 1
#define	SYSSECURITY_COPY_SID		SYSSECURITY_INVALID_SID + 1
#define SYSSECURITY_GET_OSVER		SYSSECURITY_COPY_SID + 1
#define SYSSECURITY_NOT_WIN32_NT	SYSSECURITY_GET_OSVER + 1
#define	SYSSECURITY_LOCALHOST		SYSSECURITY_NOT_WIN32_NT + 1

//////////////////////////////////////////////////////////////////////////////////

class CSysSecurity : public CObject
{

public:
	CSysSecurity();
	virtual ~CSysSecurity();

	//////////////////////////////////////////////////////////////////////////////

public:
	bool					CurrentUser(void);
	bool					DetectWindowsNT(void);
	CString					LocalHostName(void);

	CString					ErrorMessage(DWORD p_Error);
	long					Result(void);
	DWORD					Error(void);
	bool					OpenState(void);
	CString					DomainName(void);
	CString					UserName(void);

	DWORD					SecurityError(void);
	CString					SecurityErrorMessage(DWORD p_Error);
protected:

	//////////////////////////////////////////////////////////////////////////////
	
public:

protected:
	CString					g_HostName;

	CString					g_UserName;
	CString					g_DomainName;

	long					g_Result;
	bool					g_Open;
	DWORD					g_Error;
	int						g_SecurityError;

	OSVERSIONINFO			g_OSVerInfo;

private:
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#endif
