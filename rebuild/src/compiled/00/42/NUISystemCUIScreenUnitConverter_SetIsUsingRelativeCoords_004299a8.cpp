// NUISystem::CUIScreenUnitConverter::SetIsUsingRelativeCoords(bool relative) -- switch coordinate mode; entering relative mode re-reads the render-target size
// NF114 (2026-11-20, q11634..q11638): CUIScreenUnitConverter's five statics are private (`@@0`: Ego_d.pdb PUB32 + FableWin.pdb
//   LF_STMEMBER attrs = private; DefaultBackBufferWidth / Height are the original names of the 640 / 480 words 0x01375CD4 / 0x01375CD8).
// Leaf: ?SetIsUsingRelativeCoords@CUIScreenUnitConverter@NUISystem@@SIX_N@Z
// Retail: Fable.exe @ 0x004299A8, 60 bytes, `ret` (static __fastcall, flag in cl); one caller (0x0041E4C2); Ego_r.pdb
// ?SetIsUsingRelativeCoords@CUIScreenUnitConverter@NUISystem@@SIX_N@Z.
// Body: if the static flag (0x013B8768) differs, store it; when now true, GFGetSystemManager() (0x009A4EC0, __fastcall)
// ->PDisplayManager (+0x60, CCountedPointer data) ->GetRenderTargetDimensions() (0x009BEDC0, returns const C2DExtentsI by
// hidden pointer: X = +0x194, Y = +0x198), then the static back-buffer width/height floats (0x013B876C / 0x013B8770)
// = (float)X / (float)Y (`fild / fstp`).
// Layout: Ego_r.pdb CSystemManager PDisplayManager +0x60 (CCountedPointer<CDisplayManager>); C2DExtentsI {long X, Y} with a
// constructor (returned through memory).
// Shape: the system manager bound to a local first, so the call precedes the hidden return-slot push.
// Opt: size TU with frame pointers (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
class C2DExtentsI {
public:
    long X;
    long Y;
    C2DExtentsI() {}
};
class CDisplayManager {
public:
    const C2DExtentsI GetRenderTargetDimensions();       // 0x009BEDC0
};
class CSystemManager {
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;                    // +0x60 CCountedPointer<CDisplayManager>::Data
};
CSystemManager* __fastcall GFGetSystemManager();         // 0x009A4EC0
namespace NUISystem {
class CUIScreenUnitConverter {
private:
    static bool  IsUsingRelativeCoords;                  // 0x013B8768
    static float BackBufferWidth;                        // 0x013B876C
    static float BackBufferHeight;                       // 0x013B8770
public:
    static void __fastcall SetIsUsingRelativeCoords(bool relative);
};
void __fastcall CUIScreenUnitConverter::SetIsUsingRelativeCoords(bool relative)
{
    if (IsUsingRelativeCoords != relative)
    {
        IsUsingRelativeCoords = relative;
        if (relative)
        {
            CSystemManager* system = GFGetSystemManager();
            C2DExtentsI dimensions = system->PDisplayManager->GetRenderTargetDimensions();
            BackBufferWidth = (float)dimensions.X;
            BackBufferHeight = (float)dimensions.Y;
        }
    }
}
}