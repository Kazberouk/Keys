#include "stdafx.h"
#include "SSPro60.h"
#include "SSProMain.h"

#include "Interfaces/DLL_INC/REALPRO_CX_DLL_INC.h"
#include "CmnUtils/SysLogManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

BOOL IsWebAppRun() {return FALSE;}

CSSProMain *CSSProMain::g_pProMain = nullptr;
CSSProMain *CSSProMain::GetProMain()
{
	if (!g_pProMain)
		g_pProMain = new CSSProMain;
	g_pProMain->AddRef();
	return g_pProMain;
}

// export function to access to IProMain interface
extern "C" __declspec( dllexport ) IProMain* GetProMainInterface() 
{
	return CSSProMain::GetProMain();
}

CSSProMain::CSSProMain()
{
}

EXTERN_C typedef HRESULT(STDAPICALLTYPE *GetPROInterfaceFunc)(REFGUID guid, void** ppI, int nOwnerType, DWORD dwThis, IProperties* pIPROParameters);

ULONG CSSProMain::GetInterfacesCount()
{
	long nICount = 0;

	#undef DECLARE_STATIC_PRO_INTERFACE
	#define DECLARE_STATIC_PRO_INTERFACE(InterfaceIID, FuncPtr)  ++nICount;

	#undef DECLARE_DYNAMIC_PRO_INTERFACE
	#define DECLARE_DYNAMIC_PRO_INTERFACE(Interface, DLLName, FuncName) ++nICount;

	#undef DECLARE_DYNAMIC_PRO_INTERFACE_STD
	#define DECLARE_DYNAMIC_PRO_INTERFACE_STD(Interface, DLLName) ++nICount;

	#undef DECLARE_DYNAMIC_PRO_INTERFACE_EXT
	#define DECLARE_DYNAMIC_PRO_INTERFACE_EXT(Interface, DLLName) ++nICount;

//	#include "Metadata/DLL_EXP/REALPRO_CX_DLL_EXP.hpp"

	#undef DECLARE_STATIC_PRO_INTERFACE
	#undef DECLARE_DYNAMIC_PRO_INTERFACE
	#undef DECLARE_DYNAMIC_PRO_INTERFACE_STD
	#undef DECLARE_DYNAMIC_PRO_INTERFACE_EXT

	return nICount + CProMain::GetInterfacesCount();
}

void CSSProMain::LoadInterfacesMap()
{
	CProMain::LoadInterfacesMap();

	#undef DECLARE_STATIC_PRO_INTERFACE
	#define DECLARE_STATIC_PRO_INTERFACE(InterfaceIID, FuncPtr) \
		REGISTRY_STATIC_PRO_INTERFACE(InterfaceIID, FuncPtr)

	#undef DECLARE_DYNAMIC_PRO_INTERFACE
	#define DECLARE_DYNAMIC_PRO_INTERFACE(Interface, DLLName, FuncName) \
		REGISTRY_DYNAMIC_PRO_INTERFACE(Interface, DLLName, FuncName)

	#undef DECLARE_DYNAMIC_PRO_INTERFACE_STD
	#define DECLARE_DYNAMIC_PRO_INTERFACE_STD(Interface, DLLName) \
		DECLARE_DYNAMIC_PRO_INTERFACE(Interface, DLLName, "CreateInstance")

	#undef DECLARE_DYNAMIC_PRO_INTERFACE_EXT
	#define DECLARE_DYNAMIC_PRO_INTERFACE_EXT(Interface, DLLName) \
		DECLARE_DYNAMIC_PRO_INTERFACE(Interface, DLLName, "CreateInstance")

//DECLARE_DYNAMIC_PRO_INTERFACE_EXT(ICryptographyHelper, _DLL_NAME)

	#include "Metadata/DLL_EXP/REALPRO_CX_DLL_EXP.hpp"

	#undef DECLARE_STATIC_PRO_INTERFACE
	#undef DECLARE_DYNAMIC_PRO_INTERFACE
	#undef DECLARE_DYNAMIC_PRO_INTERFACE_STD
	#undef DECLARE_DYNAMIC_PRO_INTERFACE_EXT
}

HRESULT CSSProMain::get_interface(const IID& ID, void** pI, int OwnerType, DWORD dwThis, IProperties* pIPROParameters)
{
	PRO_MANAGE_STATE;
/*	if (IID_ITreeLoaderManager == ID)
	{
		CLGTreeLoaderManager *pTreeLoaderManager = 
			new CLGTreeLoaderManager;
		pTreeLoaderManager->AddRef();
		*pI = pTreeLoaderManager;
		return S_OK;
	}
	else if (IID_ISysLogManager == ID)
	{
		*pI = CreateISysLogManager();
		return S_OK;
	}*/

	return CProMain::get_interface(ID, pI, OwnerType, dwThis, pIPROParameters);
}

///////////////////////////////////////////////////////////////////////////////
/*
BEGIN_QUERY_INTERFACE(CLGTreeLoaderManager)
	SUPPORT_INTERFACE(ITreeLoaderManager)
END_QUERY_INTERFACE

IMPLEMENT_ADDREF(CLGTreeLoaderManager)

IMPLEMENT_RELEASE(CLGTreeLoaderManager)

CLGTreeLoaderManager::CLGTreeLoaderManager()
{
}

CLGTreeLoaderManager::~CLGTreeLoaderManager()
{
}

STDMETHODIMP CLGTreeLoaderManager::CreateTreeLoaderInstance(int tl_type, int mode, ITreeLoader** pTL, DWORD dwFilter = 0)
{
	if (NULL == pTL)
		return E_FAIL;

	ITreeLoader *pTreeLoader = NULL;
	switch (tl_type)
	{
		case tltLGExplorer:
		{
			CRealProApp *pApp = (CRealProApp *) AfxGetApp();
			pTreeLoader = CreateLGExplorerTreeLoader(&pApp->m_pLGExplorerDoc);
			break;
		}
	}
	if (NULL == pTreeLoader)
		return E_OUTOFMEMORY;

	*pTL = pTreeLoader;

	return S_OK;
}
*/