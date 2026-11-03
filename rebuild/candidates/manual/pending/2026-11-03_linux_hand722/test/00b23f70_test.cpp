// Behaviour test for 0x00B23F70 CEngine::DeallocateShadowBuffer: the forwarder must call CEngineShadowRenderer::XBoxDeallocateShadowBuffer on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineShadowRenderer { public:
    bool XBoxDeallocateShadowBuffer()
    {
        ++gCalls; gThis = this;
        return true;
    }
};
static CEngineShadowRenderer gComponent;
CEngineShadowRenderer* g_pShadowRenderer_01436e60 = &gComponent;
class CEngine { public:
    virtual bool DeallocateShadowBuffer();
};
bool CEngine::DeallocateShadowBuffer()
{
    return g_pShadowRenderer_01436e60->XBoxDeallocateShadowBuffer();
}
int main()
{
    CEngine engine;
    bool r = engine.DeallocateShadowBuffer();
    if (gCalls != 1 || gThis != &gComponent || !r) {
        std::printf("00b23f70_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23f70_TEST PASS\n");
    return 0;
}
