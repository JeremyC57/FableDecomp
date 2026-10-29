// GFVYToAY(float) -- convert a virtual (65536-unit) Y to actual render-target pixels: y * renderTargetHeight / VirtualHeight
// Leaf: ?GFVYToAY@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1E40, length 41 bytes (+ int3 padding).
// PDB (FableWin): ?GFVYToAY@@YIMM@Z -- __fastcall with the float on the stack (`ret 4`). The catalog label
// "GFAYToVY" is propagated and wrong: GFAYToVY is 0x009E1DD0 (VirtualHeight * y / height); this body
// multiplies by the render-target height and divides by the virtual height.
// Callees: GFGetSystemManager (0x009A4EC0; +0x60 = display manager), CDisplayManager::GetRenderTargetDimensions
// (0x009BEDC0, __fastcall, C2DExtentsI {X, Y} by hidden pointer into the 8-byte frame temporary).
// Global: the virtual height, double 65536.0 at 0x013961F0.
// Shape: `fild [eax+4] ; fmul [y] ; fdiv qword [g]` -- the int dimension is loaded first because it can only be
// converted on load, then the double divide. The display manager is a named local: written inline in the
// expression, the hidden result slot's `lea ; push` is hoisted above the GFGetSystemManager call.
class C2DExtentsI
{ public:
    long X;
    long Y;
};

class CDisplayManager
{
public:
    const C2DExtentsI GetRenderTargetDimensions();     // 0x009BEDC0
};

class CSystemManager
{
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* m_pDisplayManager;                // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0
extern double g_VirtualHeight;                         // 0x013961F0 (65536.0)

float __fastcall GFVYToAY(float y)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (float)(y * display->GetRenderTargetDimensions().Y / g_VirtualHeight);
}