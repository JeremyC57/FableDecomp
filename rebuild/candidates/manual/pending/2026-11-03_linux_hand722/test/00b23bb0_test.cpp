// Behaviour test for 0x00B23BB0 CEngine::AddPrimitive: the forwarder must call CEnginePrimitiveRenderer::AddPrimitive on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void AddPrimitive(const CEnginePrimitive& primitive)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &primitive;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80 = &gComponent;
class CEngine { public:
    virtual void AddPrimitive(const CEnginePrimitive& primitive);
};
void CEngine::AddPrimitive(const CEnginePrimitive& primitive)
{
    g_pPrimitiveRenderer_01436e80->AddPrimitive(primitive);
}
int main()
{
    CEnginePrimitive o0;
    CEngine engine;
    engine.AddPrimitive(o0);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0) {
        std::printf("00b23bb0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23bb0_TEST PASS\n");
    return 0;
}
