// Behaviour test for 0x00B23BE0 CEngine::SetPrimitiveAnimation: the forwarder must call CEnginePrimitiveRenderer::SetPrimitiveAnimation on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
class CEnginePrimitiveAnimation { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void SetPrimitiveAnimation(CEnginePrimitiveHandle& handle, const CEnginePrimitiveAnimation& animation)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
        gPtr[1] = &animation;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80 = &gComponent;
class CEngine { public:
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle& handle, const CEnginePrimitiveAnimation& animation);
};
void CEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle& handle, const CEnginePrimitiveAnimation& animation)
{
    g_pPrimitiveRenderer_01436e80->SetPrimitiveAnimation(handle, animation);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEnginePrimitiveAnimation o1;
    CEngine engine;
    engine.SetPrimitiveAnimation(o0, o1);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gPtr[1] != &o1) {
        std::printf("00b23be0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23be0_TEST PASS\n");
    return 0;
}
