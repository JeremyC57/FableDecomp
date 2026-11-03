#include "engine/GFRoundFloatNear.h"
struct X { unsigned long a:11; unsigned long b:14; unsigned long c:7; void f1(float); void f2(float); void f3(float); };
float gf; unsigned long gu;
void T(float*);
void X::f1(float v) { T(&v); c = GFFloatToLongNear(GFRoundFloatNear(v, 2) * 127.0f); gu = b; }
void X::f2(float v) { T(&v); c = GFFloatToLongNear(GFRoundFloatNear(v, 2) * 127.0f); float w = b * (1.0f/16383.0f); T(&w); }
void X::f3(float v) { T(&v); c = GFFloatToLongNear(v * 127.0f); float w = b * (1.0f/16383.0f); T(&w); }
