// GFVXToAX(float) -- convert a virtual (65536-unit) X to actual render-target pixels: x * renderTargetWidth / VirtualWidth
// Leaf: ?GFVXToAX@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1E10, length 41 bytes (+ int3 padding).
// PDB (FableWin): ?GFVXToAX@@YIMM@Z -- __fastcall with the float on the stack (`ret 4`). The catalog label
// "GFAXToVX" is propagated and wrong: GFAXToVX is the landed 0x009E1DA0 (VirtualWidth * x / width); this body
// multiplies by the render-target width and divides by the virtual width.
// Callees: GFGetSystemManager (0x009A4EC0; +0x60 = display manager), CDisplayManager::GetRenderTargetDimensions
// (0x009BEDC0, __fastcall, C2DExtentsI {X, Y} by hidden pointer into the 8-byte frame temporary).
// Global: the virtual width, double 65536.0 at 0x013961E8.
// Shape: `fild [eax] ; fmul [x] ; fdiv qword [g]` -- the int dimension is loaded first because it can only be
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
extern double g_VirtualWidth;                          // 0x013961E8 (65536.0)

float __fastcall GFVXToAX(float x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (float)(x * display->GetRenderTargetDimensions().X / g_VirtualWidth);
}