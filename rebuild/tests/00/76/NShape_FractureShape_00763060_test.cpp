// Behaviour test for 0x00763060 NShape::FractureShape: the row text verbatim (self-contained: NShape.h's CCountedPointer specialisations
// are strong, so a test object cannot link beside the row object); the two out-of-file callees (the find_if predicate and
// C2DLineF::IntersectsWith) are controlled here. Fixture after Codex pass 26 (0x007634E0's test),
// plus cases that take a second find_if pass after a candidate is rejected by either callee and push both splices.
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_COMPARE
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_BODY
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DLINEF_FROM3D
#define FABLE_RETAIL_C2DLINEF_INTERSECTS_LINE
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

#include "engine/NShape.h"
#include "engine/NShape_STLHelpers.h"
#include "stlport/fable_algorithm.h"
#include <cstdio>
static int errors=0;
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d %s\n",__LINE__,#x);}}while(0)

extern const float DELTA_NOTIONAL_ZERO = 0.0001220703125f;
// Predicate control: mode 0 accept all, 1 accept none, 2 accept only the point with X == accept_x. Every point offered is logged.
static int pred_mode = 0; static float accept_x = 0; static float offered[32]; static int n_offered = 0;
bool NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation::operator()(const C3DVector& p) const
{
    if (n_offered < 32) offered[n_offered++] = p.X;
    return pred_mode == 0 || (pred_mode == 2 && p.X == accept_x);
}
// Intersection control: when reject_on, every polygon edge "crosses" a candidate line ending at (reject_x, reject_y).
static bool reject_on = false; static float reject_x = 0, reject_y = 0;
bool C2DLineF::IntersectsWith(const C2DLineF& other, C2DVector&, float tolerance) const
{
    CHECK(tolerance == DELTA_NOTIONAL_ZERO);
    return reject_on && other.Point2.X == reject_x && other.Point2.Y == reject_y;
}
// The row text (0x00763060 with its two retail TU-mates), verbatim.
namespace NShape {

// Fidelity: same-TU strong definition of 0x00760020 (retail TU-mate), the owner row's body verbatim. Declaration-only measured
// DIFFER for this leaf (0x00760020 declared: 1156v1146; 0x00760190 declared: 1164v1146): VC7.1 uses the TU-mate's register use.
bool __fastcall IsLineIntersectingWithOthersInList(const C2DLineF& line, const std::vector<C3DVector>& points_list)
{
    long number_of_intersections = 0;
    std::vector<C3DVector>::const_iterator it = points_list.begin();
    C3DVector start_pos = *it++;
    bool quit = false;
    while (!quit)
    {
        C3DVector end_pos;
        if (it == points_list.end())
        {
            end_pos = *points_list.begin();
            quit = true;
        }
        else
        {
            end_pos = *it++;
        }
        C2DLineF this_line(start_pos, end_pos);
        if ((this_line.Point1 - line.Point1).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO && (this_line.Point2 - line.Point2).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO)
            number_of_intersections++;
        else if ((this_line.Point1 - line.Point2).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO && (this_line.Point2 - line.Point1).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO)
            number_of_intersections++;
        else if (this_line.IntersectsWith(line, C2DVector(), DELTA_NOTIONAL_ZERO))
            number_of_intersections++;
        start_pos = end_pos;
    }
    return number_of_intersections > 4;
}
// Fidelity: same-TU strong definition of 0x00760190 (retail TU-mate), the owner row's body verbatim. Declaration-only measured
// DIFFER for this leaf (0x00760020 declared: 1156v1146; 0x00760190 declared: 1164v1146): VC7.1 uses the TU-mate's register use.
bool __fastcall IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle, const std::vector<C3DVector>& p_list, ECirculationOrder circ)
{
    long number_of_points_inside = 0;
    for (std::vector<C3DVector>::const_iterator it = p_list.begin(); it != p_list.end(); ++it)
    {
        std::vector<C3DVector>::const_iterator it_trig = triangle.begin();
        C3DVector start_pos = *it_trig++;
        bool quit = false;
        long inside_count = 0;
        while (!quit)
        {
            C3DVector end_pos;
            if (it_trig == triangle.end())
            {
                end_pos = *triangle.begin();
                quit = true;
            }
            else
            {
                end_pos = *it_trig++;
            }
            C3DVector line = end_pos - start_pos;
            C3DVector edge = *it - start_pos;
            C3DVector perp = line.CrossWithUp();
            if (circ == CIRCULATION_ANTICLOCKWISE)
                perp *= -1.0f;
            if (perp.Dot(edge) > -DELTA_NOTIONAL_ZERO)
                inside_count++;
            start_pos = end_pos;
        }
        if (inside_count == 3)
            number_of_points_inside++;
    }
    if (number_of_points_inside > 3)
        return true;
    return false;
}

void FABLE_FASTCALL FractureShape(const std::vector<C3DVector>& test_shape,
                                 ECirculationOrder& circ,
                                 std::vector<std::vector<C3DVector> >& convex_shapes,
                                 std::vector<std::vector<C3DVector> >& concave_shapes)
{
    std::vector<C3DVector> duplicate_shape(test_shape);
    std::vector<C3DVector>::iterator it_start = duplicate_shape.begin();
    C3DVector a = *it_start++;
    std::vector<C3DVector>::iterator it_anchor = it_start++;
    C3DVector b = *it_anchor;

    bool found_appropriate = false;
    bool quit = false;
    std::vector<C3DVector>::iterator it_result;
    std::vector<C3DVector>::iterator it_begin = it_start;
    while (!quit) {
        it_result = std::find_if(it_begin, duplicate_shape.end(),
            CIsPointFinalLinkInTriangleWithMatchingCirculation(a, b, circ));
        it_begin++;
        if (it_result != duplicate_shape.end()) {
            const C3DVector& result_point = *it_result;
            C2DLineF line_a(a, result_point);
            C2DLineF line_b(b, result_point);
            if (!IsLineIntersectingWithOthersInList(line_a, duplicate_shape) &&
                !IsLineIntersectingWithOthersInList(line_b, duplicate_shape)) {
                std::vector<C3DVector> triangle;
                triangle.push_back(a);
                triangle.push_back(b);
                triangle.push_back(*it_result);
                if (!IsTriangleWithPointsInside(triangle, duplicate_shape, circ)) {
                    found_appropriate = true;
                    quit = true;
                }
            }
        } else {
            quit = true;
        }
    }

    long distance = std::distance(duplicate_shape.begin(), it_result);
    if (found_appropriate) {
        std::vector<C3DVector> triangle;
        triangle.push_back(a);
        triangle.push_back(b);
        triangle.push_back(*it_result);
        convex_shapes.push_back(triangle);

        std::vector<C3DVector>::iterator it_del_start;
        std::vector<C3DVector>::iterator it_del_end;
        std::vector<C3DVector> splice_one(duplicate_shape);
        it_del_start = splice_one.begin();
        ++it_del_start;
        it_del_end = splice_one.begin();
        it_del_end += distance;
        splice_one.erase(it_del_start, it_del_end);
        long size_1 = splice_one.size();
        if (size_1 > 2) concave_shapes.push_back(splice_one);

        std::vector<C3DVector> splice_two(duplicate_shape);
        it_del_start = splice_two.begin();
        it_del_start += distance + 1;
        it_del_end = splice_two.end();
        splice_two.erase(it_del_start, it_del_end);
        splice_two.erase(splice_two.begin());
        long size_2 = splice_two.size();
        if (size_2 > 2) concave_shapes.push_back(splice_two);
        return;
    } else if (circ == CIRCULATION_CLOCKWISE) {
        circ = CIRCULATION_ANTICLOCKWISE;
        concave_shapes.push_back(test_shape);
    }
}
}
using namespace NShape;
typedef std::vector<C3DVector> Shape;
typedef std::vector<Shape> Shapes;
static Shape make(const float* xy, int n) { Shape s; for (int k = 0; k < n; ++k) s.push_back(C3DVector(xy[2*k], xy[2*k+1], 0)); return s; }
static bool same(const Shape& s, const float* xy, int n)
{
    if ((int)s.size() != n) return false;
    for (int k = 0; k < n; ++k) if (s[k].X != xy[2*k] || s[k].Y != xy[2*k+1]) return false;
    return true;
}
static void reset() { pred_mode = 0; n_offered = 0; reject_on = false; }

int main()
{
    // Codex's fixture: a triangle with no ear accepted, then accepted; a square.
    {
        const float tri[] = {0,0, 0,4, 4,0};
        Shape triangle = make(tri, 3); Shapes convex, concave; ECirculationOrder order = CIRCULATION_CLOCKWISE;
        reset(); pred_mode = 1;
        FractureShape(triangle, order, convex, concave);
        CHECK(order == CIRCULATION_ANTICLOCKWISE && convex.empty() && concave.size() == 1 && same(concave[0], tri, 3));
        CHECK(n_offered == 1 && offered[0] == 4);
        concave.clear(); reset(); pred_mode = 1;
        FractureShape(triangle, order, convex, concave);   // anticlockwise and no ear: nothing queued, order kept
        CHECK(order == CIRCULATION_ANTICLOCKWISE && convex.empty() && concave.empty());
        order = CIRCULATION_CLOCKWISE; reset();
        FractureShape(triangle, order, convex, concave);
        CHECK(order == CIRCULATION_CLOCKWISE && convex.size() == 1 && concave.empty() && same(convex[0], tri, 3));
        const float sq[] = {0,0, 0,4, 4,4, 4,0};
        Shape square = make(sq, 4); convex.clear(); reset();
        FractureShape(square, order, convex, concave);
        const float sq_tri[] = {0,0, 0,4, 4,4}, sq_one[] = {0,0, 4,4, 4,0};
        // distance 2: splice_one = {p0,p2,p3} (pushed); splice_two = {p1,p2} (two points, dropped)
        CHECK(convex.size() == 1 && same(convex[0], sq_tri, 3) && concave.size() == 1 && same(concave[0], sq_one, 3));
    }
    // Hexagon (clockwise), candidate p2 rejected by IsLineIntersectingWithOthersInList (all six edges cross line_a), so find_if runs again
    // from it_begin (advanced to p3): ear (p0,p1,p3), distance 3, both splices pushed.
    {
        const float hex[] = {0,0, 0,4, 3,6, 6,4, 6,0, 3,-2};
        const float ear[] = {0,0, 0,4, 6,4}, one[] = {0,0, 6,4, 6,0, 3,-2}, two[] = {0,4, 3,6, 6,4};
        Shape h = make(hex, 6); Shapes convex, concave; ECirculationOrder order = CIRCULATION_CLOCKWISE;
        reset(); reject_on = true; reject_x = 3; reject_y = 6;
        FractureShape(h, order, convex, concave);
        CHECK(n_offered == 2 && offered[0] == 3 && offered[1] == 6);
        CHECK(order == CIRCULATION_CLOCKWISE && convex.size() == 1 && same(convex[0], ear, 3));
        CHECK(concave.size() == 2 && same(concave[0], one, 4) && same(concave[1], two, 3));
        CHECK(same(h, hex, 6));   // test_shape untouched
    }
    // Hexagon, the predicate accepts X == 6 only (p3, p4) and p3's lines are rejected: pass 1 offers p2, p3 (rejected); it_begin advances one
    // point per pass (not past the result), so pass 2 offers p3 again (rejected) and pass 3 offers p4: ear (p0,p1,p4), distance 4.
    {
        const float hex[] = {0,0, 0,4, 3,6, 6,4, 6,0, 3,-2};
        const float ear[] = {0,0, 0,4, 6,0}, one[] = {0,0, 6,0, 3,-2}, two[] = {0,4, 3,6, 6,4, 6,0};
        Shape h = make(hex, 6); Shapes convex, concave; ECirculationOrder order = CIRCULATION_CLOCKWISE;
        reset(); pred_mode = 2; accept_x = 6;
        reject_on = true; reject_x = 6; reject_y = 4;
        FractureShape(h, order, convex, concave);
        CHECK(n_offered == 4 && offered[0] == 3 && offered[1] == 6 && offered[2] == 6 && offered[3] == 6);
        CHECK(convex.size() == 1 && same(convex[0], ear, 3));
        CHECK(concave.size() == 2 && same(concave[0], one, 3) && same(concave[1], two, 4));
    }
    // Concave pentagon: candidate p2 rejected by IsTriangleWithPointsInside (p3 lies in (p0,p1,p2)), then p3 accepted: distance 3.
    {
        const float pent[] = {0,0, 0,8, 8,8, 2,4, 8,0};
        const float ear[] = {0,0, 0,8, 2,4}, one[] = {0,0, 2,4, 8,0}, two[] = {0,8, 8,8, 2,4};
        Shape p = make(pent, 5); Shapes convex, concave; ECirculationOrder order = CIRCULATION_CLOCKWISE;
        const float t2[] = {0,0, 0,8, 8,8};
        CHECK(IsTriangleWithPointsInside(make(t2, 3), p, CIRCULATION_CLOCKWISE));
        reset();
        FractureShape(p, order, convex, concave);
        CHECK(n_offered == 2 && offered[0] == 8 && offered[1] == 2);
        CHECK(convex.size() == 1 && same(convex[0], ear, 3));
        CHECK(concave.size() == 2 && same(concave[0], one, 3) && same(concave[1], two, 3));
    }
    std::puts(errors ? "00763060_TEST FAIL" : "00763060_TEST PASS");
    return errors ? 1 : 0;
}