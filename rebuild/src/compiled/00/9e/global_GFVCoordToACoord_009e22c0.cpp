// GFVCoordToACoord(const C2DVector&, C2DVector&) -- convert a virtual 2D point to actual render-target pixels, one axis at a time through GFVXToAX / GFVYToAY
// Leaf: ?GFVCoordToACoord@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E22C0, length 98 bytes (+ int3 padding).
// PDB (FableWin): ?GFVCoordToACoord@@YIXABVC2DVector@@AAV1@@Z -- __fastcall, source in ecx, result in edx.
// Inlined: GFVXToAX (0x009E1E10) and GFVYToAY (0x009E1E40), each re-fetching the system manager and the
// render-target dimensions. C2DVector = {X, Y}.
// Shape: each inlined float argument is copied into the frame word [esp+8] (`mov eax,[edi] ; mov [esp+8],eax`)
// before the GFGetSystemManager call; the converters keep the display manager in a named local.
class C2DExtentsI
{ public:
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

void __fastcall GFVCoordToACoord(const C2DVector& v, C2DVector& a)
{
    a.X = GFVXToAX(v.X);
    a.Y = GFVYToAY(v.Y);
}