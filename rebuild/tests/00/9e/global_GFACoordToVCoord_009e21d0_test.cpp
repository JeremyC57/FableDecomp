// Behaviour test for GFACoordToVCoord (0x009E21D0): actual render-target pixels -> virtual 65536-unit coordinates.
#include <cstdio>

class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
    C2DExtentsI(const C2DExtentsI& o) : X(o.X), Y(o.Y) { }
};
static int g_dimsCalls = 0;
class CDisplayManager { public:
    long Width, Height;
    const C2DExtentsI GetRenderTargetDimensions() { ++g_dimsCalls; return C2DExtentsI(Width, Height); }
};
template <class T> class CCountedPointer { public:
    T* Data; void* Info;
    T* Get() const { return Data; }
};
class CSystemManager { public:
    CCountedPointer<CDisplayManager> PDisplayManager;
    CDisplayManager* GetDisplayManager() { return PDisplayManager.Get(); }
};
static CSystemManager g_system;
static CSystemManager* GFGetSystemManager() { return &g_system; }
static double VIRTUAL_SCREEN_WIDTH = 65536.0;
static double VIRTUAL_SCREEN_HEIGHT = 65536.0;
class C2DVector { public: float X, Y; };

inline float GFAXToVX(float x)
{
    CSystemManager* system = GFGetSystemManager();
    return ((float)VIRTUAL_SCREEN_WIDTH * x) / system->GetDisplayManager()->GetRenderTargetDimensions().X;
}
inline float GFAYToVY(float y)
{
    CSystemManager* system = GFGetSystemManager();
    return (float)((float)VIRTUAL_SCREEN_HEIGHT * y) / system->GetDisplayManager()->GetRenderTargetDimensions().Y;
}
void GFACoordToVCoord(const C2DVector& a, C2DVector& v)
{
    v.X = GFAXToVX(a.X);
    v.Y = GFAYToVY(a.Y);
}

int main()
{
    CDisplayManager display; display.Width = 1024; display.Height = 512;
    g_system.PDisplayManager.Data = &display; g_system.PDisplayManager.Info = 0;
    C2DVector a; a.X = 512.0f; a.Y = 128.0f;
    C2DVector v; v.X = -1.0f; v.Y = -1.0f;
    GFACoordToVCoord(a, v);
    bool ok = v.X == 32768.0f && v.Y == 16384.0f && g_dimsCalls == 2;
    VIRTUAL_SCREEN_HEIGHT = 1000.0; display.Height = 250;
    a.Y = 25.0f;
    GFACoordToVCoord(a, v);
    if (v.Y != 100.0f || g_dimsCalls != 4) ok = false;
    if (!ok) { std::printf("009e21d0_TEST FAIL\n"); return 1; }
    std::printf("009e21d0_TEST PASS\n");
    return 0;
}