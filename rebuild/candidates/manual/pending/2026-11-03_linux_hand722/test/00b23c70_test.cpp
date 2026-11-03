// Behaviour test for 0x00B23C70 CEngine::ClearAllChildPrimitives: the forwarder must call CEnginePrimitiveRenderer::ClearAllChildPrimitives on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void ClearAllChildPrimitives(CEnginePrimitiveHandle& handle)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80 = &gComponent;
class CEngine { public:
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle& handle);
};
void CEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle& handle)
{
    g_pPrimitiveRenderer_01436e80->ClearAllChildPrimitives(handle);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEngine engine;
    engine.ClearAllChildPrimitives(o0);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0) {
        std::printf("00b23c70_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23c70_TEST PASS\n");
    return 0;
}
