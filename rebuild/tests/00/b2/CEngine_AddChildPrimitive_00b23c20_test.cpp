// Behaviour test for 0x00B23C20 CEngine::AddChildPrimitive: the forwarder must call CEnginePrimitiveRenderer::AddChildPrimitive on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    unsigned long AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
        gPtr[1] = &primitive;
        gVal[2] = (double)flags;
        return 0x1234u;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer = &gComponent;
class CEngine { public:
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);
};
unsigned long CEngine::AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags)
{
    return g_pEnginePrimitiveRenderer->AddChildPrimitive(handle, primitive, flags);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEnginePrimitive o1;
    CEngine engine;
    unsigned long r = engine.AddChildPrimitive(o0, o1, 42u);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gPtr[1] != &o1 || gVal[2] != 42.0 || r != 0x1234u) {
        std::printf("00b23c20_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23c20_TEST PASS\n");
    return 0;
}