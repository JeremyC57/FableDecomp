// 007634e0 NShape::CBasicShape::UpdateSubdivisions
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
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_VECTOR_C
// Full shared shape classes and original TestShapes/ConvexShapes/iterators; no subdivisions or triangle aliases.
// Leaf: ?UpdateSubdivisions@CBasicShape@NShape@@UAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_NSHAPE_CONSTRUCTOR_BODIES_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
// NF397 (T3 nf395 0x00762440): __destroy_aux<CCountedPointer<NShape::CSubdivisionTriangle>*> is pick-any here (engine/NShape_STLHelpers.h
// opt-in), as retail's implicit STLport instantiation: its out-of-line owner is 0x00764EE0, whose copy is byte-and-reloc identical.
#define FABLE_RETAIL_NSHAPE_DESTROY_AUX_PICKANY
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

// Inline-budget: retail expands the element fill in this concrete vector insertion.
namespace std {
template<> FABLE_FORCEINLINE C3DVector* __uninitialized_fill_n(C3DVector* first, unsigned int n, const C3DVector& x, const __false_type&) {
    for (; n > 0; --n, ++first) _Construct(first,x);
    return first;
}
}

#include "engine/NShape.h"
#include "engine/NShape_CBasicShape.h"
#include "engine/NShape_CSubdivisionTriangle.h"
#include "engine/NShape_STLHelpers.h"
#include "stlport/fable_algorithm.h"
// Fidelity: FableWin and Ego_d agree on the frame slots for it_start, it_anchor and it_begin.
// FableWin 025D456C and 025D45B3 advance it_start twice. The second returned iterator is it_anchor.
// FableWin 025D4657 passes it_begin to find_if, then 025D46B1 advances it_begin after the call.
namespace NShape {

// Fidelity: same-TU strong definition of 0x00760020
bool FABLE_FASTCALL IsLineIntersectingWithOthersInList(const C2DLineF& line, const std::vector<C3DVector>& points_list)
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
        else {
            C2DVector intersection;
            if (this_line.IntersectsWith(line, intersection, DELTA_NOTIONAL_ZERO))
                number_of_intersections++;
        }
        start_pos = end_pos;
    }
    return number_of_intersections > 4;
}
// Fidelity: same-TU strong definition of 0x00760190
bool FABLE_FASTCALL IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle, const std::vector<C3DVector>& p_list, ECirculationOrder circ)
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
            C3DVector perp(line.Y, -line.X, 0.0f);
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

// Fidelity: same-TU strong definition of 0x00763060 (retail TU-mate), the owner row's body verbatim. Declaration-only measured
// DIFFER for this leaf (639v627, frame 0x34 v 0x30).
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

void NShape::CBasicShape::UpdateSubdivisions()
{
 Subdivisions.clear();
 ECirculationOrder order=CIRCULATION_CLOCKWISE;
 std::vector<std::vector<C3DVector> > TestShapes;
 std::vector<std::vector<C3DVector> > ConvexShapes;
 TestShapes.push_back(Points);
 while(!TestShapes.empty()) {
  std::vector<std::vector<C3DVector> >::iterator it=TestShapes.begin();
  std::vector<C3DVector> test_shape=*it;
  TestShapes.erase(it);
  FractureShape(test_shape,order,ConvexShapes,TestShapes);
 }
 for(std::vector<std::vector<C3DVector> >::iterator it_add=ConvexShapes.begin();it_add!=ConvexShapes.end();++it_add) {
  std::vector<C3DVector>& shape_to_add=*it_add;
  Subdivisions.push_back(CCountedPointer<CSubdivisionTriangle>(new CSubdivisionTriangle(Owner,shape_to_add)));
 }
}