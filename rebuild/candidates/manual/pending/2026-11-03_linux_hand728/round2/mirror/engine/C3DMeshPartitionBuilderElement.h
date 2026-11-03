#ifndef FABLE_ENGINE_C3DMESHPARTITIONBUILDERELEMENT_H
#define FABLE_ENGINE_C3DMESHPARTITIONBUILDERELEMENT_H
// C3DMeshPartitionBuilderElement (Ego_r class V; fields StartTriangle, TriangleCount, BoundingBox, BoundingSphere, Triangles,
// SubElements). Retail sizes: 12-byte STLport CArray, so Triangles +0x30, SubElements +0x3C, sizeof 0x44 (0x00AC2BE0: `push 44h`
// before operator new, [edi+3Ch]/[edi+40h] child pointers). Ctor / dtor: Ego_r 0x00C4A360 / 0x00C4A390, FableWin 0x03497EF0 /
// 0x03497F50; inline in retail (0x00AC2BE0 expands both; the scalar deleting destructor is 0x00AC39C0).
// PartitionSpace: Ego_r 0x00C4A520 / FableWin 0x03498310 ?PartitionSpace@C3DMeshPartitionBuilderElement@@AAEJPBVC3DVertex2@@JM@Z.
// Every row that includes this header defines FABLE_RETAIL_C3DBOUNDINGBOX (BuildFromPoints / GetCenter).
#include "engine/CArray.h"
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DVertex2.h"
#include "engine/C3DTriangle2.h"
class C3DMeshPartitionBuilderElement {
    long StartTriangle,TriangleCount;
    C3DBoundingBox BoundingBox;
    C3DBoundingSphere BoundingSphere;
    CArray<C3DTriangle2> Triangles;
    C3DMeshPartitionBuilderElement* SubElements[2];
    long PartitionSpace(const C3DVertex2*,long,float);
public:
    C3DMeshPartitionBuilderElement() { SubElements[0]=0; SubElements[1]=0; }
    ~C3DMeshPartitionBuilderElement() { delete SubElements[0]; delete SubElements[1]; }
};
#endif
