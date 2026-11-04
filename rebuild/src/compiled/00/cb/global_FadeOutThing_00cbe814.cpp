// FadeOutThing(CScriptThing& thing, float seconds, float targetAlpha) -- fade the thing's alpha down from 1 until it reaches targetAlpha, in steps of 1/(seconds*15) per script frame
// Leaf: ?FadeOutThing@@YIXAAVCScriptThing@@MM@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE814, 107 bytes (`ret 8`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Model: CGameScriptInterface through pDSTGame (0x0143E8F8): slot 7 NewScriptFrame (+0x1C), 108
//   RemoveThing (+0x1B0), 510 EntitySetAlpha (+0x7F8). Constants: 15.0f 0x0124E640, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Identity: Ego_r.pdb `?FadeOutThing@@YIXAAVCScriptThing@@MM@Z`.
// Callees: CScriptThing slot 75 IsAlive (+0x12C); EntitySetAlpha / NewScriptFrame.
// Shape: a top-tested `while (alpha > targetAlpha)` (the `jmp` to the test), alpha starts at 1.
#pragma optimize("s", on)
class CCharString {
public:
    void* m_pStringData;
};
#include "engine/C3DVector.h"
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

void __fastcall FadeOutThing(CScriptThing& thing, float seconds, float targetAlpha)
{
    float step = 1.0f / (seconds * 15.0f);
    float alpha = 1.0f;
    while (alpha > targetAlpha) {
        if (thing.IsAlive())
            pDSTGame->EntitySetAlpha(thing, alpha, true);
        pDSTGame->NewScriptFrame();
        alpha -= step;
    }
}