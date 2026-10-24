// Behaviour test for 009e1f50 (GF virtual/actual coordinate conversion family): fake system manager with a 1024x768 render target.
#include <cstdio>

struct C2DExtentsI { long X; long Y; };

class CDisplayManager
{
public:
    long m_width, m_height;
    C2DExtentsI GetRenderTargetDimensions() const { C2DExtentsI e; e.X = m_width; e.Y = m_height; return e; }
};

class CSystemManager
{
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* m_pDisplayManager;
};

static CDisplayManager g_display;
static CSystemManager g_system;
static int g_managerCalls = 0;
CSystemManager* GFGetSystemManager() { ++g_managerCalls; return &g_system; }
double g_VirtualWidth = 65536.0;
double g_VirtualHeight = 65536.0;

static bool Near(float a, float b) { float d = a - b; return d < 0.01f && d > -0.01f; }

static void Setup() { g_display.m_width = 1024; g_display.m_height = 768; g_system.m_pDisplayManager = &g_display; g_managerCalls = 0; }
class C2DVector { public: float X; float Y; };
class C2DCoordI { public: long X; long Y; };
class C2DBoxF { public: float TLX; float TLY; float BRX; float BRY; };
class C2DBoxI { public: long TLX; long TLY; long BRX; long BRY; };
inline float GFVXToAX(float x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (float)(x * display->GetRenderTargetDimensions().X / g_VirtualWidth);
}
inline float GFVYToAY(float y)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (float)(y * display->GetRenderTargetDimensions().Y / g_VirtualHeight);
}
inline long GFVXToAXLong(long x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (long)((float)x * display->GetRenderTargetDimensions().X / g_VirtualWidth);
}
inline long GFVYToAYLong(long y)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return (long)((float)y * display->GetRenderTargetDimensions().Y / g_VirtualHeight);
}
void GFVBoxToABox(const C2DBoxI& v, C2DBoxI& a)
{
    a.TLX = GFVXToAXLong(v.TLX);
    a.TLY = GFVYToAYLong(v.TLY);
    a.BRX = GFVXToAXLong(v.BRX);
    a.BRY = GFVYToAYLong(v.BRY);
}

int main()
{
    Setup();
    C2DBoxI v = { 0, 16384, 65536, 32769 };
    C2DBoxI a;
    GFVBoxToABox(v, a);
    if (a.TLX != 0 || a.TLY != 192 || a.BRX != 1024 || a.BRY != 384) { std::printf("009e1f50_TEST FAIL\n"); return 1; }
    if (g_managerCalls != 4) { std::printf("009e1f50_TEST FAIL\n"); return 1; }
    std::printf("009e1f50_TEST PASS\n");
    return 0;
}