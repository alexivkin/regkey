#include "stdafx.h"
#include "SysRegKeyService.h"

//////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////
//
//			CServiceRegKey Class - Derived from CSysRegKey
//
//////////////////////////////////////////////////////////////////////////////////
//
//		General purpose routines giving access to
//			Service Registry parameters.
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

//////////////////////////////////////////////////////////////////////////////////
//
//				Constructor
//
//////////////////////////////////////////////////////////////////////////////////
CServiceRegKey::CServiceRegKey(HKEY p_hKeyParent,
					     CString p_KeyName,
						 CString p_ServiceName,
						 REGSAM p_samDesired):
				CSysRegKey(p_hKeyParent,
						   p_KeyName,
						   p_samDesired)
{
	if (g_regOpen)
		Close();
	p_hKeyParent = HKEY_LOCAL_MACHINE;
	p_KeyName += p_ServiceName + "\\Parameters";
	if (! Create(p_hKeyParent,
			     p_KeyName,
				 REG_OPTION_NON_VOLATILE,
				 p_samDesired))
		return;
	ReLoadData();
}

//////////////////////////////////////////////////////////////////////////////////
//
//				Destructor
//
//////////////////////////////////////////////////////////////////////////////////
CServiceRegKey::~CServiceRegKey()
{
}

//////////////////////////////////////////////////////////////////////////////////
//
//				ReLoadData
//
//////////////////////////////////////////////////////////////////////////////////
bool CServiceRegKey::ReLoadData()
{
	g_regState = GetOrCreateString("Application", 
								   &g_RunApp, 
								   "");

	g_regState = g_regState & GetOrCreateString("AppParameters", 
												&g_Param, 
												"");
	return g_regState;
}

//////////////////////////////////////////////////////////////////////////////////
//
//				StoreData
//
//////////////////////////////////////////////////////////////////////////////////
bool CServiceRegKey::StoreData()
{
	g_regState = SetStringValue("Application",
								g_RunApp);

	g_regState = g_regState & SetStringValue("AppParameters",
											 g_Param);
	return g_regState;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

