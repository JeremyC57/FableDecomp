#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWHEALTH_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWHEALTH_H
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWHEALTH_PASS28
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CIntelligentPointer.h"
class CEnginePrimitivePrepareInfo;
namespace NParticleEngine { class CParticleEmitter; }
namespace NPlayerGui {
// Ego_r CDrawHealth fields after the CDrawBase subobject. Both STLport vectors are four bytes smaller than the donor equivalents.
class CDrawHealth : public CDrawBase {
public:
#ifdef FABLE_RETAIL_CDRAWHEALTH_ORIGINAL_EFFECTS_PASS28
    // Original GUI helper at 00644644 forwards Effects to CDrawBase, matching the FableWin member call.
    void ClearEffects() { CDrawBase::ClearEffects(Effects); }
#endif
    void Draw(const CEnginePrimitivePrepareInfo& info); // Ego_r and FableWin, retail 0x00647AB4
private:
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;
    float PreviousHealth;
    CFadeInFadeOutBase HealthIncreaseFade;
    float HealthReduction;
    float HealthReductionAlpha;
    float HealthFlashTimer;
    float LowHealthPulseScale;
    float LowHealthPulseDirection;
    long PreviousEffect;
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawHealth) == 0x6c);
#endif
#endif
