#include "stdafx.h"
#include "SysSecurity.h"

//////////////////////////////////////////////////////////////////////////////////
//
//			CSysSecurity Class
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////
//
//				Constructor
//
//		Clear result, error and state flags
//		Set open state = false
//
//		Extend this method with constructor for
//		extended class.  The code here will execute
//		prior to the new class constructor
//
//////////////////////////////////////////////////////////
CSysSecurity::CSysSecurity()
{
	g_Result = ERROR_SUCCESS;
	g_Error = 0;
	g_SecurityError = SYSSECURITY_NOERROR;

	g_Open = false;

	g_HostName = LocalHostName();
	if (g_HostName.IsEmpty())
		return;
	if (! DetectWindowsNT())
		return;
	g_Open = CurrentUser();
}

//////////////////////////////////////////////////////////
//
//				Destructor
//
//////////////////////////////////////////////////////////
//
//		Extend this method with destructor for
//		extended class.  The code here will execute
//		prior to the new class destructor
//
//////////////////////////////////////////////////////////
CSysSecurity::~CSysSecurity()
{
}

//////////////////////////////////////////////////////////
//
//		DetectWindowsNT
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			true if WindowsNT or Windows 2000.
//      Parameters:
//			None.
//		Remarks:
//			Calls GetVersionEx to fill in g_OSVerInfo,
//			a global OSVERSIONINFO structure.
//
//////////////////////////////////////////////////////////
bool CSysSecurity::DetectWindowsNT()
{
	CString l_ErrorMessage;

	g_Result = 0;
	g_Error = ERROR_SUCCESS;
	g_SecurityError = SYSSECURITY_NOERROR;

	g_OSVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	if (GetVersionEx(&g_OSVerInfo) == 0)
	{
		g_SecurityError = SYSSECURITY_GET_OSVER;
	    g_Result++;
	    g_Error = GetLastError();
		return false;
	}
	
	if (g_OSVerInfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
	{
		g_SecurityError = SYSSECURITY_NOT_WIN32_NT;
	    g_Result++;
	    g_Error = GetLastError();
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////
//
//		CurrentUser
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			true if successful
//      Parameters:
//			None.
//		Remarks
//			g_HostName must be set prior to calling
//			CurrentUser.  Can be NULL to look up on local
//			host.
//
//			Sets g_UserName and g_DomainName.
//			Sets g_UserSID, a PSID structure.
//
//////////////////////////////////////////////////////////
bool CSysSecurity::CurrentUser()
{
	BYTE					l_SIDBuffer[ 4096 ];
	DWORD					l_SIDBufferSize;

	TCHAR					l_UserName[256];
	DWORD					l_SizeUserName;

	TCHAR					l_DomainName[256];
	DWORD					l_SizeDomainName;

	SID_NAME_USE			l_sidTypeSecurityId;

	g_SecurityError = SYSSECURITY_NOERROR;
    g_Result = 0;
    g_Error = ERROR_SUCCESS;

	l_SizeDomainName = sizeof l_DomainName;
	l_SizeUserName	 = sizeof l_UserName;
	l_SIDBufferSize	 = sizeof l_SIDBuffer;

	ZeroMemory(l_UserName, l_SizeUserName);
	ZeroMemory(l_DomainName, l_SizeDomainName);
	ZeroMemory(l_SIDBuffer, l_SIDBufferSize);

	if (GetUserName(l_UserName, &l_SizeUserName) == 0)
	{
		g_SecurityError = SYSSECURITY_GET_USERNAME;
	    g_Result++;
	    g_Error = GetLastError();
		return false;
	}
	
	g_UserName.Format("%s", l_UserName);

	if (LookupAccountName(g_HostName,
						  l_UserName,
						  &l_SIDBuffer,
						  &l_SIDBufferSize,
						  l_DomainName,
						  &l_SizeDomainName,
						  &l_sidTypeSecurityId) == 0)
	{
		g_SecurityError = SYSSECURITY_LOOKUP_ACCOUNT;
	    g_Result++;
	    g_Error = GetLastError();
		return false;
	}
	g_DomainName.Format("%s", l_DomainName);

	if (IsValidSid (PSID(l_SIDBuffer)) == 0)
	{
		g_SecurityError = SYSSECURITY_INVALID_SID;
	    g_Result++;
	    g_Error = GetLastError();
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////
//
//		LocalHostName
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			Local host name if successful. Empty string
//			if unable to get name.
//      Parameters:
//			None.
//		Remarks
//			Sets g_HostName to returned value.
//
//////////////////////////////////////////////////////////
CString CSysSecurity::LocalHostName()
{
	char	l_MachineName [MAX_COMPUTERNAME_LENGTH+1];
	DWORD	l_NameSize = sizeof(l_MachineName);
	
	g_Result = 0;
	g_Error = ERROR_SUCCESS;
	g_SecurityError = SYSSECURITY_NOERROR;

	g_HostName.Empty();
	if (! GetComputerName(l_MachineName, &l_NameSize))
	{
		g_Result++;
		g_Error = GetLastError();
		g_SecurityError = SYSSECURITY_LOCALHOST;
		return g_HostName;
	}
	g_HostName.Format("%s", l_MachineName);

	return g_HostName;
}

//////////////////////////////////////////////////////////
//
//		UserName
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			CString containing g_UserName, set as result 
//			of previous call to CurrentUser
//      Parameters:
//			None.
//		Remarks
//			Must call CurrentUser prior to UserName.
//
//////////////////////////////////////////////////////////
CString CSysSecurity::UserName()
{
	return g_UserName;
}

//////////////////////////////////////////////////////////
//
//		DomainName
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			CString containing g_DomainName, set as result 
//			of previous call to CurrentUser
//      Parameters:
//			None.
//		Remarks
//			Must call CurrentUser prior to DomainName.
//
//////////////////////////////////////////////////////////
CString CSysSecurity::DomainName()
{
	return g_DomainName;
}

//////////////////////////////////////////////////////////
//
//		Result
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			g_Result, set by previous method call.
//      Parameters:
//			None.
//		Remarks
//			Returns result of last system operation which
//			returned a result.
//
//////////////////////////////////////////////////////////
long CSysSecurity::Result()
{
	return g_Result;
}

//////////////////////////////////////////////////////////
//
//		OpenState
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			bool value of g_Open.
//      Parameters:
//			None.
//		Remarks
//			g_Open is true if construction of the class
//			completed successfully.
//
//////////////////////////////////////////////////////////
bool CSysSecurity::OpenState()
{
	return g_Open;
}

//////////////////////////////////////////////////////////
//
//		Error
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			DWORD value stored in g_Error.
//      Parameters:
//			None.
//		Remarks
//			g_Error is set to GetLastError() for operations
//			which support it.
//
//////////////////////////////////////////////////////////
DWORD CSysSecurity::Error()
{
	return g_Error;
}

//////////////////////////////////////////////////////////
//
//		ErrorMessage
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			CString containing system error message corresponding
//			to p_Error
//      Parameters:
//			p_Error
//				DWORD representing value of system error to
//				lookup.
//		Remarks:
//			Returns system error message.
//
//////////////////////////////////////////////////////////
CString CSysSecurity::ErrorMessage(DWORD p_Error)
{
	char		l_ErrorBuffer[4096];
	CString		l_ErrorString;

	memset (l_ErrorBuffer, 0, sizeof(l_ErrorBuffer));
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,
				  p_Error,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  l_ErrorBuffer,
				  sizeof(l_ErrorBuffer),
				  NULL);
	l_ErrorString.Format(l_ErrorBuffer);
	return l_ErrorString;
}

//////////////////////////////////////////////////////////
//
//		SecurityError
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			g_SecurityError set by previous call to a
//			CSysSecurity method.
//      Parameters:
//			None.
//		Remarks
//			Value in g_SecurityError is set by each
//			CSysSecurity method.
//
//////////////////////////////////////////////////////////
DWORD CSysSecurity::SecurityError()
{
	return g_SecurityError;
}

//////////////////////////////////////////////////////////
//
//		SecurityErrorMessage
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			CString containing a SecurityErrorMessage.
//      Parameters:
//			p_Error
//				DWORD containing security error to lookup.
//		Remarks:
//			Returns string representing the error corresponding
//			to p_Error.
//
//////////////////////////////////////////////////////////
CString CSysSecurity::SecurityErrorMessage(DWORD p_Error)
{
	CString		l_ErrorString;

	switch (p_Error)
	{
	case SYSSECURITY_NOERROR:
		l_ErrorString = "No error.";
		break;
	case SYSSECURITY_GET_USERNAME:
		l_ErrorString = "Get user error.";
		break;
	case SYSSECURITY_LOOKUP_ACCOUNT:
		l_ErrorString = "Cannot lookup account.";
		break;
	case SYSSECURITY_INVALID_SID:
		l_ErrorString = "Invalid SID.";
		break;
	case SYSSECURITY_COPY_SID:
		l_ErrorString = "Unable to copy SID.";
		break;
	case SYSSECURITY_GET_OSVER:
		l_ErrorString = "Unable to determine OS version.";
		break;
	case SYSSECURITY_NOT_WIN32_NT:
		l_ErrorString = "OS is not WindowsNT.";
		break;
	case SYSSECURITY_LOCALHOST:
		l_ErrorString = "Unable to determine local host name.";
		break;
	default:
		l_ErrorString = "Unknown error.";
	}
	return l_ErrorString;
}

