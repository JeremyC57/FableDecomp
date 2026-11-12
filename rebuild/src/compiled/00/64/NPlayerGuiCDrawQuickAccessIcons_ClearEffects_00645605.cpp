// NPlayerGui::CDrawQuickAccessIcons::ClearEffects() -- clear the quick-access icon drawer's particle effects: CDrawBase::ClearEffects(Effects)
// Leaf: ?ClearEffects@CDrawQuickAccessIcons@NPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645605, 8 bytes: `add ecx,7Ch ; jmp 0x006445BA` (sibling call).
// Flags: none
// Name: Ego_r.pdb 0x0068822D + FableWin.pdb 0x020FB391 ?ClearEffects@CDrawQuickAccessIcons@NPlayerGui@@QAEXXZ (Ego_r puts it right after
//   DrawEffectAtPos 0x0068804A, as retail 0x00645605 follows DrawEffectAtPos 0x0064546F). Body: the static __fastcall
//   CDrawBase::ClearEffects 0x006445BA on Effects, retail +0x7C = Ego_r +0x88 minus the three 4-byte-smaller STLport vectors before it
//   (CDrawBase::PrimitiveHandles, SpecialPrimitiveHandles, QuickAccessSpellCrossHandles). Name-fix pass (2026-11-12): was the stand-in
//   CClass_00645605::GetTarget_00645605 (module _unattributed); the class model below is local (no shared header for
//   CDrawQuickAccessIcons) on the shared engine/NPlayerGui_CDrawBase.h.
// Callees: NPlayerGui::CDrawBase::ClearEffects 0x006445BA (jmp).
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
class CDrawQuickAccessIcons : public CDrawBase {
public:
    void ClearEffects();
private:
    unsigned char _pad28[0x7c - 0x28];             // +0x28 ChargingSoundID .. QuickAccessSpellCrossHandles (Ego_r.pdb)
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;   // +0x7c
};
}

void NPlayerGui::CDrawQuickAccessIcons::ClearEffects()
{
    CDrawBase::ClearEffects(Effects);
}
