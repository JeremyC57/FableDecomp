// Behaviour test for 0x00B23C30 CEngine::SetChildPrimitive: the forwarder must call CEnginePrimitiveRenderer::SetChildPrimitive on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle, const CEnginePrimitive& primitive, unsigned long flags)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
        gVal[1] = (double)childIndex;
        gPtr[2] = &childHandle;
        gPtr[3] = &primitive;
        gVal[4] = (double)flags;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80 = &gComponent;
class CEngine { public:
    virtual void SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle, const CEnginePrimitive& primitive, unsigned long flags);
};
void CEngine::SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle, const CEnginePrimitive& primitive, unsigned long flags)
{
    g_pPrimitiveRenderer_01436e80->SetChildPrimitive(handle, childIndex, childHandle, primitive, flags);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEnginePrimitiveHandle o2;
    CEnginePrimitive o3;
    CEngine engine;
    engine.SetChildPrimitive(o0, 41u, o2, o3, 44u);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gVal[1] != 41.0 || gPtr[2] != &o2 || gPtr[3] != &o3 || gVal[4] != 44.0) {
        std::printf("00b23c30_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23c30_TEST PASS\n");
    return 0;
}
