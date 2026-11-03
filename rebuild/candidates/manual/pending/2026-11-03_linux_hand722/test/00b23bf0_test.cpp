// Behaviour test for 0x00B23BF0 CEngine::SetPrimitivePosition: the forwarder must call CEnginePrimitiveRenderer::SetPrimitivePosition on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
class CEnginePrimitivePosition { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void SetPrimitivePosition(CEnginePrimitiveHandle& handle, const CEnginePrimitivePosition& position)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
        gPtr[1] = &position;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80 = &gComponent;
class CEngine { public:
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle& handle, const CEnginePrimitivePosition& position);
};
void CEngine::SetPrimitivePosition(CEnginePrimitiveHandle& handle, const CEnginePrimitivePosition& position)
{
    g_pPrimitiveRenderer_01436e80->SetPrimitivePosition(handle, position);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEnginePrimitivePosition o1;
    CEngine engine;
    engine.SetPrimitivePosition(o0, o1);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gPtr[1] != &o1) {
        std::printf("00b23bf0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23bf0_TEST PASS\n");
    return 0;
}
