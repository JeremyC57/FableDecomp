// QuadricOptimiserInternals::COptimisedPrimitive::FindAdjacentFaces() -- rebuild edges by pairing faces that share two unique vertices and have compatible normals.
// Grade: DIFFER(743v745) -- The shared STLport and quadric types reproduce the extent closely, but the normal dot pair and the adjacent-face erase/sweep allocation differ.
// Leaf: ?FindAdjacentFaces@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXXZ
// Retail: 0x00AC9A60, 745 bytes, thiscall.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_COPY
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_CINDEXLIST_INTERSECTION
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_JOINADJACENTFACES
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_INITIALISE
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"

namespace QuadricOptimiserInternals {
void COptimisedPrimitive::FindAdjacentFaces()
{
    Edges.clear();
    for (unsigned long c_face = 0; c_face < Faces.size(); ++c_face) {
        COptimisedFace& face = Faces[c_face];
        for (unsigned long c_side = 0; c_side < 3; ++c_side) {
            if (face.Edges[c_side] == -1) {
                unsigned long unique1 = face.UniqueVertexIndices[c_side];
                unsigned long unique2 = face.UniqueVertexIndices[(c_side + 1) % 3];
                CIndexList facelist_1(UniqueVertices[unique1].FaceList);
                CIndexList facelist_2(UniqueVertices[unique2].FaceList);
                CIndexList facelist = facelist_1.GetIntersection(facelist_2);
                for (std::set<unsigned long>::iterator face_itr = facelist.Indices.begin();
                     face_itr != facelist.Indices.end(); ++face_itr) {
                    unsigned long face_index = *face_itr;
                    if (face_index != c_face) {
                        COptimisedFace& face2 = Faces[face_index];
                        if (GetFaceNormal(face).Dot(GetFaceNormal(face2)) > -0.99f)
                            JoinAdjacentFaces(c_face, face_index);
                    }
                }
            }
        }
    }
}
}
