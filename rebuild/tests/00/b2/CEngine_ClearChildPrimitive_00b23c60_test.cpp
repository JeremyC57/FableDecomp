// Behaviour test for 0x00B23C60 CEngine::ClearChildPrimitive: the forwarder must call CEnginePrimitiveRenderer::ClearChildPrimitive on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void ClearChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
        gVal[1] = (double)childIndex;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer = &gComponent;
class CEngine { public:
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex);
};
void CEngine::ClearChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex)
{
    g_pEnginePrimitiveRenderer->ClearChildPrimitive(handle, childIndex);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEngine engine;
    engine.ClearChildPrimitive(o0, 41u);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gVal[1] != 41.0) {
        std::printf("00b23c60_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23c60_TEST PASS\n");
    return 0;
}