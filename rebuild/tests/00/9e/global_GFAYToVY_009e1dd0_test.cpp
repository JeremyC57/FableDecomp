// Behaviour test for GFAYToVY (0x009E1DD0).
#include <cstdio>
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
    C2DExtentsI(const C2DExtentsI& o) : X(o.X), Y(o.Y) { }
};
class CDisplayManager { public:
    long Width, Height;
    const C2DExtentsI GetRenderTargetDimensions() { return C2DExtentsI(Width, Height); }
};
template <class T> class CCountedPointer { public: T* Data; void* Info; T* Get() const { return Data; } };
class CSystemManager { public:
    CCountedPointer<CDisplayManager> PDisplayManager;
    CDisplayManager* GetDisplayManager() { return PDisplayManager.Get(); }
};
static CSystemManager g_system;
static CSystemManager* GFGetSystemManager() { return &g_system; }
static double VIRTUAL_SCREEN_HEIGHT = 65536.0;
float GFAYToVY(float y)
{
    CSystemManager* system = GFGetSystemManager();
    return (float)((float)VIRTUAL_SCREEN_HEIGHT * y) / system->GetDisplayManager()->GetRenderTargetDimensions().Y;
}
int main()
{
    CDisplayManager display; display.Width = 640; display.Height = 480;
    g_system.PDisplayManager.Data = &display; g_system.PDisplayManager.Info = 0;
    if (GFAYToVY(240.0f) != 32768.0f || GFAYToVY(0.0f) != 0.0f) { std::printf("009e1dd0_TEST FAIL\n"); return 1; }
    std::printf("009e1dd0_TEST PASS\n");
    return 0;
}