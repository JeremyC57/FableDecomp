#ifndef FABLE_ENGINE_CENGINEPRIMITIVE2DSTATICMESH_H
#define FABLE_ENGINE_CENGINEPRIMITIVE2DSTATICMESH_H
// New shared header (chunk 749, row 0x005502E0 NUISystem::CMesh::Draw): Ego_r.pdb CEnginePrimitive2DStaticMesh, sizeof 0x6C:
// CEnginePrimitive base +0, MeshIndex +0xC, ClipBox +0x10 (C2DBoxF), Alpha +0x20, Orientation +0x24
// (C3DOrientationInterpolator), BoundingSphere +0x58, TintColour +0x68. Name set: Ego_r / FableWin.pdb
// ??0..QAE@_KKABVC2DBoxF@@ABVC3DOrientationInterpolator@@W4EEngineInterpolationSet@@EVCRGBColour@@@Z (out of line at
// 0x00550940 in the CMesh TU, `ret 28h`), ??1..QAE@XZ (inline: CMesh::Draw destroys the Orientation member with a direct
// ~CBaseClass call at +0x24).
#include "rebuild_abi.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CRGBColour.h"
class CEnginePrimitive2DStaticMesh : public CEnginePrimitive {
public:
    CEnginePrimitive2DStaticMesh(unsigned __int64, unsigned long, const C2DBoxF&, const C3DOrientationInterpolator&,
                                 EEngineInterpolationSet, unsigned char, CRGBColour);    // 0x00550940
    unsigned long MeshIndex;                                             // +0x0c
    C2DBoxF ClipBox;                                                     // +0x10
    unsigned char Alpha;                                                 // +0x20
    C3DOrientationInterpolator Orientation;                              // +0x24
    C3DBoundingSphere BoundingSphere;                                    // +0x58
    CRGBColour TintColour;                                               // +0x68
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DStaticMesh) == 0x6c);
#endif
