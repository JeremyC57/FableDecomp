// Behaviour test for 0x009E23F0 GFXToY: x * height / width of the render target.
#include <cstdio>

class C2DExtentsI { public: int X; int Y; };

static int gWidth = 640, gHeight = 480, gCalls = 0;

class CDisplayManager
{ public:
    C2DExtentsI GetRenderTargetDimensions() const { ++gCalls; C2DExtentsI e; e.X = gWidth; e.Y = gHeight; return e; }
};

class CSystemManager { public: CDisplayManager* DisplayManager; };

static CDisplayManager gDisplay;
static CSystemManager gSystem = { &gDisplay };
static CSystemManager* GFGetSystemManager() { return &gSystem; }

static float GFXToY(float x)
{
    CDisplayManager* heightSource = GFGetSystemManager()->DisplayManager;
    CDisplayManager* widthSource = GFGetSystemManager()->DisplayManager;
    return x * heightSource->GetRenderTargetDimensions().Y
             / widthSource->GetRenderTargetDimensions().X;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    bool ok = Near(GFXToY(640.0f), 480.0f) && Near(GFXToY(100.0f), 75.0f) && gCalls == 4;
    gWidth = 1280; gHeight = 720;
    ok = ok && Near(GFXToY(1280.0f), 720.0f) && Near(GFXToY(-64.0f), -36.0f);
    if (!ok) { std::printf("009e23f0_TEST FAIL\n"); return 1; }
    std::printf("009e23f0_TEST PASS\n");
    return 0;
}