// NBBBMaths::CSphere::GetFirstIntersectionWithTriangle(const std::vector<C3DVector>&, C3DVector&, C3DVector&) const -- first contact point and outward normal of the sphere with a triangle
// Grade: DIFFER(2211v2207) -- 155 difflines: register colouring (retail keeps this in ebx and points' start in edi, ours the reverse), coef_b's terms load X first (retail Y first) and coef_c loads s.Y first (retail s.X), the lea of the indexed point scheduled differently; separate float locals for the line fix the term order but move the frame slots (line pair lowest)
// Next idea: keep C2DVector line but spell coef_b/coef_c through a by-value C2DVector Dot helper or a second inline (C2DVector::GetSquaredMagnitude) so the operand order flips without new named locals; then chase the ebx/edi swap by reordering the three result blocks' shared copy
// Leaf: ?GetFirstIntersectionWithTriangle@CSphere@NBBBMaths@@QBE_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAVC3DVector@@1@Z
// Flags: /Gr
#include <math.h>
#pragma intrinsic(sqrt,fabs)
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_C2DVECTOR
#include "engine/NBBBMaths_CSphere.h"
#include "engine/C2DVector.h"
extern const float DELTA_NOTIONAL_ZERO;
namespace NBBBMaths {
bool IsPointInsideTriangleInThePlane(const std::vector<C2DVector>& points, const C2DVector& pos);   // 0x00A58A60
}
namespace {
class CTransformWorldCoordsToPlaneCoords {
public:
    CTransformWorldCoordsToPlaneCoords(const C3DVector& origin, const C3DVector& axis_x, const C3DVector& axis_y)
        : Origin(&origin), AxisX(&axis_x), AxisY(&axis_y) { }
    C2DVector operator()(const C3DVector& pos) const;
private:
    const C3DVector* Origin;
    const C3DVector* AxisX;
    const C3DVector* AxisY;
};
}
namespace std {
template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value) { container->push_back(__value); return *this; }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
    back_insert_iterator<_Container>& operator++(int) { return *this; }
};
template <class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& __x) { return back_insert_iterator<_Container>(__x); }
template <class _InputIter, class _OutputIter, class _UnaryOperation>
_OutputIter transform(_InputIter __first, _InputIter __last, _OutputIter __result, _UnaryOperation __opr);   // 0x00A5A9D0
// STLport 4.6 vector::push_back, out of line in this retail TU (vector<C2DVector> 0x004428B0): a same-TU noinline body.
template <> inline __declspec(noinline) void vector<C2DVector, allocator<C2DVector> >::push_back(const C2DVector& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    } else
        _M_insert_overflow(this->_M_finish, __x, __true_type(), 1, true);
}
}

bool NBBBMaths::CSphere::GetFirstIntersectionWithTriangle(const std::vector<C3DVector>& triangle, C3DVector& intersection_pos, C3DVector& outward_normal) const
{
    C3DVector normal = (triangle[1] - triangle[0]).Cross(triangle[2] - triangle[0]);
    if (!(normal.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
        return false;
    normal.AccurateNormalise();
    float dist = normal.Dot(triangle[0]) - normal.Dot(Base);
    if (!((float)fabs(dist) < Radius && (float)fabs(dist) > DELTA_NOTIONAL_ZERO))
        return false;
    float radius_sq = Radius * Radius;
    float circle_radius_sq = radius_sq - dist * dist;
    if (circle_radius_sq < 0.0f)
        circle_radius_sq = 0.0f;
    else if (circle_radius_sq > radius_sq)
        circle_radius_sq = radius_sq;
    C3DVector circle_centre = Base + normal * dist;
    C3DVector axis_x;
    axis_x.Z = 0.0f;
    if ((float)fabs(normal.X) > (float)fabs(normal.Y)) {
        axis_x.Y = 1.0f;
        axis_x.X = -(normal.Y / normal.X);
    } else {
        axis_x.X = 1.0f;
        axis_x.Y = -(normal.X / normal.Y);
    }
    axis_x.AccurateNormalise();
    C3DVector axis_y = axis_x.Cross(normal);
    axis_y.AccurateNormalise();
    std::vector<C2DVector> points;
    std::transform(triangle.begin(), triangle.end(), std::back_inserter(points), CTransformWorldCoordsToPlaneCoords(circle_centre, axis_x, axis_y));
    std::vector<C2DVector> intersections;
    for (long i = 0; i < 3; ++i) {
        const C2DVector& line_start = points[(i + 2) % 3];
        C2DVector line = points[(i + 3) % 3] - line_start;
        float coef_a = line.X * line.X + line.Y * line.Y;
        float coef_b = 2.0f * line.Dot(line_start);
        float coef_c = line_start.Y * line_start.Y + line_start.X * line_start.X - Radius * Radius;
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
        C2DVector pos = *intersections.begin();
        intersection_pos = circle_centre + axis_x * pos.X + axis_y * pos.Y;
        outward_normal = intersection_pos - Base;
        if (outward_normal.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            outward_normal.AccurateNormalise();
        return true;
    }
    long inside = 0;
    for (const C2DVector* it = points.begin(); it != points.end(); ++it)
        if (it->X * it->X + it->Y * it->Y < circle_radius_sq)
            ++inside;
    if (inside != 0) {
        C2DVector pos = *points.begin();
        intersection_pos = circle_centre + axis_x * pos.X + axis_y * pos.Y;
        outward_normal = intersection_pos - Base;
        if (outward_normal.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            outward_normal.AccurateNormalise();
        return true;
    }
    if (IsPointInsideTriangleInThePlane(points, C2DVector(0.0f, 0.0f))) {
        C2DVector pos = *points.begin();
        intersection_pos = circle_centre + axis_x * pos.X + axis_y * pos.Y;
        outward_normal = intersection_pos - Base;
        if (outward_normal.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            outward_normal.AccurateNormalise();
        return true;
    }
    return false;
}
