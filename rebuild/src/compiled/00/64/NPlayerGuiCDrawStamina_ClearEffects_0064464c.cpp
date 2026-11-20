// NPlayerGui::CDrawStamina::ClearEffects() -- clear the drawer's particle effects through the static CDrawBase::ClearEffects(vector&)
// Leaf: ?ClearEffects@CDrawStamina@NPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064464C, `push esi ; mov esi,ecx ; lea ecx,[esi+28h] ; call 0x006445BA ; lea ecx,[esi+34h] ; pop esi ; jmp 0x006445BA`.
// Name: Ego_r.pdb 0x006876FD + FableWin.pdb 0x020DC2BE ?ClearEffects@CDrawStamina@NPlayerGui@@QAEXXZ (public, non-virtual; Ego_r puts the
//   CDrawHealth / CDrawStamina pair 8 bytes apart, as retail 0x00644644 / 0x0064464C); the one landed caller 0x00438230 (CPlayerGui)
//   already binds this spelling at this address (callers.py / reloc_bindings). Was the placeholder ?Destroy@Pair@@QAEXXZ (NF177, q11775).
// Layout: the landed CDrawStamina ctor 0x0064E24F (Effects +0x28, IncreaseEffects +0x34); CDrawBase is the shared engine/NPlayerGui_CDrawBase.h (0x28 bytes). The class has no shared header (only forward
//   declarations in engine/CPlayerGui.h), so it is modelled here in the landed sibling 0x00645605 shape (NF177 NOTES).
// Callees: NPlayerGui::CDrawBase::ClearEffects 0x006445BA (static __fastcall, the vector in ecx).
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
class CDrawStamina : public CDrawBase {
public:
    void ClearEffects();
private:
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;           // +0x28
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > IncreaseEffects;   // +0x34
};
}

void NPlayerGui::CDrawStamina::ClearEffects()
{
    CDrawBase::ClearEffects(Effects);
    CDrawBase::ClearEffects(IncreaseEffects);
}
