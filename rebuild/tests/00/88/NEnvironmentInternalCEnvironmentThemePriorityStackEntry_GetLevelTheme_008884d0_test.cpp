#ifndef _MSC_VER
#ifndef _GLIBCXX_INCLUDE_NEXT_C_HEADERS
#define _GLIBCXX_INCLUDE_NEXT_C_HEADERS
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_WEIGHTED
#define FABLE_RETAIL_BLENDED_WEIGHTED
#define FABLE_RETAIL_CARRAY

#ifndef FABLE_FIXTURE_DE0EC8C259647D2B
#define FABLE_FIXTURE_DE0EC8C259647D2B
#ifndef FABLE_CENVIRONMENTTHEMEPRIORITYSTACKENTRY_H
#define FABLE_CENVIRONMENTTHEMEPRIORITYSTACKENTRY_H

#ifndef FABLE_FIXTURE_A415B5D708E2189B
#define FABLE_FIXTURE_A415B5D708E2189B
#ifndef FABLE_CENVIRONMENTTHEMERESOURCEPALLETTE_H
#define FABLE_CENVIRONMENTTHEMERESOURCEPALLETTE_H

#ifndef FABLE_FIXTURE_CCE2FE7D5605B98F
#define FABLE_FIXTURE_CCE2FE7D5605B98F
#ifndef FABLE_CBLENDEDENVIRONMENTTHEME_H
#define FABLE_CBLENDEDENVIRONMENTTHEME_H

#ifndef FABLE_FIXTURE_DEE487FEFA03F09F
#define FABLE_FIXTURE_DEE487FEFA03F09F
#ifndef FABLE_ENGINE_CRGBFLOATCOLOUR_H
#define FABLE_ENGINE_CRGBFLOATCOLOUR_H

#ifndef FABLE_FIXTURE_785DB80B2874F35F
#define FABLE_FIXTURE_785DB80B2874F35F







#ifndef FABLE_COMPILER_H
#define FABLE_COMPILER_H

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#else
#define FABLE_FORCEINLINE inline
#endif

#endif 

#endif

#ifndef FABLE_FIXTURE_3AC84EBD4026C558
#define FABLE_FIXTURE_3AC84EBD4026C558

#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#ifndef _MSC_VER
#define __declspec(x)
#endif

#endif

#ifndef FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#endif

#ifndef FABLE_FIXTURE_561BC30B935C2421
#define FABLE_FIXTURE_561BC30B935C2421
#ifndef FABLE_ENGINE_C4DVECTOR_H
#define FABLE_ENGINE_C4DVECTOR_H

#ifndef FABLE_FIXTURE_40623C510A02F26D
#define FABLE_FIXTURE_40623C510A02F26D
#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H

#if defined(_MSC_VER)
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#include <math.h>

#ifndef FABLE_FIXTURE_77D83767BDEA5F3F
#define FABLE_FIXTURE_77D83767BDEA5F3F
#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H

#ifdef FABLE_RETAIL_C2DCOORDI_TOFLOAT




class C2DVector;
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}
    C2DVector ToC2DCoordF() const;
};

#ifndef FABLE_FIXTURE_E1C74A2216515C37
#define FABLE_FIXTURE_E1C74A2216515C37
#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H

#ifdef FABLE_RETAIL_C2DVECTOR
#include <math.h>





extern const float DELTA_NOTIONAL_ZERO;                 
#pragma pack(push, 1)
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    
    
    
    FABLE_FORCEINLINE
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
#ifdef FABLE_RETAIL_C2DVECTOR_INITIALISE
    
    
    void Initialise(float x,float y) { X=x; Y=y; }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
    
    
    C2DVector(const class C3DVector& v);
#endif
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
    
    const C2DVector operator-() const { return C2DVector(-X,-Y); }
    const C2DVector operator*(float scalar) const { return C2DVector(X*scalar,Y*scalar); }
    const C2DVector& operator+=(const C2DVector& vector) { X+=vector.X; Y+=vector.Y; return *this; }
    
    
    const C2DVector operator/(float scalar) const { float inverse = 1.0f/scalar; return C2DVector(X*inverse,Y*inverse); }
    
    const C2DVector& operator*=(const C2DVector& vector) { X*=vector.X; Y*=vector.Y; return *this; }
    float Dot(const C2DVector& vector) const { return X*vector.X+Y*vector.Y; }
    
    
#ifdef FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
    
    
    
    float GetSquaredMagnitude() const { float squared=Y*Y; squared+=X*X; return squared; }
#else
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
#endif
    
    
    
    float GetMagnitude() const { return sqrtf(X*X+Y*Y); }
    
    
    
    
    
    
    FABLE_FORCEINLINE bool operator==(const C2DVector& vector) const
    {
        return fabs(X-vector.X)<=DELTA_NOTIONAL_ZERO && fabs(Y-vector.Y)<=DELTA_NOTIONAL_ZERO;
    }
#ifdef FABLE_RETAIL_C2DVECTOR_NORMALISE
    
    
    
    
    C2DVector& Normalise();                                                  
    
    const C2DVector& operator*=(float scalar) { X*=scalar; Y*=scalar; return *this; }
    bool IsNonZero() const { return GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO; }
#endif
};
#pragma pack(pop)
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D

inline C2DVector::C2DVector(const C3DVector& v) : X(v.X),Y(v.Y) { }
#endif
#else
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    
    
    
    FABLE_FORCEINLINE
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#endif
#endif

#endif

inline C2DVector C2DCoordI::ToC2DCoordF() const { return C2DVector((float)X,(float)Y); }
#else
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
#endif
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
#endif

#endif

#ifdef FABLE_RETAIL_C3DVECTOR
extern const float DELTA_NOTIONAL_ZERO;                                 



extern unsigned long GFastOneOverSqrtTable[128];                        
#ifdef FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN





FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-(r*r*value))*r*0.5f;}
#else
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-r*r*value)*r*0.5f;}
#endif
#endif
class C3DVector {
public:
    float X,Y,Z;
#ifdef FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
    
    bool operator!=(const C3DVector& v) const {
        return fabs(X-v.X)>DELTA_NOTIONAL_ZERO || fabs(Y-v.Y)>DELTA_NOTIONAL_ZERO || fabs(Z-v.Z)>DELTA_NOTIONAL_ZERO;
    }
#endif
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
#ifdef FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
    
    
    
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector((X*scale),Y*scale,Z*scale);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR
    
    FABLE_VECTOR_INLINE const C3DVector operator-()const{return C3DVector(-X,-Y,-Z);}
    
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#endif
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
#ifdef FABLE_RETAIL_C3DVECTOR
    
    
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
    
    
    
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Z*Z+Y*Y);}
#endif
    
    
    
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
    
    
    
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
    
    
    
    FABLE_VECTOR_INLINE float GetAngleXY() const {
        float angle=(float)atan2(X,Y)*0.15915494f;
        if(angle<0.0f || angle>=1.0f) {
            angle=(float)fmod((double)angle,1.0);
            if(angle<0.0f) angle+=1.0f;
        }
        return angle;
    }
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
    
    
    void RotateXY(float angle);                                          
    
    
    static C3DVector __fastcall GetRandomUnitVector(unsigned long* seed);  
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
    
    
    
    
    
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(X*X+Z*Z+Y*Y);X*=factor;Y*=factor;Z*=factor;return *this;}
#else
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(GetSquaredMagnitude());X*=factor;Y*=factor;Z*=factor;return *this;}
#endif
    
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
    
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z+Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#else
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z;squared+=Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#endif
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

#endif

#ifdef FABLE_RETAIL_C4DVECTOR
class CRGBFloatColour;
#endif
#ifdef FABLE_RETAIL_C4DVECTOR_RGBCOLOUR
class CRGBColour;
#endif
class C4DVector {
public:
    float X,Y,Z,W;
    C4DVector() { }
    C4DVector(float x,float y,float z,float w) : X(x),Y(y),Z(z),W(w) { }
    C4DVector(const C3DVector& v,float w) : X(v.X),Y(v.Y),Z(v.Z),W(w) { }
    void Initialise(const C3DVector& vect,float w) { X=vect.X; Y=vect.Y; Z=vect.Z; W=w; }
    C3DVector ToC3DVector() const { return C3DVector(X,Y,Z); }
#ifdef FABLE_RETAIL_C4DVECTOR
    
    
    
    
    inline C4DVector(const CRGBFloatColour& colour);
    void Initialise(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w; }
    const C4DVector operator*(float sc) const { return C4DVector(X*sc,Y*sc,Z*sc,W*sc); }
    
    
    
    
    const C4DVector operator-(const C4DVector& v) const { return C4DVector(X-v.X,Y-v.Y,Z-v.Z,W-v.W); }
    const C4DVector operator+(const C4DVector& v) const { return C4DVector(X+v.X,Y+v.Y,Z+v.Z,W+v.W); }
    FABLE_FORCEINLINE const C4DVector Interpolate(const C4DVector& v, float t) const { return (v - *this) * t + *this; }
#endif
#ifdef FABLE_RETAIL_C4DVECTOR_RGBCOLOUR
    
    
    C4DVector(const CRGBColour& colour);
#endif
};
#endif

#endif


#ifndef FABLE_FIXTURE_9A076E2F1BD8E830
#define FABLE_FIXTURE_9A076E2F1BD8E830
#ifndef FABLE_ENGINE_CRGBCOLOUR_H
#define FABLE_ENGINE_CRGBCOLOUR_H

class CRGBColour {
public:
    union {
        struct { unsigned char B,G,R,A; };
        unsigned long IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
#ifdef FABLE_RETAIL_CRGBCOLOUR
    
    
    
    
    void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
    CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
    
    
    CRGBColour(unsigned char r,unsigned char g,unsigned char b) { Initialise(r,g,b,255); }
#else
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
#endif
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
};
FABLE_STATIC_ASSERT(sizeof(CRGBColour)==4);
#endif

#endif

#ifdef FABLE_RETAIL_CRGBFLOATCOLOUR

#endif
class CRGBFloatColour {
public:
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    
    
    FABLE_FORCEINLINE CRGBFloatColour(const CRGBColour& colour)
        : R(colour.R * (1.0f / 255.0f)), G(colour.G * (1.0f / 255.0f)), B(colour.B * (1.0f / 255.0f)), A(colour.A * (1.0f / 255.0f)) { }
    void Initialise(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
    CRGBFloatColour& operator*=(const CRGBFloatColour& other) { R *= other.R; G *= other.G; B *= other.B; A *= other.A; return *this; }
#ifdef FABLE_RETAIL_CRGBFLOATCOLOUR
    
    
    
    
    
    FABLE_FORCEINLINE CRGBColour GetRGBColour() const
    {
        return CRGBColour((unsigned char)(R * 255.0f), (unsigned char)(G * 255.0f), (unsigned char)(B * 255.0f),
                          (unsigned char)(A * 255.0f));
    }
    
    
    
    
    
    
    
    
    CRGBFloatColour operator+(const CRGBFloatColour& c) const { return CRGBFloatColour(R + c.R, G + c.G, B + c.B, A + c.A); }
    CRGBFloatColour operator-(const CRGBFloatColour& c) const { return CRGBFloatColour(R - c.R, G - c.G, B - c.B, A - c.A); }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    FABLE_FORCEINLINE CRGBFloatColour Interpolate(const CRGBFloatColour& colour, float t) const { return *this * (1.0f - t) + colour * t; }
    FABLE_FORCEINLINE CRGBColour GetRGBColourClamped() const
    {
        long r = GFFloatToLongNear(R * 255.0f);
        long g = GFFloatToLongNear(G * 255.0f);
        long b = GFFloatToLongNear(B * 255.0f);
        long a = GFFloatToLongNear(A * 255.0f);
        if (r < 0) r = 0; else if (r > 255) r = 255;
        if (g < 0) g = 0; else if (g > 255) g = 255;
        if (b < 0) b = 0; else if (b > 255) b = 255;
        if (a < 0) a = 0; else if (a > 255) a = 255;
        return CRGBColour((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
    }
#endif
#ifdef FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
    
    
    
    
    
    
    
    FABLE_FORCEINLINE operator CRGBColour() const
    {
        return CRGBColour((unsigned char)(R * 255.0f), (unsigned char)(G * 255.0f), (unsigned char)(B * 255.0f),
                          (unsigned char)(A * 255.0f));
    }
#endif
#ifdef FABLE_RETAIL_CRGBFLOATCOLOUR_CLAMP
    
    
    
    FABLE_FORCEINLINE void Clamp()
    {
        if (R > 1.0f) R = 1.0f; else if (R < 0.0f) R = 0.0f;
        if (G > 1.0f) G = 1.0f; else if (G < 0.0f) G = 0.0f;
        if (B > 1.0f) B = 1.0f; else if (B < 0.0f) B = 0.0f;
        if (A > 1.0f) A = 1.0f; else if (A < 0.0f) A = 0.0f;
    }
#endif
#ifdef FABLE_RETAIL_CRGBFLOATCOLOUR_WEIGHTED
    
    CRGBFloatColour& operator+=(const CRGBFloatColour& colour) {
        R+=colour.R; G+=colour.G; B+=colour.B; A+=colour.A; return *this;
    }
#ifndef FABLE_RETAIL_CRGBFLOATCOLOUR
    CRGBFloatColour operator*(float value) const {
        return CRGBFloatColour(R*value,G*value,B*value,A*value);
    }
#endif
#endif
    float R;                                                             
    float G;                                                             
    float B;                                                             
    float A;                                                             
};
FABLE_STATIC_ASSERT(sizeof(CRGBFloatColour) == 0x10);
inline CRGBFloatColour operator*(const CRGBFloatColour& a, const CRGBFloatColour& b)
{
    return CRGBFloatColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A);
}

inline C4DVector::C4DVector(const CRGBFloatColour& colour) : X(colour.R), Y(colour.G), Z(colour.B), W(colour.A) { }
#endif 

#endif


#ifndef FABLE_FIXTURE_DB20F12087069B3B
#define FABLE_FIXTURE_DB20F12087069B3B
#ifndef FABLE_CIDRAWENVIRONMENTTEXTURESET_H
#define FABLE_CIDRAWENVIRONMENTTEXTURESET_H

#ifndef FABLE_FIXTURE_610F7D5A59F0A00F
#define FABLE_FIXTURE_610F7D5A59F0A00F
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

#ifndef FABLE_FIXTURE_ECC47EF7209CDC04
#define FABLE_FIXTURE_ECC47EF7209CDC04
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H





#include <stddef.h>
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE) || defined(FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE) || defined(FABLE_RETAIL_STLP_SORT_INTERNALS) || defined(FABLE_RETAIL_STLP_HEAP)
#ifdef _MSC_VER
#define FABLE_STLP_NOINLINE __declspec(noinline)
#define FABLE_STLP_FASTCALL __fastcall
#else
#define FABLE_STLP_NOINLINE
#define FABLE_STLP_FASTCALL
#endif
#endif
namespace std {




struct __true_type { __true_type() { } };
#ifdef FABLE_RETAIL_STLP_FALSE_TAG_POD


struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
template<class T> struct __type_traits {
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};
#define FABLE_STLP_SCALAR_TRAITS(T) template<> struct __type_traits<T> { \
    typedef __true_type has_trivial_default_constructor; typedef __true_type has_trivial_copy_constructor; \
    typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; typedef __true_type is_POD_type; };
FABLE_STLP_SCALAR_TRAITS(bool)
FABLE_STLP_SCALAR_TRAITS(char)
FABLE_STLP_SCALAR_TRAITS(signed char)
FABLE_STLP_SCALAR_TRAITS(unsigned char)
FABLE_STLP_SCALAR_TRAITS(short)
FABLE_STLP_SCALAR_TRAITS(unsigned short)
FABLE_STLP_SCALAR_TRAITS(int)
FABLE_STLP_SCALAR_TRAITS(unsigned int)
FABLE_STLP_SCALAR_TRAITS(long)
FABLE_STLP_SCALAR_TRAITS(unsigned long)
FABLE_STLP_SCALAR_TRAITS(__int64)
FABLE_STLP_SCALAR_TRAITS(unsigned __int64)
FABLE_STLP_SCALAR_TRAITS(float)
FABLE_STLP_SCALAR_TRAITS(double)
FABLE_STLP_SCALAR_TRAITS(long double)
#undef FABLE_STLP_SCALAR_TRAITS
template<class T> struct __type_traits<T*> {
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }

inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#ifdef FABLE_RETAIL_STLP_FILL_LOCAL_ITERATOR
template<class I,class T> inline void fill(I begin,I last,const T& x) { I first=begin; for(;first!=last;++first) *first=x; }
#else
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
#endif
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class I,class O,class D> inline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*) {
    for(D n=last-first;n>0;--n) { *result=*first;++first;++result; } return result;
}
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __true_type&) {
    return (Output)__copy_trivial(first,last,result);
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
#ifdef FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD


template<class R,class B,class D> inline B __copy_backward(R first,R last,B result,const random_access_iterator_tag&,D*) {
    for(D n=last-first;n>0;--n) *--result=*--last;
    return result;
}
template<class I,class O> inline O __copy_backward_ptrs(I first,I last,O result,const __false_type&) {
    return __copy_backward(first,last,result,random_access_iterator_tag(),(typename iterator_traits<I>::difference_type*)0);
}
#else
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif

template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#if defined(FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE) && defined(_MSC_VER)

template<class I> __declspec(noinline) void __destroy_aux(I first,I last,const __false_type&) {
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
#endif
    for(;first!=last;++first) _Destroy(&*first);
}
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}

#ifdef FABLE_STLP_OUTOFLINE_UNINIT_COPY



#pragma auto_inline(off)
template<class I,class O> __declspec(noinline) O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_UNINIT_OUTOFLINE)



#pragma auto_inline(off)
template<class I,class O> O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#pragma auto_inline(on)
#else
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#endif
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
#ifdef FABLE_STLP_OUTOFLINE_UNINIT_FILL_N




#pragma auto_inline(off)
template<class I,class N,class T> __declspec(noinline) I __fastcall __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_UNINIT_OUTOFLINE)
#pragma auto_inline(off)
template<class I,class N,class T> I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {   
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#pragma auto_inline(on)
#else
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __true_type&) {
    return std::fill_n(first,n,x);
}
template<class I,class N,class T> inline I uninitialized_fill_n(I first,N n,const T& x) {
    typedef typename iterator_traits<I>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,x,IsPOD());
}

template<class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template<class _Tp1> allocator(const allocator<_Tp1>&) { }
    ~allocator() { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
#if defined(FABLE_RETAIL_STLP_VECTOR_SWAP) || defined(FABLE_RETAIL_STLP_SORT_INTERNALS)




template<class _Tp> inline void swap(_Tp& a,_Tp& b) { _Tp tmp=a; a=b; b=tmp; }
#endif

template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=_M_end_of_storage.allocate(n);
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
    vector(size_type n,const _Tp& value,const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(n,a) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,value);
    }
#ifdef FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
    
    
    
    
    explicit vector(size_type n);
#else
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
#endif
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#ifdef FABLE_RETAIL_STLP_VECTOR_ASSIGN
    
    
    vector& operator=(const vector& x);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_C
    
    
    
    
    vector& operator=(const vector& x);
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data-this->_M_start); }
    template <class _ForwardIter> _Tp* _M_allocate_and_copy(size_type n,_ForwardIter first,_ForwardIter last) {
        _Tp* result=this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first,last,result,_IsPODType());
        return result;
    }
#endif
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
#ifdef FABLE_RETAIL_STLP_VECTOR_SWAP_OUTOFLINE
    
    
    
    
    void swap(vector& x);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_SWAP
    
    void swap(vector& x) { std::swap(this->_M_start,x._M_start); std::swap(this->_M_finish,x._M_finish); std::swap(this->_M_end_of_storage,x._M_end_of_storage); }
#endif
    _Tp& front() { return *this->_M_start; }
    _Tp& back() { return *(end()-1); }
    const _Tp& back() const { return *(end()-1); }
    void pop_back() { --this->_M_finish; _Destroy(this->_M_finish); }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
    
    
    
    
    void push_back(const _Tp& x);
#else
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#endif
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#ifdef FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
    
    
    
    
    iterator erase(iterator position);
#else
    iterator erase(iterator position) {
        if(position+1!=end()) __copy_ptrs(position+1,this->_M_finish,position,_IsPODType());
        --this->_M_finish;_Destroy(this->_M_finish);return position;
    }
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
    
    
    
    iterator erase(iterator first,iterator last);
#else
    iterator erase(iterator first,iterator last) {
        iterator result=__copy_ptrs(last,this->_M_finish,first,_IsPODType());
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
#endif
    void clear() { erase(begin(),end()); }
    
    
#ifdef FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
    
    
    
    
    
    
    __declspec(noinline) void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#else
    void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
    
    
    void resize(size_type n);
#else
    void resize(size_type n) { resize(n,_Tp()); }
#endif
    void _M_fill_insert(iterator position,size_type n,const _Tp& x);
#ifdef FABLE_RETAIL_STLP_VECTOR
    
    
    void reserve(size_type n);
#endif
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd);
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd);
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
};

#ifdef FABLE_RETAIL_STLP_VECTOR_C


template<class _Tp,class _Alloc> vector<_Tp,_Alloc>& vector<_Tp,_Alloc>::operator=(const vector<_Tp,_Alloc>& x) {
    if(&x!=this) {
        const size_type xlen=x.size();
        if(xlen>capacity()) {
            _Tp* tmp=_M_allocate_and_copy(xlen,(const _Tp*)x._M_start+0,(const _Tp*)x._M_finish+0);
            _M_clear();
            this->_M_start=tmp;
            this->_M_end_of_storage._M_data=this->_M_start+xlen;
        } else if(size()>=xlen) {
            _Tp* i=__copy_ptrs((const _Tp*)x._M_start+0,(const _Tp*)x._M_finish+0,(_Tp*)this->_M_start,_IsPODType());
            _Destroy(i,this->_M_finish);
        } else {
            __copy_ptrs((const _Tp*)x._M_start,(const _Tp*)x._M_start+size(),(_Tp*)this->_M_start,_IsPODType());
            __uninitialized_copy((const _Tp*)x._M_start+size(),(const _Tp*)x._M_finish+0,this->_M_finish,_IsPODType());
        }
        this->_M_finish=this->_M_start+xlen;
    }
    return *this;
}
#endif
#ifndef FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY

template<class _Tp,class _Alloc> void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
    newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
    if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
    else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
    if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
template<class _Tp,class _Alloc> void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    _Tp* newStart=this->_M_end_of_storage.allocate(len);
    _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_RESERVE





template<class _Tp,class _Alloc> __declspec(noinline) void vector<_Tp,_Alloc>::reserve(size_type n) {
    if(size_type(this->_M_end_of_storage._M_data-this->_M_start)<n) {
        const size_type old_size=size();
        _Tp* tmp;
        if(this->_M_start) {
            tmp=this->_M_end_of_storage.allocate(n);
            __uninitialized_copy((const _Tp*)this->_M_start,(const _Tp*)this->_M_finish,tmp,_IsPODType());
            _M_clear();
        } else {
            tmp=this->_M_end_of_storage.allocate(n);
        }
        _M_set(tmp,tmp+old_size,tmp+n);
    }
}
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR




template<class _Tp,class _Alloc> void vector<_Tp,_Alloc>::_M_fill_insert(iterator position,size_type n,const _Tp& x) {
    if(n!=0) {
        if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
            _Tp x_copy=x;
            const size_type elems_after=this->_M_finish-position;
            _Tp* old_finish=this->_M_finish;
            if(elems_after>n) {
                __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                this->_M_finish+=n;
                __copy_backward_ptrs(position,old_finish-n,old_finish,_IsPODType());
                fill(position,position+n,x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish,n-elems_after,x_copy);
                this->_M_finish+=n-elems_after;
                __uninitialized_copy(position,old_finish,this->_M_finish,_IsPODType());
                this->_M_finish+=elems_after;
                fill(position,old_finish,x_copy);
            }
        } else _M_insert_overflow(position,x,_IsPODType(),n,false);
    }
}
#endif

template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair():first(_T1()),second(_T2()){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
    template<class U1,class U2> pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
};
template<class _T1,class _T2> inline pair<_T1,_T2> __cdecl make_pair(const _T1& a,const _T2& b) { return pair<_T1,_T2>(a,b); }
#ifdef FABLE_RETAIL_STLP_MAKE_PAIR_ARRAYS




template<class _T1,class _T2,int _Sz> inline pair<_T1,_T2 const*> __cdecl make_pair(_T1 const& a,_T2 const (&b)[_Sz]) { return pair<_T1,_T2 const*>(a,static_cast<_T2 const*>(b)); }
template<class _T1,class _T2,int _Sz> inline pair<_T1 const*,_T2> __cdecl make_pair(_T1 const (&a)[_Sz],_T2 const& b) { return pair<_T1 const*,_T2>(static_cast<_T1 const*>(a),b); }
template<class _T1,class _T2,int _Sz1,int _Sz2> inline pair<_T1 const*,_T2 const*> __cdecl make_pair(_T1 const (&a)[_Sz1],_T2 const (&b)[_Sz2]) { return pair<_T1 const*,_T2 const*>(static_cast<_T1 const*>(a),static_cast<_T2 const*>(b)); }
#endif

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
#ifdef FABLE_RETAIL_STLP_LIST
    
    void _M_decr() { _M_node=_M_node->_M_prev; }
#endif
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
#if defined(FABLE_RETAIL_STLP_LIST_NOINLINE_NE) && defined(_MSC_VER)
    
    
    
    
    __declspec(noinline)
#endif
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#ifdef FABLE_RETAIL_STLP_LIST
    _List_iterator& operator--() { this->_M_decr(); return *this; }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_REVERSE
    
    
    _List_iterator operator--(int) { _List_iterator tmp=*this; this->_M_decr(); return tmp; }
#endif
};
#ifdef FABLE_RETAIL_STLP_LIST_REVERSE



template<class _Iterator> class reverse_iterator {
protected:
    _Iterator current;
public:
    typedef _Iterator iterator_type;
    typedef typename _Iterator::reference reference;
    typedef typename _Iterator::pointer pointer;
    reverse_iterator() { }
    explicit reverse_iterator(iterator_type x) : current(x) { }
    reverse_iterator(const reverse_iterator& x) : current(x.current) { }
    iterator_type base() const { return current; }
    reference operator*() const { _Iterator tmp=current; return *--tmp; }
    pointer operator->() const { return &(operator*()); }
    reverse_iterator& operator++() { --current; return *this; }
    reverse_iterator operator++(int) { reverse_iterator tmp=*this; --current; return tmp; }
    reverse_iterator& operator--() { ++current; return *this; }
    reverse_iterator operator--(int) { reverse_iterator tmp=*this; ++current; return tmp; }
    bool operator==(const reverse_iterator& y) const { return current==y.current; }
    bool operator!=(const reverse_iterator& y) const { return current!=y.current; }
};
#endif
#ifdef FABLE_RETAIL_STLP_ADVANCE



template<class _Tp,class _Traits> inline bidirectional_iterator_tag __ITERATOR_CATEGORY(const _List_iterator<_Tp,_Traits>&) { return bidirectional_iterator_tag(); }
template<class _BidirectionalIterator,class _Distance> inline void __advance(_BidirectionalIterator& i,_Distance n,const bidirectional_iterator_tag&) {
    if(n>0) while(n--) ++i; else while(n++) --i;
}
template<class _InputIterator,class _Distance> inline void advance(_InputIterator& i,_Distance n) { __advance(i,n,__ITERATOR_CATEGORY(i)); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE


template<class _InputIterator> inline ptrdiff_t __distance(const _InputIterator& first,const _InputIterator& last,const input_iterator_tag&) {
    ptrdiff_t n=0; _InputIterator it(first); while(it!=last) { ++it; ++n; } return n;
}
template<class _InputIterator> inline ptrdiff_t distance(const _InputIterator& first,const _InputIterator& last) {
    return __distance(first,last,input_iterator_tag());
}
#endif
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
    
    
    
    
    _List_base(const _Alloc&);
    ~_List_base();
#else
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
    
    
    
    
    void clear();
#else
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) { insert(begin(),x.begin(),x.end()); }
#ifdef FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
    
    
    
    FABLE_STLP_NOINLINE ~list() { }
#else
    ~list() { }
#endif
    _Alloc get_allocator() const { return _Alloc(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
#ifdef FABLE_RETAIL_STLP_LIST_REVERSE
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
#endif
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
    template<class Input> void insert(iterator position,Input first,Input last) { _M_insert_dispatch(position,first,last,__false_type()); }
    template<class Input> void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) { for(;first!=last;++first) insert(position,*first); }
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
#ifdef FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
    
    
    
    FABLE_STLP_NOINLINE iterator insert(iterator position,const _Tp& x) {
#else
    iterator insert(iterator position,const _Tp& x) {
#endif
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#ifdef FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT
    
    
    
    
    
    iterator insert(iterator position);
    void push_back() { insert(end()); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST
    
    
    
    
#ifdef FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
    
    
    
    size_t size() const { size_t result=distance(begin(),end()); return result; }
#else
    size_t size() const { ptrdiff_t n=0; for(const_iterator first=begin(),last=end();first!=last;++first) ++n; return n; }
#endif
    _Tp& front() { return *begin(); }
    void push_front(const _Tp& x) { insert(begin(),x); }
    
    
    _Tp& back() { iterator tmp=end(); return *(--tmp); }
    const _Tp& back() const { const_iterator tmp=end(); return *(--tmp); }
    
    
    iterator erase(iterator position) {
        _List_node_base* next_node=position._M_node->_M_next;
        _List_node_base* prev_node=position._M_node->_M_prev;
        _List_node<_Tp>* n=(_List_node<_Tp>*)position._M_node;
        prev_node->_M_next=next_node;
        next_node->_M_prev=prev_node;
        _Destroy(&n->_M_data);
        this->_M_node.deallocate(n,1);
        return iterator(next_node);
    }
    void pop_front() { erase(begin()); }
#ifdef FABLE_RETAIL_STLP_LIST_ERASE_RANGE
    
    
    
    iterator erase(iterator first,iterator last) {
        while(first!=last) erase(first++);
        return last;
    }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_POPBACK
    
    
    void pop_back() { iterator tmp=end(); erase(--tmp); }
#endif
#endif
#ifdef FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
    
    
    template<class _Predicate> void remove_if(_Predicate pred);
#endif
};

template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
#ifdef FABLE_RETAIL_STLP_BITSET_INDEX
    _WordT& _M_getword(size_t n) { return _M_w[_S_whichword(n)]; }
#endif
};
inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
#ifdef FABLE_RETAIL_STLP_BITSET_TEST_DECL
    
    
    
    bool test(size_t n) const;
#else
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
#endif
#ifdef FABLE_RETAIL_STLP_BITSET_INDEX
    
    
    
    
    
    struct reference {
        typedef _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> _Bitset_base;
        typedef bitset<_Nb> _Bitset;
        unsigned long* _M_wp;
        size_t _M_bpos;
        reference() {}
        reference(_Bitset& b,size_t pos) { _M_wp=&b._M_getword(pos); _M_bpos=_Bitset_base::_S_whichbit(pos); }
        ~reference() {}
        reference& operator=(bool x) { if(x) *_M_wp|=_Bitset_base::_S_maskbit(_M_bpos); else *_M_wp&=~_Bitset_base::_S_maskbit(_M_bpos); return *this; }
        reference& operator=(const reference& j) { if((*(j._M_wp)&_Bitset_base::_S_maskbit(j._M_bpos))) *_M_wp|=_Bitset_base::_S_maskbit(_M_bpos); else *_M_wp&=~_Bitset_base::_S_maskbit(_M_bpos); return *this; }
        bool operator~() const { return (*(_M_wp)&_Bitset_base::_S_maskbit(_M_bpos))==0; }
        operator bool() const { return (*(_M_wp)&_Bitset_base::_S_maskbit(_M_bpos))!=0; }
        reference& flip() { *_M_wp^=_Bitset_base::_S_maskbit(_M_bpos); return *this; }
    };
    reference operator[](size_t pos) { return reference(*this,pos); }
    bool operator[](size_t pos) const { return _Unchecked_test(pos); }
#endif
};

template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _Arg1,class _Arg2,class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template<class _Tp> struct less : public binary_function<_Tp,_Tp,bool> {
    bool operator()(const _Tp& x,const _Tp& y) const { return x<y; }
};
#ifdef FABLE_RETAIL_STLP_SORT_FASTCALL



template<class _RandomAccessIter,class _Compare> void __fastcall sort(_RandomAccessIter,_RandomAccessIter,_Compare);
#else
template<class _RandomAccessIter,class _Compare> void sort(_RandomAccessIter,_RandomAccessIter,_Compare);
#endif
#ifdef FABLE_RETAIL_STLP_SORT2



template<class _RandomAccessIter> void sort(_RandomAccessIter,_RandomAccessIter);
#endif
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
#ifdef FABLE_RETAIL_STLP_BACK_INSERTER




template<class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    typedef _Container container_type;
    explicit back_insert_iterator(_Container& x) : container(&x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& value) { container->push_back(value); return *this; }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
    back_insert_iterator<_Container> operator++(int) { return *this; }
};
template<class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& x) { return back_insert_iterator<_Container>(x); }
template<class _InputIter,class _OutputIter> _OutputIter __fastcall copy(_InputIter first,_InputIter last,_OutputIter result);
#endif
#ifdef FABLE_RETAIL_STLP_SORT_INTERNALS






template<class _ForwardIter1,class _ForwardIter2> inline void iter_swap(_ForwardIter1 a,_ForwardIter2 b) { swap(*a,*b); }
template<class _RandomAccessIter,class _BidirectionalIter,class _Distance>
FABLE_STLP_NOINLINE _BidirectionalIter FABLE_STLP_FASTCALL __copy_backward(_RandomAccessIter first,_RandomAccessIter last,
                                                                           _BidirectionalIter result,const random_access_iterator_tag&,_Distance*) {
    for(_Distance n=last-first;n>0;--n) *--result=*--last;
    return result;
}
template<class _BI1,class _BI2> inline _BI2 copy_backward(_BI1 first,_BI1 last,_BI2 result) {
    return __copy_backward(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
template<class _RandomAccessIter,class _Tp,class _Compare>
_RandomAccessIter FABLE_STLP_FASTCALL __unguarded_partition(_RandomAccessIter first,_RandomAccessIter last,_Tp pivot,_Compare comp) {
    for(;;) {
        while(comp(*first,pivot)) ++first;
        --last;
        while(comp(pivot,*last)) --last;
        if(!(first<last)) return first;
        iter_swap(first,last);
        ++first;
    }
}
template<class _RandomAccessIter,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __unguarded_linear_insert(_RandomAccessIter last,_Tp val,_Compare comp);
template<class _RandomAccessIter,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __linear_insert(_RandomAccessIter first,_RandomAccessIter last,_Tp val,_Compare comp) {
    if(comp(val,*first)) {
        copy_backward(first,last,last+1);
        *first=val;
    } else
        __unguarded_linear_insert(last,val,comp);
}
#endif
#ifdef FABLE_RETAIL_STLP_HEAP



template<class _RandomAccessIterator,class _Distance,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __adjust_heap(_RandomAccessIterator first,_Distance holeIndex,_Distance len,_Tp val,_Compare comp);
template<class _RandomAccessIterator,class _Tp,class _Compare,class _Distance>
void FABLE_STLP_FASTCALL __pop_heap(_RandomAccessIterator first,_RandomAccessIterator last,_RandomAccessIterator result,
                                    _Tp val,_Compare comp,_Distance*) {
    *result=*first;
    __adjust_heap(first,_Distance(0),_Distance(last-first),val,comp);
}
#endif
}
#endif

#endif

template<class T> class CArray : public std::vector<T> {
public:
    CArray() : std::vector<T>() { }
#ifdef FABLE_RETAIL_CARRAY
    
    
    explicit CArray(unsigned int n) : std::vector<T>(n) { }
#else
    explicit CArray(unsigned int n) : std::vector<T>(n,T()) { }
#endif
    CArray(unsigned int n,const T& x) : std::vector<T>(n,x) { }
#ifdef FABLE_RETAIL_CARRAY
    
    
    
    T& operator[](unsigned int i) { return *(this->begin() + i); }
    const T& operator[](unsigned int i) const { return *(this->begin() + i); }
#else
    T& operator[](unsigned int i) { return this->_M_start[i]; }
    const T& operator[](unsigned int i) const { return this->_M_start[i]; }
#endif
#ifdef FABLE_RETAIL_CARRAY_GETASCARRAY_START
    
    
    
    T* GetAsCArray() { return this->size()>0 ? this->_M_start : GetZeroSizeArray(); }
#else
    T* GetAsCArray() { return this->size()>0 ? &this->front() : GetZeroSizeArray(); }
#endif
    const T* GetAsCArray() const { return this->size()>0 ? this->_M_start : (const T*)this; }
#ifdef FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
    
    
    
    void DeallocateUnusedMemory();
#endif
#ifdef FABLE_RETAIL_CARRAY_PUSHBACK
    
    
    
    
    
    
    void push_back(const T& x) { std::vector<T>::push_back(x); }
    void push_back() { T element; push_back(element); }
#endif
private:
    T* GetZeroSizeArray() { return (T*)this; }
};
#endif

#endif

struct CIDrawEnvironmentTextureSet {
public:
    CArray<long> Textures;
    CArray<float> Blends;
};
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentTextureSet)==24);
#endif

#endif

class CEnvironmentDef;
class CEnvironmentThemeDaySetDef;
class CEnvironmentThemeDef;
namespace NEnvironmentInternal {
class CEnvironmentLookupTexture;
class CBlendedEnvironmentTheme {
public:
    CBlendedEnvironmentTheme();
    ~CBlendedEnvironmentTheme();
    void BuildFromSource(const CEnvironmentDef&,const CEnvironmentLookupTexture&,const CEnvironmentThemeDaySetDef&,const CEnvironmentThemeDef&,long);
    void BlendTextureSets(const CIDrawEnvironmentTextureSet&,const CIDrawEnvironmentTextureSet&,CIDrawEnvironmentTextureSet&,float) const;
    void Blend(CBlendedEnvironmentTheme&,const CBlendedEnvironmentTheme&,float) const;
    void CompactTextureSet(CIDrawEnvironmentTextureSet&,long) const;
    void SetAsWeightedBlendOfThemes(const CArray<CBlendedEnvironmentTheme>&,const CArray<float>&);
    float TimeOfDay;
    CRGBFloatColour Colours[19];
    float Scalers[30];
    CIDrawEnvironmentTextureSet TextureSets[3];
};
}
FABLE_STATIC_ASSERT(sizeof(NEnvironmentInternal::CBlendedEnvironmentTheme)==500);
#ifdef FABLE_RETAIL_BLENDED_WEIGHTED
void NEnvironmentInternal::CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes(const CArray<CBlendedEnvironmentTheme>& themes,const CArray<float>& weights) {
    if(weights.empty() || themes.empty()) return;
    for(long n=0;n<19;++n) {
        Colours[n].Initialise(0.0f,0.0f,0.0f,0.0f);
        for(long index=0;index<themes.size();++index)
            Colours[n]+=themes[index].Colours[n]*weights[index];
    }
    for(long n=0;n<30;++n) {
        Scalers[n]=0.0f;
        for(long index=0;index<themes.size();++index)
            Scalers[n]+=themes[index].Scalers[n]*weights[index];
    }
    for(long n=0;n<3;++n) {
        TextureSets[n].Textures.clear();
        TextureSets[n].Blends.clear();
        for(long index=0;index<themes.size();++index) {
            for(long ts=0;ts<themes[index].TextureSets[n].Textures.size();++ts) {
                TextureSets[n].Textures.push_back(themes[index].TextureSets[n].Textures[ts]);
                TextureSets[n].Blends.push_back(themes[index].TextureSets[n].Blends[ts]*weights[index]);
            }
        }
        CompactTextureSet(TextureSets[n],themes[0].TextureSets[n].Textures.size());
    }
}
#endif

#endif

#endif

class CGameDefinitionManager;
namespace NEnvironmentInternal {
class CEnvironmentThemeSetDay {
public:
    CArray<CBlendedEnvironmentTheme> Themes;
public:
    void Build(const CGameDefinitionManager&,const CEnvironmentDef&,const CEnvironmentLookupTexture&,const CEnvironmentThemeDaySetDef&);
    void GetBlendedTheme(CBlendedEnvironmentTheme&,float) const;
};
class CEnvironmentThemeResourcePallette {
public:
    CArray<CEnvironmentThemeSetDay> DaySets;
public:
    void Build(const CGameDefinitionManager&,const CEnvironmentDef&,const CEnvironmentLookupTexture&);
    long GetCount() const;
    void GetBlendedTheme(CBlendedEnvironmentTheme& result,long index,float time) const {
        DaySets[index].GetBlendedTheme(result,time);
    }
};
}
#endif

#endif

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
    struct CThemeWeight {
        long ThemeID;
        float Weight;
        bool LockWeight;
    };
    float BaseWeight;
    bool BaseWeightLocked;
    CArray<CThemeWeight> Themes;
    long ThemeTransitionTargetID;
    float ThemeTransitionInitialWeight;
    bool ThemeTransitionActive;
    double ThemeTransitionStartTime;
    float ThemeTransitionDuration;
public:
    void SetTheme(long);
    void RestoreDefaultTheme();
    void SetThemeWeight(long,float,bool);
    void SetDefaultThemeWeight(float,bool);
    void StartThemeTransition(long,float);
    void TransitionToDefaultTheme(float);
    float GetLevelThemeWeight() const;
    void BlendInLevel(CBlendedEnvironmentTheme&,const CBlendedEnvironmentTheme&,float,const CEnvironmentThemeResourcePallette&) const;
    void ProcessTransitions();
    void RebalanceWeights();
    float GetThemeWeight(long);
    void GetLevelTheme(CBlendedEnvironmentTheme&,float,const CEnvironmentThemeResourcePallette&);
};
}
FABLE_STATIC_ASSERT(sizeof(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry)==48);
#endif

#endif

void NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::GetLevelTheme(CBlendedEnvironmentTheme& result,float time_of_day,const CEnvironmentThemeResourcePallette& theme_palette) {
    if(Themes.size()==1) {
        theme_palette.GetBlendedTheme(result,Themes[0].ThemeID,time_of_day);
        return;
    }
    CArray<CBlendedEnvironmentTheme> themes;
    CArray<float> weights;
    themes.reserve(Themes.size());
    weights.reserve(Themes.size());
    for(long n=0;n<Themes.size();++n) {
        const unsigned int index=themes.size();
        themes.resize(index+1,CBlendedEnvironmentTheme());
        theme_palette.GetBlendedTheme(themes[index],Themes[n].ThemeID,time_of_day);
        weights.push_back(Themes[n].Weight);
    }
    result.SetAsWeightedBlendOfThemes(themes,weights);
}

using namespace NEnvironmentInternal;
static int cases_run=0, compact_calls=0, lookup_calls=0;
static const CEnvironmentThemeResourcePallette* active_palette;
static int active_count;
static float active_weights[4];
static void check(bool yes) { if(!yes) { fprintf(stderr,"check failed in case %d\n",cases_run); abort(); } }
// Controlled external constructor/destructor and lookup boundaries. No native-helper claim.
CBlendedEnvironmentTheme::CBlendedEnvironmentTheme() : TimeOfDay(-11.0f) {
 for(int i=0;i<19;++i) Colours[i].Initialise(-12,-13,-14,-15);
 for(int i=0;i<30;++i) Scalers[i]=-16;
}
CBlendedEnvironmentTheme::~CBlendedEnvironmentTheme() { }
void CEnvironmentThemeSetDay::GetBlendedTheme(CBlendedEnvironmentTheme& out,float time) const {
 ++lookup_calls; const CBlendedEnvironmentTheme& in=Themes[0]; out.TimeOfDay=time;
 for(int i=0;i<19;++i)out.Colours[i]=in.Colours[i];
 for(int i=0;i<30;++i)out.Scalers[i]=in.Scalers[i];
 for(int c=0;c<3;++c) {
  out.TextureSets[c].Textures.clear();out.TextureSets[c].Blends.clear();
  for(unsigned int j=0;j<in.TextureSets[c].Textures.size();++j) {
   out.TextureSets[c].Textures.push_back(in.TextureSets[c].Textures[j]);
   out.TextureSets[c].Blends.push_back(in.TextureSets[c].Blends[j]);
  }
 }
}
void CBlendedEnvironmentTheme::CompactTextureSet(CIDrawEnvironmentTextureSet& out,long first_count) const {
 int channel=compact_calls++; check(channel<3);
 check(first_count==(long)active_palette->DaySets[0].Themes[0].TextureSets[channel].Textures.size());
 unsigned int pos=0;
 for(int i=0;i<active_count;++i) {
  const CIDrawEnvironmentTextureSet& in=active_palette->DaySets[i].Themes[0].TextureSets[channel];
  for(unsigned int j=0;j<in.Textures.size();++j) {
   check(pos<out.Textures.size() && pos<out.Blends.size());
   check(out.Textures[pos]==in.Textures[j]);
   check(out.Blends[pos]==in.Blends[j]*active_weights[i]); ++pos;
  }
 }
 check(pos==out.Textures.size() && pos==out.Blends.size());
}
static void fill(CBlendedEnvironmentTheme& x,int id) {
 x.TimeOfDay=20+id;
 for(int n=0;n<19;++n) x.Colours[n].Initialise(float(id+n),float(2*id-n),float(id+2*n),float(id-n));
 for(int n=0;n<30;++n) x.Scalers[n]=float(id-n)*0.5f;
 for(int c=0;c<3;++c) for(int k=0;k<(id+c)%3;++k) {
  x.TextureSets[c].Textures.push_back(100*id+10*c+k);
  x.TextureSets[c].Blends.push_back(float(k+1)*0.25f);
 }
}
static void unchanged(const CBlendedEnvironmentTheme& x) {
 check(x.TimeOfDay==777);
 for(int n=0;n<19;++n) check(x.Colours[n].R==-12 && x.Colours[n].G==-13 && x.Colours[n].B==-14 && x.Colours[n].A==-15);
 for(int n=0;n<30;++n) check(x.Scalers[n]==-16);
 for(int c=0;c<3;++c) check(x.TextureSets[c].Textures.size()==1 && x.TextureSets[c].Textures[0]==-99 && x.TextureSets[c].Blends[0]==-2);
}
static void run(int count,int profile,float time) {
 ++cases_run; compact_calls=0;lookup_calls=0;
 CEnvironmentThemeResourcePallette palette;
 for(int i=0;i<4;++i) {
  CEnvironmentThemeSetDay day; CBlendedEnvironmentTheme theme;fill(theme,i+1);day.Themes.push_back(theme);palette.DaySets.push_back(day);
 }
 CEnvironmentThemePriorityStackEntry entry;
 const float choices[6]={0,0.25f,0.5f,1,-0.5f,2};
 for(int i=0;i<count;++i) {
  CEnvironmentThemePriorityStackEntry::CThemeWeight w;
  w.ThemeID=i;w.Weight=choices[(profile+i)%6];w.LockWeight=(i%2)!=0;
  active_weights[i]=w.Weight;entry.Themes.push_back(w);
 }
 active_palette=&palette;active_count=count;
 CBlendedEnvironmentTheme out;out.TimeOfDay=777;
 for(int c=0;c<3;++c) {out.TextureSets[c].Textures.push_back(-99);out.TextureSets[c].Blends.push_back(-2);}
 entry.GetLevelTheme(out,time,palette);
 check(lookup_calls==count);
 if(count==0) { unchanged(out);check(compact_calls==0);return; }
 check(compact_calls==(count==1?0:3));check(out.TimeOfDay==(count==1?time:777));
 for(int n=0;n<19;++n) {
  float r=0,g=0,b=0,a=0;
  for(int i=0;i<count;++i) {
   const CRGBFloatColour& in=palette.DaySets[i].Themes[0].Colours[n];float w=count==1?1:active_weights[i];
   r+=in.R*w;g+=in.G*w;b+=in.B*w;a+=in.A*w;
  }
  check(out.Colours[n].R==r && out.Colours[n].G==g && out.Colours[n].B==b && out.Colours[n].A==a);
 }
 for(int n=0;n<30;++n) {
  float v=0;for(int i=0;i<count;++i)v+=palette.DaySets[i].Themes[0].Scalers[n]*(count==1?1:active_weights[i]);
  check(out.Scalers[n]==v);
 }
 if(count==1)for(int c=0;c<3;++c) {
  const CIDrawEnvironmentTextureSet& in=palette.DaySets[0].Themes[0].TextureSets[c];
  check(out.TextureSets[c].Textures.size()==in.Textures.size());
  for(unsigned int j=0;j<in.Textures.size();++j)check(out.TextureSets[c].Textures[j]==in.Textures[j] && out.TextureSets[c].Blends[j]==in.Blends[j]);
 }
}
static void empty_guards() {
 for(int which=0;which<3;++which) {
  ++cases_run;compact_calls=0;CArray<CBlendedEnvironmentTheme> themes;CArray<float> weights;
  if(which==0)themes.push_back(CBlendedEnvironmentTheme());
  if(which==1)weights.push_back(1);
  CBlendedEnvironmentTheme out;out.TimeOfDay=777;
  for(int c=0;c<3;++c){out.TextureSets[c].Textures.push_back(-99);out.TextureSets[c].Blends.push_back(-2);}
  out.SetAsWeightedBlendOfThemes(themes,weights);unchanged(out);check(compact_calls==0);
 }
}
int main() {
 const float times[4]={-2,0,0.5f,24};
 for(int count=0;count<=4;++count)for(int profile=0;profile<6;++profile)for(int t=0;t<4;++t)run(count,profile,times[t]);
 empty_guards();check(cases_run==123);puts("008884d0_TEST PASS");return 0;
}