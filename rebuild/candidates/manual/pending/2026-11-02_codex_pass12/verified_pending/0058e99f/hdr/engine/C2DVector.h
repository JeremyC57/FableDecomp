#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#endif
// Ego_r original class 0x9ebe: public float X+0,Y+4, sizeof8.
// Native CTCPhysicsBase::GetOffsetVector slot28 at006b13d0 writes both words.
#ifdef FABLE_RETAIL_SUBTITLE
extern const float DELTA_NOTIONAL_ZERO;
#endif
#if defined(FABLE_RETAIL_C2DVECTOR_MASS) || defined(FABLE_RETAIL_STIPPLE)
// Complete original eight-byte vector; Ego_r TPI9ebe plus native Mass fields.
class C2DVector {
public:
    float X,Y;
    C2DVector() {}
#ifdef FABLE_RETAIL_TARGET_RANDOM_WALK
    C2DVector(float x,float y){Initialise(x,y);}
#else
    C2DVector(float x,float y):X(x),Y(y){}
#endif
    C2DVector(const C2DVector& p):X(p.X),Y(p.Y){}
    C2DVector operator+(const C2DVector& vector) const {return C2DVector(X+vector.X,Y+vector.Y);}
    C2DVector operator-(const C2DVector& vector) const {return C2DVector(X-vector.X,Y-vector.Y);}
    const C2DVector operator-() const {return C2DVector(-X,-Y);}
    const C2DVector operator*(float scalar) const {return C2DVector(X*scalar,Y*scalar);}
    const C2DVector& operator+=(const C2DVector& vector) {X+=vector.X;Y+=vector.Y;return *this;}
    float Dot(const C2DVector& vector) const {return X*vector.X+Y*vector.Y;}
#ifdef FABLE_RETAIL_STIPPLE
    float GetSquaredMagnitude()const{return (Y*Y+X*X);}
#endif
#ifdef FABLE_RETAIL_TARGET_RANDOM_WALK
    void Initialise(float x,float y){X=x;Y=y;}
#endif
};
#else
#if defined(FABLE_RETAIL_C2DVECTOR_UI) || defined(FABLE_RETAIL_SUBTITLE)
// Retail opt-in (Codex pass11): original lib_coord.hpp100..103,314..322;
// user copy ctor and arithmetic confirmed by UI hidden-result call sites.
class C2DVector {
public:
    float X,Y;
#ifdef FABLE_RETAIL_SUBTITLE
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
    inline bool IsNonZero() const { return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<Y*Y+X*X; }
#endif
    C2DVector() { }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    C2DVector(const C3DVector& p):X(p.X),Y(p.Y){}
    C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}
#endif
#ifdef FABLE_RETAIL_NAV_OUTPUT
    bool operator==(const C2DVector&)const;
#endif
#ifdef FABLE_RETAIL_NSHAPE
    // Original public conversion: FableWin ctor38bytes, vect parameter; native line endpoints read XY.
    C2DVector(const C3DVector& vect) : X(vect.X),Y(vect.Y){}
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
#ifdef FABLE_RETAIL_STATBOX
    const C2DVector& operator+=(const C2DVector& vector){X+=vector.X;Y+=vector.Y;return *this;}
    C2DVector ToC2DCoordF()const{return C2DVector(X,Y);}
#endif
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#else
class C2DVector { public: float X,Y; };
#endif
#endif
#endif
