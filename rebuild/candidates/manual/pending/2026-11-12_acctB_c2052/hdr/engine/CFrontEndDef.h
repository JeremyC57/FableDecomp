#ifndef FABLE_ENGINE_CFRONTENDDEF_H
#define FABLE_ENGINE_CFRONTENDDEF_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). RTTI .?AVCFrontEndDef@@ : CParentDefClassBase (packed 0x39);
// Ego_r.pdb members vAttractModeMovie (vector<CCharString>, retail STLport 0xC at +0x3C: Run 0x0042F115 reads _M_start +0x3C /
// _M_finish +0x40), ErrorMessageBackgroundGraphic +0x48, ButtonABigGraphic +0x4C, ButtonBBigGraphic +0x50; sizeof 0x54.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CFrontEndDef : public CParentDefClassBase {
public:
    std::vector<CCharString> vAttractModeMovie;                               // +0x3C
    long ErrorMessageBackgroundGraphic;                                       // +0x48
    long ButtonABigGraphic;                                                   // +0x4C
    long ButtonBBigGraphic;                                                   // +0x50
};
#endif
