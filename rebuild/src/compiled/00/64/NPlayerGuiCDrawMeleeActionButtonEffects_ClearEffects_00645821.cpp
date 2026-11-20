// NPlayerGui::CDrawMeleeActionButtonEffects::ClearEffects() -- clear the drawer's particle effects through the static CDrawBase::ClearEffects(vector&)
// Leaf: ?ClearEffects@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645821, `add ecx,28h ; jmp 0x006445BA`.
// Name: Ego_r.pdb 0x00688238 + FableWin.pdb 0x020FDC81 ?ClearEffects@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXXZ (public, non-virtual; Ego_r puts the
//   CDrawHealth / CDrawStamina pair 8 bytes apart, as retail 0x00644644 / 0x0064464C); the one landed caller 0x00438230 (CPlayerGui)
//   already binds this spelling at this address (callers.py / reloc_bindings). Was the placeholder ?GetTarget_00645821@CClass_00645821@@UAEPAVCBaseObject@@XZ (NF177, q11775).
// Layout: the landed CDrawMeleeActionButtonEffects rows 0x00646580 / 0x0064BE45 (Effects +0x28); CDrawBase is the shared engine/NPlayerGui_CDrawBase.h (0x28 bytes). The class has no shared header (only forward
//   declarations in engine/CPlayerGui.h), so it is modelled here in the landed sibling 0x00645605 shape (NF177 NOTES).
// Callees: NPlayerGui::CDrawBase::ClearEffects 0x006445BA (static __fastcall, the vector in ecx).
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
class CDrawMeleeActionButtonEffects : public CDrawBase {
public:
    void ClearEffects();
private:
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;   // +0x28
};
}

void NPlayerGui::CDrawMeleeActionButtonEffects::ClearEffects()
{
    CDrawBase::ClearEffects(Effects);
}
