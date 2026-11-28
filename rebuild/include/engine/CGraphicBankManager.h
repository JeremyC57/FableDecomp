#ifndef FABLE_ENGINE_CGRAPHICBANKMANAGER_H
#define FABLE_ENGINE_CGRAPHICBANKMANAGER_H

#include "engine/CBaseClassNonCopyable.h"
#ifndef FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST
#endif
#include "stlport/fable_containers.h"

// Retail RTTI at 0x0129C7E4 and the list sentinel initialized by 0x009F8690.
class CGraphicBankManager : public CBaseClassNonCopyable {
public:
    void FrameEnd();
private:
    class CGraphicDataBankInfo;
    std::list<CGraphicDataBankInfo> GraphicDataBanks;
};
FABLE_STATIC_ASSERT_AT(43190,sizeof(CGraphicBankManager)==8);
#endif
