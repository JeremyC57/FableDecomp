// GFVCoordToACoord(const C2DCoordI&, C2DCoordI&) -- integer overload: convert a virtual 2D coordinate to actual render-target pixels through the float GFVXToAX / GFVYToAY, truncating each axis
// Leaf: ?GFVCoordToACoord@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2330, length 108 bytes (+ int3 padding).
// PDB (FableWin): ?GFVCoordToACoord@@YIXABVC2DCoordI@@AAV1@@Z -- __fastcall, source in ecx, result in edx. The catalog
// label "GFABoxToVBox" is propagated and wrong (two axes, virtual-to-actual, int in and out).
// Inlined: GFVXToAX (0x009E1E10) and GFVYToAY (0x009E1E40), each re-fetching the system manager and the
// render-target dimensions; __ftol2 (0x00BFEA70) truncates each result. C2DCoordI = {X, Y} (longs).
// Shape: each axis is converted to the float argument first (`fild [edi] ; fstp [esp+8]`), then the inlined
// converter runs (`fild [eax] ; fmul [esp+8] ; fdiv g`), then `call __ftol2 ; mov [esi],eax`.
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

inline float __fastcall GFVXToAX(float x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (float)(x * display->GetRenderTargetDimensions().X / g_VirtualWidth);
}

inline float __fastcall GFVYToAY(float y)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (float)(y * display->GetRenderTargetDimensions().Y / g_VirtualHeight);
}

void __fastcall GFVCoordToACoord(const C2DCoordI& v, C2DCoordI& a)
{
    a.X = (long)GFVXToAX((float)v.X);
    a.Y = (long)GFVYToAY((float)v.Y);
}