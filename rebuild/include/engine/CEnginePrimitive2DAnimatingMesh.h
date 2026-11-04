#ifndef FABLE_ENGINE_CENGINEPRIMITIVE2DANIMATINGMESH_H
#define FABLE_ENGINE_CENGINEPRIMITIVE2DANIMATINGMESH_H
// New shared header (chunk 749, row 0x005502E0 NUISystem::CMesh::Draw): Ego_r.pdb CEnginePrimitive2DAnimatingMesh, sizeof
// 0x80: CEnginePrimitive base +0, ClipBox +0xC (C2DBoxF), MeshIndex +0x1C, Alpha +0x20, Orientation +0x24
// (C3DOrientationInterpolator), Anim +0x58 (C3DAnimationInterpolator, 0x14, opaque here), BoundingSphere +0x6C, TintColour
// +0x7C. Name set: Ego_r / FableWin.pdb ??0..QAE@_KKABVC3DAnimationBlendState@@ABVC2DBoxF@@ABVC3DBoundingSphere@@
// ABVC3DOrientationInterpolator@@W4EEngineInterpolationSet@@EVCRGBColour@@@Z (out of line at 0x00550A70 in the CMesh TU,
// `ret 30h`), ??1..QAE@XZ (out of line at 0x00550A00 in the CMesh TU).
#include "rebuild_abi.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CRGBColour.h"
class C3DAnimationBlendState;
class CEnginePrimitive2DAnimatingMesh : public CEnginePrimitive {
public:
    CEnginePrimitive2DAnimatingMesh(unsigned __int64, unsigned long, const C3DAnimationBlendState&, const C2DBoxF&,
                                    const C3DBoundingSphere&, const C3DOrientationInterpolator&, EEngineInterpolationSet,
                                    unsigned char, CRGBColour);                   // 0x00550A70
    ~CEnginePrimitive2DAnimatingMesh();                                  // 0x00550A00
    C2DBoxF ClipBox;                                                     // +0x0c
    unsigned long MeshIndex;                                             // +0x1c
    unsigned char Alpha;                                                 // +0x20
    C3DOrientationInterpolator Orientation;                              // +0x24
    unsigned char Anim[0x14];                                            // +0x58 C3DAnimationInterpolator
    C3DBoundingSphere BoundingSphere;                                    // +0x6c
    CRGBColour TintColour;                                               // +0x7c
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DAnimatingMesh) == 0x80);
#endif
