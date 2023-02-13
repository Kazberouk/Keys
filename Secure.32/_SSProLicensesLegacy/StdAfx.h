// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//#include "..\Common\Interfaces\RegScreenHelpr.h"
//#include "..\Resshare.32\ProUtilsHelper\DataTypesHelper.h"
//#include "../Resshare.32/ProUtilsHelper/OleVariantExt.h"

#include "common.h"

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcview.h>
#include <afxpriv.h>

//#include <afxdao.h>
#include <afxtempl.h>
//#include <afxole.h>
#include <Psapi.h>
#include <float.h>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <filesystem>
#include <functional>
#include <TlHelp32.h>
#include <imagehlp.h>
#include <Psapi.h>
#include <filesystem.h>

#ifndef DllImport
#define DllImport	__declspec( dllimport )
#endif
#ifndef DllExport
#define DllExport	__declspec( dllexport )
#endif

#include "cmn_stdafx.h"
#include "Metadata/RegScreenPIDs.h"

#include "cmnutils/StdAfxExt.h"
#include "cmnutils/ResourceSwitch.h"

#define _PRO_SCREEN_DYNAMIC_FILE_NAME_INC "Interfaces/DLL_INC/REALPRO_SCREENS_CX_DLL_INC.h"
#define _PRO_SCREEN_DYNAMIC_FILE_NAME_EXP "MetaData/DLL_EXP/REALPRO_SCREENS_CX_DLL_EXP.hpp"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

