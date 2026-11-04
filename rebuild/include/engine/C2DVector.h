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
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)
#pragma pack(push, 1)
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
#ifdef FABLE_RETAIL_C2DVECTOR_INITIALISE
    // Nested opt-in (chunk 768, row 0x00A76770 CNavQuadTreeNode::GetLeafNodeAt): FableWin.pdb ?Initialise@C2DVector@@QAEXMM@Z (inline in
    // retail: the arguments are built right to left, Y first, then X and Y are stored).
    void Initialise(float x,float y) { X=x; Y=y; }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
    // Nested opt-in (chunk 768, row 0x00969E70): FableWin.pdb ??0C2DVector@@QAE@ABVC3DVector@@@Z (the X / Y of a 3D position; inline in
    // retail; the body follows the class).
    C2DVector(const class C3DVector& v);
#endif
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
    // lib_coord.hpp arithmetic used by Mass (row 008eec70; Codex pass 11 Mass model, parked row).
    const C2DVector operator-() const { return C2DVector(-X,-Y); }
    const C2DVector operator*(float scalar) const { return C2DVector(X*scalar,Y*scalar); }
    const C2DVector& operator+=(const C2DVector& vector) { X+=vector.X; Y+=vector.Y; return *this; }
    // lib_coord.hpp division through the reciprocal (row 0x00547050 NUISystem::CScrollingComponent::UpdatePosition:
    // `fld 1.0 ; fdiv [time]` then two fmul; also the landed base 0x0052FFD0's local model).
    const C2DVector operator/(float scalar) const { float inverse = 1.0f/scalar; return C2DVector(X*inverse,Y*inverse); }
    // Component-wise scale (row 0x00547050: RenderPosition *= ParentZoom, `fld [zoom.X] ; fmul [pos.X]`).
    const C2DVector& operator*=(const C2DVector& vector) { X*=vector.X; Y*=vector.Y; return *this; }
    float Dot(const C2DVector& vector) const { return X*vector.X+Y*vector.Y; }
    // FableWin ?GetSquaredMagnitude@C2DVector@@QBEMXZ (0x021ED350), inline in retail (0x00B3BE10 BuildAlphaStippleTexture
    // expands it; X*X+Y*Y, (X*X+Y*Y) and Y*Y+X*X give the same bytes there).
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
    // Ego_r.pdb / FableWin.pdb ?GetMagnitude@C2DVector@@QBEMXZ, inline in retail: CNavigatorAStar::UpdateGoal 0x00A1E030 squares
    // Y then X, adds, and runs fsqrt through the float overload (`fsqrt ; fstp st(2) ; fstp st(0)`, the dead difference
    // vector popped under the result), which `(float)sqrt(X*X+Y*Y)` does not give (it stores before popping).
    float GetMagnitude() const { return sqrtf(X*X+Y*Y); }
    // Chunk 752 (row 0x0054AC90 CSlider::ProcessEvent): Ego_r.pdb ??8C2DVector@@QBE_NABV0@@Z, the epsilon compare against
    // ?DELTA_NOTIONAL_ZERO@@3MB 0x0129BA3C (`fld [a.X] ; fsub [b.X] ; fabs ; fcomp [129BA3Ch] ; test ah,41h ; jp`, then the Y test),
    // `<=` per component as the landed ~CSlider 0x0054B360 proved. __forceinline: retail expands it at every site, including the
    // low-frequency switch cases of CSlider::ProcessEvent where VC7.1 calls a plain-inline copy out of line; the five retail
    // out-of-line copies (0x0052E3D0, 0x0054A4A0, 0x006CB570, 0x00704D30, 0x00A88C50) have no caller at all (no call/jmp/jcc
    // or address reference in Fable.exe), while a plain-inline copy would be called from such sites.
    __forceinline bool operator==(const C2DVector& vector) const
    {
        return fabs(X-vector.X)<=DELTA_NOTIONAL_ZERO && fabs(Y-vector.Y)<=DELTA_NOTIONAL_ZERO;
    }
};
#pragma pack(pop)
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
#include "engine/C3DVector.h"
inline C2DVector::C2DVector(const C3DVector& v) : X(v.X),Y(v.Y) { }
#endif
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
