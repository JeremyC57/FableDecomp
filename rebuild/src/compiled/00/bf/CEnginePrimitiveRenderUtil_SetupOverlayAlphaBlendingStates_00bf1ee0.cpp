#define FABLE_RETAIL_PASS22_RGBFLOAT_NATIVE_BYTE_DIVISION
// Leaf: ?SetupOverlayAlphaBlendingStates@CEnginePrimitiveRenderUtil@@QAEXKVCRGBColour@@@Z
// Flags: (none)
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ANIMATEDMESH
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETTEXTUREFACTOR_INLINE
#define FABLE_RETAIL_CSHADERRENDERMANAGER_GETSHADERSENABLED_INLINE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_GETASINT
#define FABLE_RETAIL_CRGBCOLOUR_SETCHANNEL
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
#define FABLE_RETAIL_CTEXTURE
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
namespace NEngineInternal {
    extern CRenderManager* GPRenderManager;
    extern CShaderRenderManager* GPShaderRenderManager;
}
// Inline-budget: the ordinary SetTextureFactor call produces a 564-byte leaf. Retail expands this shader/colour branch in the 855-byte alpha-blend function.
FABLE_FORCEINLINE void FABLE_FASTCALL CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if(NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled()) {
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2,C4DVector(colour));
    } else {
        NEngineInternal::GPRenderManager->SetTextureFactor(CRGBColour(colour).GetAsInt());
    }
}
void CEnginePrimitiveRenderUtil::SetupOverlayAlphaBlendingStates(unsigned long alpha,CRGBColour colour)
{
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    colour.SetA((unsigned char)((colour.GetA()*alpha)/255));
    SetTextureFactor(CRGBFloatColour(colour));
}