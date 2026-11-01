// behaviour test: CDisplayManager::~CDisplayManager -- the user body shuts the shader render manager down only when
// it was in use, drops the render manager (scoped_ptr reset), uninitialises the back and depth surfaces and releases
// the D3D device if there is one; the members are then destroyed in reverse order.
#include <cstdio>
static int g_shaderUninit = 0, g_renderDeleted = 0, g_surfaceUninit = 0, g_deviceReleased = 0;
class CRenderManager { public: ~CRenderManager() { ++g_renderDeleted; } };
class CSurface { public: bool live; CSurface() : live(true) { } void Uninitialise() { if (live) { live = false; ++g_surfaceUninit; } } ~CSurface() { Uninitialise(); } };
struct IDevice { void Release() { ++g_deviceReleased; } };
class CDisplayManager { public:
    bool UsingShaderRenderManager; CRenderManager* PRenderManager; CSurface BackBufferSurface, DepthBufferSurface; IDevice* D3DDevice;
    ~CDisplayManager()
    {
        if (UsingShaderRenderManager) ++g_shaderUninit;  // g_ShaderRenderManager.Uninitialise()
        CRenderManager* old = PRenderManager; PRenderManager = 0; delete old;   // scoped_ptr reset: clear, then delete
        BackBufferSurface.Uninitialise();
        DepthBufferSurface.Uninitialise();
        if (D3DDevice) D3DDevice->Release();
    }
};
int main()
{
    IDevice dev;
    { CDisplayManager m; m.UsingShaderRenderManager = true; m.PRenderManager = new CRenderManager; m.D3DDevice = &dev; }
    { CDisplayManager m; m.UsingShaderRenderManager = false; m.PRenderManager = 0; m.D3DDevice = 0; }
    if (g_shaderUninit != 1 || g_renderDeleted != 1 || g_surfaceUninit != 4 || g_deviceReleased != 1) {
        std::printf("009c01c0_TEST FAIL\n"); return 1;
    }
    std::printf("009c01c0_TEST PASS\n");
    return 0;
}