// QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction(CVertexPairContraction& contraction) -- cost of collapsing an unsplit vertex pair (one position, two texture/normal seams): weighted squared texture and normal distances, plus the high penalty when the normals face apart; the pair collapses onto the first vertex's position with weight 0.5
// Leaf: ?ComputeSelfContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7EB0, 215 bytes, `ret 4`. Ego_r.pdb private (AAE). Callees: none (fsqrt inline); 0.0f 0x0122DEDC.
// Ego_d locals: sub_contraction, vertex1, vertex2 (const references), tex_diff, normal_diff, cost (float).
// FITTED (statement form): each distance is squared in place right after it is measured (`tex_diff *= tex_diff;`,
//   `normal_diff *= normal_diff;`; Ego_d lists no separate squared local) and the cost is built in two statements
//   (`cost = tex_diff * W; cost += normal_diff * W;`). Retail squares each term before weighting it: 0x00AC7F2B `fld st(1) ;
//   fmulp st(2)` (tex in place) then `fmul [ecx+74h]`, 0x00AC7F34 `fld st(1) ; fmul st(2)` then `fmul [ecx+78h]`. VC7.1
//   reassociates every chained spelling (`x * x * w`, brackets, GFSqr, one-statement sums) to `x * w * x`, and squaring
//   only one term in place or keeping a one-statement cost moves the other term (measured: 9 one-step variants, only this form
//   matches; symmetric for both terms, so one cause at both sites). The `cost +=` accumulation is FableWin.pdb's statement form
//   (its later revision builds the cost with `cost +=` per term).
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
#define FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION
#define FABLE_RETAIL_CQUADRIC_MEMBERS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_INITIALISE
#include <new>
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"
namespace QuadricOptimiserInternals {
void COptimisedPrimitive::ComputeSelfContraction(CVertexPairContraction& contraction)
{
    const CSubContraction& sub_contraction = contraction.SubContractions[0];
    const COptimisedVertex& vertex1 = Vertices[sub_contraction.Index1];
    const COptimisedVertex& vertex2 = Vertices[sub_contraction.Index2];
    float tex_diff = (vertex2.TextureCoord - vertex1.TextureCoord).GetMagnitude();
    tex_diff *= tex_diff;
    float normal_diff = (vertex2.Normal - vertex1.Normal).GetMagnitude();
    normal_diff *= normal_diff;
    float cost = tex_diff * Parameters.UnsplitTextureWeight;
    cost += normal_diff * Parameters.UnsplitNormalWeight;
    if (vertex1.Normal.Dot(vertex2.Normal) < 0.0f)
        cost += Parameters.HighPenalty;
    contraction.Cost = cost;
    contraction.ResultingPosition = vertex1.Position;
    contraction.Weight2 = 0.5f;
}
}
