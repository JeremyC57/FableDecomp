// GFVBoxToABox(const C2DBoxI&, C2DBoxI&) -- integer overload: convert a virtual box to actual render-target pixels, each corner coordinate through the inlined GFVXToAX / GFVYToAY and truncated
// Leaf: ?GFVBoxToABox@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1F50, length 203 bytes (+ int3 padding).
// PDB (FableWin): ?GFVBoxToABox@@YIXABVC2DBoxI@@AAV1@@Z -- __fastcall, source in ecx, result in edx. The catalog
// label "GFABoxToVBox" is propagated and wrong (the body multiplies by the render-target size and divides by the
// virtual size: virtual-to-actual). C2DBoxI = {TLX, TLY, BRX, BRY} (longs).
// Inlined: a LONG-parameter converter pair (GFVXToAX / GFVYToAY overloads, inline-only, so absent from the PDB name
// sets), each re-fetching the system manager (0x009A4EC0) and the render-target dimensions (0x009BEDC0);
// __ftol2 (0x00BFEA70) truncates each result. Globals: virtual width / height, doubles 65536.0 at 0x013961E8 / 0x013961F0.
// Shape: the long coordinate is the inline helper's by-value parameter, homed in the frame word before the calls
// (`mov eax,[edi] ; mov [esp+8],eax`), and the multiply is floating (`fild width ; fimul [esp+8] ; fdiv qword g`):
// `(float)x * width`. Calling the float GFVXToAX(float) with the long (implicitly or with a cast) rounds the
// argument through `fild ; fstp` first (that is the C2DCoordI overload 0x009E2330).
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

inline long __fastcall GFVXToAX(long x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (long)((float)x * display->GetRenderTargetDimensions().X / g_VirtualWidth);
}

inline long __fastcall GFVYToAY(long y)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (long)((float)y * display->GetRenderTargetDimensions().Y / g_VirtualHeight);
}

void __fastcall GFVBoxToABox(const C2DBoxI& v, C2DBoxI& a)
{
    a.TLX = GFVXToAX(v.TLX);
    a.TLY = GFVYToAY(v.TLY);
    a.BRX = GFVXToAX(v.BRX);
    a.BRY = GFVYToAY(v.BRY);
}