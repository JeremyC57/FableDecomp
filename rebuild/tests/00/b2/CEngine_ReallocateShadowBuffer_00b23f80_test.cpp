// Behaviour test for 0x00B23F80 CEngine::ReallocateShadowBuffer: the forwarder must call CEngineShadowRenderer::XBoxReallocateShadowBuffer on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineShadowRenderer { public:
    void XBoxReallocateShadowBuffer()
    {
        ++gCalls; gThis = this;
    }
};
static CEngineShadowRenderer gComponent;
CEngineShadowRenderer* g_pEngineShadowRenderer = &gComponent;
class CEngine { public:
    virtual void ReallocateShadowBuffer();
};
void CEngine::ReallocateShadowBuffer()
{
    g_pEngineShadowRenderer->XBoxReallocateShadowBuffer();
}
int main()
{
    CEngine engine;
    engine.ReallocateShadowBuffer();
    if (gCalls != 1 || gThis != &gComponent) {
        std::printf("00b23f80_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23f80_TEST PASS\n");
    return 0;
}