// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H
#include "fable_compiler.h"
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
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
#include "engine/GFAngles.h"
#endif
#if (defined(FABLE_RETAIL_UPDATE_NEAREST_INTERESTING) || defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D)) && !defined(FABLE_RETAIL_CTCTARGETINGAI_ATTACK)
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
#ifdef FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
// Retail opt-in (chunk Q8, row 0x006A4FB0 CThingPhysical::GetFirstCollision): the same body with the Newton step fully grouped,
// `((3.0f-((r*r)*value))*r)*0.5f`. Same instructions; the three extra no-code PAREN tuples move VC7.1's scheduling-region cut so
// that the epilogue pops interleave with the final Normalise as retail does (0x006A51F1 `pop edi` ahead of `fmul [eax]`, `pop
// esi / pop ebp / pop ebx` ahead of its `fstp`). Body only: default path unchanged.
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return ((3.0f-((r*r)*value))*r)*0.5f;}
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
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastOneOverSqrt(float n){TFloatIntUnion seed;seed.Int=(((0x17c-((((TFloatIntUnion*)&n)->Int>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(((TFloatIntUnion*)&n)->Int>>17)&0x7f]<<15);float r=seed.Float;r=(3.0f-r*r*n)*r*0.5f;return r;}
#else
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-r*r*value)*r*0.5f;}
#endif
#endif
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
inline float FABLE_FASTCALL GFSqr(float n){return n*n;}
#else
inline float FABLE_FASTCALL GFSqr(float value){return value*value;}
#endif
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
#ifdef FABLE_RETAIL_C3DVECTOR_TO2D
    C2DVector ToC2DVector()const{return C2DVector(X,Y);}
#endif
#ifdef FABLE_RETAIL_CPARTICLE_SPRITE
    // Original PDB signature and donor01D09580: this + scaled displacement.
    const C3DVector Interpolate(const C3DVector& vector,float interp)const{return *this+(vector-*this)*interp;}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_CLEAR
    // Retail opt-in (Codex pass21): original01D8BC00 Z/Y/X zero stores;
    // called by original particle ClearAcceleration02F41010.
    void Clear() { X=Y=Z=0.0f; }
#endif
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
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
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
#endif
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
#ifdef FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector((X-other.X),(Y-other.Y),Z-other.Z);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP
    // Original addition signature and scalar components, donor018C0B60.
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector((X+other.X),(Y+other.Y),Z+other.Z);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
    // Retail opt-in (Codex 2026-11-10,007bc6fa): grouping the X product adds one no-code
    // scheduler tuple. The81-tuple region boundary then places the physics reload at007bc824
    // before the three x87 stack pops, matching the original shot TU. Arithmetic is unchanged.
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector((X*scale),Y*scale,Z*scale);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR
    // Chunk 734 (row 0x004A1CB0): FableWin.pdb ??GC3DVector@@QBE?BV0@XZ (unary minus; no out-of-line copy in retail).
    FABLE_VECTOR_INLINE const C3DVector operator-()const{return C3DVector(-X,-Y,-Z);}
    // FableWin.pdb ??TC3DVector@@QBE?BV0@ABV0@@Z (cross product; debug copy 0x01B40A20 right after GetRight 0x01B409F0, expanded in 0x004A1CB0).
#ifdef FABLE_RETAIL_C3DVECTOR_CROSS_XY_GROUP
    // Retail opt-in (Codex pass18): grouping the first two cross expressions
    // preserves the native00a42170 copy-load schedule without extra instructions.
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),X*other.Y-Y*other.X);}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_X_GROUP)
    // Retail opt-in (Codex pass 17): preserve the first cross-product expression as a unit.
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#endif
#endif
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
#ifdef FABLE_RETAIL_C3DVECTOR
    // Retail branch: the body is one parenthesised sum. VC7.1 keeps a parenthesised sum as a unit where an inlined caller adds
    // the result into a running float total (0x00AE0710 CalculateAveragePatchSize: `fmul; faddp st(1); fmul; faddp st(1);
    // faddp st(3)`); the unparenthesised default lets it reassociate each product into the total (identical bytes everywhere
    // else). The operand order inside the parentheses does not change the bytes there.
#ifdef FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): the same parenthesised one-line sum in X, Y, Z
    // order. There the inlined Normalise loads Z, Y, X (0x0079AE93 fld [esp+24h], 0x0079AEA0 fld [esp+20h], 0x0079AEAA fld
    // [esp+1Ch]); the X, Z, Y spelling loads Y, Z, X in that TU. Body only: no layout change, default path unchanged.
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
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
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{float squared=X*X;squared+=Y*Y;return squared;}
#else
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
#endif
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_DOT_GROUPED
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{float dot=(Y*other.Y);dot+=Z*other.Z;dot+=X*other.X;return dot;}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_XYZ
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{float dot=X*other.X;dot+=Y*other.Y;dot+=Z*other.Z;return dot;}
#else
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+(Y*other.Y+Z*other.Z);}
#else
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
#endif
#endif
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE)
    // Retail opt-in (chunk 1023, row 0x00C1C460, the libpk listener-space update): the audio library's TUs call the vector members
    // out of line -- FableWin.pdb ?Cross@C3DVector@@QBE?BV1@ABV1@@Z at 0x00C25860 (landed), the dot product 0x00C256A0 (landed
    // descriptive DotProduct) and the field-by-field copy 0x00C1BFF0 (landed descriptive Copy). Declarations only.
    const C3DVector Cross(const C3DVector& other) const;                 // 0x00C25860
    float DotProduct(const C3DVector& other) const;                      // 0x00C256A0
    C3DVector Copy() const;                                              // 0x00C1BFF0
#elif defined(FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION)
    // Native007c14a3..14fb: retain the two component subtractions as source expression units.
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),(X*other.Y-Y*other.X));}
#elif defined(FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP)
    // Cross expression grouping retains native00697d79..b2 scheduling.
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector((Y*other.Z-Z*other.Y),Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#elif defined(FABLE_RETAIL_C3DVECTOR_CYLINDER_CROSS_GROUPED)
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,(Z*other.X-X*other.Z),X*other.Y-Y*other.X);}
#else
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
    FABLE_VECTOR_INLINE const C3DVector& operator-=(const C3DVector& vector){X-=vector.X;Y-=vector.Y;Z-=vector.Z;return *this;}
#endif
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
#ifdef FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
    // Original FableWin01d99fb0 and both PDB signatures; named squared sum.
    FABLE_VECTOR_INLINE float GetAccurateMagnitude()const {float magnitude=X*X+Y*Y+Z*Z;return (float)sqrt(magnitude);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
    FABLE_VECTOR_INLINE float GetMagnitude()const{return GFRoot(X*X+Y*Y+Z*Z);}
#else
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
#if defined(FABLE_RETAIL_UPDATE_NEAREST_INTERESTING) && !defined(FABLE_RETAIL_CTCTARGETINGAI_ATTACK)
    // Original FableWin01d9faa0 GetMagnitude2D, calls single-precision sqrt helper.
    inline float GetMagnitude2D()const {float squared=X*X;squared+=Y*Y;return GFFastSqrt(squared);}
#endif
#if defined(FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D) && !defined(FABLE_RETAIL_UPDATE_NEAREST_INTERESTING) && !defined(FABLE_RETAIL_CTCTARGETINGAI_ATTACK)
    // Retail opt-in (chunk Q9, row 0x0092ACE0 CAIStateGroup_ApprenticeHero::GoToLibrary): the same FableWin.pdb
    // ?GetMagnitude2D@C3DVector@@QBEMXZ body as the FABLE_RETAIL_UPDATE_NEAREST_INTERESTING branch above, without that opt-in's
    // CThingCreatureBase / CTCTargetingBase / C2DBoxI changes. Body only: default path unchanged.
    inline float GetMagnitude2D()const {return GFFastSqrt(X*X+Y*Y);}
#endif
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
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
    FABLE_VECTOR_INLINE const C3DVector& InitialiseFromAngles(float angleXY,float angleYZ,float length) {
        float cosYZ=(float)cos(angleYZ*(double)6.2831855f);
        X=(float)(sin(angleXY*(double)6.2831855f)*(cosYZ*length));
        Y=(float)(cos(angleXY*(double)6.2831855f)*(cosYZ*length));
        Z=(float)(sin(angleYZ*(double)6.2831855f)*length);
        return *this;
    }
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
#ifdef FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
    // Retail opt-in (chunk 1023, row 0x00B67270 CEngineLandscapeRenderer::BuildBlendingTables): Ego_r.pdb / FableWin.pdb
    // ?AccurateNormalise@C3DVector@@QAEAAV1@XZ, out of line in retail at 0x00A14440 (the landed bound spelling; called with ecx = the
    // vector at 0x00B673A9). Declaration only.
    C3DVector& AccurateNormalise();                                      // 0x00A14440
#endif
    // Chunk 739 (row 0x00B0CAD0): FableWin.pdb ?GetRandomUnitVector@C3DVector@@SI?AV1@PAK@Z (static __fastcall, out of line at 0x00A145C0,
    // landed) and ?Normalise@C3DVector@@QAEAAV1@XZ (the fast normalise: GFFastOneOverSqrt of GetSquaredMagnitude, expanded in retail).
    static C3DVector __fastcall GetRandomUnitVector(unsigned long* seed);  // 0x00A145C0
#if defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS)
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
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
    // (same opt-in, see Normalise above)
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z+Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#else
#ifdef FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
    // Original IsNonZero call FableWin02b9fe76; native009420cf..dd squares Z,Y,X.
    FABLE_VECTOR_INLINE bool IsNonZero()const{return Z*Z+Y*Y+X*X>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
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
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#ifndef FABLE_RETAIL_C3DVECTOR
    C3DVector& Normalise();
#endif
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#ifdef FABLE_RETAIL_C3DVECTOR_PACKED
#pragma pack(pop)
#endif
#undef FABLE_VECTOR_INLINE
#endif
