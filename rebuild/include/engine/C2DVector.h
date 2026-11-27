#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H
#include "fable_compiler.h"
#ifdef FABLE_RETAIL_SHOVE_FRAME_PASS23
#include "engine/GFAngles.h"
#endif
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
#ifdef FABLE_RETAIL_C2DVECTOR_MAGNITUDE_GFFASTSQRT_PASS26
float FABLE_FASTCALL GFFastSqrt(float);
#endif
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)
#pragma pack(push, 1)
class C2DVector {
public:
    float X,Y;
#ifdef FABLE_RETAIL_SHOVE_FRAME_PASS23
    // FableWin name set and donor 0273AB10: calls GFArcTan then GET_ROUNDED_ANGLE.
    // Native 007D0186..1D6 expands both helpers for Held->Vel.
    FABLE_FORCEINLINE float GetAngleXY() const { return GET_ROUNDED_ANGLE(GFArcTan(X,Y)); }
#endif
    C2DVector() { }
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    // Per-TU inline opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): that TU expands this constructor at all four sites
    // (0x00422EFE, 0x00422F18, 0x00426C27, 0x00426C5A: `fldz ; mov [Y],imm ; fstp [X]`) past VC7.1's caller-size inline budget,
    // which calls a plain-inline copy there.
    FABLE_FORCEINLINE
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
#ifdef FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
    // Original01898e60/01a3f600: direct two-component copies, shared lib_coord constructor form.
    C2DVector(const C2DVector& p) {X=p.X;Y=p.Y;}
#else
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_CLEAR
    // Nested opt-in (hand3c3, row 0x00B16410 NParticleEngine::CPSCOrbit::COrbitState::Update): FableWin.pdb / Ego_d.pdb
    // ?Clear@C2DVector@@QAEXXZ (FableWin 0x01F72120), inline in retail (no Fable.exe copy). Ego_d: one 26-byte line = two separate
    // zero stores (`X=Y=0.0f` would be 24; C3DVector::Clear's line is 34 = three stores).
    void Clear() { X=0.0f; Y=0.0f; }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_INITIALISE
    // Nested opt-in (chunk 768, row 0x00A76770 CNavQuadTreeNode::GetLeafNodeAt): FableWin.pdb ?Initialise@C2DVector@@QAEXMM@Z (inline in
    // retail: the arguments are built right to left, Y first, then X and Y are stored).
    void Initialise(float x,float y) { X=x; Y=y; }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
    // Nested opt-in (chunk 768, row 0x00969E70): FableWin.pdb ??0C2DVector@@QAE@ABVC3DVector@@@Z (the X / Y of a 3D position; inline in
    // retail; the body follows the class).
    C2DVector(const class C3DVector& v);
#ifdef FABLE_RETAIL_C2DVECTOR_ASSIGN3D
    void operator=(const class C3DVector& v);
#endif
#endif
    // Retail opt-ins (chunk 2048, row 0x00B64FA0 RenderClouds): FableWin.pdb's debug ??HC2DVector / ??GC2DVector / ??DC2DVector are
    // 26/68, 26/68, 24/63 (instructions/bytes), which plain VC7.1 /Od reproduces exactly with a named result local returned by value
    // (VC7.1 has no named-return optimisation: in release each expansion carries the local and an inline copy-constructor expansion,
    // IL that folds away); the unnamed `return C2DVector(...)` text is 22/54, 22/54, 20/49 (/opt/fable_lane/tools/fwsize.py).
#ifdef FABLE_RETAIL_C2DVECTOR_ADD_COPY_RHS_X_PASS23
    // MarkedFidelity,0064a639: same component sums, real copy reads RHS X without changing Y.
    // Native0064a83a/0064a85b fixes RHS address scheduling and RHS-first X addition.
    // Plain copy, sum-local, reference, operand reversal and parentheses did not select this DAG.
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(C2DVector(vector).X+X,Y+vector.Y); }
#elif defined(FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_ADD)
    C2DVector operator+(const C2DVector& vector) const { C2DVector result(X+vector.X,Y+vector.Y); return result; }
#else
    #ifdef FABLE_RETAIL_CDRAWHEALTH_ORIGINAL_EFFECTS_PASS28
    // Fidelity: both FableWin component sums and retail GUI rounding order. The const reference keeps the same source vector.
    C2DVector operator+(const C2DVector& vector) const {
        const C2DVector& v=vector;
        return C2DVector(X+v.X,Y+v.Y);
    }
#else
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
#endif
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB
    C2DVector operator-(const C2DVector& vector) const { C2DVector result(X-vector.X,Y-vector.Y); return result; }
#else
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
#endif
    // lib_coord.hpp arithmetic used by Mass (row 008eec70; Codex pass 11 Mass model, parked row).
    const C2DVector operator-() const { return C2DVector(-X,-Y); }
    #ifdef FABLE_RETAIL_NAVIGATORASTAR_PASS21
    // Original PDB parameter spelling: VECTOR_MULTIPLY_LOCALS.txt.
    const C2DVector operator*(float scale) const { return C2DVector(X*scale,Y*scale); }
#elif defined(FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_MUL)
    // (PASS21 above replaces both spellings of this operator: one operator* per define combination)
    const C2DVector operator*(float scalar) const { C2DVector result(X*scalar,Y*scalar); return result; }
#else
    const C2DVector operator*(float scalar) const { return C2DVector(X*scalar,Y*scalar); }
#endif
    const C2DVector& operator+=(const C2DVector& vector) { X+=vector.X; Y+=vector.Y; return *this; }
    // lib_coord.hpp division through the reciprocal (row 0x00547050 NUISystem::CScrollingComponent::UpdatePosition:
    // `fld 1.0 ; fdiv [time]` then two fmul; also the landed base 0x0052FFD0's local model).
#ifdef FABLE_RETAIL_C2DVECTOR_DIVASSIGN_PASS26
    // Original02190580, reciprocal then in-place X/Y; originalcaller031ac720 divides by2.
    const C2DVector& operator/=(float scale){const float oo_scale=1.0f/scale;X*=oo_scale;Y*=oo_scale;return *this;}
#endif
    const C2DVector operator/(float scalar) const { float inverse = 1.0f/scalar; return C2DVector(X*inverse,Y*inverse); }
    // Component-wise scale (row 0x00547050: RenderPosition *= ParentZoom, `fld [zoom.X] ; fmul [pos.X]`).
    const C2DVector& operator*=(const C2DVector& vector) { X*=vector.X; Y*=vector.Y; return *this; }
#ifdef FABLE_RETAIL_C2DVECTOR_DOT_GROUP
    // Grouped sum (row 0x008EEC70 Mass::Collide: the one PAREN tuple of this once-expanded Dot keeps `mov eax,esp` (0x008EEFA8)
    // out of the 81-tuple scheduler region that holds `sub esp,8`; same bytes as the default in 0x008EEA80 / 0x008EEA30 / 0x008EEB40).
    float Dot(const C2DVector& vector) const { return (X*vector.X+Y*vector.Y); }
#else
    float Dot(const C2DVector& vector) const { return X*vector.X+Y*vector.Y; }
#endif
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
#ifdef FABLE_RETAIL_C2DVECTOR_MAGNITUDE_SQUARED_LOCAL
    // Nested opt-in (row 0x00B4D370): the FableWin text of ?GetMagnitude@C2DVector@@QBEMXZ (21i / 52b at /Od, fwsize SAME): the sum
    // in a named local, then sqrtf.
    float GetMagnitude() const { float squared=X*X+Y*Y; return sqrtf(squared); }
#else
#ifdef FABLE_RETAIL_C2DVECTOR_MAGNITUDE_GFFASTSQRT_PASS26
    // Original02089cb0 calls GFFastSqrt on the direct squared sum.
    float GetMagnitude() const { return GFFastSqrt(X*X+Y*Y); }
#else
    float GetMagnitude() const { return sqrtf(X*X+Y*Y); }
#endif
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_TOC2DCOORDI
    // Opt-in (agentC39, row 0x00652FA4 CDrawQuickAccessIcons::Draw): Ego_r.pdb / FableWin.pdb ?ToC2DCoordI@C2DVector@@QBE?AVC2DCoordI@@XZ,
    // expanded in retail's size-optimised CDrawQuickAccessIcons::Draw (0x00653F29: `fld [eax] ; mov [this],eax ; call _ftol2 ; ... ;
    // fld [eax+4] ; call _ftol2` on the returned GFAnchorToBottomLeft temporary), where VC7.1 calls a plain-inline copy out of line
    // (c2 inline trace: size 37, largest expanded callee 29) -> FABLE_FORCEINLINE. Text = FableWin /Od size (18i/47b). Defined after
    // the class (needs the complete C2DCoordI).
    class C2DCoordI ToC2DCoordI() const;
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_TOC2DVECTOR
    // Opt-in (agentC41, row 0x00652FA4 CDrawQuickAccessIcons::Draw): Ego_r.pdb / FableWin.pdb ?ToC2DVector@C2DVector@@QBE?AV1@XZ, the
    // by-value copy of the vector (sibling of ToC2DCoordI). Retail's flash / d-pad sprite positions are a sum computed X then Y,
    // copied (`fld st(1) ; fstp [copy.X] ; fst [copy.Y]`) with the sum's own stores dead (0x006540BB, 0x00654161).
    C2DVector ToC2DVector() const { return *this; }
#endif
    // Chunk 752 (row 0x0054AC90 CSlider::ProcessEvent): Ego_r.pdb ??8C2DVector@@QBE_NABV0@@Z, the epsilon compare against
    // ?DELTA_NOTIONAL_ZERO@@3MB 0x0129BA3C (`fld [a.X] ; fsub [b.X] ; fabs ; fcomp [129BA3Ch] ; test ah,41h ; jp`, then the Y test),
    // `<=` per component as the landed ~CSlider 0x0054B360 proved. __forceinline: retail expands it at every site, including the
    // low-frequency switch cases of CSlider::ProcessEvent where VC7.1 calls a plain-inline copy out of line; the five retail
    // out-of-line copies (0x0052E3D0, 0x0054A4A0, 0x006CB570, 0x00704D30, 0x00A88C50) have no caller at all (no call/jmp/jcc
    // or address reference in Fable.exe), while a plain-inline copy would be called from such sites.
#ifdef FABLE_RETAIL_PASS22_SLIDER_PROCESS_EVENT
    // Original FableWin022782D0 calls operator== then negates its Boolean result.
#ifdef FABLE_RETAIL_SB_CORRIDOR_PASS29
    // Inline-budget: the original two-component inequality is expanded at the corridor goal test.
    FABLE_FORCEINLINE
#endif
    bool operator!=(const C2DVector& rhs) const { return !(*this==rhs); }
    bool operator==(const C2DVector& vector) const
    {
        if (!(fabs(X-vector.X)<=DELTA_NOTIONAL_ZERO)) return false;
        return fabs(Y-vector.Y)<=DELTA_NOTIONAL_ZERO;
    }
#else
    FABLE_FORCEINLINE bool operator==(const C2DVector& vector) const
    {
        return fabs(X-vector.X)<=DELTA_NOTIONAL_ZERO && fabs(Y-vector.Y)<=DELTA_NOTIONAL_ZERO;
    }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_ROTATE
    // Retail opt-in (chunk Q17, row 0x00A87700 CSBCAgents::Process): Ego_r.pdb ?Rotate@C2DVector@@QAEXM@Z (the angle in revolutions:
    // fpatan, scaled by 1/(2 pi), plus the turn, then fsin / fcos), out of line in retail at 0x00A141D0. Declaration only.
    void Rotate(float revolutions);                                          // 0x00A141D0
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_NORMALISE
    // Retail opt-in (chunk 857, row 0x00A1C010 CNavigatorAStar::CullWayPoints): Ego_r.pdb / FableWin.pdb ?Normalise@C2DVector@@QAEAAV1@XZ
    // (out of line in retail, 0x00A14540, landed; `lea ecx,[esp+38h] ; call 0x00A14540` at 0x00A1C164) and FableWin
    // ?IsNonZero@C2DVector@@QBE_NXZ (inline in retail: CullWayPoints / IsRouteClear 0x00A1A321 square Y then X, add, and compare with
    // DELTA_NOTIONAL_ZERO squared: `fld [129BA3Ch] ; fmul [129BA3Ch] ; fcompp ; test ah,5 ; jp`).
    C2DVector& Normalise();                                                  // 0x00A14540
    // FableWin ??XC2DVector@@QAEABV0@M@Z (scale in place; CullWayPoints scales the perpendicular in its own slot, 0x00A1C1FC..0x00A1C208).
    #ifdef FABLE_RETAIL_NAVIGATORASTAR_PASS21
    // Original PDB parameter spelling: VECTOR_MULTIPLY_LOCALS.txt.
    const C2DVector& operator*=(float scale) { X*=scale; Y*=scale; return *this; }
#else
    const C2DVector& operator*=(float scalar) { X*=scalar; Y*=scalar; return *this; }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_ROTATE
    // Retail opt-in (w22 agent2022, row 0x007D3820 CTCTavernGameCoinGolf::ProcessInput): Ego_r.pdb / FableWin.pdb
    // ?Rotate@C2DVector@@QAEXM@Z (out of line in retail at 0x00A141D0, the bound spelling; `lea ecx,[coin+0Ch] ; push angle ; call` at
    // 0x007D3B07), ?AccurateNormalise@C2DVector@@QAEAAV1@XZ (out of line at 0x00A14510, called at 0x007D3C0C) and the inline
    // ??ZC2DVector@@QAEABV0@ABV0@@Z (subtract in place: 0x007D3B7B `fld [x] ; fsub [m.X] ; fstp [x]` then Y).
    // (Rotate itself is the chunk Q17 declaration above: one declaration per opt-in.)
    C2DVector& AccurateNormalise();                                          // 0x00A14510
    const C2DVector& operator-=(const C2DVector& vector) { X-=vector.X; Y-=vector.Y; return *this; }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_ISNONZERO_DIRECT
    bool IsNonZero() const { return X*X+Y*Y>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO; }
#else
#ifdef FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE
    // Prior875 inline-budget evidence: native expands every site after natural and budget probes failed.
    FABLE_FORCEINLINE
#endif
    bool IsNonZero() const { return GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO; }
#endif
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_GETRANDOMUNITVECTOR
    // Retail opt-in (wfE8 6th agent, row 0x00B10B10): Ego_r.pdb / FableWin.pdb ?GetRandomUnitVector@C2DVector@@SI?AV1@PAK@Z (static
    // __fastcall, 0x00A13EB0: seed in edx, result slot in ecx). Declaration only.
    static C2DVector FABLE_FASTCALL GetRandomUnitVector(unsigned long* seed);
#endif
};
#pragma pack(pop)
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
inline C2DVector::C2DVector(const C3DVector& v) {X=v.X;Y=v.Y;}
#else
inline C2DVector::C2DVector(const C3DVector& v) : X(v.X),Y(v.Y) { }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_ASSIGN3D
inline void C2DVector::operator=(const C3DVector& v) {X=v.X;Y=v.Y;}
#endif
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
// Opt-in (wfE11 sixth session, row 0x004A3740 CWorld::UpdateRegionLoad): FableWin.pdb ?GFGet2DDistanceSquared@@YIMABVC2DVector@@0@Z
// (global __fastcall; FableWin COMDAT 0x01B4F090, 64 bytes / 25 instructions, emitted right after UpdateRegionLoad; no Ego_r copy: inline
// in retail). Body FITTED to the row's bytes (0x004A44C5..0x004A450F: both differences stored as separate floats, second minus first,
// then dy*dy + dx*dx); this text is 24 instructions / 62 bytes at /Od.
#ifdef FABLE_RETAIL_C2DVECTOR_ORIGINAL_DISTANCE_PASS26
// FableWin 0x01B4F090 and Ego_d locals dx/dy: read their squares in X/Y order.
inline float FABLE_FASTCALL GFGet2DDistanceSquared(const C2DVector& pos1, const C2DVector& pos2) {
    float dx = pos2.X - pos1.X;
    float dy = pos2.Y - pos1.Y;
    return dx * dx + dy * dy;
}
#else
inline float __fastcall GFGet2DDistanceSquared(const C2DVector& pos1, const C2DVector& pos2) {
    float dx = pos2.X - pos1.X;
    float dy = pos2.Y - pos1.Y;
    float squared = dy * dy;
    squared += dx * dx;
    return squared;
}
#endif
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
#if defined(FABLE_RETAIL_C2DVECTOR) && defined(FABLE_RETAIL_C2DVECTOR_TOC2DCOORDI)
#include "engine/C2DCoordI.h"
FABLE_FORCEINLINE C2DCoordI C2DVector::ToC2DCoordI() const { return C2DCoordI((long)X, (long)Y); }
#endif
#endif
