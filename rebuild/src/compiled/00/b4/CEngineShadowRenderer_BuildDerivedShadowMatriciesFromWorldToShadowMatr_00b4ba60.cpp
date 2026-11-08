// CEngineShadowRenderer::BuildDerivedShadowMatriciesFromWorldToShadowMatrix() -- derive the homogeneous-screen-space shadow matrix (world-to-shadow scaled by (2,-2,1) and translated by (-1,1,0)) and the texture-space one (world-to-shadow scaled by the shadow-buffer UV scaler, plus the colour-buffer z bias for technique 2)
// Leaf: BuildDerivedShadowMatriciesFromWorldToShadowMatrix
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4BA60, length 608 bytes, plain `ret`, no callees.
// The two inline CMatrix4x4 helpers scale the first three columns of every row and add the w column times a translation;
//   `x * 2` is `fadd st,st`, `x * 1` folds to a copy and `x + w * -1` becomes `x - w * 1`.
// Layout: engine/CEngineShadowRenderer.h; SceneData (CShadowRendererSceneData, PDB) holds WorldToShadowBufferMatrix +0x0c,
//   WorldToShadowBufferMatrixHomogenousScreenSpace +0x4c and WorldToShadowBufferMatrixTextureSpace +0x8c.
#include "fable_compiler.h"
#include "engine/CEngineShadowRenderer.h"

#include "engine/C3DVector.h"
class CMatrix4x4 { public:
    float M[4][4];
    FABLE_FORCEINLINE void Scale(const C3DVector& s)
    {
        for (int i = 0; i < 4; ++i) {
            M[i][0] *= s.X;
            M[i][1] *= s.Y;
            M[i][2] *= s.Z;
        }
    }
    FABLE_FORCEINLINE void Translate(const C3DVector& t)
    {
        for (int i = 0; i < 4; ++i) {
            M[i][0] += M[i][3] * t.X;
            M[i][1] += M[i][3] * t.Y;
            M[i][2] += M[i][3] * t.Z;
        }
    }
};
class CShadowRendererSceneData { public:
    CMatrix4x4 WorldToShadowBufferMatrix;                       // +0x00 (renderer +0x0c)
    CMatrix4x4 WorldToShadowBufferMatrixHomogenousScreenSpace;  // +0x40 (renderer +0x4c)
    CMatrix4x4 WorldToShadowBufferMatrixTextureSpace;           // +0x80 (renderer +0x8c)
};

struct CEngineShadowRenderer_Methods : CEngineShadowRenderer {
    void BuildDerivedShadowMatriciesFromWorldToShadowMatrix();
};

void CEngineShadowRenderer_Methods::BuildDerivedShadowMatriciesFromWorldToShadowMatrix()
{
    ((CShadowRendererSceneData*)SceneData)->WorldToShadowBufferMatrixHomogenousScreenSpace = ((CShadowRendererSceneData*)SceneData)->WorldToShadowBufferMatrix;
    ((CShadowRendererSceneData*)SceneData)->WorldToShadowBufferMatrixHomogenousScreenSpace.Scale(C3DVector(2.0f, -2.0f, 1.0f));
    ((CShadowRendererSceneData*)SceneData)->WorldToShadowBufferMatrixHomogenousScreenSpace.Translate(C3DVector(-1.0f, 1.0f, 0.0f));
    ((CShadowRendererSceneData*)SceneData)->WorldToShadowBufferMatrixTextureSpace = ((CShadowRendererSceneData*)SceneData)->WorldToShadowBufferMatrix;
    ((CShadowRendererSceneData*)SceneData)->WorldToShadowBufferMatrixTextureSpace.Scale(*(const C3DVector*)&ShadowBufferTextureUVScaler_X);
    if (*(long*)RenderingTechnique == 2)
        ((CShadowRendererSceneData*)SceneData)->WorldToShadowBufferMatrixTextureSpace.Translate(C3DVector(0.0f, 0.0f, ColourBufferShadowTextureZBias));
}