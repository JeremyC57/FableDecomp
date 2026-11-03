#ifndef FABLE_ENGINE_C2DEXTENTSF_H
#define FABLE_ENGINE_C2DEXTENTSF_H
// Layout: original public X+0,Y+4, native hidden result writes both floats.
class C2DExtentsF { public: float X,Y; C2DExtentsF(){} C2DExtentsF(float x,float y):X(x),Y(y){} C2DExtentsF(const C2DExtentsF& p):X(p.X),Y(p.Y){} };
#endif
