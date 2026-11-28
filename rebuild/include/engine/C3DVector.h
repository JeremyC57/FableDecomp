// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H
#include "fable_compiler.h"
#ifdef FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_PASS23
struct _CVECTOR;
#endif
// PDB C3DVector complete TPI 0x2783: public X/Y/Z at 0/4/8, size 12.
// Embedded alignment is under review; this header introduces no packing directive.
// One model (Codex pass 11 consolidation): the arithmetic helpers are __forceinline (owner 2026-11-01: C3DVector arithmetic
// and GetLength have no out-of-line copy in either PDB name set or in Fable.exe).
#if defined(_MSC_VER)
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#include <math.h>
#ifdef FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN
// Retail opt-in (chunk 2041, row 0x006AB820 CThingPlayerCreature::ResolveFacingDirection): InitialiseFromAngles through the GFCos / GFSin
// inlines (FableWin.pdb ?GFCos@@YINM@Z / ?GFSin@@YINM@Z); see InitialiseFromAngles below. Needs FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES.
#include "rebuild_abi.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#endif
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
#include "engine/GFAngles.h"
#endif
#if (defined(FABLE_RETAIL_UPDATE_NEAREST_INTERESTING) || defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D)) && !defined(FABLE_RETAIL_CTCTARGETINGAI_ATTACK)
// (FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D: the GFFastSqrt / GetMagnitude2D pair of UPDATE_NEAREST_INTERESTING without the rest of that
// row set; see GetMagnitude2D below.)
#include "rebuild_abi.h"
inline float FABLE_FASTCALL GFFastSqrt(float value){return sqrt(value);}
#endif
#include "engine/C2DCoordI.h"
#if defined(FABLE_RETAIL_C3DVECTOR_ADD2D) || defined(FABLE_RETAIL_C3DVECTOR_TO2D)
#include "engine/C2DVector.h"
#endif
#ifdef FABLE_RETAIL_C3DVECTOR
extern const float DELTA_NOTIONAL_ZERO;                                 // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f), IsNonZero's bound
// Chunk 739 (CPSCUpdateNormal::UpdateAddParticle 0x00B0CAD0): FableWin.pdb ?GFFastOneOverSqrt@@YIMM@Z, expanded in retail (0x00B0D21E:
// exponent (0x17C - e) / 2, the 128-entry mantissa table 0x013CE558, one Newton step with 3.0f / 0.5f), the body the landed
// 0x007AF0F0 / 0x006B3030 rows model locally.
extern unsigned long GFastOneOverSqrtTable[128];                        // 0x013CE558
#if (defined(FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23) && !defined(_MSC_VER)) || \
    defined(FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_BITS_PASS26)
// Host portability: the original unsigned-long bit word is32bits on VC7.1.
// Copy float bits into a32-bit unsigned int on LP64 hosts; native text is unchanged.
#include <string.h>
inline float FABLE_FASTCALL GFFastOneOverSqrt(float value) {
    unsigned int bits;
    memcpy(&bits,&value,sizeof(value));
    unsigned int estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23) |
        ((unsigned int)GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);
    float r;
    memcpy(&r,&estimate,sizeof(r));
#ifdef FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_BITS_PASS26
    // Bracket: retain the grouped seed square and original left-associated Newton products.
    return (3.0f-((r*r)*value))*r*0.5f;
#else
    return ((3.0f-((r*r)*value))*r)*0.5f;
#endif // FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_BITS_PASS26
}
#else
#ifdef FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
// Retail opt-in (chunk Q8, row 0x006A4FB0 CThingPhysical::GetFirstCollision): the same body with the Newton step fully grouped,
// `((3.0f-((r*r)*value))*r)*0.5f`. Same instructions; the three extra no-code PAREN tuples move VC7.1's scheduling-region cut so
// that the epilogue pops interleave with the final Normalise as retail does (0x006A51F1 `pop edi` ahead of `fmul [eax]`, `pop
// esi / pop ebp / pop ebx` ahead of its `fstp`). Body only: default path unchanged.
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return ((3.0f-((r*r)*value))*r)*0.5f;}
#elif defined(FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_SQUARE_PAREN)
// Q19 opt-in (rows 0x0079B6F0 CTCSummonSpell::ResetMinionPos, 0x00837220 CThingTrackNode::UpdateFacingDirection): the same body with
// the Newton step's square and product parenthesised, `(3.0f-((r*r)*value))`. Same instructions; two no-code PAREN tuples (one more
// than NEWTON_PAREN, one fewer than NEWTON_GROUPED) put VC7.1's scheduling-region cut where retail has it. Body only: default path
// unchanged.
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-((r*r)*value))*r*0.5f;}
#elif defined(FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_STEP_PAREN)
// Q19 opt-in (same rows): the other two-tuple spelling, `((3.0f-(r*r*value))*r)*0.5f`. Body only: default path unchanged.
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return ((3.0f-(r*r*value))*r)*0.5f;}
#elif defined(FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN)
// Retail opt-in (chunk 892, row 0x00963880 CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe): the same body with the
// Newton step's product parenthesised, `(3.0f-(r*r*value))`. Same instructions; the extra parenthesis is one more no-code PAREN tuple
// in VC7.1's scheduling region (81-tuple cap, c2.dll 0x107B4E6A). It puts the region cut where retail has it in two rows: inside
// 0x00963880's second Normalise estimate (0x009639C9 `mov edx,17Ch` after the table load) and after 0x0096E830's second
// subtraction (0x0096E951 `mov ecx,17Ch` and the pops ahead of `fld st(0)`). Body only: default path unchanged.
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-(r*r*value))*r*0.5f;}
#else
#ifdef FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
union TFloatIntUnion {float Float;unsigned long Int;};
#ifdef FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION_SQUARE_PAREN_PASS26
// Retail 0x0066C900 and FableWin 0x01ACE0A0 share the original union seed and Newton expression.
// Bracket: group only the seed square to reproduce the two native instruction schedules.
// Inline-budget: the original leaf expands both reciprocal-root computations at 0x0066C955
// and 0x0066C9DE. Keep the existing shared helper's FABLE_FORCEINLINE spelling.
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastOneOverSqrt(float n)
{
    TFloatIntUnion seed;
    seed.Int = (((0x17c - ((((TFloatIntUnion*)&n)->Int >> 23) & 0xff)) >> 1) << 23) |
               (GFastOneOverSqrtTable[(((TFloatIntUnion*)&n)->Int >> 17) & 0x7f] << 15);
    float r = seed.Float;
    r = (3.0f - (r * r) * n) * r * 0.5f;
    return r;
}
#else
#ifdef FABLE_RETAIL_FLASH_SQRT_SEED_PASS30
// The original donor names n, seed and r. Copy the 32-bit representation
// through unsigned int so the calculation also works when long is 64 bits.
#include <string.h>
inline float FABLE_FASTCALL GFFastOneOverSqrt(float n)
{
    unsigned int bits;
    memcpy(&bits, &n, sizeof bits);
    TFloatIntUnion seed = {0.0f};
    unsigned int rep = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
        (static_cast<unsigned int>(GFastOneOverSqrtTable[(bits >> 17) & 0x7f]) << 15);
    memcpy(&seed.Float, &rep, sizeof seed.Float);
    float r = seed.Float;
    return (3.0f - r * r * n) * r * 0.5f;
}
#elif defined(FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_SEED_FIELDS_PASS26)
// FableWin 01ACE0A0 and Ego_d agree on n, TFloatIntUnion seed, and r.
// Copy the 32-bit representation through an unsigned int so both integer widths work.
#include <string.h>
inline float FABLE_FASTCALL GFFastOneOverSqrt(float n)
{
    unsigned int bits;
    memcpy(&bits, &n, sizeof n);
    TFloatIntUnion seed;
    seed.Int = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
               (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r;
    bits = static_cast<unsigned int>(seed.Int);
    memcpy(&r, &bits, sizeof r);
    r = (3.0f - ((r * r) * n)) * r * 0.5f;
    return r;
}
#else
#ifdef FABLE_RETAIL_MESH_DUMMY_INVERSE_STEP_PASS26
// FableWin 01ACE0A0 and Ego_d identify n, seed and r. The native leaf loads float constants
// for the Newton step and schedules the exponent across the preceding vector store.
// Inline-budget: both calls expand under the existing vector-header policy.
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastOneOverSqrt(float n) {
    TFloatIntUnion seed;
    seed.Int = (((0x17c - ((((TFloatIntUnion*)&n)->Int >> 23) & 0xff)) >> 1) << 23)
             | (GFastOneOverSqrtTable[(((TFloatIntUnion*)&n)->Int >> 17) & 0x7f] << 15);
    float r = seed.Float;
    r = (3.0f - ((r * r) * n)) * r * 0.5f;
    return r;
}
#else
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastOneOverSqrt(float n){TFloatIntUnion seed;seed.Int=(((0x17c-((((TFloatIntUnion*)&n)->Int>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(((TFloatIntUnion*)&n)->Int>>17)&0x7f]<<15);float r=seed.Float;r=(3.0f-r*r*n)*r*0.5f;return r;}
#endif
#endif // FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION_SQUARE_PAREN_PASS26
#endif // FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_SEED_FIELDS_PASS26
#else
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-r*r*value)*r*0.5f;}
#endif
#endif
#endif
#endif // FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#if defined(FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23)
// Original GFSqr float helper; preserve its product as one arithmetic expression.
inline float FABLE_FASTCALL GFSqr(float n){return (n*n);}
#else
#if defined(FABLE_RETAIL_JACK_STAGE1_UPDATE) || defined(FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL)
inline float FABLE_FASTCALL GFSqr(float n){return n*n;}
#else
inline float FABLE_FASTCALL GFSqr(float value){return value*value;}
#endif
#endif // FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
inline float FABLE_FASTCALL GFRoot(float n){return sqrt(n);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_PACKED
// Explicit owner exception approved2026-11-10 for original C3DVector alignment1.
// Both original filter TPI records put OurPos at+1 after RestrictAngle bool;
// donor constructor and native00986efa use that exact member offset.
// Original CTCBoss PhaseStartPos+22 independently confirms embedded alignment1.
#pragma pack(push,1)
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
// Original FableWin01d9faf0 calls the float sqrt overload.
inline float FABLE_FASTCALL GFFastSqrt(float n){return sqrt(n);}
#endif
class C3DVector {
public:
    float X,Y,Z;
#ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
    float GetMagnitude2D()const{return GFFastSqrt(X*X+Y*Y);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
    // Retail opt-in (Codex 2026-11-09): PDB inequality and native per-axis epsilon tests in 00e5e350.
    bool operator!=(const C3DVector& v) const {
        return fabs(X-v.X)>DELTA_NOTIONAL_ZERO || fabs(Y-v.Y)>DELTA_NOTIONAL_ZERO || fabs(Z-v.Z)>DELTA_NOTIONAL_ZERO;
    }
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_PLUSASSIGN2D_PASS27
    const C3DVector& operator+=(const C2DVector& v) { X+=v.X; Y+=v.Y; return *this; }
#endif
#ifdef FABLE_RETAIL_AVOID_REGION_FRAME_PASS29
    C3DVector operator-(const C2DVector& v) const {return C3DVector(X-v.X,Y-v.Y,Z);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_ADD2D
    // Retail opt-in (Codex pass17): original non-const value return, donor01cf3ca0.
    // Fidelity: X value capture gives native006b0b97 operand order.
#ifdef FABLE_RETAIL_C3DVECTOR_ADD2D_DIRECT
    // Retail opt-in (Codex pass18): original debug body01cf3ca0 has no named
    // X snapshot (module385 locals); direct three constructor arguments.
    C3DVector operator+(const C2DVector& v) const {return C3DVector(X+v.X,Y+v.Y,Z);}
#else
    C3DVector operator+(const C2DVector& v) const {float x=v.X;return C3DVector(X+x,Y+v.Y,Z);}
#endif
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_MAGNITUDE_DECLARATION
    float GetAccurateMagnitude()const;
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
    // Opt-in (row 0x007F0F40): Ego_r/FableWin ?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ, out of line at 0x00A14480 (landed).
    float GetMagnitudeAndNormalise();
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_TO2D
    C2DVector ToC2DVector()const{return C2DVector(X,Y);}
#endif
#ifdef FABLE_RETAIL_CPARTICLE_SPRITE
    // Original PDB signature and donor01D09580: this + scaled displacement.
    const C3DVector Interpolate(const C3DVector& vector,float interp)const{return *this+(vector-*this)*interp;}
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_INTERPOLATE) && !defined(FABLE_RETAIL_CPARTICLE_SPRITE)
    // ?Interpolate@C3DVector@@QBE?BV1@ABV1@M@Z (FableWin donor 0x01D09580): the same body without the CParticle opt-in set;
    // RenderSpriteTrail 0x00BCA950 expands it (tangent .Y/.Z read through one strength-reduced address in both the subtract and the add, 0x00BCB215).
    const C3DVector Interpolate(const C3DVector& vector,float interp)const{return *this+(vector-*this)*interp;}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_CLEAR
    // Retail opt-in (Codex pass21): original01D8BC00 Z/Y/X zero stores;
    // called by original particle ClearAcceleration02F41010.
    void Clear() { X=Y=Z=0.0f; }
#endif
    C3DVector() {}
#ifdef FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
    // Original donor018A0BE0 direct X/Y/Z stores; no Initialise call.
    C3DVector(float x,float y,float z){X=x;Y=y;Z=z;}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE
    // Per-TU opt-in (row 0x00CBFB7D RunCutsceneMacro; hand885 text): ??0C3DVector@@QAE@MMM@Z has no out-of-line copy in Fable.exe
    // (rebuild/oracles/reloc_bindings.tsv) or in the Ego_r.pdb name set, and retail expands it at all 30 sites of that 74 KB
    // function, where the plain inline body is called out of line once the function's inline budget is spent.
    FABLE_FORCEINLINE
#endif
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
#endif
#ifndef FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23
#ifdef FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
    // Original donor018BFF40 and BPREL record p.
    C3DVector(const C3DVector& p){X=p.X;Y=p.Y;Z=p.Z;}
#else
    #ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    C3DVector(const C3DVector& p){X=p.X;Y=p.Y;Z=p.Z;}
#else
    C3DVector(const C3DVector& other){X=other.X;Y=other.Y;Z=other.Z;}
#endif
#else
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    C3DVector(const C3DVector& p){X=p.X;Y=p.Y;Z=p.Z;}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE
    // (same opt-in) ??0C3DVector@@QAE@ABV0@@Z: no out-of-line copy in Fable.exe or Ego_r.pdb either; retail copies by value in place.
    FABLE_FORCEINLINE
#endif
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
#endif
#endif
#endif // preserve implicit libpk copy
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
#ifdef FABLE_RETAIL_C3DVECTOR_TOC3DVECTOR
    // Retail opt-in (hand3c3, row 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere): Ego_d.pdb / FableWin.pdb
    // ?ToC3DVector@C3DVector@@QBE?AV1@XZ (FableWin 0x02C9FCA0, 28 bytes; Ego_d line table 7 / 15 / 6 = `return *this;` through the
    // copy constructor), the C3DVector twin of C4DVector::ToC3DVector; inline in retail (no Fable.exe copy). The sprite trail
    // loop copies each trail position through it (Ego_d line 253 is 19 bytes: hidden return slot + operator-> + this call).
    C3DVector ToC3DVector() const { return *this; }
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_TOCOORDF_PASS26
    // Original02584a60 and PDB signature, same copy conversion as ToC3DVector.
    C3DVector ToC3DCoordF() const { return *this; }
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB)
    // Retail opt-in (agentC17; rows 0x00BA5260 RenderMeshPrimitive, 0x00BB30A0 / 0x00BB3E80 cloth): a default-constructed named result
    // filled through Initialise. FableWin.pdb ??GC3DVector@@QBE?BV0@ABV0@@Z debug copy 34i; this text gives 34i at /Od (the named-ctor
    // form 32i, `return C3DVector(...)` 28i). Replaces the overlay-only FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB (no debug-build evidence).
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{C3DVector result;result.Initialise(X-other.X,Y-other.Y,Z-other.Z);return result;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB) || defined(FABLE_RETAIL_C3DVECTOR_SUBTRACT_NAMED_RESULT)
    // Retail opt-in (chunk 2060, row 0x00BA5260 RenderMeshPrimitive; agentC2 row 0x00BB3E80): the named-result text (as C2DVector's
    // FableWin operators). FableWin.pdb ??GC3DVector@@QBE?BV0@ABV0@@Z debug copy is 34i/90b; `return C3DVector(...)` is 28i/70b at /Od.
    // FABLE_RETAIL_C3DVECTOR_SUBTRACT_NAMED_RESULT is the same body (acctC: row 0x00795300; wfD6 row 0x00B01900
    // CPSCRenderMesh::AddToBoundingSphere; wfE8 row 0x00ACA830 COptimisedPrimitive::Initialise): same release code, one more inline
    // layer (copy constructor) per use.
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{C3DVector result(X-other.X,Y-other.Y,Z-other.Z);return result;}
#else
#ifdef FABLE_RETAIL_UPDATEAI_VECTOR_SUB_PASS29
    // Ego_d formal coord and FableWin 0x019072B0 direct three-component return in UpdateAI.
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& coord) const { return C3DVector(X - coord.X, Y - coord.Y, Z - coord.Z); }
#elif defined(FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP)
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector((X-other.X),(Y-other.Y),Z-other.Z);}
#elif defined(FABLE_RETAIL_C3DVECTOR_SUBTRACT_X_GROUP)
    // Retail opt-in (w24B, row 0x007DDA30 CTCCreatureFlyInCircles::QueueSwoop; chunk Q6 lesson 4): the X difference alone grouped. Same
    // value and same instructions; one no-code PAREN tuple. With FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED it puts the clear()'s `push 0` between the second magnitude's
    // `fld st(4)` and `fmul st(5)` (0x007DDB16); with the default parenthesised GetSquaredMagnitude it lands one instruction early, ungrouped one late, XY-grouped three early.
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector((X-other.X),Y-other.Y,Z-other.Z);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
#endif
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_INITIALISE_ADD)
    // Overlay opt-in (wfA4, row 0x00BAA870): the named default-constructed result filled through Initialise, as
    // FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB. FableWin.pdb ??HC3DVector@@QBE?BV0@ABV0@@Z debug copy 34i; this text gives 34i at /Od.
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{C3DVector result;result.Initialise(X+other.X,Y+other.Y,Z+other.Z);return result;}
#elif defined(FABLE_RETAIL_C3DVECTOR_ADD_NAMED_RESULT)
    // Opt-in (wfD6, row 0x00B01900 CPSCRenderMesh::AddToBoundingSphere): the named-result body, as SCALE_NAMED_RESULT. FableWin.pdb debug copy is
    // 34i/90b at /Od (this text SAME by fwsize; `return C3DVector(...)` is 28i/70b). Body only: default path unchanged.
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{C3DVector result(X+other.X,Y+other.Y,Z+other.Z);return result;}
#elif defined(FABLE_RETAIL_CARRYABLE_POSE_PASS29)
    // Bracket: GetCarriedPositionInfo schedules its address setup before the offset products.
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,(Z+other.Z));}
#elif defined(FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP)
    // Original addition signature and scalar components, donor018C0B60.
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector((X+other.X),(Y+other.Y),Z+other.Z);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_INITIALISE_SCALE)
    // Overlay opt-in (wfA4, row 0x00BAA870): FableWin.pdb ??DC3DVector@@QBE?BV0@M@Z debug copy 31i; this text gives 31i at /Od.
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{C3DVector result;result.Initialise(X*scale,Y*scale,Z*scale);return result;}
#elif defined(FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT)
    // Retail opt-in (wf21 row 0x00875B10 NCameraModes::CTools::TestMeshForOccupationAndGetType): the scaled copy built in a named
    // local. Same arithmetic; the extra copy tuple moves VC7.1's x87 scheduling so the Y product of `normal * cos` is
    // `fld [nY] ; fmul st(2)` (retail 0x00875DD3) instead of `fld st(1) ; fmul [nY]`. Body only: default path unchanged.
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{C3DVector result(X*scale,Y*scale,Z*scale);return result;}
#elif defined(FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN)
    // Retail opt-in (Codex 2026-11-10,007bc6fa): grouping the X product adds one no-code
    // scheduler tuple. The81-tuple region boundary then places the physics reload at007bc824
    // before the three x87 stack pops, matching the original shot TU. Arithmetic is unchanged.
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector((X*scale),Y*scale,Z*scale);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_CROSSWITHUP) && !defined(FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES)
    // Retail opt-in (row 0x00959BF0): FableWin.pdb ?CrossWithUp@C3DVector@@QBE?AV1@XZ (debug copy 0x01FA1780), the same body as the
    // FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES copy; expanded in CAIStateGroup_HoverWander::Update (fchs of X, Z = 0.0f).
    FABLE_VECTOR_INLINE C3DVector CrossWithUp() const {return C3DVector(Y,-X,0.0f);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_GETSCALED
    // Retail opt-in (wf3, row 0x00A89740 C3DMesh2::GetHelperPointPositionAnimated): FableWin.pdb ?GetScaled@C3DVector@@QBE?BV1@ABV1@@Z
    // (componentwise product; no out-of-line copy in retail, expanded at 0x00A8981C: `fld this.k ; fmul scale.k` per axis).
#ifdef FABLE_RETAIL_C3DVECTOR_GETSCALED_NAMED_RESULT
    // Nested opt-in (wfA5, rows 0x00AA0FA0 / 0x00AA2110): the FableWin.pdb ?GetScaled@C3DVector@@QBE?BV1@ABV1@@Z text: a named result
    // (FableWin debug 34i/90b = VC7.1 /Od of this body; the one-expression body is 28i/70b). Release bytes unchanged. Also wfE8 row
    // 0x00B10B10 CPSCEmitterGeneric::GenerateParticle.
    FABLE_VECTOR_INLINE const C3DVector GetScaled(const C3DVector& scale)const{C3DVector result(X*scale.X,Y*scale.Y,Z*scale.Z);return result;}
#else
    FABLE_VECTOR_INLINE const C3DVector GetScaled(const C3DVector& scale)const{return C3DVector(X*scale.X,Y*scale.Y,Z*scale.Z);}
#endif
#endif
#ifdef FABLE_RETAIL_C3DVECTOR
    // Chunk 734 (row 0x004A1CB0): FableWin.pdb ??GC3DVector@@QBE?BV0@XZ (unary minus; no out-of-line copy in retail).
#ifdef FABLE_RETAIL_INTERRUPTABLE_AUTOTURN_PASS30
    // Native 0x0087D94B..0x0087D97F preserves the X and Y negations as expression
    // units while scheduling the orientation's third component and virtual receiver.
    FABLE_VECTOR_INLINE const C3DVector operator-()const{return C3DVector((-X),(-Y),-Z);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator-()const{return C3DVector(-X,-Y,-Z);}
#endif
    // FableWin.pdb ??TC3DVector@@QBE?BV0@ABV0@@Z (cross product; debug copy 0x01B40A20 right after GetRight 0x01B409F0, expanded in 0x004A1CB0).
#ifdef FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& vector)const{return C3DVector((Y*vector.Z-Z*vector.Y),(Z*vector.X-X*vector.Z),(X*vector.Y-Y*vector.X));}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_CROSS_ORIGINAL_FORMAL
    // Both PDB operator^ signature; original donor01B40A20 parameter vector.
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& vector)const{return C3DVector(Y*vector.Z-Z*vector.Y,Z*vector.X-X*vector.Z,X*vector.Y-Y*vector.X);}
#else
#if defined(FABLE_RETAIL_C3DVECTOR_CROSS_NAMED_RESULT)
    // Opt-in (wfD6, row 0x00B01900 CPSCRenderMesh::AddToBoundingSphere): the named-result body, as SCALE_NAMED_RESULT. FableWin.pdb debug copy is
    // 49i/130b at /Od (this text SAME by fwsize; `return C3DVector(...)` is 43i/110b). Body only: default path unchanged.
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{C3DVector result(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);return result;}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP)
    // Retail opt-in (row 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions): the X and Z cross-product components parenthesised, Y
    // bare. Same arithmetic and instructions as the default; the two no-code PAREN tuples move VC7.1's scheduling-region cut so the
    // argument set-up of the first CMatrix3x4::operator* call (`push ecx ; lea edx,[esp+78h] ; push edx ; lea ecx,[esi+6Ch]`,
    // 0x0075A8CF..0x0075A8D5) stays after the r ^ n stores (`fstp [esp+2Ch]` / `fstp [esp+30h]`, 0x0075A8C3 / 0x0075A8C7). Without
    // it the set-up is hoisted to 0x0075A81A, ahead of the w ^ r product, and the frame slots of the cross-product temporaries move by 8
    // (725 v 726 bytes). Chosen by a sweep of the CROSS_* groupings (ATTEMPTS.log v23..v36). Body only: no layout change.
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),Z*other.X-X*other.Z,(X*other.Y-Y*other.X));}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
    // Retail opt-in (wf1, row 0x00A114B0 CPhysicsMeshObject::SetPositionMatrix): every cross-product component parenthesised. Same
    // instructions as the default; the three no-code PAREN tuples sit ahead of VC7.1's first scheduling-region cut (81-tuple cap) and
    // put it where retail has it: between the load of position.Y and the load of position.X (0x00A11527 / 0x00A1152E), so the store
    // of the cross product's Z (`fstp [esp+14h]`, 0x00A1152A) is the last instruction of the first region. Body only.
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),(X*other.Y-Y*other.X));}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_XY_GROUP)
    // Retail opt-in (Codex pass18): grouping the first two cross expressions
    // preserves the native00a42170 copy-load schedule without extra instructions.
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),X*other.Y-Y*other.X);}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_X_GROUP)
    // Retail opt-in (Codex pass 17): preserve the first cross-product expression as a unit.
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#endif
#endif // FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP
#endif
#endif
#endif
#ifdef FABLE_RETAIL_CREATURE_NAV_NEXT_POS_PASS30
    // Original approximate comparison at FableWin 0x02479D00 in c_creature_navigation.obj.
#ifdef _MSC_VER
#pragma intrinsic(fabs)
#endif
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other) const {
        if (!(fabs(X - other.X) <= DELTA_NOTIONAL_ZERO))
            return false;
        if (!(fabs(Y - other.Y) <= DELTA_NOTIONAL_ZERO))
            return false;
        if (!(fabs(Z - other.Z) <= DELTA_NOTIONAL_ZERO))
            return false;
        return true;
    }
#else
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR
    // Retail branch: the body is one parenthesised sum. VC7.1 keeps a parenthesised sum as a unit where an inlined caller adds
    // the result into a running float total (0x00AE0710 CalculateAveragePatchSize: `fmul; faddp st(1); fmul; faddp st(1);
    // faddp st(3)`); the unparenthesised default lets it reassociate each product into the total (identical bytes everywhere
    // else). The operand order inside the parentheses does not change the bytes there.
#if defined(FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23)
    // Original donor01907260 is57B with no named local; the row selects an inline budget.
    inline float GetSquaredMagnitude()const{return (X*X)+(Y*Y)+(Z*Z);}
#elif defined(FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ)
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): the same parenthesised one-line sum in X, Y, Z
    // order. There the inlined Normalise loads Z, Y, X (0x0079AE93 fld [esp+24h], 0x0079AEA0 fld [esp+20h], 0x0079AEAA fld
    // [esp+1Ch]); the X, Z, Y spelling loads Y, Z, X in that TU. Body only: no layout change, default path unchanged.
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
#elif defined(FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX)
    // Retail opt-in (chunk Q14, row 0x00A58790 NBBBMaths::CSphere::GetFirstIntersectionWithFiniteLine): the parenthesised one-line
    // sum in Z, Y, X order. There both squared lengths sum Z*Z, Y*Y, X*X (0x00A587B2 `fst [esp+8] ; fmul [esp+8]` then Y, X;
    // 0x00A58825 `fld st(2) ; fmul st(3)` on the offset's Z) and the first one is shared with IsNonZero's Z, Y, X sum. Body only:
    // no layout change, default path unchanged.
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (Z*Z+Y*Y+X*X);}
#else
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Z*Z+Y*Y);}
#endif
#endif
    // Chunk 732 (row 0x006AABE0 SetActionForMelee): FableWin.pdb ?GetSquaredMagnitude2D@C3DVector@@QBEMXZ (debug copy 0x01DAEC70; no
    // Ego_r.pdb copy, expanded in retail) as running statements: retail loads X then Y (0x006AACCF, 0x006AADB1, 0x006AAF97); the
    // one-line `X*X+Y*Y` lets VC7.1 load Y first there. No other FABLE_RETAIL_C3DVECTOR includer uses it.
#ifdef FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
    // Original FableWin01daec70 is18instructions/43B; the existing running-sum variant is44B.
    // Reuse the same original expression retained in the existing non-retail branch.
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{float squared=X*X;squared+=Y*Y;return squared;}
#endif
#else
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
#endif
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23)
    // Original donor01D89990 is59B with no named dot local; ordinary inline is sufficient.
    // Preserve the original return/receiver/formal and arithmetic association; group only scope.
    inline const float Dot(const C3DVector& vector)const{return (X*vector.X)+(Y*vector.Y)+(Z*vector.Z);}
#elif defined(FABLE_RETAIL_C3DVECTOR_DOT_XY_THEN_Z)
    // Retail opt-in (chunk 2053, row 0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction): the X/Y pair as one parenthesised
    // sum, then Z added to the named total. FableWin.pdb ?Dot@C3DVector@@QBE?BMABV1@@Z debug copy 0x01D89990 is 23i/59b; this text is
    // 23i/60b at plain /Od (fwsize SAME; the same +1 byte as the landed running-sum GetSquaredMagnitude2D, 18i/44b v 18i/43b). Without
    // the parentheses (identical /Od bytes) the row grades DIFFER (md.py 714 equal; matched 715). Body only.
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{float dot=(X*other.X+Y*other.Y);dot+=Z*other.Z;return dot;}
#elif defined(FABLE_RETAIL_C3DVECTOR_DOT_GROUPED)
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{float dot=(Y*other.Y);dot+=Z*other.Z;dot+=X*other.X;return dot;}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ
    // Retail opt-in (chunk Q12, row 0x008B6AD0 CCreatureAction_MidAttackSide::Initialise): the dot product as running statements in
    // Y, X, Z order. There retail forms the Y product first with this->Y on top (0x008B6BBC `fxch st(2) ; fmul st(2)`), then the X
    // product (`fmul [other.X]`) added onto it, then Z; the one-line default forms the same order with the operand roles swapped and
    // the X, Y, Z statements form X first. Body only: no layout change, default path unchanged.
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{float dot=Y*other.Y;dot+=X*other.X;dot+=Z*other.Z;return dot;}
#elif defined(FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX)
    // Retail opt-in (chunk Q14, row 0x0066C900 CRegionMinimap::IlluminateRegionMinimapTheme): the dot product as running statements in
    // Z, Y, X order (0x0066CA43 `fmul st(1)` on the two Z, then Y, then X added). Body only: no layout change, default path unchanged.
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{float dot=Z*other.Z;dot+=Y*other.Y;dot+=X*other.X;return dot;}
#elif defined(FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_XYZ)
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{float dot=X*other.X;dot+=Y*other.Y;dot+=Z*other.Z;return dot;}
#else
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+(Y*other.Y+Z*other.Z);}
#else
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
#endif
#endif
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_SWAPYZ
    // Retail opt-in (acctC wfA1, row 0x00C271F0): the libpk audio TUs convert engine (Y-up) vectors to CODA axes out of line --
    // reloc binding ?SwapYZ@C3DVector@@QBE?AV1@XZ at 0x00C1C070 (landed C3DVector_SwapYZ_00c1c070), called by the landed 0x00C1C280 /
    // 0x00C27550 and row 0x00C271F0 (call 0x00C2744C, hidden-pointer result). Declaration only.
    C3DVector SwapYZ() const;                                            // 0x00C1C070
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE)
    // Retail opt-in (chunk 1023, row 0x00C1C460, the libpk listener-space update): the audio library's TUs call the vector members
    // out of line -- FableWin.pdb ?Cross@C3DVector@@QBE?BV1@ABV1@@Z at 0x00C25860 (landed), the dot product 0x00C256A0 (landed
    // descriptive DotProduct) and the field-by-field copy 0x00C1BFF0 (landed descriptive Copy). Declarations only.
    const C3DVector Cross(const C3DVector& other) const;                 // 0x00C25860
    float DotProduct(const C3DVector& other) const;                      // 0x00C256A0
#ifdef FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_PASS23
    // NativeC1BFF0 returns plain three-float CODA storage; actual API CODA_3DSourceSetPosition accepts CVECTOR.
    _CVECTOR Copy() const;
#else
    C3DVector Copy() const;                                              // 0x00C1BFF0
#endif
#elif defined(FABLE_RETAIL_CAMERA_CROSS_FORWARD_PASS29)
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& vector) const { return *this ^ vector; }
#elif defined(FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION)
    // Native007c14a3..14fb: retain the two component subtractions as source expression units.
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),(X*other.Y-Y*other.X));}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_OPERATOR_PASS29)
    // FableWin Cross at 01D8E950 delegates to operator^ at 01B40A20.
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& vector) const { return (*this) ^ vector; }
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_ALL_GROUP_PASS23)
    // Original Cross components grouped individually; no arithmetic reassociation.
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),(X*other.Y-Y*other.X));}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_XY_GROUP)
    // Retail opt-in (hand2b_more, row 0x007BBB3B CTCShotDefinitionProjectile::InitMovement): the member Cross with its first two
    // components parenthesised, Z bare -- the CROSS_XY_GROUP text of operator^ that the same-TU sibling 0x007BBE34 landed with. Same
    // instructions as the default; the two no-code PAREN tuples move VC7.1's scheduling so `lea eax,[ebp-34h]` (ApplyAcceleration's
    // argument) is issued before the Z product of `line_to_target * speed` (0x007BBC21..0x007BBC2C). Body only: no layout change.
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),X*other.Y-Y*other.X);}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP)
    // Cross expression grouping retains native00697d79..b2 scheduling.
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#elif defined(FABLE_RETAIL_C3DVECTOR_CYLINDER_CROSS_GROUPED)
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,(Z*other.X-X*other.Z),X*other.Y-Y*other.X);}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_COMPONENTS_GROUPED)
    // wfD5 (row 0x00A593F0): the CTCSHOTDEFINITIONBASE_COLLISION Cross text alone (every component its own parenthesised unit; the Dot / IsNonZero texts of that opt-in untouched).
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),(X*other.Y-Y*other.X));}
#else
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
    FABLE_VECTOR_INLINE const C3DVector& operator-=(const C3DVector& vector){X-=vector.X;Y-=vector.Y;Z-=vector.Z;return *this;}
#endif
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
#if defined(FABLE_RETAIL_C3DVECTOR_INITIALISE_DIV)
    // Overlay opt-in (wfA4, row 0x00BAA870): FableWin.pdb ??KC3DVector@@QBE?BV0@M@Z debug copy 35i: the named result through Initialise.
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{C3DVector result;float reciprocal=1.0f/v;result.Initialise(X*reciprocal,Y*reciprocal,Z*reciprocal);return result;}
#elif defined(FABLE_RETAIL_C3DVECTOR_INITIALISE_DIV3)
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{C3DVector result;result.Initialise(X/v,Y/v,Z/v);return result;}
#elif defined(FABLE_RETAIL_C3DVECTOR_INITIALISE_DIV4)
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{C3DVector result;float reciprocal=1.0f/v;result.Initialise(reciprocal*X,reciprocal*Y,reciprocal*Z);return result;}
#elif defined(FABLE_RETAIL_C3DVECTOR_INITIALISE_DIV2)
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;C3DVector result;result.Initialise(X*reciprocal,Y*reciprocal,Z*reciprocal);return result;}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_DIV_ZSPRITE_OWNER_GROUP_PASS26
    // Bracket: preserve the native reciprocal products at 0x00BAB25C.
    FABLE_VECTOR_INLINE const C3DVector operator/(float scale) const
    {
        float one_on_scale = 1.0f / scale;
        return C3DVector(X * one_on_scale, Y * one_on_scale, (Z * one_on_scale));
    }
#else
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
#endif // FABLE_RETAIL_C3DVECTOR_DIV_ZSPRITE_OWNER_GROUP_PASS26
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_DIVASSIGN_RECIPROCAL_PAREN
    // INFERRED opt-in (hand4a w24, rows 0x00853670 / 0x008539C0 NCameraModes::CScriptModuleOpenChest / UseLamp::OnInitLeaf): the same
    // operator/= with the reciprocal parenthesised. Same instructions; the no-code PAREN tuple (plus the row's parenthesised magnitude)
    // moves VC7.1's 81-tuple scheduling-region cut by two tuples: retail's Theta store / owner load / Rad store order (0x008537A4..
    // 0x008537AA) and UseLamp's AxisY copy (0x00853A90) and `test eax,eax` (0x00853B4B). Measured neutral on ~45 landed C3DVector users
    // (all still match). FableWin.pdb ??_0C3DVector@@QAEABV0@M@Z (debug 0x01F58600: reciprocal local, three products) is consistent;
    // the parenthesis itself is not visible in any donor. Default path unchanged.
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=(1.0f/v);X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
#else
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
    // Original FableWin01d99fb0 and both PDB signatures; named squared sum.
#ifdef FABLE_RETAIL_FIREBALL_MAGNITUDE_HELPER_PASS30
    // FableWin GetAccurateMagnitude sums X, Y and Z before calling GFRoot.
    FABLE_VECTOR_INLINE float GetAccurateMagnitude() const {
        float magnitude = X * X + Y * Y + Z * Z;
        return GFRoot(magnitude);
    }
#elif defined(FABLE_RETAIL_C3DVECTOR_ACCURATE_SQUARE_GROUPS_PASS23)
    // Both original PDBs and donor01d99fb0: three squared components then sqrt.
    FABLE_VECTOR_INLINE float GetAccurateMagnitude()const {float magnitude=X*X+(Y*Y)+(Z*Z);return (float)sqrt(magnitude);}
#elif defined(FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE_PAREN_RETURN)
    // INFERRED nested opt-in (hand4a w24, rows 0x00853670 / 0x008539C0 NCameraModes::CScriptModuleOpenChest / UseLamp::OnInitLeaf):
    // the same body with the return expression bracketed. Same instructions; the no-code PAREN tuple (with DIVASSIGN_RECIPROCAL_PAREN)
    // moves VC7.1's 81-tuple scheduling-region cut to retail's store order. Default path unchanged (16/16 landed users still match);
    // NOT general -- forced on it breaks 0x00875690 and 0x00761280. FableWin ?GetAccurateMagnitude 0x01D99FB0 is consistent; the
    // parenthesis itself is in no donor.
    FABLE_VECTOR_INLINE float GetAccurateMagnitude()const {float magnitude=X*X+Y*Y+Z*Z;return ((float)sqrt(magnitude));}
#else
#ifdef FABLE_RETAIL_COMBAT_SEQUENCE_MAGNITUDE_GROUP_PASS27
    // Bracket: grouping the X and Z squares puts the null store before the world load at 006EB123.
    // Either square alone leaves the pair reversed. Grouping all three also misses.
    FABLE_VECTOR_INLINE float GetAccurateMagnitude()const {float magnitude=(X*X)+Y*Y+(Z*Z);return (float)sqrt(magnitude);}
#else
    FABLE_VECTOR_INLINE float GetAccurateMagnitude()const {float magnitude=X*X+Y*Y+Z*Z;return (float)sqrt(magnitude);}
#endif
#endif
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE_X_THEN_YZ)
    // Retail opt-in (egod e03, row 0x008E5D40 CTCPhysicsStandard::Move): the named squared sum as X plus the Y/Z pair. There retail loads
    // the copied components Z, Y, X and forms (Y*Y + Z*Z) + X*X (0x008E5D57..0x008E5D85); FableWin.pdb ?GetMagnitude@C3DVector@@QBEMXZ
    // is 26i/66b (this text, fwsize SAME). FITTED grouping (the other 26i texts give other orders there). Body only.
    FABLE_VECTOR_INLINE float GetMagnitude()const{float squared=X*X+(Y*Y+Z*Z);return (float)sqrt(squared);}
#elif defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQUARED_LOCAL)
    // Retail opt-in (row 0x00B4D370): FableWin ?GetMagnitude@C3DVector@@QBEMXZ text (26i / 66b at /Od, fwsize SAME): the sum in a named
    // local, then sqrtf (as the C2DVector FABLE_RETAIL_C2DVECTOR_MAGNITUDE_SQUARED_LOCAL opt-in).
    FABLE_VECTOR_INLINE float GetMagnitude()const{float squared=X*X+Y*Y+Z*Z;return sqrtf(squared);}
#elif defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED)
    // Retail opt-in (chunk 2060, row 0x00BA5260; agentC2 row 0x00BB3E80): the named squared sum in X, Z, Y order (retail 0x00BA547A..
    // 0x00BA548C squares X, then Z, then Y). FableWin.pdb ?GetMagnitude@C3DVector@@QBEMXZ is 26i/66b.
    FABLE_VECTOR_INLINE float GetMagnitude()const{float squared=X*X+Z*Z+Y*Y;return (float)sqrt(squared);}
#elif defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE_XY_THEN_Z)
    // Retail opt-in (chunk 2053, row 0x0087F3A0): a named squared sum, X/Y pair first. FableWin.pdb ?GetMagnitude@C3DVector@@QBEMXZ
    // debug copy 0x01B52910 is 26i/66b (same signature hash as GetAccurateMagnitude 0x01D99FB0); this text is 26i/66b at plain /Od
    // (fwsize SAME). The unparenthesised `X*X+Y*Y+Z*Z` (same /Od bytes) grades DIFFER in the row (md.py 710 equal). Body only.
    FABLE_VECTOR_INLINE float GetMagnitude()const{float squared=(X*X+Y*Y)+Z*Z;return (float)sqrt(squared);}
#elif defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF)
    FABLE_VECTOR_INLINE float GetMagnitude()const{return GFRoot(X*X+Y*Y+Z*Z);}
#else
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
#if (defined(FABLE_RETAIL_UPDATE_NEAREST_INTERESTING) || (defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D) && !defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ONE_EXPRESSION) && !defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23))) && !defined(FABLE_RETAIL_CTCTARGETINGAI_ATTACK)
    // Original FableWin01d9faa0 GetMagnitude2D, calls single-precision sqrt helper.
    // (FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D selects this body alone: row 0x009711D0 and the chunk 2029 rows 0x00915B60 / 0x0094E7B0 /
    // 0x0097BDA0. FableWin.pdb ?GetMagnitude2D@C3DVector@@QBEMXZ, no Ego_r.pdb or Fable.exe copy: inline in release. 0x00915D8F..0x00915DA3
    // squares X, adds Y squared, then fsqrt.)
#ifdef FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_SQUARED_SUM
    // Retail opt-in (egod3a, row 0x00A6C640 CPhysicsMesh::CalculateBoundingInfo): the squared sum in one parenthesised named local, then
    // GFFastSqrt. FableWin.pdb ?GetMagnitude2D@C3DVector@@QBEMXZ (0x01D9FAA0) is 52b/21i in the VC10 /Od build; this text is 49b/20i
    // at VC7.1 /Od plus VC10's one extra instruction (3 bytes) for the float pushed from memory (`push ecx ; fld ; fstp [esp]`), the
    // same correction that maps GFFastSqrt 19b/8i -> FableWin 22b/9i. The parentheses are a no-code PAREN tuple: with them the row's
    // last scheduling region (81-tuple cap) ends before the circle radius store and `pop edi` issues after `fsqrt` (0x00A6C839).
    // CAVEAT: under FableWin's /Gr a `return sqrtf(squared);` spelling lands on the same size, so the size fixes the one named sum,
    // not the callee. FITTED for the parentheses. Body only; the running-sum default below is unchanged.
    inline float GetMagnitude2D()const {float squared=(X*X+Y*Y);return GFFastSqrt(squared);}
#else
    inline float GetMagnitude2D()const {float squared=X*X;squared+=Y*Y;return GFFastSqrt(squared);}
#endif
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D) && (defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ONE_EXPRESSION) || defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23)) && !defined(FABLE_RETAIL_UPDATE_NEAREST_INTERESTING) && !defined(FABLE_RETAIL_CTCTARGETINGAI_ATTACK)
    // New ORIGINAL_SUM_PASS23 opt-in: original FableWin01d9faa0 is52B, no source locals,
    // and calls GFFastSqrt on the direct X/Y square sum. The existing ONE_EXPRESSION note follows.
    // FITTED variant (chunk Q9, row 0x0092ACE0 CAIStateGroup_ApprenticeHero::GoToLibrary, its only user): the sum as one expression. With the
    // two-statement body above -- the one every other row matches with -- that row differs at the same size (389 v 389, graded 2026-11-13).
    // One retail inline has one body, so the open question is that row's own spelling of `direction /= direction.GetMagnitude2D()`.
#ifdef FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
    inline float GetMagnitude2D()const {return GFFastSqrt(X*X+Y*Y);}
#else
    inline float GetMagnitude2D()const {return GFFastSqrt(X*X+Y*Y);}
#endif
#endif
#ifdef FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
    FABLE_FORCEINLINE C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#else
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#endif
#ifdef FABLE_RETAIL_C3DVECTOR
    // Retail branch (pass 12 header wave, 0x00AC2BE0 PartitionSpace): inline in retail; FableWin.pdb keeps out-of-line debug
    // copies ?LimitLower@C3DVector@@QAEXABV1@@Z 0x02E345C0 / ?LimitUpper@C3DVector@@QAEXABV1@@Z 0x02E34650, retail expands them
    // (0x00AC2BE0's per-axis compare-and-store runs on the triangle min/max boxes).
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    // Codex pass 13 (row 0x00EC5DE0): ?GetAngleXY@C3DVector@@QBEMXZ (FableWin 0x01ACF1E0 debug copy; no out-of-line copy in retail,
    // the row's TU expands it). Declaration only: the body is the row's (GFArcTan / GET_ROUNDED_ANGLE live in that TU).
#ifdef FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
    // Retail opt-in (Codex 2026-11-09): FableWin 01acf1e0 GetAngleXY,
    // 01dcb530 InitialiseFromAngles, 01fa1780 CrossWithUp. The 00a22ca0
    // body expands angle wrapping, heading construction and the horizontal cross product.
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
    FABLE_VECTOR_INLINE float GetAngleXY()const{return GET_ROUNDED_ANGLE(GFArcTan(X,Y));}
#else
    FABLE_VECTOR_INLINE float GetAngleXY() const {
        float angle=(float)atan2(X,Y)*0.15915494f;
        if(angle<0.0f || angle>=1.0f) {
            angle=(float)fmod((double)angle,1.0);
            if(angle<0.0f) angle+=1.0f;
        }
        return angle;
    }
#endif
    FABLE_VECTOR_INLINE C3DVector CrossWithUp() const {return C3DVector(Y,-X,0.0f);}
#ifdef FABLE_RETAIL_C3DVECTOR_ANGLE_FORMALS_MAGNITUDE_PASS26
    // Ego_d records the three arguments and cos_yz. FableWin 01DCB548 and 01DCB565
    // form magnitude * cos_yz before evaluating the sine and cosine of anglexy.
    FABLE_VECTOR_INLINE const C3DVector& InitialiseFromAngles(float anglexy, float angleyz, float magnitude) {
        float cos_yz = GFCos(angleyz);
        X = (magnitude * cos_yz) * GFSin(anglexy);
        Y = (magnitude * cos_yz) * GFCos(anglexy);
        Z = magnitude * GFSin(angleyz);
        return *this;
    }
#else
#ifdef FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN
    // Nested opt-in (chunk 2041, row 0x006AB820 ResolveFacingDirection): the body goes through GFCos / GFSin (double fastcall, float
    // angle) with the implicit double -> float stores. FableWin.pdb: ?GFSin@@YINM@Z 0x01DCB5D0 is the COMDAT right after
    // ?InitialiseFromAngles@ 0x01DCB530 (InitialiseFromAngles is GFSin's first instantiation; GFCos 0x01D9E180 came earlier). The explicit
    // `(float)(sin(..)*(cosYZ*length))` spelling below leaves a no-code PAREN tuple between the Y fmul and its fstp that moves
    // 0x006ABB81..8F in that row. Same arithmetic; body only.
    FABLE_VECTOR_INLINE const C3DVector& InitialiseFromAngles(float angleXY,float angleYZ,float length) {
        float cosYZ=GFCos(angleYZ);
        X=GFSin(angleXY)*(cosYZ*length);
        Y=GFCos(angleXY)*(cosYZ*length);
        Z=GFSin(angleYZ)*length;
        return *this;
    }
#else
    FABLE_VECTOR_INLINE const C3DVector& InitialiseFromAngles(float angleXY,float angleYZ,float length) {
        float cosYZ=(float)cos(angleYZ*(double)6.2831855f);
        X=(float)(sin(angleXY*(double)6.2831855f)*(cosYZ*length));
        Y=(float)(cos(angleXY*(double)6.2831855f)*(cosYZ*length));
        Z=(float)(sin(angleYZ*(double)6.2831855f)*length);
        return *this;
    }
#endif
#endif // FABLE_RETAIL_C3DVECTOR_ANGLE_FORMALS_MAGNITUDE_PASS26
#else
    float GetAngleXY() const;
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_GETANGLEYZ
    // Retail opt-in (chunk 1021, row 0x00A0C570 CCamera::SetAngleXY): FableWin.pdb ?GetAngleYZ@C3DVector@@QBEMXZ (debug copy
    // 0x024712F0; no Ego_r.pdb copy, expanded in retail): the pitch atan2(Z, |XY|) in turns, wrapped into [0, 1) (0x00A0C576..0x00A0C5DD
    // for Forward, 0x00A0C61A..0x00A0C680 for Up; the same sequence as the landed CCamera::GetAngleYZ 0x00A0C3E0). Body only.
    FABLE_VECTOR_INLINE float GetAngleYZ() const {
        float angle=(float)atan2(Z,sqrt(Y*Y+X*X))*0.15915494f;
        if(angle<0.0f || angle>=1.0f) {
            angle=(float)fmod((double)angle,1.0);
            if(angle<0.0f) angle+=1.0f;
        }
        return angle;
    }
#endif
    // Chunk 732 (row 0x006AABE0 CThingPlayerCreature::SetActionForMelee): Ego_r.pdb ?RotateXY@C3DVector@@QAEXM@Z (0x00B81930),
    // out of line in retail at 0x00A14040 (395 bytes, the sin/cos rotation about Z). A declaration only.
    void RotateXY(float angle);                                          // 0x00A14040
#ifdef FABLE_RETAIL_C3DVECTOR_RANDOMPOINTINSPHERE
    // Retail opt-in (wf20, row 0x00918B90 CAIStateGroup_Gaze::Update): Ego_r.pdb ?GetRandomPointWithinUnitSphere@C3DVector@@SI?AV1@PAK@Z,
    // static __fastcall, out of line in retail at 0x00A146C0 (hidden result in ecx, seed in edx). A declaration only.
    static C3DVector __fastcall GetRandomPointWithinUnitSphere(unsigned long* seed);   // 0x00A146C0
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
    // Retail opt-in (chunk Q18, row 0x00837220 CThingTrackNode::UpdateFacingDirection): Ego_r.pdb ?Rotate@C3DVector@@QAEXMM@Z, out of
    // line in retail at 0x00A14260 (landed; thiscall, two floats, `ret 8`). A declaration only.
    void Rotate(float angle_xy, float angle_yz);                         // 0x00A14260
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
    // Retail opt-in (chunk 1023, row 0x00B67270 CEngineLandscapeRenderer::BuildBlendingTables): Ego_r.pdb / FableWin.pdb
    // ?AccurateNormalise@C3DVector@@QAEAAV1@XZ, out of line in retail at 0x00A14440 (the landed bound spelling; called with ecx = the
    // vector at 0x00B673A9). Declaration only.
    C3DVector& AccurateNormalise();                                      // 0x00A14440
#endif
    // Chunk 739 (row 0x00B0CAD0): FableWin.pdb ?GetRandomUnitVector@C3DVector@@SI?AV1@PAK@Z (static __fastcall, out of line at 0x00A145C0,
    // landed) and ?Normalise@C3DVector@@QAEAAV1@XZ (the fast normalise: GFFastOneOverSqrt of GetSquaredMagnitude, expanded in retail).
    static C3DVector __fastcall GetRandomUnitVector(unsigned long* seed);  // 0x00A145C0
#if defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_YZX)
    // Retail opt-in (egod e03, row 0x008E5D40 CTCPhysicsStandard::Move): the components read into locals x, y, z, then the squared length
    // as a running sum Y, Z, X (the NORMALISE_LOCALS text in another order). There retail loads X, Y, Z (0x008E5E17..0x008E5E21) and
    // forms (Y*Y + Z*Z) + X*X. FableWin.pdb ?Normalise@C3DVector@@QAEAAV1@XZ is 48i/133b, this text 47i/130b (NORMALISE_LOCALS too).
    // FITTED order. Body only: no layout change, default path unchanged.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float x=X;float y=Y;float z=Z;float squared=y*y;squared+=z*z;squared+=x*x;float one_on_magnitude=GFFastOneOverSqrt(squared);X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_NAMED_SQUARED)
    // Retail opt-in (wfE8 8th agent, row 0x00ACA830 COptimisedPrimitive::Initialise): IsNonZero as ONE expression (FableWin.pdb
    // ?IsNonZero 0x01B4F100 31i/91b = this text at /Od) and Normalise with its squared length in a named local (FITTED: no whole-sum
    // CSE with IsNonZero, so the three products are shared and VC7.1 orders them per site, as retail does: X,Z,Y at 0x00ACABD2 /
    // 0x00ACB0C0, Z,Y,X at 0x00ACB4B2 and in GetFaceNormal 0x00AC7D51). Bodies only: default path unchanged.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float squared=Z*Z;squared+=X*X+Y*Y;float factor=GFFastOneOverSqrt(squared);X*=factor;Y*=factor;Z*=factor;return *this;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_ZXY)
    // Retail opt-in (wfE8, row 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData; bodies of the bwf1 lane's FABLE_NV_* row macros):
    // Normalise sums Z*Z + X*X + Y*Y (one x87 chain) and IsNonZero accumulates Z*Z then X*X + Y*Y, the expansions retail inlines in
    // the loader's normal / tangent loops. Bodies only: default path unchanged.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(Z*Z+X*X+Y*Y);X*=factor;Y*=factor;Z*=factor;return *this;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARED_ZXY)
    // Overlay opt-in (wfA4, row 0x00BAA870 CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList): the squared length accumulated
    // from the members in Z, X, Y order (the NORMALISE_LOCALS sum without the component copies). There the camera-up vector (Z = 0) squares
    // X then Y (0x00BAAC3C `fld st(1) ; fmul st(2)`) and the first scale multiplies the stack component by the spilled factor
    // (0x00BAACA0 `fstp [esp+10h] ; fxch st(1) ; fmul [esp+10h]`); the LOCALS copies put the factor first there. Body only.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float squared=Z*Z;squared+=X*X;squared+=Y*Y;float one_on_magnitude=GFFastOneOverSqrt(squared);X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ)
    // Overlay opt-in (wf22, row 0x007E1FC0 CTCAmbientBird::FrameUpdate): the PAREN_SQUARES_ZYX body with the squares summed X, Y, Z
    // (0x007E21F0 `fld [X*X] ; fadd [Y*Y] ; fadd [Z*Z]` from the spilled squares shared with the earlier distance test). Body only.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt((X*X)+(Y*Y)+(Z*Z));X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX)
    // Overlay opt-in (row 0x009735D0 CAIStateGroup_JackOfBladesBase::RushToPos): the squared length written Z, Y, X with each square
    // in its own parentheses. VC7.1 keeps the parenthesised squares as separate common subexpressions (0x00973609..0x0097361D: Z*Z,
    // Y*Y, X*X computed and spilled first) and does not merge this sum with the later GetMagnitude sum of the same vector (summed
    // again from the spilled squares before the fsqrt at 0x009736B8); the default body shares the whole sum. Body only.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt((Z*Z)+(Y*Y)+(X*X));X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ)
    // Retail opt-in (chunk Q20, row 0x008B7040 CCreatureAction_JumpOutFromCover::Initialise): the components read into locals first, then
    // the squared length summed in X, Y, Z source order. There the inlined Normalise of the member JumpDirection loads X, Y, Z
    // (0x008B7047..0x008B705E) and forms Z*Z, Y*Y, X*X with the three pops after the store (0x008B707E..0x008B7088); the one-line
    // default / SQUAREDMAGNITUDE_XYZ bodies load Z, Y, X and pop once early there. Body only: no layout change, default path unchanged.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float x=X;float y=Y;float z=Z;float one_on_magnitude=GFFastOneOverSqrt(x*x+y*y+z*z);X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS)
    // Retail opt-in (chunk Q9, row 0x0092B900 CAIStateGroup_OnHeroPush::Update): the components read into locals first, then the
    // squared length summed Z, X, Y. There the inlined Normalise loads X, Y, Z (0x0092B95F..0x0092B969) and sums Z*Z, X*X, Y*Y
    // (0x0092B96C `fld st(0) ; fmul st(1)`, 0x0092B970 `fld st(3) ; fmul st(4)`, 0x0092B976 `fld st(2) ; fmul st(3)`); every
    // one-expression sum (GFSqr or plain, any order) loads the components in its own sum order there. Body only: no layout change,
    // default path unchanged.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float x=X;float y=Y;float z=Z;float squared=z*z;squared+=x*x;squared+=y*y;float one_on_magnitude=GFFastOneOverSqrt(squared);X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER)
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt((GFSqr(X)+GFSqr(Y)+GFSqr(Z)));X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#else
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GFSqr(X)+GFSqr(Y)+GFSqr(Z));X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#endif
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN)
    // Retail opt-in (chunk 857, row 0x00AC8C80 COptimisedPrimitive::ComputeVertexCost): Normalise's sum written out unparenthesised
    // and IsNonZero as X*X plus the Z/Y pair. There `if (normal.IsNonZero()) normal.Normalise();` shares the three products but
    // sums them twice in the same (Y, Z) + X order (0x00AC8DCA `fld st(0); fadd st(2); fadd st(3)`, then 0x00AC8DE9 `fadd st(1)`,
    // 0x00AC8DF0 `fadd st(2)`) and pops the two left-over products around the exponent shift (0x00AC8DFC / 0x00AC8E01); the default
    // bodies give (Z, X) + Y and both pops together. Body only: no layout change, default path unchanged.
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(X*X+Z*Z+Y*Y);X*=factor;Y*=factor;Z*=factor;return *this;}
#else
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(GetSquaredMagnitude());X*=factor;Y*=factor;Z*=factor;return *this;}
#endif
    // Chunk 732 (row 0x006AABE0): FableWin.pdb ?IsNonZero@C3DVector@@QBE_NXZ (debug copy 0x01B4F100; no Ego_r.pdb copy, expanded in
    // retail). Body from the row's bytes (0x006AAE00: X, Z, Y loaded in source order, then `> DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO`);
    // GetSquaredMagnitude's one-line sum cannot be reused (it loads Z, Y, X there, and 0x00AE0710 needs that one-line form).
#ifdef FABLE_RETAIL_FIREBALL_NONZERO_HELPER_PASS30
    // FableWin IsNonZero reads Y, X and Z and converts the comparison to bool.
    FABLE_VECTOR_INLINE bool IsNonZero() const {
        return Y * Y + X * X + Z * Z > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO ? true : false;
    }
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_NAMED_SQUARED)
    // (same opt-in, see Normalise above): the one-expression sum, FableWin 31i/91b at /Od = this text (fwsize SAME).
    FABLE_VECTOR_INLINE bool IsNonZero()const{return X*X+Y*Y+Z*Z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_ZXY)
    // (same opt-in, see Normalise above)
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=Z*Z;squared+=X*X+Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#elif defined(FABLE_RETAIL_C3DVECTOR_SUN_NONZERO_PASS30)
    FABLE_VECTOR_INLINE bool IsNonZero() const { return Y*Y+X*X+(Z*Z) > DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO; }
#elif defined(FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ)
    // Original donor01B4F100: Y square, X square, Z square, then the threshold.
    FABLE_VECTOR_INLINE bool IsNonZero()const{return Y*Y+X*X+Z*Z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
    // (same opt-in, see Normalise above)
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z+Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#else
#if defined(FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX) && defined(FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX_SQUARED)
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=Z*Z;squared+=Y*Y;squared+=X*X;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#elif defined(FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX) && defined(FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX_PAREN)
    FABLE_VECTOR_INLINE bool IsNonZero()const{return (Z*Z+Y*Y+X*X)>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#elif defined(FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX)
    // Original IsNonZero call FableWin02b9fe76; native009420cf..dd squares Z,Y,X.
    FABLE_VECTOR_INLINE bool IsNonZero()const{return Z*Z+Y*Y+X*X>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#elif defined(FABLE_RETAIL_C3DVECTOR_ISNONZERO_YZX_BRACKET)
    // Bracket (B24A6, row 0x00975BB0): the Z square alone grouped. Retail 0x0097614F..0x0097618B squares Z, Y, X once (shared with the
    // inlined Normalise) and sums (Y*Y+Z*Z)+X*X twice; flat, (Y*Y+Z*Z) and (Y*Y)/(X*X) placements fail. FableWin 0x01B4F100
    // ?IsNonZero@C3DVector@@QBE_NXZ is 31i/91b, this text SAME (the bracket emits no code). Body only: default path unchanged.
    FABLE_VECTOR_INLINE bool IsNonZero()const{return Y*Y+(Z*Z)+X*X>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#elif defined(FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ)
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
    // Original IsNonZero donor 0x01B4F100 reads Y, X, Z; native 0x007C1F58 preserves that order.
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=Y*Y;squared+=X*X;squared+=Z*Z;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#else
    FABLE_VECTOR_INLINE bool IsNonZero()const{return X*X+Y*Y+Z*Z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#endif
#else
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
    FABLE_VECTOR_INLINE bool IsNonZero()const{const float& x=X;const float& y=Y;const float& z=Z;return y*y+x*x+z*z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#else
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z;squared+=Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#endif
#endif
#endif
#endif
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#ifndef FABLE_RETAIL_C3DVECTOR
    C3DVector& Normalise();
#endif
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
#if defined(FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES) && !defined(FABLE_RETAIL_C3DVECTOR)
    // (FABLE_RETAIL_C3DVECTOR declares the same member in its own ROTATE_ANGLES block above.)
    // Retail opt-in (w22 agent2022, row 0x007D3820 CTCTavernGameCoinGolf::ProcessInput, whose TU also holds GetCamera 0x007D3560):
    // Ego_r.pdb / FableWin.pdb ?Rotate@C3DVector@@QAEXMM@Z, out of line in retail at 0x00A14260 (the bound spelling; GetCamera calls
    // it twice). Declaration only.
    void Rotate(float angleXY,float angleYZ);                            // 0x00A14260
#endif
};

#ifdef FABLE_RETAIL_C3DVECTOR_PACKED
#pragma pack(pop)
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_BODY_PASS23
#include "engine/CodaEnvironment.h"
inline _CVECTOR C3DVector::Copy()const{_CVECTOR result;result.x=X;result.y=Y;result.z=Z;return result;}
#endif
#undef FABLE_VECTOR_INLINE
#ifdef FABLE_RETAIL_CPHYSICSMESH_SWEEP_TRIANGLE_PASS30
// FableWin 01D9E1B0 and the point-collision sibling sum X, Y, then Z into a local.
inline float FABLE_FASTCALL GFDotProduct(const C3DVector& a,const C3DVector& b) {
    float result=a.X*b.X+a.Y*b.Y+a.Z*b.Z;
    return result;
}
#endif

#ifdef FABLE_RETAIL_CUTSCENE_VECTOR3_GROW_EXTERNAL_PASS30
namespace std {
template<> void vector<C3DVector>::_M_insert_overflow(iterator position, const C3DVector& x, const __false_type&, size_type fillLength, bool atEnd);
}
#endif
#ifdef FABLE_RETAIL_CUTSCENE_VECTOR3_APPEND_PASS30
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
namespace std {
template<> inline void vector<C3DVector>::push_back(const C3DVector& x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, x);
        ++this->_M_finish;
    } else {
        _M_insert_overflow(this->_M_finish, x, _IsPODType(), 1, true);
    }
}
}
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#endif

#endif
