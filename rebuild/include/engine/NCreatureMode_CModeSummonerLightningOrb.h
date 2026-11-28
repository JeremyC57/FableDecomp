#ifndef FABLE_ENGINE_NCREATUREMODE_CMODESUMMONERLIGHTNINGORB_H
#define FABLE_ENGINE_NCREATUREMODE_CMODESUMMONERLIGHTNINGORB_H
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
namespace NCreatureMode {
class CModeSummonerLightningOrb : public CModeStandBase {
public:
    static unsigned long FABLE_FASTCALL StatGetType();
    virtual unsigned long GetType() const;
    static CTCCreatureModeManager::CModeBase* FABLE_FASTCALL Alloc(CThing&);
    CModeSummonerLightningOrb(const CModeSummonerLightningOrb&);
    CModeSummonerLightningOrb(CThing&, CThing&);
    CModeSummonerLightningOrb(CThing&);
    virtual const CAnimFilterBase& GetAnimFilter() const;
    virtual void OnActivate();
    virtual void FrameUpdate();
    virtual void OnDeactivate();
private:
    CAnimFilterOfGroup Filter;
    CIntelligentPointer<CThing> PTarget;
    CIntelligentPointer<CThing> PChargeEffect;
    CIntelligentPointer<CThing> POrbChargeEffect;
    long Timer;
};
}
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeSummonerLightningOrb)==0x90);
#endif
