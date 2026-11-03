#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H
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
#include "engine/C2DCoordI.h"
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE
// Retail opt-in (2026-11-03, chunk 720 row 0x00659800 CTCVillage::CalculateSecuritySweepTasks): the fast inverse square root
// and the members retail expands from it. Ego_r.pdb public ?GFastOneOverSqrtTable@@3PAKA (0x013CE558, 128 mantissas) and
// FableWin.pdb ?GFFastOneOverSqrt@@YIMM@Z (__fastcall, out of line only in the debug build): exponent (0x17C - e) / 2, the
// table mantissa, one Newton step (3 - x r r) r / 2 -- the body the landed rows 0x007AF0F0 / 0x007A6E20 spell. Retail expands
// it in place (0x00659A42..0x00659A99). Declarations and inline bodies only: no layout change.
extern unsigned long GFastOneOverSqrtTable[128];                         // 0x013CE558
__forceinline float __fastcall GFFastOneOverSqrt(float value)
{
    unsigned long bits = *reinterpret_cast<unsigned long*>(&value);
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *reinterpret_cast<float*>(&estimate);
    return (3.0f - r * r * value) * r * 0.5f;
}
#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
    C3DVector& AccurateNormalise();
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE
    // FableWin.pdb ?Normalise@C3DVector@@QAEAAV1@XZ / ?GetMagnitude2D@C3DVector@@QBEMXZ (inline in retail) and
    // ?RotateXY@C3DVector@@QAEXM@Z (Ego_r 0x00B81930 / FableWin; out of line in retail at 0x00A14040).
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(X*X+Y*Y+Z*Z);X*=factor;Y*=factor;Z*=factor;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude2D()const{return (float)sqrt(GetSquaredMagnitude2D());}
    void RotateXY(float angle);                                          // 0x00A14040
#endif
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif
