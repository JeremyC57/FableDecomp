// 00acd0e0 QuadricOptimiserInternals::COptimisedPrimitive::DetectFoldOver
// Leaf: ?DetectFoldOver@COptimisedPrimitive@QuadricOptimiserInternals@@AAE_NABVCSubContraction@2@ABVC3DVector@@@Z
// Flags: /d2inlT60
// Inline-budget: retail calls the recursive tree erase directly when destroying the face list.
#define FABLE_RETAIL_QUADRIC_FACE_FOLD_PASS27
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_DETECTFOLDOVER_SUB
#define FABLE_RETAIL_CARRAY_AT
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_STLP_MULTIMAP_BEGIN_END
#define FABLE_RETAIL_STLP_TREE_INSERT_EQUAL_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_CROSS_ORIGINAL_FORMAL
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
#define FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION
#define FABLE_RETAIL_CQUADRIC_MEMBERS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_INITIALISE
#define FABLE_RETAIL_ASSERT_DEAD_ARM

#define FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27
#define FABLE_RETAIL_CARRAY_FOLD_ORIGINAL_AT_PASS27
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#include <new>
#include "fable_assert.h"
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"
namespace QuadricOptimiserInternals {
bool COptimisedPrimitive::DetectFoldOver(const CSubContraction& sub_contraction,const C3DVector& position)
{
    const COptimisedVertex& merged_vertex=Vertices.At(sub_contraction.Index1);
    const COptimisedVertex& remove_vertex=Vertices.At(sub_contraction.Index2);
    CIndexList facelist=merged_vertex.FaceList.GetSymmetricDifference(remove_vertex.FaceList);
    std::set<unsigned long>::iterator itr;
    for (itr=facelist.Begin(); itr!=facelist.End(); ++itr) {
        unsigned long face_index=*itr;
        const COptimisedFace& face=Faces.At(face_index);
        if (!face.Exists) continue;
        unsigned long center_vertex=face.FindVertex(sub_contraction.Index1);
        if (center_vertex==static_cast<unsigned long>(-1))
            center_vertex=face.FindVertex(sub_contraction.Index2);
        const COptimisedVertex& vertex_a=Vertices.At(face.GetA());
        const COptimisedVertex& vertex_b=Vertices.At(face.GetB());
        const COptimisedVertex& vertex_c=Vertices.At(face.GetC());
        C3DVector normal=(vertex_c.Position-vertex_a.Position)^(vertex_b.Position-vertex_a.Position);
        if (normal.IsNonZero()) normal.Normalise();
        COptimisedVertex& edge_vertex_1=Vertices.At(face.GetVertexIndex((center_vertex+1)%3));
        COptimisedVertex& edge_vertex_2=Vertices.At(face.GetVertexIndex((center_vertex+2)%3));
        C3DVector vect12=edge_vertex_2.Position-edge_vertex_1.Position;
        C3DVector edge_normal=normal^vect12;
        if(edge_normal.IsNonZero()) edge_normal.Normalise();
        // Donor +41B and +42D evaluate position first, then the first edge vertex.
        if (edge_normal.Dot(position) > edge_normal.Dot(edge_vertex_1.Position))
            return true;
    }
    return false;
}
}