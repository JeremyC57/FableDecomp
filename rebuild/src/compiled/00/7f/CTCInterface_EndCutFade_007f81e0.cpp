// CTCInterface::EndCutFade() -- clears the cut-fade flag (+0xc, inline helper @ 0x007F7700) and posts the end-of-fade message 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F81E0, length 19 bytes.
// Callees: flag clear @ 0x007F7700 (`mov byte ptr [ecx+0Ch],0`), message post @ 0x007F8160 (manifest "TCInterface_HandleEndCutFade",
// one immediate argument 0).
// Class attribution: manifest label "CGameScriptInterface::EndCutFade" is propagated; the callee's label names a TC interface,
// so the class is provisional. Shape: two thiscalls on `this` kept in esi.
struct CTCInterface {
    void EndCutFade();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x007F7700 ?Deactivate@CTCActivationReceptorBase@@UAEXXZ (was ?ClearCutFade@CTCInterface@@QAEXXZ)
//   0x007F8160 ?SetTriggerState@CTCActivationReceptorRelay@@AAEX_N@Z (was ?HandleEndCutFade@CTCInterface@@QAEXJ@Z)
struct CTCInterface;
class CTCActivationReceptorBase { public: virtual void Deactivate(); };
class CTCActivationReceptorRelay { public: private: void SetTriggerState(bool); public: friend struct ::CTCInterface; };
void CTCInterface::EndCutFade()
{
    reinterpret_cast<CTCActivationReceptorBase*>(this)->CTCActivationReceptorBase::Deactivate();
    reinterpret_cast<CTCActivationReceptorRelay*>(this)->SetTriggerState(0);
}