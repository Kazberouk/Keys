#pragma once

//#include "Interfaces/ITreeLoader.h"
#include "PROCOM.SRC/promain.h"

class CLGProMain: public CProMain
{
public:
	CLGProMain();
	static CLGProMain *GetProMain();

protected:
	virtual HRESULT get_interface(const IID& ID, void** pI, int nOwnerType, DWORD dwThis, IProperties* pIPROParameters) override;
	virtual ULONG GetInterfacesCount() override;
	virtual void LoadInterfacesMap() override;

	virtual long GetModuleType() const override { return epmtLicenseGenerator; }
private:
	static CLGProMain *g_pProMain;
};
