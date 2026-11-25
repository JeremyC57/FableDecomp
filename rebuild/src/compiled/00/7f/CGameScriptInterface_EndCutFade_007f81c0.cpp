#include "rebuild_abi.h"
// Leaf: EndCutFade
// CGameScriptInterface::EndCutFade @ 0x007f81c0
// push esi; mov esi,ecx; call h1; push 1; mov ecx,esi; call h2; pop esi; ret
struct FableEndCut_007f81c0
{
    void FABLE_FASTCALL h1();
    void EndCutFade();
};
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x007F8160 ?SetTriggerState@CTCActivationReceptorRelay@@AAEX_N@Z (was ?h2@FableEndCut_007f81c0@@QAEXH@Z)
struct FableEndCut_007f81c0;
class CTCActivationReceptorRelay { public: private: void SetTriggerState(bool); public: friend struct ::FableEndCut_007f81c0; };
void FableEndCut_007f81c0::EndCutFade()
{
    this->h1();
    reinterpret_cast<CTCActivationReceptorRelay*>(this)->SetTriggerState(1);
}