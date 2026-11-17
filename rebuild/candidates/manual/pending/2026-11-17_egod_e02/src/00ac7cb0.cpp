// QuadricOptimiserInternals::COptimisedPrimitive::GetFaceNormal(const COptimisedFace& face) const -- unit normal of a face: (c - a) ^ (b - a) over its three vertices, fast-normalised unless (nearly) zero
// Leaf: ?GetFaceNormal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABVCOptimisedFace@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7CB0, 350 bytes, thiscall `ret 8` (hidden C3DVector result, face).
// Identity: Ego_r.pdb / FableWin.pdb (the leaf above, private const). Ego_d quadric optimiser TU: locals vertex_a, vertex_b, vertex_c
//   (const COptimisedVertex&) and normal (C3DVector) only -- no edge locals: the two differences are temporaries of one expression.
// Shape: the TU's opt-in set of the landed sibling COptimisedPrimitive::Initialise 0x00ACA830 (NORMALISE_NAMED_SQUARED,
//   SUBTRACT_NAMED_RESULT, CROSS_XZ_GROUP, MAGNITUDE_SQRTF). The draft's two named edges were the residue (IsNonZero sum order).
// Globals (inside the shared C3DVector inlines): DELTA_NOTIONAL_ZERO 0x0129BA3C, GFFastOneOverSqrt table 0x013CE558, 3.0f, 0.5f.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_NAMED_SQUARED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_NAMED_RESULT
#define FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#include "engine/C3DVector.h"
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"

namespace QuadricOptimiserInternals {

C3DVector COptimisedPrimitive::GetFaceNormal(const COptimisedFace& face) const
{
    const COptimisedVertex& vertex_a = Vertices[face.VertexIndices[0]];
    const COptimisedVertex& vertex_b = Vertices[face.VertexIndices[1]];
    const COptimisedVertex& vertex_c = Vertices[face.VertexIndices[2]];
    C3DVector normal = (vertex_c.Position - vertex_a.Position) ^ (vertex_b.Position - vertex_a.Position);
    if (normal.IsNonZero())
        normal.Normalise();
    return normal;
}

}
