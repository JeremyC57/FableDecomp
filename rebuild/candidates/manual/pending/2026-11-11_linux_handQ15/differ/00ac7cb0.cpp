// QuadricOptimiserInternals::COptimisedPrimitive::GetFaceNormal(const COptimisedFace& face) const -- unit normal of a face: (p2 - p0) ^ (p1 - p0), fast-normalised unless it is (nearly) zero
// Grade: DIFFER(ours v retail) -- 350v350 (was 346v350 on the local model): ported onto engine/COptimisedPrimitive.h + QuadricOptimiserTypes.h + C3DVector.h; edge1 = v1-v0, edge2 = v2-v0, normal = edge2 ^ edge1, IsNonZero()/Normalise() fix the length, cross terms and final scale. Left: the IsNonZero sum -- retail squares Z, Y, X then sums fld st(0) ; fadd st(2) ; fadd st(3) (X2+Y2+Z2, 0x00AC7D51..0x00AC7D61); every IsNonZero/Normalise body probed (running / one-line / GFSqr; XYZ, ZYX, YXZ, YZX) sums fld st(1) ; fadd st(3) ; fadd st(1), and one-line IsNonZero forms lose the shared squares (338v350)
// Leaf: ?GetFaceNormal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABVCOptimisedFace@2@@Z
// Retail: Fable.exe 0x00AC7CB0, 350 bytes, thiscall `ret 8` (hidden C3DVector result, face); Ego_r.pdb name above.
// Callers: ComputeVertexCost 0x00AC8C80 (landed). Globals: DELTA_NOTIONAL_ZERO 0x0129BA3C, the GFFastOneOverSqrt mantissa table
//   0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C (all inside the shared C3DVector::IsNonZero / Normalise).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#include "engine/C3DVector.h"
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"

namespace QuadricOptimiserInternals {

C3DVector COptimisedPrimitive::GetFaceNormal(const COptimisedFace& face) const
{
    const COptimisedVertex& vertex0 = Vertices[face.VertexIndices[0]];
    const COptimisedVertex& vertex1 = Vertices[face.VertexIndices[1]];
    const COptimisedVertex& vertex2 = Vertices[face.VertexIndices[2]];
    C3DVector edge1 = vertex1.Position - vertex0.Position;
    C3DVector edge2 = vertex2.Position - vertex0.Position;
    C3DVector normal = edge2 ^ edge1;
    if (normal.IsNonZero())
        normal.Normalise();
    return normal;
}

}
