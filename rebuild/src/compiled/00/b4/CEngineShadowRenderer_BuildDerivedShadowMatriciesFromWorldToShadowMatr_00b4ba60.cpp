// 00b4ba60 CEngineShadowRenderer::BuildDerivedShadowMatriciesFromWorldToShadowMatrix
// Leaf: ?BuildDerivedShadowMatriciesFromWorldToShadowMatrix@CEngineShadowRenderer@@AAEXXZ
// Flags: /O2 /Oy
#define FABLE_RETAIL_MATRIX_POST_MULTIPLY_PASS30
#define FABLE_RETAIL_SHADOW_TRANSFORMS_PASS30
#define FABLE_RETAIL_MATRIX_IDENTITY_ROW_ORDER_PASS30
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESCENE
#define FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESPACE
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PACKED_VECTORS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_CALCLOCALWORLD
#define FABLE_RETAIL_CENGINESHADOWRENDERER_CLIPVOLUMES_PASS22
#define FABLE_RETAIL_SPOTLIGHT_MATRIX_BUILD_PASS30
#define FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
#define FABLE_RETAIL_CMATRIX4X4_SHADOWSPACE
#define FABLE_RETAIL_CMATRIX3X4_PRETRANSLATE_PASS23
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"


void CEngineShadowRenderer::BuildDerivedShadowMatriciesFromWorldToShadowMatrix()
{
    SceneData.WorldToShadowBufferMatrixHomogenousScreenSpace = SceneData.WorldToShadowBufferMatrix;
    SceneData.WorldToShadowBufferMatrixHomogenousScreenSpace.PostScale(C3DVector(2.0f, -2.0f, 1.0f));
    SceneData.WorldToShadowBufferMatrixHomogenousScreenSpace.PostTranslate(C3DVector(-1.0f, 1.0f, 0.0f));
    SceneData.WorldToShadowBufferMatrixTextureSpace = SceneData.WorldToShadowBufferMatrix;
    SceneData.WorldToShadowBufferMatrixTextureSpace.PostScale(ShadowBufferTextureUVScaler);
    if (RenderingTechnique == SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER)
        SceneData.WorldToShadowBufferMatrixTextureSpace.PostTranslate(C3DVector(0.0f, 0.0f, ColourBufferShadowTextureZBias));
}
