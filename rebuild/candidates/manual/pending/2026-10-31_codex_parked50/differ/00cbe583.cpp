// 00cbe583 GetSquaredDistanceBetweenThings
// Codex parked-50 best attempt: DIFFER(61v61); residue: 61/61 bytes; squared-distance Z-X-Y ordering restored, but virtual-call vtable register and related scheduling remain different.
// Leaf: ?GetSquaredDistanceBetweenThings@@YIMABVCScriptThing@@0@Z
// Flags:
// Layout: C3DVector X/Y/Z at 0/4/8; original packed class and FableWin member operator-.
#pragma pack(push,1)
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z) {}
    const C3DVector operator-(const C3DVector& v) const {
        C3DVector r; r.X=X-v.X; r.Y=Y-v.Y; r.Z=Z-v.Z; return r;
    }
    float GetSquaredMagnitude() const {
        float result=Z*Z; result+=X*X; result+=Y*Y; return result;
    }
};
#pragma pack(pop)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a,const CScriptThing& b) {
    return (a.GetPos()-b.GetPos()).GetSquaredMagnitude();
}
