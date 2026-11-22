// 00a58790 NBBBMaths::CSphere::GetFirstIntersectionWithFiniteLine
// Leaf: ?GetFirstIntersectionWithFiniteLine@CSphere@NBBBMaths@@QBE_NABVC3DVector@@0AAV3@1@Z
// Flags:
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX_SQUARED
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
#include "engine/CSphere.h"

namespace NBBBMaths {
bool CSphere::GetFirstIntersectionWithFiniteLine(
    const C3DVector& line_start, const C3DVector& line_end,
    C3DVector& intersection_pos, C3DVector& outward_normal) const
{
    C3DVector line_dir = line_end - line_start;
    if (!line_dir.IsNonZero())
        return false;

    C3DVector start_base = line_start - Base;
    C3DVector line_start_prime = line_start - Base;
    float a = line_dir.Dot(line_dir);
    float b = 2.0f * line_start_prime.Dot(line_dir);
    float c = line_start_prime.Dot(line_start_prime) - GFSqr(Radius);
    float desc = b * b - 4.0f * a * c;
    if (desc < 0.0f)
        return false;

    float root = GFRoot(desc);
    float lambda1 = (-b + root) / (2.0f * a);
    float lambda2 = (-b - root) / (2.0f * a);
    float best_lambda = 2.0f;
    C3DVector best_intersection_pt;
    if (lambda1 > 0.0f && lambda1 < 1.0f && lambda1 < best_lambda) {
        best_lambda = lambda1;
        best_intersection_pt = line_start + line_dir * lambda1;
    }
    if (lambda2 > 0.0f && lambda2 < 1.0f && lambda2 < best_lambda) {
        best_lambda = lambda2;
        best_intersection_pt = line_start + line_dir * lambda2;
    }
    if ((lambda1 < 0.0f && lambda2 > 1.0f) || (lambda2 < 0.0f && lambda1 > 1.0f)) {
        best_intersection_pt = line_start;
        best_lambda = 0.5f;
    }
    if (best_lambda > 0.0f && best_lambda < 1.0f) {
        intersection_pos = best_intersection_pt;
        outward_normal = intersection_pos - Base;
        outward_normal.AccurateNormalise();
        return true;
    }
    return false;
}
}