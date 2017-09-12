#include "stdafx.h"
#include "SysRegKey.h"

//////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////
//																				//
//			CSysRegKey Class - Derived from CRegKey, base class					//
//																				//
//		CSysRegKey extends the CRegKey MFC class by adding 
//		the following protected methods to manipulate the CRegKey class:
//
//			GetStringData
//			SetStringValue
//			GetOrCreateString
//
//			GetDWORDValue
//			SetDWORDValue
//			GetOrCreateWord
//
//		and the following public methods:
//
//			ReLoadData
//			State
//			OpenState
//			Result
//			Error
//
//		It also implements the following protected variables:
//
//			long g_regResult
//			bool g_regOpen
//			int  g_regError
//			bool g_regState
//
//////////////////////////////////////////////////////////////////////////////////
//
//		Written by  Jay Wheeler
//					EarthWalk Software
//					March, 1998.
//
//		Version 2 - July, 2000.
//
//////////////////////////////////////////////////////////////////////////////////
//
//		CSysRegKey is copyright © 1998, 1999, 2000. EarthWalk Software.
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
CSysRegKey::CSysRegKey(HKEY p_hKeyParent,
					   CString p_KeyName,
					   REGSAM p_samDesired)
{
	g_regResult = ERROR_SUCCESS;
	g_regError = 0;
	g_regState = false;
	g_regOpen = false;
	
	g_SubKeys = 0;
    g_MaxSubKeyNameLen = 0;

	g_hKeyParent = NULL;
	g_KeyName = "";
	g_sam=KEY_ALL_ACCESS;
	g_Options=REG_OPTION_NON_VOLATILE;

	g_regCharValueSize = sizeof g_regCharValue;
	memset(g_regCharValue, 0, g_regCharValueSize);
	g_regState = true;
	if ((p_hKeyParent == NULL) ||
		(p_KeyName.IsEmpty()))
		return;
	g_regOpen = Open(p_hKeyParent, p_KeyName, p_samDesired);
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
CSysRegKey::~CSysRegKey()
{
	Close();
}

//////////////////////////////////////////////////////////
//
//		Open
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			true if successful, else false
//			(g_regResult = result code)
//      Parameters:
//			p_hKeyParent
//				The handle of an open key
//			p_KeyName
//				Specifies the subkey of hKeyParent
//				to be created or opened
//			p_samDesired
//				The security access for the key. 
//		Remarks
//			Opens the specified key.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::Open(HKEY p_hKeyParent,
					  CString p_KeyName,
					  REGSAM p_samDesired)
{
	g_hKeyParent = p_hKeyParent;
	g_KeyName = p_KeyName;
	g_sam = p_samDesired;

	return ReOpen();
}

//////////////////////////////////////////////////////////
//
//		ReOpen
//
//////////////////////////////////////////////////////////
//
//		bool ReOpen(void);
//
//		Return Value:
//			true if successful, else false
//			(g_regResult = result code)
//      Parameters:
//			None
//		Remarks
//			Re-opens the specified key. The key must
//			have already been opened once using either
//			a Open or Create method.
//
//			If the key is already open, it is first closed
//			and then re-opened.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::ReOpen()
{
	if (g_regOpen)
		Close();
	g_regOpen = false;

	g_regResult = CRegKey::Open(g_hKeyParent, g_KeyName, g_sam);
	if (g_regResult != ERROR_SUCCESS)
		return false;
	g_regOpen = true;
	return true;
}

//////////////////////////////////////////////////////////
//
//		Create
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			true if successful, else false
//			(g_regResult = result code)
//      Parameters:
//			p_hKeyParent
//				The handle of an open key
//			p_KeyName
//				Specifies the subkey of hKeyParent
//				to be created or opened
//			p_Options
//				Options for the key
//			p_samDesired
//				The security access for the key. 
//		Remarks:
//			Creates or opens the specified key.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::Create(HKEY p_hKeyParent,
					    CString p_KeyName,
					    DWORD p_Options,
					    REGSAM p_samDesired)
{
	g_hKeyParent = p_hKeyParent;
	g_KeyName = p_KeyName;
	g_Options = p_Options;
	g_sam = p_samDesired;

	g_regResult = CRegKey::Create(g_hKeyParent,
					              g_KeyName,
						          REG_NONE,
						          g_Options,
						          g_sam,
						          NULL,
						          NULL);
	if (g_regResult != ERROR_SUCCESS)
		return false;
	g_regOpen = true;
	return true;
}

//////////////////////////////////////////////////////////
//
//		Close
//
//////////////////////////////////////////////////////////
//
//		void Close(void); 
//
//		Return Value: 
//			None 
//		Parameters: 
//			None 
//		Remarks: 
//			Close (release) the current key (CRegKey::m_hKey) 
//		    and store result in g_regResult.
//
//////////////////////////////////////////////////////////
void CSysRegKey::Close()
{
	g_regResult = ERROR_SUCCESS;
	if (! g_regOpen)
		return;
	g_regResult = CRegKey::Close();
	g_regOpen = false;
}

//////////////////////////////////////////////////////////
//
//		ReLoadData
//
//////////////////////////////////////////////////////////
//
//		This public method shoulc be overriden to
//		load the individual elements from the registry
//
//////////////////////////////////////////////////////////
bool CSysRegKey::ReLoadData()
{
	g_regState = false;
	return false;
}

//////////////////////////////////////////////////////////
//
//		StoreData
//
//////////////////////////////////////////////////////////
//
//		This public method shoulc be overriden to
//		store the individual elements into the registry
//
//////////////////////////////////////////////////////////
bool CSysRegKey::StoreData()
{
	g_regState = false;
	return false;
}

//////////////////////////////////////////////////////////
//
//		QueryInfoKey
//
//////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////
bool CSysRegKey::QueryInfoKey()
{
	g_SubKeys = 0;
    g_MaxSubKeyNameLen = 0;
	
	g_regResult = RegQueryInfoKey(CRegKey::m_hKey, 
						          NULL, 
						          NULL, 
						          NULL, 
						          &g_SubKeys, 
						          &g_MaxSubKeyNameLen, 
						          NULL, 
						          NULL, 
						          NULL, 
						          NULL, 
								  NULL, 
								  NULL);
	if (g_regResult == ERROR_SUCCESS)
		return true;
	return false;
}

//////////////////////////////////////////////////////////
//
//		EnumKey
//
//////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////
CString CSysRegKey::EnumKey(int p_EnumIndex)
{
	CString l_KeyName;
	l_KeyName.Empty();

	g_NameSize = MAX_PATH + 1;
	g_regResult = RegEnumKeyEx(CRegKey::m_hKey,
							   p_EnumIndex,
							   g_NameBuffer,
							   &g_NameSize,
						       NULL,
							   NULL,
							   NULL,
							   &g_LastWriteTime);
	if (g_regResult == ERROR_SUCCESS)
	{
		l_KeyName.Format("%s", g_NameBuffer);
	}
	return l_KeyName;
}

//////////////////////////////////////////////////////////
//
//		Result
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			The current setting of g_regResult
//		Parameters:
//			None
//		Remarks:
//			g_regResult is a protected data member
//			which contains the result of the last
//			Open, Create, Get..., Set..., GetOrSet...
//			operation.
//			The Result method exposes the current value 
//			stored in g_regResult.
//
//////////////////////////////////////////////////////////
long CSysRegKey::Result()
{
	return g_regResult;
}

//////////////////////////////////////////////////////////
//
//		RegKeyHandle
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			The current setting of CRegKey::m_hKey
//		Parameters:
//			None
//		Remarks:
//
//////////////////////////////////////////////////////////
HKEY CSysRegKey::RegKeyHandle()
{
	return CRegKey::m_hKey;
}

//////////////////////////////////////////////////////////
//
//		Error
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			The current setting of g_regError
//		Parameters:
//			None
//		Remarks:
//			g_regError is a protected data member
//			available to class extensions.
//			The Error method exposes the current value 
//			stored in g_regError.
//
//////////////////////////////////////////////////////////
int CSysRegKey::Error()
{
	return g_regError;
}

//////////////////////////////////////////////////////////
//
//		State
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			The current setting of g_regState
//		Parameters:
//			None
//		Remarks:
//			g_regState is a boolean protected data member
//			available to class extensions.
//			The State method exposes the current value 
//			stored in g_regState.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::State()
{
	return g_regState;
}

//////////////////////////////////////////////////////////
//
//		OpenState
//
//////////////////////////////////////////////////////////
//
//		Return the current key open state
//
//		Return Value:
//			The current setting of g_regOpen
//		Parameters:
//			None
//		Remarks:
//			g_regOpen is a protected data member which reflects
//			the open status of the current key:
//				g_regOpen = true if the key is open.
//				g_regOpen = false if the key is not open.
//			The State method exposes the current value 
//			stored in g_regOpen.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::OpenState()
{
	return g_regOpen;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//
//						String Values
//
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//
//		GetStringValue
//
//////////////////////////////////////////////////////////
//
//		Get a string value
//
//		Return Value:
//			If successful, returns boolean true, else false
//		Parameters:
//			p_ValueName
//				Specifies the value field name to return
//			p_pValue
//				A pointer to a CString to receive the
//				field value
//		Remarks:
//			GetStringValue retrieves the string value
//			stored in p_ValueName.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::GetStringValue(CString p_ValueName, CString * p_pValue)
{
	if (! g_regOpen)
	{
		g_regResult = -1;
		return false;
	}

	g_regCharValueSize = sizeof g_regCharValue;
	memset(g_regCharValue, 0, g_regCharValueSize);

	g_regResult = QueryValue(g_regCharValue,
							 p_ValueName,
							 &g_regCharValueSize);
	if (g_regResult != ERROR_SUCCESS)
		return false;

	p_pValue->Format("%s", g_regCharValue);
	return true;
}

//////////////////////////////////////////////////////////
//
//		SetStringValue
//
//////////////////////////////////////////////////////////
//
//		Set a string value
//
//		Return Value:
//			If successful, returns boolean true, else false
//		Parameters:
//			p_ValueName
//				Specifies the value field name to return
//			p_ValueDefault
//				A CString containing the value to set the field to
//		Remarks:
//			SetStringValue sets the string value
//			stored in p_ValueName.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::SetStringValue(CString p_ValueName, CString p_ValueDefault)
{
	if (! g_regOpen)
	{
		g_regResult = -1;
		return false;
	}

	g_regResult = SetValue(p_ValueDefault,
						   p_ValueName);
	if (g_regResult != ERROR_SUCCESS)
		return false;
	return true;
}

//////////////////////////////////////////////////////////
//
//		GetOrCreateString
//
//////////////////////////////////////////////////////////
//
//
//		Return Value:
//			If successful, returns boolean true, else false
//		Parameters:
//			p_ValueName
//				Specifies the value field name to return
//			p_pValue
//				A pointer to a CString to receive the
//				field value
//			p_ValueDefault
//				A CString containing the value to set the field to
//		Remarks:
//			Retrieves an existing string value.  If not found,
//			attempts to create the string with p_ValueDefault.
//			If successful, retrieves the string value.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::GetOrCreateString(CString p_ValueName,
								   CString * p_pValue,
								   CString p_ValueDefault)
{
	if (! g_regOpen)
	{
		g_regResult = -1;
		return false;
	}

	if (GetStringValue(p_ValueName, p_pValue))
		return true;
	if (! SetStringValue(p_ValueName, p_ValueDefault))
		return false;
	return GetStringValue(p_ValueName, p_pValue);
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//
//						DWORD Values
//
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//
//		GetDWORDValue
//
//////////////////////////////////////////////////////////
//
//		Get a DWORD value
//
//		Return Value:
//			If successful, returns boolean true, else false
//		Parameters:
//			p_ValueName
//				Specifies the value field name to return
//			p_pValue
//				The address of a DWORD to receive the
//				field value
//		Remarks:
//			GetDWORDValue retrieves the DWORD value
//			stored in p_ValueName.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::GetDWORDValue(CString p_ValueName, DWORD & p_pValue)
{
	if (! g_regOpen)
	{
		g_regResult = -1;
		return false;
	}

	g_regResult = QueryValue(p_pValue,
							 p_ValueName);
	if (g_regResult != ERROR_SUCCESS)
		return false;

	return true;
}

//////////////////////////////////////////////////////////
//
//		SetDWORDValue
//
//////////////////////////////////////////////////////////
//
//		Set a DWORD value
//
//		Return Value:
//			If successful, returns boolean true, else false
//		Parameters:
//			p_ValueName
//				Specifies the value field name to return
//			p_ValueDefault
//				A DWORD containing the value to set the field to
//		Remarks:
//			SetDWORDValue sets the DWORD value stored in p_ValueName.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::SetDWORDValue(CString p_ValueName, DWORD p_ValueDefault)
{
	if (! g_regOpen)
	{
		g_regResult = -1;
		return false;
	}

	g_regResult = SetValue(p_ValueDefault,
						   p_ValueName);
	if (g_regResult != ERROR_SUCCESS)
		return false;
	return true;
}

//////////////////////////////////////////////////////////
//
//		GetOrCreateDWORD
//
//////////////////////////////////////////////////////////
//
//		Return Value:
//			If successful, returns boolean true, else false
//		Parameters:
//			p_ValueName
//				Specifies the value field name to return
//			p_pValue
//				A pointer to a CString to receive the
//				field value
//			p_ValueDefault
//				A CString containing the value to set the field to
//		Remarks:
//			Retrieves an existing DWORD value.  If not found,
//			attempts to create the DWORD with p_ValueDefault.
//			If successful, retrieves the DWORD value.
//
//////////////////////////////////////////////////////////
bool CSysRegKey::GetOrCreateDWORD(CString p_ValueName,
								  DWORD & p_pValue,
								  DWORD p_ValueDefault)
{
	if (! g_regOpen)
	{
		g_regResult = -1;
		return false;
	}

	if (GetDWORDValue(p_ValueName, p_pValue))
		return true;
	if (! SetDWORDValue(p_ValueName, p_ValueDefault))
		return false;
	return GetDWORDValue(p_ValueName, p_pValue);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
