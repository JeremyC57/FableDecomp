// GFVBoxToABox(const C2DBoxF&, C2DBoxF&) -- convert a virtual box to actual render-target pixels: the size and the top-left corner are converted, the bottom-right is corner + size
// Leaf: ?GFVBoxToABox@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1E70, length 209 bytes (+ int3 padding).
// PDB (FableWin): ?GFVBoxToABox@@YIXABVC2DBoxF@@AAV1@@Z -- __fastcall, source in ecx, result in edx.
// Inlined: GFVXToAX (0x009E1E10) and GFVYToAY (0x009E1E40), each re-fetching the system manager and the
// render-target dimensions. C2DBoxF = {TLX, TLY, BRX, BRY} (floats).
// Shape: width and height (BR - TL) are converted FIRST into two float locals (frame words [esp+0Ch] / [esp+10h], the
// second one reusing the dead dimension temporary), then the corner; the bottom-right adds the stored corner
// (`fld size ; fadd [esi]` = `a.TLX + width`). Each inlined argument is homed in the frame word [esp+8].
struct C2DExtentsI
{
    long X;
    long Y;
};

class C2DBoxF
{
public:
    float TLX;
    float TLY;
    float BRX;
    float BRY;
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

void __fastcall GFVBoxToABox(const C2DBoxF& v, C2DBoxF& a)
{
    float width = GFVXToAX(v.BRX - v.TLX);
    float height = GFVYToAY(v.BRY - v.TLY);
    a.TLX = GFVXToAX(v.TLX);
    a.TLY = GFVYToAY(v.TLY);
    a.BRX = a.TLX + width;
    a.BRY = a.TLY + height;
}