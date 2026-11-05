// CTCFadeOutAndIn::FadeOut(float seconds, bool immediately) -- start a fadeout over seconds (frames-per-second * seconds rounded to a frame count), the alpha set at once when immediately, else taken from the thing's graphic
// Leaf: ?FadeOut@CTCFadeOutAndIn@@QAEXM_N@Z
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Flags: /O2 /Oy /W3
// Retail-asm: 0x006C3B87 `fld dword ptr [esp+8]` + 0x006C3B8B `fistp dword ptr [esp]` -- the engine's inline-assembly ROUND helper: the frame count product is stored into the dead `seconds` argument slot and reloaded for a bare dword fistp into the `push ecx` frame word; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` without the reload, so the dword fistp is the original's inline-asm.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongNear.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C3B70, length 89 bytes. Twin of 0x006C3B70 / 0x006C3BD0 (FadeOut / FadeIn).
// Layout: PDB CTCFadeOutAndIn (40 bytes) over CTCBase (engine/CTCBase.h): FadingOut +0xc, FadingIn +0xd, Alpha +0x10,
// FramesLeftForFade +0x14. The thing's graphic record hangs off +0x64 with its alpha byte at +0x5a.
// Shape: the frame count is `fild CMainGameComponent::GetConstantFPS() (0x01375550) ; fmul seconds`; the graphic alpha is `fild` of the
// zero-extended byte through the dead `immediately` slot times 1/255 (pool constant @ 0x01231724).
#include "engine/CTCBase.h"
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    unsigned char _pad[0x5a];
    unsigned char m_alpha;                         // +0x5a
};
}
using NGraphicAppearance::CTCGraphicAppearance;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
class CTCFadeOutAndIn : public CTCBase { public:
    bool  FadingOut;                               // +0x0c
    bool  FadingIn;                                // +0x0d
    float Alpha;                                   // +0x10
    long  FramesLeftForFade;                       // +0x14
    void FadeIn(float seconds, bool immediately);  // 0x006C3BD0
    void FadeOut(float seconds, bool immediately); // 0x006C3B70
};
// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains inline-asm in
// place; its by-value parameter is homed in the dead `seconds` argument slot (`fstp [esp+8] ; fld [esp+8]`)
#include "engine/GFFloatToLongNear.h"
void CTCFadeOutAndIn::FadeOut(float seconds, bool immediately)
{
    FadingOut = true;
    FadingIn = false;
    FramesLeftForFade = GFFloatToLongNear(CMainGameComponent::GetConstantFPS() * seconds);
    if (immediately)
        Alpha = 1.0f;
    else if (ParentThing->GetTCGraphicAppearance())
        Alpha = ParentThing->GetTCGraphicAppearance()->m_alpha * (1.0f / 255.0f);
}