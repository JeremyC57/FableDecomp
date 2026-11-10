#include <cstdio>
#include <vector>
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}C3DVector Cross(const C3DVector& v)const{return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X);}float Dot(const C3DVector& v)const{return X*v.X+Y*v.Y+Z*v.Z;}};
namespace NShape {class CSubdivisionTriangle{public:std::vector<C3DVector> Points;C3DVector UpDirection;bool IsPosInShape(const C3DVector&)const;};}
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

int main(){int ok=1,cases=0;for(int sx=-1;sx<=1;sx+=2)for(int sy=-1;sy<=1;sy+=2)for(int origin=0;origin<2;++origin)for(int up=-1;up<=1;++up)for(int tilt=0;tilt<2;++tilt){float ox=origin?1.0f:0,oy=origin?-1.0f:0;NShape::CSubdivisionTriangle triangle;triangle.Points.push_back(C3DVector(ox,oy,8));triangle.Points.push_back(C3DVector(ox,oy+2*sy,-4));triangle.Points.push_back(C3DVector(ox+4*sx,oy,16));triangle.UpDirection=C3DVector(tilt?2.0f:0,tilt?-4.0f:0,(float)up);for(int ix=-6;ix<=6;++ix)for(int iy=-6;iy<=6;++iy)for(int iz=0;iz<2;++iz){C3DVector point(float(ix)/2,float(iy)/2,iz?32.0f:-16.0f);double u=(point.X-ox)/(4.0*sx),v=(point.Y-oy)/(2.0*sy);bool expected=up==0||(up*sx*sy>0&&u>=0&&v>=0&&u+v<=1);ok &= triangle.IsPosInShape(point)==expected;++cases;}ok &= triangle.Points[0].Z==8&&triangle.Points[1].Z==-4&&triangle.Points[2].Z==16;}
ok &= cases==16224;std::printf(ok?"007610d0_TEST PASS\n":"007610d0_TEST FAIL\n");return ok?0:1;}