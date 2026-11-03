#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H
#ifdef FABLE_RETAIL_PARTICLE_PROGRAM_TRANSFORM
#include <math.h>
#endif
// PDB C3DVector complete TPI 0x2783: public X/Y/Z at 0/4/8, size 12.
// Embedded alignment is under review; this header introduces no packing directive.
#if defined(_MSC_VER) && !defined(FABLE_RETAIL_TARGET_RANDOM_WALK) && !defined(FABLE_RETAIL_PARTICLE_PROGRAM_TRANSFORM)
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#if defined(FABLE_RETAIL_VECTOR_GEOMETRY) || defined(FABLE_RETAIL_ANIMATION_RETRY)
#include "engine/GFMaths.h"
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_VALIDPOSITIONS)
#include "engine/C2DCoordI.h"
#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
#ifdef FABLE_RETAIL_VECTOR_SPLINE_CTOR
    C3DVector(float x,float y,float z){X=x;Y=y;Z=z;}
#else
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    C3DVector(float x,float y,float z){Initialise(x,y,z);}
#else
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
#endif
#endif
#ifdef FABLE_RETAIL_VECTOR_COPY_INITIALISE
    C3DVector(const C3DVector& p){Initialise(p.X,p.Y,p.Z);}
#elif defined(FABLE_RETAIL_VECTOR_GEOMETRY) || defined(FABLE_RETAIL_VECTOR_SPLINE_COPY)
    C3DVector(const C3DVector& p){*this=p;}
#else
#ifdef FABLE_RETAIL_PRIMITIVE_BUMP
    C3DVector(const C3DVector& p):X(p.X),Y(p.Y),Z(p.Z){}
#else
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
#endif
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
#if defined(FABLE_RETAIL_PARTICLE_PATCH_SIZE)
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
#elif defined(FABLE_RETAIL_ANIMATION_SEQUENCE) || defined(FABLE_RETAIL_PARTICLE_PROGRAM_TRANSFORM)
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return Z*Z+Y*Y+X*X;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
#endif
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
#ifdef FABLE_RETAIL_PRIMITIVE_BUMP
    inline const float Dot(const C3DVector& vector)const{return X*vector.X+Y*vector.Y+Z*vector.Z;}
    inline const C3DVector Cross(const C3DVector& vector)const{return C3DVector(Y*vector.Z-Z*vector.Y,Z*vector.X-X*vector.Z,X*vector.Y-Y*vector.X);}
#else
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#endif
#ifdef FABLE_RETAIL_ANIMATION_RETRY
    // Original lib_coord.hpp416-418 and564-566, direct arithmetic bodies.
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(Y*Y+(X*X+Z*Z));}
    FABLE_VECTOR_INLINE bool IsNonZero()const{return Z*Z+Y*Y+X*X>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#endif
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
    FABLE_VECTOR_INLINE void Clear(){X=Y=Z=0.0f;}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
#ifdef FABLE_RETAIL_QUADRIC_VERTEX_COST_RETRY
    FABLE_VECTOR_INLINE bool IsNonZero()const{return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<X*X+(Y*Y+Z*Z);}
#else
    FABLE_VECTOR_INLINE bool IsNonZero()const{return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<X*X+Y*Y+Z*Z;}
#endif
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GetSquaredMagnitude());X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#endif
#ifdef FABLE_RETAIL_VECTOR_SPLINE
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_VALIDPOSITIONS)
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#endif
#ifdef FABLE_RETAIL_PARTICLE_PROGRAM_TRANSFORM
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif
