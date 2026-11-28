#ifndef FABLE_ENGINE_CTCBURNTEFFECT_H
#define FABLE_ENGINE_CTCBURNTEFFECT_H
#ifdef FABLE_RETAIL_CTC_BURNT_EFFECT_FLASH_PASS30
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesBurntEffectDef;
class CThing;
// Both donor PDBs name the static accessor; the retail RTTI base is CTCBase.
class CTCBurntEffect : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_BURNT_EFFECT; }
private:
    CDefPointer<const CSpecialAbilitiesBurntEffectDef> PDef;
    long CurTime, FadeInTime, FadeOutTimeStart, FadeOutTime;
    std::vector<CIntelligentPointer<CThing> > Effects;
    std::vector<CIntelligentPointer<CThing> > TintList;
};
FABLE_STATIC_ASSERT(sizeof(CTCBurntEffect) == 0x38);
#endif
#endif
