// Behaviour test for 009e2330 (GF virtual/actual coordinate conversion family): fake system manager with a 1024x768 render target.
#include <cstdio>

class C2DExtentsI { public: long X; long Y; };

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
void GFVCoordToACoord(const C2DCoordI& v, C2DCoordI& a)
{
    a.X = (long)GFVXToAX((float)v.X);
    a.Y = (long)GFVYToAY((float)v.Y);
}

int main()
{
    Setup();
    C2DCoordI v = { 16384, 49153 };
    C2DCoordI a;
    GFVCoordToACoord(v, a);
    if (a.X != 256 || a.Y != 576) { std::printf("009e2330_TEST FAIL\n"); return 1; }
    std::printf("009e2330_TEST PASS\n");
    return 0;
}