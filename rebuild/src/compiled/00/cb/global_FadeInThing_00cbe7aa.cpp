// FadeInThing(CScriptThing& thing, float seconds) -- fade the thing's alpha from 0 to 1 over the given seconds (one step per script frame at 15 frames a second)
// Leaf: ?FadeInThing@@YIXAAVCScriptThing@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE7AA, 106 bytes (`ret 4`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Model: CGameScriptInterface through g_pGameScriptInterface (0x0143E8F8): slot 7 NewScriptFrame (+0x1C), 108
//   RemoveThing (+0x1B0), 510 EntitySetAlpha (+0x7F8). Constants: 15.0f 0x0124E640, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Identity: Ego_r.pdb `?FadeInThing@@YIXAAVCScriptThing@@M@Z`.
// Callees: CScriptThing slot 75 IsAlive (+0x12C); EntitySetAlpha / NewScriptFrame.
// Shape: do-while `alpha < 1` with the alpha set only while the thing is alive (the frame still advances).
#pragma optimize("s", on)
class CCharString {
public:
    void* m_pStringData;
};
class C3DVector {
public:
    float X, Y, Z;
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

void __fastcall FadeInThing(CScriptThing& thing, float seconds)
{
    float step = 1.0f / (seconds * 15.0f);
    float alpha = 0.0f;
    do {
        if (thing.IsAlive())
            g_pGameScriptInterface->EntitySetAlpha(thing, alpha, true);
        g_pGameScriptInterface->NewScriptFrame();
        alpha += step;
    } while (alpha < 1.0f);
}