// BEGIN exact source snapshot SHA256 1f28ba67d1d3a17af830c815ab96897bc769e9aa57c2cfed69186c05c7674932
// 00b209c0 NParticleEngine::CPSCSplineBase::GetApproximateSplineLength, 316 bytes.
// Root of sum of squared consecutive-point distances (not sum of segment lengths).
// Catalog inherited slot: NParticleEngine::CPSCSingleSprite::vfunc_31.
// Recovered owner/name: Ego_r PDB ?GetApproximateSplineLength@CPSCSplineBase@NParticleEngine@@UAEMXZ.
// Layout: generated namespace header absent. PDB CPSCSplineBase::Spline at+0x10;
// its CArray<C3DVector> ControlPoints starts at+0x10/+0x14, as the retail loads.
// Point is a C3DVector-compatible local model; no generated C3DVector header exists.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern const float Zero_0122dedc; // 0x0122dedc
struct Point {
 float X,Y,Z;
 Point() {}
 Point(const Point& b):X(b.X),Y(b.Y),Z(b.Z) {}
 Point(float x,float y,float z):X(x),Y(y),Z(z) {}
 Point operator-(const Point& b) const { Point r; r.X=X-b.X; r.Y=Y-b.Y; r.Z=Z-b.Z; return r; }
 float SquaredLength() const { return Y*Y+(Z*Z+X*X); }
};
namespace NParticleEngine {
struct CPSCSplineBase {
 unsigned char InheritedState[0x10];
 Point* PointsStart;
 Point* PointsFinish;
 Point* PointsLimit;
 Point* begin() const { return PointsStart; }
 Point& at(int i) const { return *(begin()+i); }
 int size() const { return PointsFinish-begin(); }
 float GetApproximateSplineLength();
};
float CPSCSplineBase::GetApproximateSplineLength() {
 float total=Zero_0122dedc;
 int count=size();
 for(int i=0;i<count-1;++i) {
  total+=(at(i)-at(i+1)).SquaredLength();
 }
 float result=(float)sqrt(total);
 return result;
}
}

// END exact source snapshot
#include <stdio.h>
const float Zero_0122dedc=0;
int main() {
 NParticleEngine::CPSCSplineBase s;
 Point points[6]={Point(0,0,0),Point(3,0,0),Point(3,4,0),Point(3,4,0),Point(3,4,0),Point(3,4,0)};
 s.PointsStart=points;
 s.PointsFinish=points;
 if(s.GetApproximateSplineLength()!=0) return 1;
 s.PointsFinish=points+1;
 if(s.GetApproximateSplineLength()!=0) return 2;
 s.PointsFinish=points+3;
 if(s.GetApproximateSplineLength()!=5) return 3;
 s.PointsFinish=points+6;
 if(s.GetApproximateSplineLength()!=5) return 4;
 puts("00b209c0_TEST PASS"); return 0;
}