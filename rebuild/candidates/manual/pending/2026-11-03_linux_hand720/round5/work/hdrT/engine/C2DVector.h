#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H
// Ego_r original class 0x9ebe: public float X+0,Y+4, sizeof8.
// Native CTCPhysicsBase::GetOffsetVector slot28 at006b13d0 writes both words.
// Retail opt-in (Codex pass11): original lib_coord.hpp100..103,314..322;
// user copy ctor and arithmetic confirmed by UI hidden-result call sites.
#ifdef FABLE_RETAIL_C2DVECTOR
#include <math.h>
// Retail opt-in (retry11b, row 0x00631C60): C2DVector is declared byte-packed (alignment 1). Evidence: retail
// NUISystem::CUIDef::Transfer 0x00631C60 transfers PositionOffset.X/.Y at +0x142/+0x146 (`lea eax,[esi+142h]`,
// `lea ecx,[esi+146h]` -> Transfer<float> 0x00431061) right after Inverted at +0x141; Ego_r.pdb places the same member
// at CUIDef+0x15E and COldFileFormatThingBrushInfo::PositionOffset at +0x11 (struct_layouts_egor.tsv), neither reachable
// with 4-byte alignment; the raw C2DVector TPI records (Ego_r 0x9ebe, FableWin 0x9b09) carry the packed property bit.
#pragma pack(push, 1)
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
    // lib_coord.hpp arithmetic used by Mass (row 008eec70; Codex pass 11 Mass model, parked row).
    const C2DVector operator-() const { return C2DVector(-X,-Y); }
    const C2DVector operator*(float scalar) const { return C2DVector(X*scalar,Y*scalar); }
    const C2DVector& operator+=(const C2DVector& vector) { X+=vector.X; Y+=vector.Y; return *this; }
    float Dot(const C2DVector& vector) const { return X*vector.X+Y*vector.Y; }
    // Ego_r.pdb / FableWin.pdb ?GetMagnitude@C2DVector@@QBEMXZ, inline in retail: CNavigatorAStar::UpdateGoal 0x00A1E030 squares
    // Y then X, adds, and runs fsqrt through the float overload (`fsqrt ; fstp st(2) ; fstp st(0)`, the dead difference
    // vector popped under the result), which `(float)sqrt(X*X+Y*Y)` does not give (it stores before popping).
    float GetMagnitude() const { return sqrtf(X*X+Y*Y); }
};
#pragma pack(pop)
#else
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#endif
#endif
