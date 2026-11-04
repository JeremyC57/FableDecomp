// FadeOutAndKillEntity(const CScriptThing& thing, float seconds, bool killAfterFade, bool killPersistent) -- fade the thing's alpha from 1 to 0 over the given seconds (one step per script frame at 15 frames a second), then optionally remove it
// Leaf: ?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE716, 148 bytes (`ret 8`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Model: CGameScriptInterface through pDSTGame (0x0143E8F8): slot 7 NewScriptFrame (+0x1C), 108
//   RemoveThing (+0x1B0), 510 EntitySetAlpha (+0x7F8). Constants: 15.0f 0x0124E640, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Identity: Ego_r.pdb `?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z`.
// Callees: CScriptThing slot 75 IsAlive (+0x12C); the interface slots above.
// Shape: the step `1 / (seconds * 15)` and alpha live in the frame/argument slot; the loop is a do-while that leaves
//   (`je` past the loop) as soon as the thing is dead, then the optional RemoveThing(thing, false, killPersistent).
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

void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float seconds, bool killAfterFade, bool killPersistent)
{
    float step = 1.0f / (seconds * 15.0f);
    float alpha = 1.0f;
    do {
        if (!thing.IsAlive())
            break;
        pDSTGame->EntitySetAlpha(thing, alpha, true);
        pDSTGame->NewScriptFrame();
        alpha -= step;
    } while (alpha > 0.0f);
    if (killAfterFade && thing.IsAlive())
        pDSTGame->RemoveThing(thing, false, killPersistent);
}