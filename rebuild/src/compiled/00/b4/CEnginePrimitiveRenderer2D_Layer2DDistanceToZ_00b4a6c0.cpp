// CEnginePrimitiveRenderer2D::Layer2DDistanceToZ(float distance) -- map a 2D layer's view distance to the z-buffer value: the view-space z of the layer point through the engine camera's view transform, reciprocal, scaled by the camera's ZBufferFactor plus ZBufferOffset
// Leaf: ?Layer2DDistanceToZ@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4A6C0, 54 bytes. __fastcall (this unused), `ret 4`, float in st0.
// Identity: FableWin.pdb `?Layer2DDistanceToZ@CEnginePrimitiveRenderer2D@@QAEMM@Z` (and Ego_r.pdb); it follows the two
//   landed Layer2DToDistance bodies 0x00B4A6A0 / 0x00B4A6B0, and every caller (CEngineSubPrimitive2DClockSprite 0x00BACE80,
//   0x00BB0428, 0x00BB135A) feeds it Layer2DToDistance's result with the renderer still in ecx. The catalog label
//   "Perspective_TransformDepth" is descriptive.
// Layout: engine/CEngineCamera.h (ViewTransform +0x80, ZBufferFactor +0xdc, ZBufferOffset +0xe0 -- the PDB offsets
//   retail uses); CMatrix3x4 as the PDB's E11..E43 (row-major, translation row E41..E43). The camera is the engine's
//   current camera global 0x01436EA0 (named g_pEngineCamera in the landed primitive-manager sources). 1.0f pooled at
//   0x0122DED8 (`fdivr`).
// Shape: one left-to-right expression `distance * E33 + E43 + E23 + E13` (the view z of the layer point, retail's exact
//   fadd order), then the reciprocal as its own statement (`1.0f / z * F` in one
//   expression lets VC7.1 fold it to `F / z`), times ZBufferFactor, plus ZBufferOffset.
#include "engine/CEngineCamera.h"

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

extern CEngineCamera* g_pEngineCamera;                  // 0x01436EA0

class CEnginePrimitiveRenderer2D {
public:
    float Layer2DDistanceToZ(float distance);
};

float CEnginePrimitiveRenderer2D::Layer2DDistanceToZ(float distance)
{
    const CEngineCamera* camera = g_pEngineCamera;
    const CMatrix3x4& view = *(const CMatrix3x4*)camera->ViewTransform;
    float z = distance * view.E33 + view.E43 + view.E23 + view.E13;
    float oneOverZ = 1.0f / z;
    return oneOverZ * camera->ZBufferFactor + camera->ZBufferOffset;
}