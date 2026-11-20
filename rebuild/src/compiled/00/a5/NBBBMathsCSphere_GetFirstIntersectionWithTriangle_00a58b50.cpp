// NBBBMaths::CSphere::GetFirstIntersectionWithTriangle(const std::vector<C3DVector>&, C3DVector&, C3DVector&) const -- first contact point of the sphere with a triangle and its outward normal: the sphere cut by the triangle plane is a circle; the first edge/circle crossing, else the first corner inside the circle, else (circle centre inside the triangle) the first corner
// Inline-budget: no flag, no pragma, no new device. Header contracts only: the C3DVector helpers are FABLE_VECTOR_INLINE (__forceinline, owner 2026-11-01; IsNonZero under FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ, the same contract in every variant, so that opt-in changes the body, not expanded vs called) and GFSqr is a plain inline expanded as retail. The same-TU IsPointInsideTriangleInThePlane (0x00A58A60) is called, not expanded; its visible body (the Fidelity copy below, COMDAT byte-equal to retail 0x00A58A60) is needed: declaration only DIFFER(2211v2207). push_back<C2DVector> stays the plain STLport inline and is called (0x004428B0): FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (the draft's noinline specialization) is byte-neutral and dropped; the declaration-only FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE gives DIFFER(2169v2207).
// Leaf: ?GetFirstIntersectionWithTriangle@CSphere@NBBBMaths@@QBE_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAVC3DVector@@1@Z
// Flags: /Gr
// Retail: Fable.exe 0x00A58B50, 2,207 bytes, `ret 0xc` (ebx = this), frame 0x78, no EH frame; TU-mate of IsPointInsideTriangleInThePlane 0x00A58A60 (precedes it) and CCylinder::GetFirstIntersectionWithTriangle 0x00A593F0 (follows it); calls AccurateNormalise 0x00A14440, the transform 0x00A5A9D0, push_back<C2DVector> 0x004428B0, IsPointInsideTriangleInThePlane, free 0x00BFEA14 (the STLport allocator's release of the two local vectors).
// Identity: Ego_r.pdb / FableWin.pdb ?GetFirstIntersectionWithTriangle@CSphere@NBBBMaths@@QBE_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAVC3DVector@@1@Z (Radius +0x00, Base +0x04 = PDB CSphere). Local names and block nesting from Ego_d.pdb (debug length 3520; the line table is not a line-for-line fit, see the README): plane_normal, plane_dist, distance_plane_to_point; {sphere_rad_sqr, circle_rad_sqr, circle_centre, x_axis, y_axis, trig_prime, intersections, num_enclosed_pts, circle_rad} with {c1 {p1, p2, line, c_a, c_b, c_c, desc {lambda1 {int_pos} lambda2 {int_pos}}}}, {intersect2d}, {it {pt}}, {intersect2d}, {intersect2d}. circle_rad is the one Ego_d local with no retail trace (a dead `circle_rad = sqrt(circle_rad_sqr)` is byte-neutral) and is left out.
// Ego_d shape: the four magnitude tests are one 15-byte line each in Ego_d (lines 134/215/237/249); `if (v.IsNonZero())` is 15 B at /Od, the spelled-out test 27 B = C3DVector::IsNonZero (FableWin ?IsNonZero@C3DVector@@QBE_NXZ, its donor body FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ; IsNonZero itself is byte-neutral against the spelled-out test, the default IsNonZero body is DIFFER, 52 lines); no separate sqrt line between `if (desc > 0)` (187) and the lambdas (190/191, 55 B each; ours 44 B with sqrt inside, 25 B without), so sqrt(desc) sits in both lambda expressions (byte-neutral; c2 shares the one fsqrt); int_pos + push_back(int_pos) (Ego_d 196/197, 47 + 15 B; ours 51 + 14) and pt (225/226, 17 + 23 B; ours 10 + 20) are named as Ego_d has them (byte-neutral).
// Inferred: c_a and c_c through GFSqr (FableWin/Ego_d ?GFSqr@@YIMM@Z), retail sites 0x00A58E19..0x00A58E3D. Ego_d corroborates the call form: its c_c line is 34 B longer than its c_a line (82 - 48), as the GFSqr form's are (65 - 31 = 34; the inline form gives 28), i.e. one more GFSqr call in c_c than in c_a. All three of Ego_d's lines 182/183/184 still run over ours (c_a +17, c_b 43 v 21 +22, c_c +17): a construct common to those lines is unidentified. The bytes: with the inline product c_c is DIFFER (10 lines: p1.X/p1.Y load order), c_a DIFFER (2 lines: the lea of &p2 at 0x00A58E3D). c_b and desc are not pinned by the bytes (GFSqr(c_b), the spelled-out dot product: neutral).
// FITTED: GFSqr's body is the header's `(n*n)` spelling (FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER + FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23, existing opt-ins; the helper gate also swaps Normalise's body, which this row never expands): `n*n` (GFWorldDistance.h's copy, or the gate without GROUPED) leaves the 0x00A58E3D lea scheduled two instructions late (DIFFER, 2 lines). FABLE_RETAIL_C3DVECTOR (the retail C3DVector branch, 760 landed rows) is needed by IsNonZero and carries the retail GetSquaredMagnitude text; without it (IsNonZero spelled out) c_b's two products swap at 0x00A58E19 (DIFFER, 4 lines).
// EVIDENCED: the same-TU IsPointInsideTriangleInThePlane is defined above the row with its landed text (without it: DIFFER(2211v2207), 145 lines, the this/points register web); CTransformWorldCoordsToPlaneCoords and std::transform are the landed TU-mate 0x00A593F0's text (each byte-neutral against a declaration).
#include <math.h>
#pragma intrinsic(sqrt,fabs)
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_CSPHERE_TRIANGLE
#include "engine/CSphere.h"
#include "engine/C2DVector.h"
extern const float DELTA_NOTIONAL_ZERO;
namespace NBBBMaths {
// Fidelity: same-TU strong definition of 0x00A58A60 (retail TU-mate)
bool __fastcall IsPointInsideTriangleInThePlane(const std::vector<C2DVector>& points, const C2DVector& pos)
{
    C2DVector edge = points[0] - points[2];
    C2DVector toPos = pos - points[2];
    float sign = edge.X * toPos.Y - edge.Y * toPos.X;
    for (long i = 0; i != 2; ++i) {
        edge = points[i + 1] - points[i];
        toPos = pos - points[i];
        float cross = edge.X * toPos.Y - edge.Y * toPos.X;
        if ((sign > 0.0f && cross < 0.0f) || (sign < 0.0f && cross > 0.0f))
            return false;
    }
    return true;
}
}
namespace {
class CTransformWorldCoordsToPlaneCoords {
public:
    CTransformWorldCoordsToPlaneCoords(const C3DVector& plane_origin, const C3DVector& axis_x, const C3DVector& axis_y)
        : PlaneOrigin(&plane_origin), AxisX(&axis_x), AxisY(&axis_y) { }
    C2DVector operator()(const C3DVector& pos) const
    {
        C3DVector offset = pos - *PlaneOrigin;
        return C2DVector(offset.Dot(*AxisX), offset.Dot(*AxisY));
    }
private:
    const C3DVector* PlaneOrigin;
    const C3DVector* AxisX;
    const C3DVector* AxisY;
};
}
namespace std {
template <class _InputIter, class _OutputIter, class _UnaryOperation>
_OutputIter transform(_InputIter __first, _InputIter __last, _OutputIter __result, _UnaryOperation __opr)
{
    for ( ; __first != __last; ++__first, ++__result)
        *__result = __opr(*__first);
    return __result;
}
}

bool NBBBMaths::CSphere::GetFirstIntersectionWithTriangle(const std::vector<C3DVector>& triangle, C3DVector& intersection_pos, C3DVector& outward_normal) const
{
    C3DVector plane_normal = (triangle[1] - triangle[0]).Cross(triangle[2] - triangle[0]);

    if (!plane_normal.IsNonZero())
        return false;

    plane_normal.AccurateNormalise();

    float plane_dist = plane_normal.Dot(triangle[0]);
    float distance_plane_to_point = plane_dist - plane_normal.Dot(Base);

    if ((float)fabs(distance_plane_to_point) < Radius && (float)fabs(distance_plane_to_point) > DELTA_NOTIONAL_ZERO)
    {
        float sphere_rad_sqr = Radius * Radius;
        float circle_rad_sqr = sphere_rad_sqr - distance_plane_to_point * distance_plane_to_point;
        if (circle_rad_sqr < 0.0f)
            circle_rad_sqr = 0.0f;
        else if (circle_rad_sqr > sphere_rad_sqr)
            circle_rad_sqr = sphere_rad_sqr;
        C3DVector circle_centre = Base + plane_normal * distance_plane_to_point;
        C3DVector x_axis;
        x_axis.Z = 0.0f;
        if ((float)fabs(plane_normal.X) > (float)fabs(plane_normal.Y)) {
            x_axis.Y = 1.0f;
            x_axis.X = -(plane_normal.Y / plane_normal.X);
        } else {
            x_axis.X = 1.0f;
            x_axis.Y = -(plane_normal.X / plane_normal.Y);
        }
        x_axis.AccurateNormalise();
        C3DVector y_axis = x_axis.Cross(plane_normal);
        y_axis.AccurateNormalise();
        std::vector<C2DVector> trig_prime;
        std::transform(triangle.begin(), triangle.end(), std::back_inserter(trig_prime), CTransformWorldCoordsToPlaneCoords(circle_centre, x_axis, y_axis));
        std::vector<C2DVector> intersections;
        for (long c1 = 0; c1 < 3; ++c1) {
            const C2DVector& p1 = trig_prime[(c1 + 2) % 3];
            const C2DVector& p2 = trig_prime[(c1 + 3) % 3];
            C2DVector line = p2 - p1;
            float c_a = GFSqr(line.X) + GFSqr(line.Y);
            float c_b = 2.0f * line.Dot(p1);
            float c_c = GFSqr(p1.X) + GFSqr(p1.Y) - GFSqr(Radius);
            float desc = c_b * c_b - 4.0f * c_a * c_c;
            if (desc > 0.0f) {
                float lambda1 = (-c_b + (float)sqrt(desc)) / (2.0f * c_a);
                float lambda2 = (-c_b - (float)sqrt(desc)) / (2.0f * c_a);
                if (lambda1 > -DELTA_NOTIONAL_ZERO && lambda1 < 1.0f + DELTA_NOTIONAL_ZERO)
                {
                    C2DVector int_pos = p1 + C2DVector(lambda1 * line.X, lambda1 * line.Y);
                    intersections.push_back(int_pos);
                }
                if (lambda2 > -DELTA_NOTIONAL_ZERO && lambda2 < 1.0f + DELTA_NOTIONAL_ZERO)
                {
                    C2DVector int_pos = p1 + C2DVector(lambda2 * line.X, lambda2 * line.Y);
                    intersections.push_back(int_pos);
                }
            }
        }
        if (intersections.begin() != intersections.end()) {
            C2DVector intersect2d = *intersections.begin();
            intersection_pos = circle_centre + x_axis * intersect2d.X + y_axis * intersect2d.Y;
            outward_normal = intersection_pos - Base;
            if (outward_normal.IsNonZero())
                outward_normal.AccurateNormalise();
            return true;
        }
        long num_enclosed_pts = 0;
        for (std::vector<C2DVector>::const_iterator it = trig_prime.begin(); it != trig_prime.end(); ++it) {
            const C2DVector& pt = *it;
            if (pt.GetSquaredMagnitude() < circle_rad_sqr)
                ++num_enclosed_pts;
        }
        if (num_enclosed_pts != 0) {
            C2DVector intersect2d = *trig_prime.begin();
            intersection_pos = circle_centre + x_axis * intersect2d.X + y_axis * intersect2d.Y;
            outward_normal = intersection_pos - Base;
            if (outward_normal.IsNonZero())
                outward_normal.AccurateNormalise();
            return true;
        }
        if (IsPointInsideTriangleInThePlane(trig_prime, C2DVector(0.0f, 0.0f))) {
            C2DVector intersect2d = *trig_prime.begin();
            intersection_pos = circle_centre + x_axis * intersect2d.X + y_axis * intersect2d.Y;
            outward_normal = intersection_pos - Base;
            if (outward_normal.IsNonZero())
                outward_normal.AccurateNormalise();
            return true;
        }
    }
    return false;
}