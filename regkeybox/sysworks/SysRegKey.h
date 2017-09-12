#if !defined(_SysRegKey_h__)
#define _SysRegKey_h__

//////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////
//
//		CSysRegKey Class - Derived from CRegKey
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

#include <atlbase.h>

//////////////////////////////////////////////////////////////////////////////////

class CSysRegKey : public CRegKey
{

public:
					CSysRegKey(HKEY p_hKeyParent = NULL,
							   CString p_KeyName = "",
							   REGSAM p_samDesired=KEY_ALL_ACCESS);
	virtual			~CSysRegKey();

	//////////////////////////////////////////////////////////////////////////////

public:
	int				Error(void);
	bool			OpenState(void);
	long			Result(void);
	bool			State(void);
	virtual bool	ReLoadData(void);
	virtual bool	StoreData(void);

	bool			Open(HKEY p_hKeyParent,
					     CString p_KeyName,
					     REGSAM p_samDesired=KEY_ALL_ACCESS);
	bool			ReOpen(void);
	bool			Create(HKEY p_hKeyParent,
						   CString p_KeyName,
						   DWORD p_Options=REG_OPTION_NON_VOLATILE,
						   REGSAM p_samDesired=KEY_ALL_ACCESS);
	void			Close(void);
	HKEY			RegKeyHandle(void);
	bool			QueryInfoKey(void);
	CString			EnumKey(int p_EnumIndex);

	bool			GetStringValue(CString p_ValueName, CString * p_pValue);

protected:
	
	bool			SetStringValue(CString p_ValueName, CString p_ValueDefault);
	bool			GetOrCreateString(CString p_ValueName,
								      CString * p_pValue,
								      CString p_ValueDefault);

	bool			GetDWORDValue(CString p_ValueName, DWORD & p_pValue);
	bool			SetDWORDValue(CString p_ValueName, DWORD p_ValueDefault);
	bool			GetOrCreateDWORD(CString p_ValueName,
								     DWORD & p_pValue,
								     DWORD p_ValueDefault);

	//////////////////////////////////////////////////////////////////////////////
	
public:
	DWORD			g_SubKeys;
    DWORD			g_MaxSubKeyNameLen;

protected:
	long			g_regResult;
	bool			g_regOpen;
	int				g_regError;
	bool			g_regState;

	HKEY			g_hKeyParent;
	CString			g_KeyName;
	REGSAM			g_sam;
	DWORD			g_Options;

	char			g_NameBuffer[MAX_PATH + 1];
	DWORD			g_NameSize;
	FILETIME		g_LastWriteTime;

private:
	char			g_regCharValue [MAX_PATH + 1];
	DWORD			g_regCharValueSize;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#endif
