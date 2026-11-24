#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWMELEEACTIONBUTTONEFFECTS_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWMELEEACTIONBUTTONEFFECTS_H
// NPlayerGui::CDrawMeleeActionButtonEffects (new, NF239 2026-11-22): Ego_d.pdb TPI 0x3DF4F (field list 0x3DF4E): public base
// NPlayerGui::CDrawBase at 0; methods ctor (CPlayerGui&, vector<CDrawBase*>&), Update, Draw(const CEnginePrimitivePrepareInfo&),
// ClearEffects, virtual ClearPrimitivesIfNecessary, SetFlourishEffect(long); private members Effects (vector<CIntelligentPointer<
// NParticleEngine::CParticleEmitter> >) / FlashTimer / ComboLoadingIconTimer / FlourishCounter / FlourishEffect / PMeleeAbilityDef
// (CDefPointer<const CMeleeCombatAbilityDef>) at Ego_d 48 / 68 / 72 / 76 / 80 / 84. Retail offsets (CDrawBase retail 0x28, STLport
// vector 12 bytes): +0x28 / +0x34 / +0x38 / +0x3C / +0x40 / +0x44 (Ego_r layout less the two 16 -> 12-byte vectors; retail
// CPlayerGui::SetFlourishEffect 0x00437650 stores +0x40). SetFlourishEffect is inline: FableWin has an out-of-line
// ?SetFlourishEffect@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXJ@Z, Ego_r and retail none (the CPlayerGui wrapper stores in place).
// Only the members up to the accessors' needs are typed; PMeleeAbilityDef is declared when the retail def model is on.
#include "engine/NPlayerGui_CDrawBase.h"
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
class CMeleeCombatAbilityDef;
#endif
template<class T> class CIntelligentPointer;
namespace NParticleEngine { class CParticleEmitter; }
namespace NPlayerGui {
class CDrawMeleeActionButtonEffects : public CDrawBase {
public:
    void SetFlourishEffect(long effect) { FlourishEffect = effect; }
private:
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;   // +0x28
    long FlashTimer;                                                       // +0x34
    long ComboLoadingIconTimer;                                            // +0x38
    long FlourishCounter;                                                  // +0x3c
    long FlourishEffect;                                                   // +0x40
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef;            // +0x44
#endif
};
}
#endif
