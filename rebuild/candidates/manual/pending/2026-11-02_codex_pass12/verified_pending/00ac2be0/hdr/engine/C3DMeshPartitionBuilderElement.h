#ifndef FABLE_ENGINE_C3DMESHPARTITIONBUILDERELEMENT_H
#define FABLE_ENGINE_C3DMESHPARTITIONBUILDERELEMENT_H
#include "engine/CArray.h"
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DVertex2.h"
#include "engine/C3DTriangle2.h"
// Original PDB class and all six fields. Retail12-byte CArray gives size68,
// Triangles+48 and SubElements+60; constructor and native accesses prove both.
class C3DMeshPartitionBuilderElement {
 long StartTriangle,TriangleCount;C3DBoundingBox BoundingBox;
 C3DBoundingSphere BoundingSphere;CArray<C3DTriangle2>Triangles;
 C3DMeshPartitionBuilderElement*SubElements[2];
 long PartitionSpace(const C3DVertex2*,long,float);
public:
 C3DMeshPartitionBuilderElement(){SubElements[0]=0;SubElements[1]=0;}
 ~C3DMeshPartitionBuilderElement(){delete SubElements[0];delete SubElements[1];}
};
#endif
