// Behaviour test for 0x00B23EB0 CEngine::Set2DPrimitiveLightingParameters: the forwarder must call CEnginePrimitiveRenderer2D::SetLightingParameters on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class C3DVector { public: int v; };
class CRGBFloatColour { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer2D { public:
    void SetLightingParameters(const C3DVector& lightDirection, const CRGBFloatColour& diffuse, const CRGBFloatColour& ambient, const CRGBFloatColour& backlight)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &lightDirection;
        gPtr[1] = &diffuse;
        gPtr[2] = &ambient;
        gPtr[3] = &backlight;
    }
};
static CEnginePrimitiveRenderer2D gComponent;
CEnginePrimitiveRenderer2D* g_pEnginePrimitiveRenderer2D = &gComponent;
class CEngine { public:
    virtual void Set2DPrimitiveLightingParameters(const C3DVector& lightDirection, const CRGBFloatColour& diffuse, const CRGBFloatColour& ambient, const CRGBFloatColour& backlight);
};
void CEngine::Set2DPrimitiveLightingParameters(const C3DVector& lightDirection, const CRGBFloatColour& diffuse, const CRGBFloatColour& ambient, const CRGBFloatColour& backlight)
{
    g_pEnginePrimitiveRenderer2D->SetLightingParameters(lightDirection, diffuse, ambient, backlight);
}
int main()
{
    C3DVector o0;
    CRGBFloatColour o1;
    CRGBFloatColour o2;
    CRGBFloatColour o3;
    CEngine engine;
    engine.Set2DPrimitiveLightingParameters(o0, o1, o2, o3);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gPtr[1] != &o1 || gPtr[2] != &o2 || gPtr[3] != &o3) {
        std::printf("00b23eb0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23eb0_TEST PASS\n");
    return 0;
}
