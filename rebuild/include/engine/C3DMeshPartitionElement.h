#ifndef FABLE_ENGINE_C3DMESHPARTITIONELEMENT_H
#define FABLE_ENGINE_C3DMESHPARTITIONELEMENT_H
#ifdef FABLE_RETAIL_MESH_PARTITION_BUILD_OUTPUT_PASS30
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
// Ego_d class 0xc65c and Ego_r give a private 56-byte record. The builder
// assigns each field directly in the original FableWin BuildOutput.
class C3DMeshPartitionElement {
    friend class C3DMeshPartitionBuilderElement;
    C3DBoundingBox BoundingBox;
    long StartTriangle;
    long TriangleCount;
    C3DMeshPartitionElement* SubElements[2];
    C3DBoundingSphere BoundingSphere;
public:
    C3DMeshPartitionElement* BuildSubElement(long index);
};
#endif
#endif
