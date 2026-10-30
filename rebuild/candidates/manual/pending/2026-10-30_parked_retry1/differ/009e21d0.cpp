// GFACoordToVCoord(const C2DVector&, C2DVector&) -- convert an actual (render-target pixel) 2D point to the virtual 65536-unit space, one axis at a time
// Grade: DIFFER(106v106) -- retry1: `(float)g_Virtual* * x` spelling fixes both fld qword/fmul operand orders; left: in the inlined GFAYToVY `mov ecx,eax` is scheduled before `fstp [esp+0Ch]` (retail after), same as 0x009E1DA0 sibling 0x009E1DD0.
// Leaf: ?GFACoordToVCoord@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E21D0, length 106 bytes (+ int3 padding).
// PDB (FableWin): ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z -- __fastcall, source in ecx, result in edx. C2DVector = {X, Y}.
// Inlined: the float converters GFAXToVX (0x009E1DA0, landed: VirtualWidth * x / width after the dimension call) and
// GFAYToVY (0x009E1DD0: VirtualHeight * y formed BEFORE the dimension call and parked as a float), each re-fetching
// GFGetSystemManager (0x009A4EC0; +0x60 = display manager) and CDisplayManager::GetRenderTargetDimensions
// (0x009BEDC0, C2DExtentsI by hidden pointer). Globals: virtual width / height, doubles 65536.0 at 0x013961E8 / 0x013961F0.
// Tried: the landed one-expression GFAXToVX body (product hoisted above the dimension call and spilled as a double),
// the dimension result bound to a const reference first (this source: the call now comes first, the operand order
// and the `fidiv` source still differ).
// Also tried (this source): the X product through an inline `ScaleToVirtual(double virtualSize, x, const C2DExtentsI& size)`
// helper -- VC evaluates its arguments right to left, so the dimension call now comes first and the divide reads
// `[eax]` like retail; then `x * virtualSize` / `y * g` (canonicalised back to the same order, 0x009E21D0).
struct C2DExtentsI
{
    long X;
    long Y;
};

class C2DVector
{
public:
    float X;
    float Y;
};

class CDisplayManager
{
public:
    C2DExtentsI GetRenderTargetDimensions() const;     // 0x009BEDC0
};

class CSystemManager
{
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* m_pDisplayManager;                // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0
extern double g_VirtualWidth;                          // 0x013961E8 (65536.0)
extern double g_VirtualHeight;                         // 0x013961F0 (65536.0)

inline float ScaleToVirtual(double virtualSize, float x, const C2DExtentsI& size)
{
    return (float)virtualSize * x / size.X;
}

inline float __fastcall GFAXToVX(float x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return ScaleToVirtual(g_VirtualWidth, x, display->GetRenderTargetDimensions());
}

inline float __fastcall GFAYToVY(float y)
{
    CSystemManager* system = GFGetSystemManager();
    float scaled = (float)g_VirtualHeight * y;
    CDisplayManager* display = system->m_pDisplayManager;
    return scaled / display->GetRenderTargetDimensions().Y;
}

void __fastcall GFACoordToVCoord(const C2DVector& a, C2DVector& v)
{
    v.X = GFAXToVX(a.X);
    v.Y = GFAYToVY(a.Y);
}
