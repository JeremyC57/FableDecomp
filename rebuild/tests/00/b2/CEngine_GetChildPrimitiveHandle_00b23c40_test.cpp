// Behaviour test for 0x00B23C40 CEngine::GetChildPrimitiveHandle: the forwarder must call CEnginePrimitiveRenderer::GetChildPrimitiveHandle on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
        gVal[1] = (double)childIndex;
        gPtr[2] = &childHandle;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer = &gComponent;
class CEngine { public:
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle);
};
void CEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle)
{
    g_pEnginePrimitiveRenderer->GetChildPrimitiveHandle(handle, childIndex, childHandle);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEnginePrimitiveHandle o2;
    CEngine engine;
    engine.GetChildPrimitiveHandle(o0, 41u, o2);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gVal[1] != 41.0 || gPtr[2] != &o2) {
        std::printf("00b23c40_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23c40_TEST PASS\n");
    return 0;
}