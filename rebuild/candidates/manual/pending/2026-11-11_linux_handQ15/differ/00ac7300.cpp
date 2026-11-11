// QuadricOptimiserInternals::COptimisedPrimitive::FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2, float t0, float t1, float t2) -- the texture-space tangent of a triangle for one texture coordinate: interpolate along the min..max coordinate edge to the middle vertex's coordinate, take the in-plane direction perpendicular to that iso-line, and scale it so it spans one coordinate unit
// Grade: DIFFER(ours v retail) -- 1077v1091 (local model 1075v1091), ported onto engine/C3DVector.h (operator^, Normalise, Dot) + COptimisedPrimitive.h retail branch (FindTextureTangent decl, AAE): the same x87 tail residue -- retail spills the final scale to the dead parameter slot [esp+6Ch] and loads all three scaled components before the stores (0x00AC770B..0x00AC7717), keeps a dead duplicate product in the third cross component (0x00AC7655 fld st(0) .. fstp st(0)), and copies the three points through mov eax,edx (0x00AC7310); one grade on the port
// Leaf: ?FindTextureTangent@COptimisedPrimitive@QuadricOptimiserInternals@@AAE?AVC3DVector@@ABV3@00MMM@Z
// Retail: Fable.exe 0x00AC7300, 1091 bytes, thiscall `ret 1Ch` (hidden C3DVector result + 6 args; Ego_r.pdb name above).
// Caller: FindFaceTextureTangents 0x00AC7E10 (landed). Globals: DELTA_NOTIONAL_ZERO 0x0129BA3C, 1.0f 0x0122DED8, and the shared
//   C3DVector::Normalise (GFFastOneOverSqrt: table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#include "engine/C3DVector.h"
#include "engine/COptimisedPrimitive.h"

namespace QuadricOptimiserInternals {

C3DVector COptimisedPrimitive::FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2,
                                                  float t0, float t1, float t2)
{
    C3DVector points[3] = { p0, p1, p2 };
    float coords[3] = { t0, t1, t2 };
    unsigned long max_index = (t0 > t1 && t0 > t2) ? 0 : (t1 > t2 ? 1 : 2);
    unsigned long min_index = (t0 < t1 && t0 < t2) ? 0 : (t1 < t2 ? 1 : 2);
    unsigned long mid_index = 0;
    if (coords[min_index] + DELTA_NOTIONAL_ZERO >= coords[max_index])
        return C3DVector(0.0f, 0.0f, 0.0f);
    for (unsigned long i = 0; i < 3; ++i) {
        if (i != max_index && i != min_index) {
            mid_index = i;
            break;
        }
    }
    float min_coord = coords[min_index];
    float range = coords[max_index] - min_coord;
    float fraction = (coords[mid_index] - min_coord) / range;
    const C3DVector& min_point = points[min_index];
    const C3DVector& max_point = points[max_index];
    const C3DVector& mid_point = points[mid_index];
    C3DVector iso_direction = max_point * fraction + min_point * (1.0f - fraction) - mid_point;
    iso_direction.Normalise();
    C3DVector normal = (p2 - p0) ^ (p1 - p0);
    normal.Normalise();
    C3DVector tangent = iso_direction ^ normal;
    tangent.Normalise();
    float scale = range / (max_point - min_point).Dot(tangent);
    return tangent * scale;
}

}
