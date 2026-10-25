// GFACoordToVCoord(const C2DCoordI&, C2DCoordI&) -- integer overload: convert an actual (render-target pixel) coordinate to the virtual space through the float converters, truncating each axis
// Leaf: ?GFACoordToVCoord@
// Recipe (agent902, pass 2): the double global is read as `(float)g_Virtual...` in each product (`(float)virtualSize * x`,
//   `y *= (float)g` / `float scaled = (float)g * y`). Without /Op the cast emits nothing, but VC7.1 then loads the global
//   first and takes the coordinate as the memory operand (`fld qword [g] ; fmul dword / fimul dword [x]`), the order the
//   uncast double product canonicalises away (the landed 0x009E1DA0 needed a never-cached (cv) qualifier for it; this body matches it too).
//   The Y converter reads the system manager into a local, forms the product, then reads the display manager into a
//   named local (eax, copied to ecx at the call) -- that places the product's `fstp` after the result-slot push as retail.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2240, length 116 bytes (+ int3 padding).
// PDB (FableWin): ?GFACoordToVCoord@@YIXABVC2DCoordI@@AAV1@@Z -- __fastcall, source in ecx, result in edx. The catalog
// label "GFABoxToVBox" is propagated and wrong (two axes). C2DCoordI = {X, Y} (longs); __ftol2 (0x00BFEA70) truncates.
// Each axis is rounded to the float argument first (`fild [edi] ; fstp [esp+8]`): an explicit `(float)` cast.
// Inlined: the float converters GFAXToVX (0x009E1DA0, landed: VirtualWidth * x / width after the dimension call) and
// GFAYToVY (0x009E1DD0: VirtualHeight * y formed BEFORE the dimension call and parked as a float), each re-fetching
// GFGetSystemManager (0x009A4EC0; +0x60 = display manager) and CDisplayManager::GetRenderTargetDimensions
// (0x009BEDC0, C2DExtentsI by hidden pointer). Globals: virtual width / height, doubles 65536.0 at 0x013961E8 / 0x013961F0.
struct C2DExtentsI
{
    long X;
    long Y;
};

class C2DCoordI
{
public:
    long X;
    long Y;
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
    y *= (float)g_VirtualHeight;
    CDisplayManager* display = system->m_pDisplayManager;
    return y / display->GetRenderTargetDimensions().Y;
}

void __fastcall GFACoordToVCoord(const C2DCoordI& a, C2DCoordI& v)
{
    v.X = (long)GFAXToVX((float)a.X);
    v.Y = (long)GFAYToVY((float)a.Y);
}