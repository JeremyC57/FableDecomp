// NBBBMaths::CCylinder::GetFirstIntersectionWithTriangle(const std::vector<C3DVector>&, C3DVector&, C3DVector&) const -- first contact point of the curved cylinder surface with a triangle (the cylinder cut by the triangle plane is an ellipse)
// Retail: Fable.exe 0x00A593F0, 3,027 bytes, last instruction `ret 0xc` (__thiscall, edi = this); no EH frame. One caller (call at 0x00815CCC).
// FITTED: FABLE_RETAIL_C3DVECTOR_CROSS_COMPONENTS_GROUPED (new overlay opt-in: the landed CTCSHOTDEFINITIONBASE_COLLISION Cross text, every component parenthesised) -- chosen by grading; it alone moves retail's `fdiv [esp+20h]` ahead of the minor_radius integer copy at 0x00A59728 (the default Cross text leaves that one scheduling swap). EVIDENCED: the same-TU callee IsPointInsideTriangleInThePlane (0x00A58A60, precedes the row in the retail TU) is defined above the row with its landed text; c2 then knows it neither captures nor writes `points` (closed the old 51-hunk register web). CTransformWorldCoordsToPlaneCoords: FableWin.pdb ??0CTransformWorldCoordsToPlaneCoords@?A0xfeefc00d@@QAE@ABVC3DVector@@00@Z (anonymous namespace) + Ego_r ??$_Transform@...VCTransformWorldCoordsToPlaneCoords@?A0xfeefc00d@@; std::transform is STLport's text (retail 0x00A5A9D0 out of line).
// Leaf: ?GetFirstIntersectionWithTriangle@CCylinder@NBBBMaths@@QBE_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAVC3DVector@@1@Z
// Flags: /Gr
#include <math.h>
#pragma intrinsic(sqrt,fabs)
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_COMPONENTS_GROUPED
#define FABLE_RETAIL_STLP_BACK_INSERTER
#include "engine/NBBBMaths_CCylinder.h"
#include "engine/C2DVector.h"
extern const float DELTA_NOTIONAL_ZERO;
namespace NBBBMaths {
bool __fastcall IsPointInsideTriangleInThePlane(const std::vector<C2DVector>& points, const C2DVector& pos)   // 0x00A58A60 (same TU, landed text)
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

bool NBBBMaths::CCylinder::GetFirstIntersectionWithTriangle(const std::vector<C3DVector>& triangle, C3DVector& intersection_pos, C3DVector& outward_normal) const
{
    C3DVector cyl_ax = AxisTop - AxisBottom;
    float cyl_height = cyl_ax.GetMagnitude();
    C3DVector normal = (triangle[1] - triangle[0]).Cross(triangle[2] - triangle[0]);
    if (!(normal.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
        return false;
    normal.AccurateNormalise();
    float plane_dist = normal.Dot(triangle[0]);
    float cos_angle = normal.Dot(cyl_ax) / cyl_height;
    if (cos_angle < -1.0f)
        cos_angle = -1.0f;
    else if (cos_angle > 1.0f)
        cos_angle = 1.0f;
    if ((float)fabs(cos_angle) > DELTA_NOTIONAL_ZERO) {
        float axis_param = (plane_dist - normal.Dot(AxisBottom)) / (cos_angle * cyl_height);
        C3DVector plane_centre = AxisBottom + cyl_ax * axis_param;
        C3DVector minor_axis = cyl_ax.Cross(normal);
        if (!(minor_axis.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)) {
            minor_axis.Z = 0.0f;
            if ((float)fabs(normal.X) > (float)fabs(normal.Y)) {
                minor_axis.Y = 1.0f;
                if ((float)fabs(normal.X) > DELTA_NOTIONAL_ZERO)
                    minor_axis.X = -(normal.Y / normal.X);
                else
                    minor_axis.X = 0.0f;
            } else {
                minor_axis.X = 1.0f;
                if ((float)fabs(normal.Y) > DELTA_NOTIONAL_ZERO)
                    minor_axis.Y = -(normal.X / normal.Y);
                else
                    minor_axis.Y = 0.0f;
            }
        }
        minor_axis.AccurateNormalise();
        C3DVector major_axis = minor_axis.Cross(normal);
        major_axis.AccurateNormalise();
        float minor_radius = Radius;
        float major_radius = Radius / cos_angle;
        std::vector<C2DVector> points;
        std::transform(triangle.begin(), triangle.end(), std::back_inserter(points), CTransformWorldCoordsToPlaneCoords(plane_centre, major_axis, minor_axis));
        std::vector<C2DVector> intersections;
        float major_sq = major_radius * major_radius;
        float minor_sq = minor_radius * minor_radius;
        float product = major_radius * minor_radius;
        float product_sq = product * product;
        for (long i = 0; i < 3; ++i) {
            const C2DVector& line_start = points[(i + 2) % 3];
            C2DVector line = points[(i + 3) % 3] - line_start;
            C2DVector scaled_line(line.X * minor_radius, line.Y * major_radius);
            float coef_a = scaled_line.X * scaled_line.X + scaled_line.Y * scaled_line.Y;
            float coef_b = 2.0f * (line.Y * (line_start.Y * major_sq) + line.X * (line_start.X * minor_sq));
            C2DVector scaled_start(line_start.X * minor_radius, line_start.Y * major_radius);
            float coef_c = scaled_start.X * scaled_start.X + scaled_start.Y * scaled_start.Y - product_sq;
            float desc = coef_b * coef_b - 4.0f * coef_a * coef_c;
            if (desc > 0.0f) {
                desc = (float)sqrt(desc);
                float lambda1 = (-coef_b + desc) / (2.0f * coef_a);
                float lambda2 = (-coef_b - desc) / (2.0f * coef_a);
                if (lambda1 > -DELTA_NOTIONAL_ZERO && lambda1 < 1.0f + DELTA_NOTIONAL_ZERO)
                    intersections.push_back(line_start + C2DVector(lambda1 * line.X, lambda1 * line.Y));
                if (lambda2 > -DELTA_NOTIONAL_ZERO && lambda2 < 1.0f + DELTA_NOTIONAL_ZERO)
                    intersections.push_back(line_start + C2DVector(lambda2 * line.X, lambda2 * line.Y));
            }
        }
        if (intersections.begin() != intersections.end()) {
            for (std::vector<C2DVector>::iterator it = intersections.begin(); it != intersections.end(); ) {
                C2DVector pos = *it++;
                C3DVector int_pos = plane_centre + major_axis * pos.X + minor_axis * pos.Y;
                C3DVector out_line = int_pos - AxisBottom;
                float height_param = out_line.Dot(cyl_ax) / (cyl_height * cyl_height);
                if (height_param > -DELTA_NOTIONAL_ZERO && height_param < 1.0f + DELTA_NOTIONAL_ZERO) {
                    intersection_pos = int_pos;
                    outward_normal = out_line - cyl_ax * height_param;
                    return true;
                }
            }
        }
        long inside = 0;
        for (std::vector<C2DVector>::iterator it = points.begin(); it != points.end(); ++it) {
            const C2DVector& pos = *it;
            float limit;
            if ((float)fabs(pos.Y) > DELTA_NOTIONAL_ZERO) {
                float slope = pos.X / pos.Y;
                slope = slope / major_radius * minor_radius;
                slope = slope * slope;
                limit = (major_sq * slope + minor_sq) / (slope + 1.0f);
            } else
                limit = major_sq;
            if (pos.X * pos.X + pos.Y * pos.Y < limit)
                ++inside;
        }
        if (inside > 0) {
            for (std::vector<C2DVector>::iterator it = points.begin(); it != points.end(); ) {
                C2DVector pos = *it++;
                C3DVector int_pos = plane_centre + major_axis * pos.X + minor_axis * pos.Y;
                C3DVector out_line = int_pos - AxisBottom;
                float height_param = out_line.Dot(cyl_ax) / (cyl_height * cyl_height);
                if (height_param > -DELTA_NOTIONAL_ZERO && height_param < 1.0f + DELTA_NOTIONAL_ZERO) {
                    intersection_pos = int_pos;
                    outward_normal = out_line - cyl_ax * height_param;
                    return true;
                }
            }
        }
        if (IsPointInsideTriangleInThePlane(points, C2DVector(0.0f, 0.0f))) {
            C2DVector pos = points.front();
            C3DVector int_pos = plane_centre + major_axis * pos.X + minor_axis * pos.Y;
            C3DVector out_line = int_pos - AxisBottom;
            float height_param = out_line.Dot(cyl_ax) / (cyl_height * cyl_height);
            if (height_param > -DELTA_NOTIONAL_ZERO && height_param < 1.0f + DELTA_NOTIONAL_ZERO) {
                intersection_pos = int_pos;
                outward_normal = out_line - cyl_ax * height_param;
                return true;
            }
        }
    }
    return false;
}
