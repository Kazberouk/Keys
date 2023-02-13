#pragma once

#include "PROCOM.SRC/promain.h"

class CSSProMain: public CProMain
{
public:
	CSSProMain();
	static CSSProMain *GetProMain();

protected:
	virtual HRESULT get_interface(const IID& ID, void** pI, int nOwnerType, DWORD dwThis, IProperties* pIPROParameters) override;
	virtual ULONG GetInterfacesCount() override;
	virtual void LoadInterfacesMap() override;

	virtual long GetModuleType() const override { return epmtLicenseGenerator; }

private:
	static CSSProMain *g_pProMain;
};
