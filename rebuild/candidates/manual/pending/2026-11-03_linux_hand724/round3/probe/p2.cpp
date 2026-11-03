#include "engine/GFRoundFloatNear.h"
struct X { unsigned long a:11; unsigned long b:14; };
float g1, g2, g3, g4, g5, g6;
void f1(X& x) { g1 = GFRoundFloatNear(x.a * (1.0f / 2047.0f) * (20.0f - 0.0f) + 0.0f, 2); }
void f2(X& x) { g2 = GFRoundFloatNear((x.a * (1.0f / 2047.0f)) * (20.0f - 0.0f) + 0.0f, 2); }
void f3(X& x) { g3 = GFRoundFloatNear(20.0f * (x.a / 2047.0f), 2); }
void f4(X& x) { g4 = x.a * (1.0f / 2047.0f) * (20.0f - 0.0f) + 0.0f; }
void f5(X& x) { g5 = (x.a * (1.0f / 2047.0f)) * 20.0f; }
void f6(X& x) { g6 = x.a * (1.0f / 2047.0f) * 20.0f; }
