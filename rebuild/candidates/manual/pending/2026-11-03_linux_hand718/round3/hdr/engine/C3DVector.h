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
    // Retail opt-in (chunk 718, row 0x0088EDC0 CameraDoConversation): FableWin.pdb ?Normalise@C3DVector@@QAEAAV1@XZ (no Ego_r.pdb
    // copy: the release build expands it) -- the table-driven fast 1/sqrt (GFastOneOverSqrtTable 0x013CE558, exponent (380 - e) / 2,
    // one Newton step `(3 - r*r*lenSq) * r * 0.5`), as the landed C2DVector::Normalise 0x00A14540 and the expanded copies in
    // CTCAssassinRush::SetupFacingDirection 0x007A6E20; and FableWin.pdb ??ZC3DVector@@QAEABV0@ABV0@@Z (operator-=).
    FABLE_VECTOR_INLINE const C3DVector& operator-=(const C3DVector& v){X-=v.X;Y-=v.Y;Z-=v.Z;return *this;}
    FABLE_VECTOR_INLINE C3DVector& Normalise();
    void Rotate(float angleXY, float angleZ);                            // 0x00A14260 ?Rotate@C3DVector@@QAEXMM@Z (Ego_r 0x00B81BF0)
#endif
    void Rotate(const C3DVector&,float);
};
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE
extern unsigned long GFastOneOverSqrtTable[128];                      // 0x013CE558 ?GFastOneOverSqrtTable@@3PAKA (Ego_r public)
FABLE_VECTOR_INLINE C3DVector& C3DVector::Normalise()
{
    float lengthSquared = X * X + Y * Y + Z * Z;
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    r = (3.0f - r * r * lengthSquared) * r * 0.5f;
    X *= r;
    Y *= r;
    Z *= r;
    return *this;
}
#endif
#undef FABLE_VECTOR_INLINE
#endif
