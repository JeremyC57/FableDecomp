#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWSTAMINA_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWSTAMINA_H
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CIntelligentPointer.h"

class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {

// Native ctor 0064E24F and both PDBs agree on CDrawBase and the five stamina fields.
class CDrawStamina : public CDrawBase {
public:
    CDrawStamina(CPlayerGui&, std::vector<CDrawBase*>&);
    void Draw(const CEnginePrimitivePrepareInfo& info);
private:
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > IncreaseEffects;
    long PreviousStamina;
    long PreviousMaxStamina;
    CFadeInFadeOutBase StaminaIncreaseFade;
};
}

FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawStamina) == 0x64);
#endif
