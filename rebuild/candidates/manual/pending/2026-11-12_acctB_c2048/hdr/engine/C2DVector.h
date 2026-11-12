#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H
#include "fable_compiler.h"
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
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    // Per-TU inline opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): that TU expands this constructor at all four sites
    // (0x00422EFE, 0x00422F18, 0x00426C27, 0x00426C5A: `fldz ; mov [Y],imm ; fstp [X]`) past VC7.1's caller-size inline budget,
    // which calls a plain-inline copy there.
    FABLE_FORCEINLINE
#endif
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
    // Retail opt-ins (chunk 2048, row 0x00B64FA0 RenderClouds): FableWin.pdb's debug ??HC2DVector / ??GC2DVector / ??DC2DVector are
    // 26/68, 26/68, 24/63 (instructions/bytes), which plain VC7.1 /Od reproduces exactly with a named result local returned by value
    // (VC7.1 has no named-return optimisation: in release each expansion carries the local and an inline copy-constructor expansion,
    // IL that folds away); the unnamed `return C2DVector(...)` text is 22/54, 22/54, 20/49 (/opt/fable_lane/tools/fwsize.py).
#ifdef FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_ADD
    C2DVector operator+(const C2DVector& vector) const { C2DVector result(X+vector.X,Y+vector.Y); return result; }
#else
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB
    C2DVector operator-(const C2DVector& vector) const { C2DVector result(X-vector.X,Y-vector.Y); return result; }
#else
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
#endif
    // lib_coord.hpp arithmetic used by Mass (row 008eec70; Codex pass 11 Mass model, parked row).
    const C2DVector operator-() const { return C2DVector(-X,-Y); }
#ifdef FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_MUL
    const C2DVector operator*(float scalar) const { C2DVector result(X*scalar,Y*scalar); return result; }
#else
    const C2DVector operator*(float scalar) const { return C2DVector(X*scalar,Y*scalar); }
#endif
    const C2DVector& operator+=(const C2DVector& vector) { X+=vector.X; Y+=vector.Y; return *this; }
    // lib_coord.hpp division through the reciprocal (row 0x00547050 NUISystem::CScrollingComponent::UpdatePosition:
    // `fld 1.0 ; fdiv [time]` then two fmul; also the landed base 0x0052FFD0's local model).
    const C2DVector operator/(float scalar) const { float inverse = 1.0f/scalar; return C2DVector(X*inverse,Y*inverse); }
    // Component-wise scale (row 0x00547050: RenderPosition *= ParentZoom, `fld [zoom.X] ; fmul [pos.X]`).
    const C2DVector& operator*=(const C2DVector& vector) { X*=vector.X; Y*=vector.Y; return *this; }
    float Dot(const C2DVector& vector) const { return X*vector.X+Y*vector.Y; }
    // FableWin ?GetSquaredMagnitude@C2DVector@@QBEMXZ (0x021ED350), inline in retail (0x00B3BE10 BuildAlphaStippleTexture
    // expands it; X*X+Y*Y, (X*X+Y*Y) and Y*Y+X*X give the same bytes there).
#ifdef FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
    // Nested opt-in (chunk 857, row 0x00A1C010 CNavigatorAStar::CullWayPoints): the same sum with Y squared first and X added to it.
    // That TU (and IsRouteClear 0x00A1A704) squares Y then X for both of the two distances it compares (`fld st(2) ; fmul st(3) ;
    // fld st(4) ; fmul st(5) ; faddp`, 0x00A1C551); VC7.1 canonicalises the one-expression `X*X+Y*Y` / `Y*Y+X*X` to X first there.
    float GetSquaredMagnitude() const { float squared=Y*Y; squared+=X*X; return squared; }
#else
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
#endif
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
    FABLE_FORCEINLINE bool operator==(const C2DVector& vector) const
    {
        return fabs(X-vector.X)<=DELTA_NOTIONAL_ZERO && fabs(Y-vector.Y)<=DELTA_NOTIONAL_ZERO;
    }
#ifdef FABLE_RETAIL_C2DVECTOR_NORMALISE
    // Retail opt-in (chunk 857, row 0x00A1C010 CNavigatorAStar::CullWayPoints): Ego_r.pdb / FableWin.pdb ?Normalise@C2DVector@@QAEAAV1@XZ
    // (out of line in retail, 0x00A14540, landed; `lea ecx,[esp+38h] ; call 0x00A14540` at 0x00A1C164) and FableWin
    // ?IsNonZero@C2DVector@@QBE_NXZ (inline in retail: CullWayPoints / IsRouteClear 0x00A1A321 square Y then X, add, and compare with
    // DELTA_NOTIONAL_ZERO squared: `fld [129BA3Ch] ; fmul [129BA3Ch] ; fcompp ; test ah,5 ; jp`).
    C2DVector& Normalise();                                                  // 0x00A14540
    // FableWin ??XC2DVector@@QAEABV0@M@Z (scale in place; CullWayPoints scales the perpendicular in its own slot, 0x00A1C1FC..0x00A1C208).
    const C2DVector& operator*=(float scalar) { X*=scalar; Y*=scalar; return *this; }
#ifdef FABLE_RETAIL_C2DVECTOR_ISNONZERO_DIRECT
    bool IsNonZero() const { return X*X+Y*Y>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO; }
#else
    bool IsNonZero() const { return GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO; }
#endif
#endif
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
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    // Per-TU inline opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): that TU expands this constructor at all four sites
    // (0x00422EFE, 0x00422F18, 0x00426C27, 0x00426C5A: `fldz ; mov [Y],imm ; fstp [X]`) past VC7.1's caller-size inline budget,
    // which calls a plain-inline copy there.
    FABLE_FORCEINLINE
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#endif
#endif
