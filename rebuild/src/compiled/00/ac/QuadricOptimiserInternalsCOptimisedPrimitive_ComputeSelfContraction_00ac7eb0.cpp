// QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction(CVertexPairContraction& contraction) -- cost of collapsing an unsplit vertex pair (one position, two texture/normal seams): weighted squared texture and normal distances, plus the high penalty when the normals face apart; the pair collapses onto the first vertex's position with weight 0.5
// Leaf: ?ComputeSelfContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7EB0, 215 bytes, `ret 4`. Ego_r.pdb private (AAE). Callees: none (fsqrt inline); 0.0f 0x0122DEDC.
// Ego_d locals: sub_contraction, vertex1, vertex2 (const references), tex_diff, normal_diff, cost (float).
// Shape: FableWin.pdb's /Od statement form for this function, as read by the hand3c author (NOTES.md:65; single read, to be
//   reconfirmed: FableWin 0x034C9560 should call ?GFSqr@@YIMM@Z 0x01ACE080 twice). FableWin's body (284 b) has Ego_d's debug length:
//   `cost = 0; cost += GFSqr(tex_diff) * UnsplitTextureWeight; cost += GFSqr(normal_diff) * UnsplitNormalWeight;` with
//   GFSqr = ?GFSqr@@YIMM@Z (float __fastcall, inline). Ego_d lists no squared local, consistent with GFSqr.
// GFSqr text: C3DVector.h's `(n*n)` body (FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER + _SQUARE_GROUPED_PASS23), the landed
//   0x00789CE0 recipe: an unbracketed GFSqr body lets c2 reassociate the term to `x * w * x` (measured: 213v215 with the default
//   `value*value` body and with the ORIGINAL_FORMAL `n*n` body). With `(n*n)`, retail's 0x00AC7F2B `fld st(1) ; fmulp st(2)` (tex
//   squared) then `fmul [ecx+74h]`, and 0x00AC7F34 `fld st(1) ; fmul st(2)` then `fmul [ecx+78h]` (normal squared, then weighted)
//   come out exactly; the `cost = 0` start folds away.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
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
    float normal_diff = (vertex2.Normal - vertex1.Normal).GetMagnitude();
    float cost = 0;
    cost += GFSqr(tex_diff) * Parameters.UnsplitTextureWeight;
    cost += GFSqr(normal_diff) * Parameters.UnsplitNormalWeight;
    if (vertex1.Normal.Dot(vertex2.Normal) < 0.0f)
        cost += Parameters.HighPenalty;
    contraction.Cost = cost;
    contraction.ResultingPosition = vertex1.Position;
    contraction.Weight2 = 0.5f;
}
}