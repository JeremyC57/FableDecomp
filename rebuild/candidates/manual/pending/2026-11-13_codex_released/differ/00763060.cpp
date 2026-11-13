// NShape::FractureShape(std::vector<C3DVector,std::allocator<C3DVector> > const &,NShape::ECirculationOrder &,std::vector<std::vector<C3DVector,std::allocator<C3DVector> >,std::allocator<std::vector<C3DVector,std::allocator<C3DVector> > > > &,std::vector<std::vector<C3DVector,std::allocator<C3DVector> >,std::allocator<std::vector<C3DVector,std::allocator<C3DVector> > > > &) -- split a polygon into a triangle and residual polygons.
// Leaf: ?FractureShape@NShape@@YIXABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAW4ECirculationOrder@1@AAV?$vector@V?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@V?$allocator@V?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@2@@3@2@Z
// Grade: DIFFER(1143v1146) -- local frame and register allocation: with both C2DLineF arguments as condition temporaries the frame is 0x60 (retail 0x5C; Codex's named lines give 0x6C); retail keeps the duplicate's begin in ebx for the a/b copies and the XY copies schedule differently. 12 shapes (line temporaries, a/b references, distance scope, result by value, it_start increment, splice blocks/iterators, size locals, indexed a/b) leave it. Graded in Codex's header context (FABLE_RETAIL_C2DVECTOR_UI / FABLE_RETAIL_NSHAPE per-feature macros = header debt)
// Flags: /Gr
#define FABLE_RETAIL_C2DVECTOR_UI
#define FABLE_RETAIL_NSHAPE
#include "engine/NShape.h"
#pragma auto_inline(off)
extern const float DELTA_NOTIONAL_ZERO;
__declspec(noinline) bool __fastcall NShape::IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle,const std::vector<C3DVector>& points,ECirculationOrder circ){
 long inside=0;
 for(std::vector<C3DVector>::const_iterator p=points.begin();p!=points.end();++p){
  std::vector<C3DVector>::const_iterator edge=triangle.begin();C3DVector previous=*edge++;long sides=0;bool quit=false;
  do{
   C3DVector point;
   if(edge==triangle.end()){point=*triangle.begin();quit=true;}else point=*edge++;
   C3DVector direction(point.X-previous.X,point.Y-previous.Y,0.0f);
   C3DVector relative=*p-previous;
   C3DVector normal(direction.Y,-direction.X,0.0f);
   if(circ==CIRCULATION_ANTICLOCKWISE)normal=normal*-1.0f;
   if(normal.Dot(relative)>-DELTA_NOTIONAL_ZERO)++sides;
   previous=point;
  }while(!quit);
  if(sides==3)++inside;
 }
 return inside>3;
}
namespace std {
template<> C3DVector* __fastcall __copy<C3DVector*,C3DVector*,int>(C3DVector* first,C3DVector* last,C3DVector* result,const random_access_iterator_tag&,int*){
 for(int n=last-first;n>0;--n){*result=*first;++first;++result;}return result;
}
template<> C3DVector* __fastcall __find_if<C3DVector*,NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation>(C3DVector* first,C3DVector* last,NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation pred,const random_access_iterator_tag&){
 ptrdiff_t trip_count=(last-first)>>2;
 for(;trip_count>0;--trip_count){
  if(pred(*first))return first;++first;
  if(pred(*first))return first;++first;
  if(pred(*first))return first;++first;
  if(pred(*first))return first;++first;
 }
 switch(last-first){
  case 3:if(pred(*first))return first;++first;
  case 2:if(pred(*first))return first;++first;
  case 1:if(pred(*first))return first;
  case 0:default:return last;
 }
}
template<> vector<C3DVector>::vector(const vector& x):_Vector_base<C3DVector,allocator<C3DVector> >(x.size(),x.get_allocator()) {
 this->_M_finish=__uninitialized_copy((const C3DVector*)x._M_start,(const C3DVector*)x._M_finish,this->_M_start,__false_type());
}
}
#pragma auto_inline(on)
namespace NShape {
void __fastcall FractureShape(const std::vector<C3DVector>& test_shape,ECirculationOrder& circ,std::vector<std::vector<C3DVector> >& convex_shapes,std::vector<std::vector<C3DVector> >& concave_shapes)
{
 std::vector<C3DVector> duplicate_shape(test_shape);
 std::vector<C3DVector>::iterator it_begin=duplicate_shape.begin();
 std::vector<C3DVector>::iterator it_anchor=it_begin+1;
 C3DVector a=*it_begin;
 C3DVector b=*it_anchor;
 bool found_appropriate=false;bool quit=false;
 std::vector<C3DVector>::iterator it_start=it_anchor;
 ++it_start;
 std::vector<C3DVector>::iterator it_result;
 while(!quit){
  it_result=std::find_if(it_start++,duplicate_shape.end(),CIsPointFinalLinkInTriangleWithMatchingCirculation(a,b,circ));
  if(it_result==duplicate_shape.end())break;
  const C3DVector& result_point=*it_result;
  if(!IsLineIntersectingWithOthersInList(C2DLineF(a,result_point),duplicate_shape) && !IsLineIntersectingWithOthersInList(C2DLineF(b,result_point),duplicate_shape)){
   std::vector<C3DVector> triangle;
   triangle.push_back(a);triangle.push_back(b);triangle.push_back(result_point);
   if(!IsTriangleWithPointsInside(triangle,duplicate_shape,circ)){found_appropriate=true;quit=true;}
  }
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
 }else if(circ==CIRCULATION_CLOCKWISE){circ=CIRCULATION_ANTICLOCKWISE;concave_shapes.push_back(test_shape);}
}
}
