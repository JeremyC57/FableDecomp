// Behaviour test for 0x00B23C50 CEngine::SetChildPrimitive: the forwarder must call CEnginePrimitiveRenderer::SetChildPrimitive on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex, const CEnginePrimitive& primitive, unsigned long flags)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
        gVal[1] = (double)childIndex;
        gPtr[2] = &primitive;
        gVal[3] = (double)flags;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer = &gComponent;
class CEngine { public:
    virtual void SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex, const CEnginePrimitive& primitive, unsigned long flags);
};
void CEngine::SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex, const CEnginePrimitive& primitive, unsigned long flags)
{
    g_pEnginePrimitiveRenderer->SetChildPrimitive(handle, childIndex, primitive, flags);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEnginePrimitive o2;
    CEngine engine;
    engine.SetChildPrimitive(o0, 41u, o2, 43u);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gVal[1] != 41.0 || gPtr[2] != &o2 || gVal[3] != 43.0) {
        std::printf("00b23c50_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23c50_TEST PASS\n");
    return 0;
}