// GFABoxToVBox(const C2DBoxI&, C2DBoxI&) -- integer overload: convert an actual (render-target pixel) box to the virtual 65536-unit space, each corner coordinate through inlined long converters
// Grade: DIFFER(219v219) -- only the x87 operand order is left (sizes equal): retail `fld qword [g] ; fimul dword [x]` in every axis, ours `fild dword [x] ; fmul qword [g]`, and the Y product's `fstp` is scheduled before the result-slot push (lane gap: VC7.1 commutative x87 operand canonicalisation, GOTCHAS chunk 293; the landed GFAXToVX 0x009E1DA0 needs a never-cached (cv) qualifier on the global for this order)
// Leaf: ?GFABoxToVBox@
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
// Tried: this source (RTM), `/G7` (the helpers are no longer inlined).
// Also tried (this source): the X product through an inline `ScaleToVirtual(double virtualSize, x, const C2DExtentsI& size)`
// helper -- VC evaluates its arguments right to left, so the dimension call now comes first and the divide reads
// `[eax]` like retail; then `x * virtualSize` / `y * g` (canonicalised back to the same order, 0x009E21D0).
struct C2DExtentsI
{
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

inline long ScaleToVirtual(double virtualSize, long x, const C2DExtentsI& size)
{
    return (long)(virtualSize * x / size.X);
}

inline long __fastcall GFAXToVX(long x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return ScaleToVirtual(g_VirtualWidth, x, display->GetRenderTargetDimensions());
}

inline long __fastcall GFAYToVY(long y)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    float scaled = (float)(g_VirtualHeight * y);
    return (long)(scaled / display->GetRenderTargetDimensions().Y);
}

void __fastcall GFABoxToVBox(const C2DBoxI& a, C2DBoxI& v)
{
    v.TLX = GFAXToVX(a.TLX);
    v.TLY = GFAYToVY(a.TLY);
    v.BRX = GFAXToVX(a.BRX);
    v.BRY = GFAYToVY(a.BRY);
}
