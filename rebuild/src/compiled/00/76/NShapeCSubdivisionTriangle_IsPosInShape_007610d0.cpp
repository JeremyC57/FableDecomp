// 007610d0 NShape::CSubdivisionTriangle::IsPosInShape -- test flattened position against every inward-facing edge.
// Leaf: ?IsPosInShape@CSubdivisionTriangle@NShape@@UBE_NABVC3DVector@@@Z
// Flags: none
// Retail:007610d0,237 bytes.
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NShape_CSubdivisionTriangle.h"
bool NShape::CSubdivisionTriangle::IsPosInShape(const C3DVector& pos)const
{
 C3DVector new_pos(pos.X,pos.Y,0.0f);
 std::vector<C3DVector>::const_iterator it_next=Points.begin();
 it_next++;
 for(std::vector<C3DVector>::const_iterator it=Points.begin();it!=Points.end();++it){
  C3DVector first=*it;
  first.Z=0;
  C3DVector next=*it_next;
  next.Z=0;
  C3DVector inward_normal=(next-first).Cross(UpDirection);
  C3DVector line=new_pos-first;
  if(line.Dot(inward_normal)<0.0f)return false;
  ++it_next;
  if(it_next==Points.end())it_next=Points.begin();
 }
 return true;
}