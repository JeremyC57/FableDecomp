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
#ifdef FABLE_RETAIL_C3DVECTOR
    // Retail branch (chunk 732, row 0x006AABE0 SetActionForMelee): the sums are running statements. VC7.1 reassociates a one-line
    // `X*X+Z*Z+Y*Y` and loads the components in its own order (Z, Y, X on a stack local); retail loads them in source order X, Z, Y
    // (and X, Y for the 2D form, 0x006AACCF / 0x006AADB1 / 0x006AAE00 / 0x006AAF97), which only the statement form reproduces.
    // It also keeps the sum a unit where an inlined caller adds it to a running total (0x00AE0710 CalculateAveragePatchSize, the
    // reason for the earlier parenthesised body; rechecked, as are every FABLE_RETAIL_C3DVECTOR includer).
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Z*Z+Y*Y);}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{float squared=X*X;squared+=Y*Y;return squared;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
#endif
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#ifdef FABLE_RETAIL_C3DVECTOR
    // Retail branch (pass 12 header wave, 0x00AC2BE0 PartitionSpace): inline in retail; FableWin.pdb keeps out-of-line debug
    // copies ?LimitLower@C3DVector@@QAEXABV1@@Z 0x02E345C0 / ?LimitUpper@C3DVector@@QAEXABV1@@Z 0x02E34650, retail expands them
    // (0x00AC2BE0's per-axis compare-and-store runs on the triangle min/max boxes).
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    // Codex pass 13 (row 0x00EC5DE0): ?GetAngleXY@C3DVector@@QBEMXZ (FableWin 0x01ACF1E0 debug copy; no out-of-line copy in retail,
    // the row's TU expands it). Declaration only: the body is the row's (GFArcTan / GET_ROUNDED_ANGLE live in that TU).
    float GetAngleXY() const;
    // Chunk 732 (row 0x006AABE0 CThingPlayerCreature::SetActionForMelee): Ego_r.pdb ?RotateXY@C3DVector@@QAEXM@Z (0x00B81930),
    // out of line in retail at 0x00A14040 (395 bytes, the sin/cos rotation about Z). A declaration only.
    void RotateXY(float angle);                                          // 0x00A14040
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif
