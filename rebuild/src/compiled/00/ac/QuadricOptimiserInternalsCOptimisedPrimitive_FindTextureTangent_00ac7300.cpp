// 00ac7300 QuadricOptimiserInternals::COptimisedPrimitive::FindTextureTangent
// Leaf: ?FindTextureTangent@COptimisedPrimitive@QuadricOptimiserInternals@@AAE?AVC3DVector@@ABV3@00MMM@Z
// Flags:
// QuadricOptimiserInternals::COptimisedPrimitive::FindTextureTangent, retail 00AC7300.
// Bracket: the shared grouped sum of GFSqr(X), GFSqr(Y), GFSqr(Z) and grouped seed product
// schedule the return-vector load and x87 stores at 00AC770B. Grouping the three Dot products
// individually moves that load. The ordinary shared Dot spelling matches all 339 instructions.
#include <math.h>
#include "fable_assert.h"
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_MESH_DUMMY_INVERSE_STEP_PASS26
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#include "engine/C3DVector.h"
#include "engine/COptimisedPrimitive.h"

namespace QuadricOptimiserInternals {

C3DVector COptimisedPrimitive::FindTextureTangent(const C3DVector& pa, const C3DVector& pb, const C3DVector& pc,
                                                  float ta, float tb, float tc)
{
    C3DVector pos[3];
    pos[0] = pa;
    pos[1] = pb;
    pos[2] = pc;
    float tex[3];
    tex[0] = ta;
    tex[1] = tb;
    tex[2] = tc;
    unsigned long hi_index = (tex[0] > tex[1] && tex[0] > tex[2]) ? 0 : (tex[1] > tex[2] ? 1 : 2);
    unsigned long lo_index = (tex[0] < tex[1] && tex[0] < tex[2]) ? 0 : (tex[1] < tex[2] ? 1 : 2);
    if (tex[lo_index] + DELTA_NOTIONAL_ZERO >= tex[hi_index])
        return C3DVector(0.0f, 0.0f, 0.0f);
    unsigned long mid_index = 0;
    for (unsigned long c_index = 0; c_index < 3; ++c_index) {
        if (c_index != hi_index && c_index != lo_index) {
            mid_index = c_index;
            break;
        }
    }
    float mid_t = tex[mid_index];
    float lo_t = tex[lo_index];
    float hi_t = tex[hi_index];
    FABLE_RETAIL_ASSERT(fabs(hi_t - lo_t) > DELTA_NOTIONAL_ZERO);
    float fraction = (mid_t - lo_t) / (hi_t - lo_t);
    C3DVector iso_pos = pos[hi_index] * fraction + pos[lo_index] * (1.0f - fraction);
    C3DVector iso_vector = iso_pos - pos[mid_index];
    iso_vector.Normalise();
    C3DVector face_normal = (pc - pa) ^ (pb - pa);
    face_normal.Normalise();
    C3DVector tangent_vector = iso_vector ^ face_normal;
    tangent_vector.Normalise();
    float mult = (hi_t - lo_t) / tangent_vector.Dot(pos[hi_index] - pos[lo_index]);
    tangent_vector *= mult;
    return tangent_vector;
}

}