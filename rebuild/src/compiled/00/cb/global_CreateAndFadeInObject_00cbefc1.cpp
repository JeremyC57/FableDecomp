// CreateAndFadeInObject(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, float unused, float fadeTime) -- create the object through the game script interface, then fade it in from alpha 0 to 1 over fadeTime seconds (15 steps a second, one script frame each)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?CreateAndFadeInObject@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBEFC1, 143 bytes, `__fastcall` global returning CScriptThing by value
//   (hidden result pointer in ecx, defName in edx, four stack arguments: `ret 10h`)
//   (PDB ?CreateAndFadeInObject@@YI?AVCScriptThing@@ABVCCharString@@ABVC3DVector@@0MM@Z).
// Callees: CGameScriptInterface (global @ 0x0143E8F8, vtable 0x01260F0C) slot 98 CreateObject (+0x188, 0x00894820), slot 510 EntitySetAlpha (+0x7f8, 0x0089DA70), slot 7
//   NewScriptFrame (+0x1c, 0x006E7410); CScriptThing copy constructor @ 0x004ABE90, ~CScriptThing @ 0x004AA840.
// Data: 15.0f @ 0x0124E640, 1.0f @ 0x0122DED8.
// Shape: frame-pointer size TU. The step `1.0f / (15.0f * fadeTime)` (`fld [fadeTime] ; fmul [15] ; fdivr [1]`) and the
//   alpha are float locals homed in the dead position / scriptName argument slots; the alpha starts at the constant 0,
//   so the `while (alpha < 1.0f)` entry test is folded away (the bottom test is `fcomp ; test ah,5 ; jnp` to the top);
//   the fourth argument is unused. No NRVO: the local is copied into the hidden result and destroyed.
// Label: the catalog name "CreateAndFadeInCreature" is propagated from the twin 0x00CBEF30; this body calls CreateObject
//   and is the PDB's CreateAndFadeInObject (same signature).
class CCharString {
public:
    void* m_pStringData;
};
class C3DVector { public:
    float X, Y, Z;
};
class CScriptThing { public:
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
    void* m_vtable;
    void* m_pImpData;
    void* m_pImpInfo;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

CScriptThing __fastcall CreateAndFadeInObject(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, float unused, float fadeTime)
{
    CScriptThing thing = g_pGameScriptInterface->CreateObject(defName, position, scriptName);
    float step = 1.0f / (15.0f * fadeTime);
    float alpha = 0.0f;
    while (alpha < 1.0f) {
        g_pGameScriptInterface->EntitySetAlpha(thing, alpha, true);
        g_pGameScriptInterface->NewScriptFrame();
        alpha += step;
    }
    return thing;
}