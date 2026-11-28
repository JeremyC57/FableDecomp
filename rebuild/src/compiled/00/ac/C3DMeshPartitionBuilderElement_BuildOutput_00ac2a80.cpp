// 00ac2a80 C3DMeshPartitionBuilderElement::BuildOutput
// Leaf: ?BuildOutput@C3DMeshPartitionBuilderElement@@AAEXPAVC3DMeshPartitionElement@@PAV?$CArray@VC3DTriangle2@@@@@Z
// Flags: /Gr
#define FABLE_RETAIL_MESH_PARTITION_BUILD_OUTPUT_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/C3DMeshPartitionBuilderElement.h"
#include "engine/C3DMeshPartitionElement.h"
void C3DMeshPartitionBuilderElement::BuildOutput(C3DMeshPartitionElement* element, CArray<C3DTriangle2>* ordered_tri_list)
{
    element->BoundingBox = BoundingBox;
    element->BoundingSphere = BoundingSphere;
    element->StartTriangle = StartTriangle;
    element->TriangleCount = TriangleCount;
    if (SubElements[0]) {
        SubElements[0]->BuildOutput(element->BuildSubElement(0), ordered_tri_list);
        SubElements[1]->BuildOutput(element->BuildSubElement(1), ordered_tri_list);
    } else {
        for (long n = 0; n < Triangles.size(); ++n)
            ordered_tri_list->push_back(Triangles[n]);
    }
}
