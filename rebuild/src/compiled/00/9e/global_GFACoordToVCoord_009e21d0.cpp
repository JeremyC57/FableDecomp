// GFACoordToVCoord(const C2DVector&, C2DVector&) -- convert an actual (render-target pixel) coordinate to virtual coordinates: v = (VIRTUAL_SCREEN_WIDTH * a.X / width, VIRTUAL_SCREEN_HEIGHT * a.Y / height)
// Leaf: ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E21D0, length 106 bytes (+ int3 padding), __fastcall (a in ecx, v in edx).
// PDB (Ego_r): ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z; it inlines its two same-TU helpers GFAXToVX
// (?GFAXToVX@@YIMM@Z, out of line @ 0x009E1DA0) and GFAYToVY (?GFAYToVY@@YIMM@Z, out of line @ 0x009E1DD0).
// Callees: GFGetSystemManager @ 0x009A4EC0 (?GFGetSystemManager@@YIPAVCSystemManager@@XZ; display manager counted
// pointer at +0x60 through the PDB inline CSystemManager::GetDisplayManager()), CDisplayManager::
// GetRenderTargetDimensions @ 0x009BEDC0 (Ego_r ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ,
// a const C2DExtentsI by hidden pointer into the 8-byte frame temporary).
// Globals: VIRTUAL_SCREEN_WIDTH 0x013961E8 / VIRTUAL_SCREEN_HEIGHT 0x013961F0 (doubles, Ego_r data symbols ?..@@3NA).
// Shape: each half copies its float argument into the frame slot at [esp+8] before the GFGetSystemManager call.
// X half: `((float)VIRTUAL_SCREEN_WIDTH * x) / dims.X` -- the (float) cast keeps `fld qword [W] ; fmul dword [x]`
// (the plain double product loads x first) and the product is evaluated after the dimensions call (`fidiv [eax]`).
// Y half: retail computes the product BEFORE the dimensions call and spills it into y's slot between `push ecx`
// and `mov ecx,eax` (`fld qword [H] ; fmul [y] ; ... push ecx ; fstp [esp+0ch] ; mov ecx,eax`). That is an
// explicitly truncated operand: `(float)((float)VIRTUAL_SCREEN_HEIGHT * y) / dims.Y`. The forms `y = (float)H * y;`,
// `y *= (float)H`, a named `scaled` float, a named display pointer or `operator->` all store it after the
// `mov ecx,eax`; `(float)(H * y)` loads y first; a named `dims` local stores it before the push.
// Layout: CSystemManager derives from CInitBaseClass (vptr) and has no generated engine header; the accessed
// prefix is modelled locally with its Ego_r member name (PDisplayManager, CCountedPointer<CDisplayManager> +0x60).
#include "engine/CCountedPointer.h"

class C2DExtentsI {
public:
    long X;                                                     // +0x0
    long Y;                                                     // +0x4
    C2DExtentsI();
    C2DExtentsI(long x, long y);
    C2DExtentsI(const C2DExtentsI& other);
};

class CDisplayManager {
public:
    const C2DExtentsI GetRenderTargetDimensions();              // 0x009BEDC0
};

class CSystemManager {
public:
    unsigned char _pad_0x00[0x60];                              // CInitBaseClass base .. LibDebugManager / PInputManager
    CCountedPointer<CDisplayManager> PDisplayManager;          // +0x60
    CDisplayManager* GetDisplayManager() { return PDisplayManager.Get(); }   // Ego_r ?GetDisplayManager@CSystemManager@@QAEPAVCDisplayManager@@XZ
};

CSystemManager* __fastcall GFGetSystemManager();                // 0x009A4EC0
extern double VIRTUAL_SCREEN_WIDTH;                             // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                            // 0x013961F0

class C2DVector {
public:
    float X;                                                    // +0x0
    float Y;                                                    // +0x4
};

inline float __fastcall GFAXToVX(float x)                       // 0x009E1DA0 out of line
{
    CSystemManager* system = GFGetSystemManager();
    return ((float)VIRTUAL_SCREEN_WIDTH * x) / system->GetDisplayManager()->GetRenderTargetDimensions().X;
}

inline float __fastcall GFAYToVY(float y)                       // 0x009E1DD0 out of line
{
    CSystemManager* system = GFGetSystemManager();
    return (float)((float)VIRTUAL_SCREEN_HEIGHT * y) / system->GetDisplayManager()->GetRenderTargetDimensions().Y;
}

void __fastcall GFACoordToVCoord(const C2DVector& a, C2DVector& v)
{
    v.X = GFAXToVX(a.X);
    v.Y = GFAYToVY(a.Y);
}