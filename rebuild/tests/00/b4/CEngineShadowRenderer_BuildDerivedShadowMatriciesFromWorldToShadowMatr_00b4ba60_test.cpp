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


// The fixture provides matrix storage; operations below come from the shared retail headers.
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct MatrixFixture {
    struct Matrices {
        CMatrix4x4 WorldToShadowBufferMatrix;
        CMatrix4x4 WorldToShadowBufferMatrixHomogenousScreenSpace;
        CMatrix4x4 WorldToShadowBufferMatrixTextureSpace;
    } SceneData;
    C3DVector ShadowBufferTextureUVScaler;
    float ColourBufferShadowTextureZBias;
    CEngineShadowRenderer::EShadowRenderingTechnique RenderingTechnique;
    void BuildDerivedShadowMatriciesFromWorldToShadowMatrix();
};
void MatrixFixture::BuildDerivedShadowMatriciesFromWorldToShadowMatrix()
{
    SceneData.WorldToShadowBufferMatrixHomogenousScreenSpace = SceneData.WorldToShadowBufferMatrix;
    SceneData.WorldToShadowBufferMatrixHomogenousScreenSpace.PostScale(C3DVector(2.0f, -2.0f, 1.0f));
    SceneData.WorldToShadowBufferMatrixHomogenousScreenSpace.PostTranslate(C3DVector(-1.0f, 1.0f, 0.0f));
    SceneData.WorldToShadowBufferMatrixTextureSpace = SceneData.WorldToShadowBufferMatrix;
    SceneData.WorldToShadowBufferMatrixTextureSpace.PostScale(ShadowBufferTextureUVScaler);
    if (RenderingTechnique == CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER)
        SceneData.WorldToShadowBufferMatrixTextureSpace.PostTranslate(C3DVector(0.0f, 0.0f, ColourBufferShadowTextureZBias));
}

int main()
{
    for (int technique = 0; technique < 3; ++technique) {
        for (int seed = -4; seed <= 4; ++seed) {
            MatrixFixture fixture;
            fixture.RenderingTechnique = static_cast<CEngineShadowRenderer::EShadowRenderingTechnique>(technique);
            fixture.ShadowBufferTextureUVScaler = C3DVector(2.0f, -0.5f, 0.25f);
            fixture.ColourBufferShadowTextureZBias = 0.125f * seed;
            for (int row = 0; row < 4; ++row)
                for (int col = 0; col < 4; ++col)
                    fixture.SceneData.WorldToShadowBufferMatrix.Data[row][col] = (row * 4 + col - 5) * 0.25f * seed;
            CMatrix4x4 before = fixture.SceneData.WorldToShadowBufferMatrix;
            fixture.BuildDerivedShadowMatriciesFromWorldToShadowMatrix();
            assert(memcmp(&before, &fixture.SceneData.WorldToShadowBufferMatrix, sizeof(before)) == 0);
            for (int r = 0; r < 4; ++r) {
                const float* old = before.Data[r];
                const float* screen = fixture.SceneData.WorldToShadowBufferMatrixHomogenousScreenSpace.Data[r];
                const float* tex = fixture.SceneData.WorldToShadowBufferMatrixTextureSpace.Data[r];
                assert(screen[0] == 2 * old[0] - old[3]);
                assert(screen[1] == -2 * old[1] + old[3]);
                assert(screen[2] == old[2] && screen[3] == old[3]);
                assert(tex[0] == 2 * old[0] && tex[1] == -0.5f * old[1]);
                const float expectedZ = 0.25f * old[2] + (technique == 2 ? old[3] * fixture.ColourBufferShadowTextureZBias : 0.0f);
                assert(tex[2] == expectedZ && tex[3] == old[3]);
            }
        }
    }
    puts("00b4ba60_TEST PASS");
    return 0;
}
