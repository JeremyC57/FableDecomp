#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H
// Ego_r original class 0x9ebe: public float X+0,Y+4, sizeof8.
// Native CTCPhysicsBase::GetOffsetVector slot28 at006b13d0 writes both words.
// Retail opt-in (Codex pass11): original lib_coord.hpp100..103,314..322;
// user copy ctor and arithmetic confirmed by UI hidden-result call sites.
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#endif
