// Leaf: ?PeekGlobalDiffuseColour@CIEngine@@UBE?BVCRGBFloatColour@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CIEngine.h (class-model switch FABLE_RETAIL_CIENGINE; was ?PeekGlobalDiffuseColour@CIEngine_Methods@@QAE?AVCRGBFloatColour@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// CIEngine::PeekGlobalDiffuseColour @ 00b28b40

// Preamble: the define / include block of the landed precedent CIEngine_Refresh_00b23340.cpp.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"
class CRGBFloatColour { public:
    float r, g, b, a;
    CRGBFloatColour(float rr, float gg, float bb, float aa) : r(rr), g(gg), b(bb), a(aa) {}
    CRGBFloatColour() {}
};

const CRGBFloatColour CIEngine::PeekGlobalDiffuseColour() const
{
    return CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f);
}