// GFABoxToVBox(const C2DBoxI&, C2DBoxI&) -- integer overload: convert an actual (render-target pixel) box to the virtual 65536-unit space, each corner coordinate through inlined long converters
// Leaf: ?GFABoxToVBox@
// Recipe (agent902, pass 2): the double global is read as `(float)g_Virtual...` in each product (`(float)virtualSize * x`,
//   `y *= (float)g` / `float scaled = (float)g * y`). Without /Op the cast emits nothing, but VC7.1 then loads the global
//   first and takes the coordinate as the memory operand (`fld qword [g] ; fmul dword / fimul dword [x]`), the order the
//   uncast double product canonicalises away (the landed 0x009E1DA0 needed a never-cached (cv) qualifier for it; this body matches it too).
//   The Y converter reads the system manager into a local, forms the product, then reads the display manager into a
//   named local (eax, copied to ecx at the call) -- that places the product's `fstp` after the result-slot push as retail.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E20F0, length 219 bytes (+ int3 padding).
// PDB (FableWin): ?GFABoxToVBox@@YIXABVC2DBoxI@@AAV1@@Z -- __fastcall, source in ecx, result in edx.
// C2DBoxI = {TLX, TLY, BRX, BRY} (longs).
// Inlined: a LONG-parameter converter pair (GFAXToVX / GFAYToVY overloads, inline-only, absent from the PDB name
// sets; the float pair is 0x009E1DA0 / 0x009E1DD0), each re-fetching the system manager (0x009A4EC0) and the
// render-target dimensions (0x009BEDC0); __ftol2 (0x00BFEA70) truncates each result. Globals: virtual width /
// height, doubles 65536.0 at 0x013961E8 / 0x013961F0.
// Shape: the long coordinate is homed in the frame word [esp+8] before the calls. X: `fld g ; fimul [x] ; fidiv
// width` after the dimension call (one expression). Y: the scaled value is formed BEFORE the dimension call and parked
// as a FLOAT in the same frame word (`fstp [esp+0Ch]` after the push), then `fild height ; fdivr` -- a separate
// float statement, as in the float GFAYToVY.
class C2DExtentsI
{ public:
    long X;
    long Y;
};

class C2DBoxI
{
public:
    long TLX;
    long TLY;
    long BRX;
    long BRY;
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
extern double g_VirtualHeight;                         // 0x013961F0 (65536.0)

inline long ScaleToVirtual(double virtualSize, long x, const C2DExtentsI& size)
{
    return (long)((float)virtualSize * x / size.X);
}

inline long __fastcall GFAXToVX(long x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return ScaleToVirtual(g_VirtualWidth, x, display->GetRenderTargetDimensions());
}

inline long __fastcall GFAYToVY(long y)
{
    CSystemManager* system = GFGetSystemManager();
    float scaled = (float)g_VirtualHeight * y;
    CDisplayManager* display = system->m_pDisplayManager;
    return (long)(scaled / display->GetRenderTargetDimensions().Y);
}

void __fastcall GFABoxToVBox(const C2DBoxI& a, C2DBoxI& v)
{
    v.TLX = GFAXToVX(a.TLX);
    v.TLY = GFAYToVY(a.TLY);
    v.BRX = GFAXToVX(a.BRX);
    v.BRY = GFAYToVY(a.BRY);
}