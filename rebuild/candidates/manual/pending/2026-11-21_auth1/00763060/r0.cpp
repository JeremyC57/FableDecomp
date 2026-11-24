// NShape::FractureShape(const std::vector<C3DVector>&, NShape::ECirculationOrder&, std::vector<std::vector<C3DVector> >&, std::vector<std::vector<C3DVector> >&) -- cut one triangle (first two points + the first later point that closes it with matching circulation, whose two new edges cross no polygon edge and that contains no polygon point) off the polygon: the triangle goes to convex_shapes, the two remaining pieces (if more than two points) to concave_shapes; with no such triangle, flip a clockwise circulation and requeue the shape
// PARKED (p25L): Ego_d-conformant best source, DIFFER(1132v1146) 160 lines; see NOTES.txt.
// Bracket: evidence/bracket_match_anchor.cpp (this file with the matched helper copies) is a fitted RELOCATION_MATCH (a declared before it_anchor; `const C3DVector& anchor=*it_anchor++; C3DVector b=anchor;`; it_start=it_anchor; find_if(it_start++,...)) -- rejected (Ego_d contradicts it), kept only as the bracket.
// Leaf: ?FractureShape@NShape@@YIXABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAW4ECirculationOrder@1@AAV?$vector@V?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@V?$allocator@V?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@2@@3@2@Z
// Flags: /Gr
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DLINEF_FROM3D
#define FABLE_RETAIL_C2DLINEF_INTERSECTS_LINE
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#include "engine/NShape.h"
#include "stlport/fable_algorithm.h"
namespace NShape {
// Fidelity: same-TU strong definition of 0x00760020
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
// Fidelity: same-TU strong definition of 0x00760190
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
void __fastcall FractureShape(const std::vector<C3DVector>& test_shape,ECirculationOrder& circ,std::vector<std::vector<C3DVector> >& convex_shapes,std::vector<std::vector<C3DVector> >& concave_shapes)
{
 std::vector<C3DVector> duplicate_shape(test_shape);
 std::vector<C3DVector>::iterator it_begin=duplicate_shape.begin();
 std::vector<C3DVector>::iterator it_anchor=it_begin+1;
 C3DVector a=*it_begin;
 C3DVector b=*it_anchor;
 bool found_appropriate=false;bool quit=false;
 std::vector<C3DVector>::iterator it_result;
 std::vector<C3DVector>::iterator it_start=it_anchor;
 while(!quit){
  it_result=std::find_if(++it_start,duplicate_shape.end(),CIsPointFinalLinkInTriangleWithMatchingCirculation(a,b,circ));
  if(it_result!=duplicate_shape.end()){
  const C3DVector& result_point=*it_result;
  C2DLineF line_a(a,result_point);
  C2DLineF line_b(b,result_point);
  if(!IsLineIntersectingWithOthersInList(line_a,duplicate_shape) && !IsLineIntersectingWithOthersInList(line_b,duplicate_shape)){
   std::vector<C3DVector> triangle;
   triangle.push_back(a);triangle.push_back(b);triangle.push_back(result_point);
   if(!IsTriangleWithPointsInside(triangle,duplicate_shape,circ)){found_appropriate=true;quit=true;}
  }
  }else quit=true;
 }
 long distance=it_result-it_begin;
 if(found_appropriate){
  std::vector<C3DVector> triangle;
  triangle.push_back(a);triangle.push_back(b);triangle.push_back(*it_result);
  convex_shapes.push_back(triangle);
  std::vector<C3DVector> splice_one(duplicate_shape);
  std::vector<C3DVector>::iterator it_del_start=splice_one.begin()+1;
  std::vector<C3DVector>::iterator it_del_end=splice_one.begin()+distance;
  splice_one.erase(it_del_start,it_del_end);
  long size_1=splice_one.size();
  if(size_1>2)concave_shapes.push_back(splice_one);
  std::vector<C3DVector> splice_two(duplicate_shape);
  it_del_start=splice_two.begin()+distance+1;
  it_del_end=splice_two.end();
  splice_two.erase(it_del_start,it_del_end);
  splice_two.erase(splice_two.begin());
  long size_2=splice_two.size();
  if(size_2>2)concave_shapes.push_back(splice_two);
 return;   // Inferred: retail gives the else path its own epilogue (push ebx ; call free at 0x007634C7); Ego_d line 280 is 143 B
 }else if(circ==CIRCULATION_CLOCKWISE){circ=CIRCULATION_ANTICLOCKWISE;concave_shapes.push_back(test_shape);}
}
}
