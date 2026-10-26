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
class CPSCSplineBase { public:
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