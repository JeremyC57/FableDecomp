// 00a577b0 C2DLineF::IntersectsWith
// Flags: /O2 /Oy /W3
// Leaf: IntersectsWith
namespace X_00a577b0 {
// Layout: PDB C2DLineF Point1+0/Point2+8; C2DBoxF TLX0,TLY4,BRX8,BRY12; no active headers.
struct C2DVector{float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}__forceinline C2DVector operator-(const C2DVector&o)const{return C2DVector(X-o.X,Y-o.Y);}
__forceinline float Dot(const C2DVector&o)const{return X*o.X+Y*o.Y;}};
struct C2DBoxF{float TLX,TLY,BRX,BRY;};
struct C2DLineF{C2DVector Point1,Point2;C2DLineF(){}C2DLineF(float x1,float y1,float x2,float y2):Point1(x1,y1),Point2(x2,y2){}__forceinline C2DVector GetNormal()const{C2DVector d=Point2-Point1;return C2DVector(-d.Y,d.X);}
bool OnLeft(const C2DVector&)const;bool IntersectsWith(const C2DBoxF&)const;};
bool C2DLineF::OnLeft(const C2DVector&p)const{C2DVector normal=GetNormal();C2DVector relative=p-Point1;return normal.Dot(relative)>0.0f;}
bool C2DLineF::IntersectsWith(const C2DBoxF&b)const{
 C2DLineF top(b.TLX,b.TLY,b.BRX,b.TLY),right(b.BRX,b.TLY,b.BRX,b.BRY),bottom(b.BRX,b.BRY,b.TLX,b.BRY),left(b.TLX,b.BRY,b.TLX,b.TLY);
 if((!top.OnLeft(Point1)&&!right.OnLeft(Point1)&&!bottom.OnLeft(Point1)&&!left.OnLeft(Point1))||(!top.OnLeft(Point2)&&!right.OnLeft(Point2)&&!bottom.OnLeft(Point2)&&!left.OnLeft(Point2)))return true;return false;
}
}

#include <assert.h>
#include <stdio.h>
int main(){using namespace X_00a577b0;C2DBoxF b={0,10,10,0};assert(C2DLineF(1,1,2,2).IntersectsWith(b));assert(C2DLineF(-1,5,5,5).IntersectsWith(b));assert(C2DLineF(5,5,11,5).IntersectsWith(b));assert(!C2DLineF(-2,5,-1,5).IntersectsWith(b));assert(!C2DLineF(-2,5,12,5).IntersectsWith(b));assert(C2DLineF(0,0,-2,-2).IntersectsWith(b));assert(C2DLineF(10,10,12,12).IntersectsWith(b));assert(!C2DLineF(1,11,9,12).IntersectsWith(b));puts("00a577b0_TEST PASS");}