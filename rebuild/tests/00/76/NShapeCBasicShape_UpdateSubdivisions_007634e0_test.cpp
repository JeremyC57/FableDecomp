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
#include <cstdio>
static int errors=0,live=0,deleted_old=0,calls=0,sequence[10];
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d %s\n",__LINE__,#x);}}while(0)

// These two out-of-file dependencies are controlled to exercise the recovered polygon routines.
extern const float DELTA_NOTIONAL_ZERO = 0.0001220703125f;
static bool allow_proposal = true;
static bool intersect_external = false;
bool NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation::operator()(const C3DVector&) const
{
    return allow_proposal;
}
bool C2DLineF::IntersectsWith(const C2DLineF&, C2DVector&, float tolerance) const
{
    CHECK(tolerance == DELTA_NOTIONAL_ZERO);
    return intersect_external;
}
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

namespace NShape {
struct FixtureManager {int id;FixtureManager():id(19){}};
struct FixtureTriangle {FixtureManager&owner;std::vector<C3DVector>points;FixtureTriangle(FixtureManager&o,const std::vector<C3DVector>&p):owner(o),points(p){++live;CHECK(o.id==19);}~FixtureTriangle(){--live;if(points.size()==1)++deleted_old;}};
struct FixtureShape {FixtureManager&Owner;std::vector<C3DVector>Points;std::vector<CCountedPointer<FixtureTriangle> >Subdivisions;FixtureShape(FixtureManager&o):Owner(o){}void UpdateSubdivisions();};
static void FixtureFracture(const std::vector<C3DVector>&shape,ECirculationOrder order,std::vector<std::vector<C3DVector> >&convex,std::vector<std::vector<C3DVector> >&test){CHECK(order==CIRCULATION_CLOCKWISE&&calls<10);sequence[calls++]=shape.empty()?-1:(int)shape[0].X;if(shape.size()<3)return;if(shape.size()==3){convex.push_back(shape);return;}for(unsigned i=1;i+1<shape.size();++i){std::vector<C3DVector>piece;piece.push_back(shape[0]);piece.push_back(shape[i]);piece.push_back(shape[i+1]);test.push_back(piece);}}
}
#define CBasicShape FixtureShape
#define CSubdivisionTriangle FixtureTriangle
#define FractureShape FixtureFracture
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

#undef CBasicShape
#undef CSubdivisionTriangle
#undef FractureShape

static void CheckOriginalGeometry()
{
    using namespace NShape;
    std::vector<C3DVector> triangle;
    triangle.push_back(C3DVector(0, 0, 0));
    triangle.push_back(C3DVector(0, 4, 0));
    triangle.push_back(C3DVector(4, 0, 0));
    std::vector<C3DVector> points(triangle);
    CHECK(!IsTriangleWithPointsInside(triangle, points, CIRCULATION_CLOCKWISE));
    points.push_back(C3DVector(1, 1, 0));
    CHECK(IsTriangleWithPointsInside(triangle, points, CIRCULATION_CLOCKWISE));
    CHECK(!IsTriangleWithPointsInside(triangle, points, CIRCULATION_ANTICLOCKWISE));

    std::vector<std::vector<C3DVector> > convex;
    std::vector<std::vector<C3DVector> > concave;
    ECirculationOrder order = CIRCULATION_CLOCKWISE;
    allow_proposal = false;
    FractureShape(triangle, order, convex, concave);
    CHECK(order == CIRCULATION_ANTICLOCKWISE && convex.empty() && concave.size() == 1);
    concave.clear();
    FractureShape(triangle, order, convex, concave);
    CHECK(convex.empty() && concave.empty());
    order = CIRCULATION_CLOCKWISE;
    allow_proposal = true;
    FractureShape(triangle, order, convex, concave);
    CHECK(order == CIRCULATION_CLOCKWISE && convex.size() == 1 && concave.empty());
    if (!convex.empty()) CHECK(convex[0].size() == 3 && convex[0][2].X == 4);

    std::vector<C3DVector> square;
    square.push_back(C3DVector(0, 0, 0));
    square.push_back(C3DVector(0, 4, 0));
    square.push_back(C3DVector(4, 4, 0));
    square.push_back(C3DVector(4, 0, 0));
    convex.clear();
    FractureShape(square, order, convex, concave);
    CHECK(convex.size() == 1 && concave.size() == 1);
    if (!concave.empty()) CHECK(concave[0].size() == 3 && concave[0][2].X == 4);
    square.push_back(C3DVector(0, -4, 0));
    C2DLineF edge(square[0], square[1]);
    intersect_external = false;
    CHECK(!IsLineIntersectingWithOthersInList(edge, square));
    intersect_external = true;
    CHECK(IsLineIntersectingWithOthersInList(edge, square));
    intersect_external = false;
}

int main(){CheckOriginalGeometry();using namespace NShape;for(unsigned n=0;n<=7;++n){CHECK(live==0);FixtureManager manager;{FixtureShape owner(manager);for(unsigned p=0;p<n;++p)owner.Points.push_back(C3DVector((float)p,2,3));std::vector<C3DVector>oldpoints;oldpoints.push_back(C3DVector(-1,0,0));owner.Subdivisions.push_back(CCountedPointer<FixtureTriangle>(new FixtureTriangle(manager,oldpoints)));CCountedPointer<FixtureTriangle>keep=owner.Subdivisions[0];bool hold=n==6;if(!hold)keep.Reset(0);deleted_old=calls=0;owner.UpdateSubdivisions();unsigned expected=n<3?0:n-2;CHECK(owner.Subdivisions.size()==expected&&deleted_old==(hold?0:1));CHECK(calls==(n>3?1+(int)expected:1)&&sequence[0]==(n?0:-1));CHECK(live==(int)expected+(hold?1:0));for(unsigned j=0;j<expected;++j){CHECK(owner.Subdivisions[j].GetReferenceCount()==1);FixtureTriangle&t=*owner.Subdivisions[j];CHECK(&t.owner==&manager&&t.points.size()==3&&t.points[0].X==0&&t.points[1].X==(float)(j+1)&&t.points[2].X==(float)(j+2));}keep.Reset(0);CHECK(deleted_old==1);owner.Points.clear();calls=0;owner.UpdateSubdivisions();CHECK(owner.Subdivisions.empty()&&calls==1&&live==0);}CHECK(live==0);}std::puts(errors?"007634e0_TEST FAIL":"007634e0_TEST PASS");return errors?1:0;}