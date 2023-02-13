// Test1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseGenerator.h"
#include "LicenseGeneratorDlg.h"
#include "../Utils.32/StarlingPro/SSPro60Sec.h"
#include "../Utils.32/StarlingPro/SSPro60Sec.h"
#define KEY_UPDATE(number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, modules)
#include "../Secure.32/SSProList/UpdateKeys.h"
#undef KEY_UPDATE

//#include "CmnUtils/ResourceSwitch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum eKeyUpdaterRunCounterMode
{
	KURor = 0,
	KURthen = 1,
	KURnone = 2,
};

#define CLIENT_ADDITIONAL_STRING ""
#define LICENSES_SERVER_FOLDER "T:\\Licenses\\Server\\"
#define LICENSES_CLIENTS_FOLDER "T:\\Licenses\\Clients\\"
#define LICENSES_REPORT_FOLDER "T:\\Licenses\\"
static CString strEncryptedFilePath;
static CString strStaticEncryptedFileNamePrefix;
static BOOL bStaticAppend = FALSE;
static WORD Static_Updater_Version = 0;
static BOOL bNotFirst = FALSE;

static void SetDefaultKeyLicenseFilePath(CString &strLicenseFilePath, int iKeyNumber, LPSTR strFileFolder) {
	strLicenseFilePath = strFileFolder;

	CString strLicenseFileName;
	SetDefaultKeyLicenseFileNameStarSec(strLicenseFileName, iKeyNumber);
	strLicenseFilePath += strLicenseFileName;
}

#ifndef FILE_NAME_PREFIX
#define FILE_NAME_PREFIX ""
#endif

static void SetDefaultClientLicenseFilePath(CString &strLicenseFilePath, LPSTR strFileFolder) {
	strLicenseFilePath.Format("%s%s_KeyUpdater-%s%s.KEY", strFileFolder, strStaticEncryptedFileNamePrefix.GetBuffer(0), CLIENT_UPDATER_YEAR, CLIENT_ADDITIONAL_STRING);
}

class CKeyUpdater {
public:
	CKeyUpdater(BOOL bReport, BOOL bSingle, WORD key_num, CString key_name, long valid_year, long valid_month, long valid_day, long lic_year, long lic_month, long lic_day, 
				eKeyUpdaterRunCounterMode run_counter_mode, long counter, long* pmodules) {

		ASSERT(pmodules);
		CSSPro60Sec Key;
		Key.m_KeyData.m_SerNum			= 0x01C5;
		Key.m_KeyData.m_DevID			= 0xDF6A;
		Key.m_KeyData.m_KeyNumber		= key_num;

		Key.m_KeyData.m_ExpDate.m_Year	= valid_year;
		Key.m_KeyData.m_ExpDate.m_Month	= valid_month;
		Key.m_KeyData.m_ExpDate.m_Day	= valid_day;
		Key.m_KeyData.m_LicDate.m_Year	= lic_year;
		Key.m_KeyData.m_LicDate.m_Month	= lic_month;
		Key.m_KeyData.m_LicDate.m_Day	= lic_day;

		SYSTEMTIME CurrentTime;
		GetSystemTime(&CurrentTime);
		Key.m_KeyData.m_BurnDate.m_Day = CurrentTime.wDay;
		Key.m_KeyData.m_BurnDate.m_Month = CurrentTime.wMonth;
		Key.m_KeyData.m_BurnDate.m_Year = CurrentTime.wYear - 2000;

		if (run_counter_mode == KURthen)
			Key.m_KeyData.m_ExpUpdate |= _SSP60_EXPIRE_ORDER_FLAG_;
		else
			Key.m_KeyData.m_ExpUpdate &= ~_SSP60_EXPIRE_ORDER_FLAG_;

		if (counter > 100)
			counter = 100;
		Key.m_KeyData.m_ExpCounter = WORD(counter | _SSP60_COUNTER_FLAG_);
		strncpy(Key.m_KeyData.m_KeyName, key_name, _KEY_NAME_LENGTHW_SSP60_*2-1);


#define GET_DATA(mask) WORD(_SSP60_##mask##_MASK_)

#define _UPDATE_DATA_(mem_var, mask)																						\
	case SKM_##mask##:																											\
		Key.m_KeyData.##mem_var = (WORD)((Key.m_KeyData.##mem_var & ~GET_DATA(mask)) | GET_DATA(mask));	\
		break;

#define _UPDATE_DOUBLE_DATA_(mem_var1, mask1, mem_var2, mask2)																\
	case SKM_##mask1##:																													\
		Key.m_KeyData.##mem_var1 = (WORD)((Key.m_KeyData.##mem_var1 & ~GET_DATA(mask1)) | GET_DATA(mask1));	\
		Key.m_KeyData.##mem_var2 = (WORD)((Key.m_KeyData.##mem_var2 & ~GET_DATA(mask2)) | GET_DATA(mask2));	\
		break;

#define _UPDATE_LEGACY_DATA_(mem_var, mask)																				\
	case SKM_LEGACY_##mask##:																									\
		Key.m_KeyData.##mem_var = (WORD)((Key.m_KeyData.##mem_var & ~GET_DATA(mask)) | GET_DATA(mask));	\
		break;


		int index = 0;
		while(SKM_UNKNOWN != pmodules[index]) {m_ExpUpdate
			switch(pmodules[index]) {
				_UPDATE_DOUBLE_DATA_(m_Fracpro			, FRACPRO		, m_FracScr_Stimpro, FRAC_SCREEN);
				_UPDATE_DATA_(m_FracScr_Stimpro			, STIMPRO		);
				_UPDATE_DATA_(m_FracScr_Stimpro			, FRAC_SCREEN	);
				_UPDATE_DATA_(m_Respro_Cementpro			, RESPRO			);
				_UPDATE_DATA_(m_Respro_Cementpro			, CEMENTPRO		);
				_UPDATE_DATA_(m_Realpro						, REALPRO		);
				_UPDATE_DATA_(m_WTPA_Toolpro				, TOOLPRO		);
				_UPDATE_DOUBLE_DATA_(m_WTPA_Toolpro		, WTPA			, m_PorPro_EcoPro, FLOPRO);
				_UPDATE_DATA_(m_MaxproVM					, MAXPRO			);
				_UPDATE_DATA_(m_MaxproVM					, VM				);
				_UPDATE_DATA_(m_Fieldpro					, FIELDPRO		);
				_UPDATE_DATA_(m_PorPro_EcoPro				, FLOPRO			);
				_UPDATE_DATA_(m_PorPro_EcoPro				, ECOPRO			);
				_UPDATE_DATA_(m_GeoPro_LogPro				, LOGPRO			);

				_UPDATE_LEGACY_DATA_(m_WTPA_Toolpro		, GRADIENT		);
				_UPDATE_LEGACY_DATA_(m_PorPro_EcoPro	, COST			);
				_UPDATE_LEGACY_DATA_(m_GeoPro_LogPro	, GEOPRO			);
				_UPDATE_LEGACY_DATA_(m_TransPro			, TRANSPRO		);
				_UPDATE_LEGACY_DATA_(m_eFieldpro			, EFIELDPRO		);
			}
			index++;
		}
#undef GET_DATA
#undef _UPDATE_DATA_
#undef _UPDATE_DOUBLE_DATA_
#undef _UPDATE_LEGACY_DATA_

		Key.m_KeyData.m_Updater_Version	= Static_Updater_Version;

		if (bReport) {
			CFile fEncryptedFile;
			fEncryptedFile.Open(strEncryptedFilePath, CFile::modeNoTruncate | CFile::modeWrite);
			fEncryptedFile.SeekToEnd();

			CString StrLine;
			if (!bStaticAppend) {
				if (bNotFirst)
					StrLine.Format("\n%s:\n", strStaticEncryptedFileNamePrefix.GetBuffer(0));
				else {
					StrLine.Format("%s:\n", strStaticEncryptedFileNamePrefix.GetBuffer(0));
					bNotFirst = TRUE;

				}
				fEncryptedFile.Write(StrLine.GetBuffer(0), StrLine.GetLength());
			}
			StrLine.Format("%d\n", Key.m_KeyData.m_KeyNumber);
			fEncryptedFile.Write(StrLine.GetBuffer(0), StrLine.GetLength());
			fEncryptedFile.Close();
			bStaticAppend = TRUE;
		}

		else {
			if (!bSingle) {
				SetDefaultKeyLicenseFilePath(strEncryptedFilePath, key_num, LICENSES_SERVER_FOLDER);
				bStaticAppend = FALSE;
			}
			else {
				if (!bStaticAppend)
					SetDefaultClientLicenseFilePath(strEncryptedFilePath, LICENSES_CLIENTS_FOLDER);
			}

			bStoreKeyLicenseInfoStarSec(&Key, Key.m_KeyData.m_KeyNumber, strEncryptedFilePath, bStaticAppend);
			if (bSingle)
				bStaticAppend = TRUE;
		}
	};
};

#ifdef CLIENT_NEW_SECTION
#undef CLIENT_NEW_SECTION
#endif

#define CLIENT_NEW_SECTION				\
	bStaticAppend = FALSE;				\
	strStaticEncryptedFileNamePrefix = 	FILE_NAME_PREFIX;

void GenerateSingleLicenseFile() {
	bStaticAppend = FALSE;

#define KEY_UPDATE(number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, modules)											\
Static_Updater_Version = UPDATER_VERSION_NUMBER;																															\
static long g_Key##number[] = {/modules/, SKM_UNKNOWN};																														\
static CKeyUpdater g_KeyUpdater##number(FALSE, TRUE, number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, g_Key##number);	\

#include "../SSProList/UpdateKeys.h"
#undef KEY_UPDATE

}

void GenerateMultipleLicenseFiles() {
	bStaticAppend = FALSE;

#define KEY_UPDATE(number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, modules)											\
Static_Updater_Version = UPDATER_VERSION_NUMBER;																															\
static long g_Key##number[] = {/modules/, SKM_UNKNOWN};																														\
static CKeyUpdater g_KeyUpdater##number(FALSE, FALSE, number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, g_Key##number);	\

#include "../SSProList/UpdateKeys.h"
#undef KEY_UPDATE

}

void GenerateLicenseFilesReport() {
	bStaticAppend = FALSE;

	strEncryptedFilePath.Format("%sKeyUpdaterReport-%s.TXT", LICENSES_REPORT_FOLDER, CLIENT_UPDATER_YEAR);

	CFile fEncryptedFile;
	fEncryptedFile.Open(strEncryptedFilePath, CFile::modeCreate | CFile::modeWrite);
	fEncryptedFile.Close();

#define KEY_UPDATE(number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, modules)											\
Static_Updater_Version = UPDATER_VERSION_NUMBER;																															\
static long g_Key##number[] = {/modules/, SKM_UNKNOWN};																														\
static CKeyUpdater g_KeyUpdater##number(TRUE, TRUE, number, name, valid_year, valid_month, valid_day, lic_year, lic_month, lic_day, run_counter, counter, g_Key##number);	\

#include "../SSProList/UpdateKeys.h"
#undef KEY_UPDATE

}

