// 00b209c0 NParticleEngine::CPSCSplineBase::GetApproximateSplineLength, 316 bytes.
// Root of sum of squared consecutive-point distances (not sum of segment lengths).
// Catalog inherited slot: NParticleEngine::CPSCSingleSprite::vfunc_31.
// Recovered owner/name: Ego_r PDB ?GetApproximateSplineLength@CPSCSplineBase@NParticleEngine@@UAEMXZ.
// Layout: generated namespace header absent. PDB CPSCSplineBase::Spline at+0x10;
// its CArray<C3DVector> ControlPoints starts at+0x10/+0x14, as the retail loads.
// Point is a C3DVector-compatible local model; no generated C3DVector header exists.
// Leaf: ?GetApproximateSplineLength@CPSCSplineBase@NParticleEngine@@UAEMXZ
// Names (agentNF250, 2026-11-22, CODEXP27 L118): was the non-virtual ?GetApproximateSplineLength@CPSCSplineBase@NParticleEngine@@QAEMXZ.
//   Ego_r.pdb and FableWin.pdb S_PUB32 spell it UAEMXZ (public virtual), and retail holds 0x00B209C0 in slot 31 of the vtables
//   0x0129FB84 / 0x012A0934 / 0x012A0B14 (CPSCSingleSprite and its siblings inherit it). In-place respell of the HEAD local model:
//   the member is declared virtual and the vptr takes the first 4 of the 0x10 inherited bytes (PointsStart stays at +0x10). No landed
//   row binds the address: the vtable emitters 0x00B09360 / 0x00B1B350 / 0x00B21530 model at most two slots.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
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
 virtual float GetApproximateSplineLength();
 unsigned char InheritedState[0xC];
 Point* PointsStart;
 Point* PointsFinish;
 Point* PointsLimit;
 Point* begin() const { return PointsStart; }
 Point& at(int i) const { return *(begin()+i); }
 int size() const { return PointsFinish-begin(); }
};
float CPSCSplineBase::GetApproximateSplineLength() {
 float total=0.0f;
 int count=size();
 for(int i=0;i<count-1;++i) {
  total+=(at(i)-at(i+1)).SquaredLength();
 }
 float result=(float)sqrt(total);
 return result;
}
}