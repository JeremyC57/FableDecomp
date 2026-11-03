#include "engine/GFFloatToLongNear.h"
struct X { unsigned long a:11; unsigned long b:14; unsigned long c:7; };
long gl; float gf; unsigned long gu;
void f1(X& x) { x.c = gl; gu = x.b; }
void f2(X& x) { x.c = GFFloatToLongNear(gf); gu = x.b; }
void f3(X& x) { long v = GFFloatToLongNear(gf); x.c = v; gu = x.b; }
void f4(X& x) { x.c = GFFloatToLongNear(gf) & 0x7f; gu = x.b; }
void f5(X& x) { x.c = (unsigned long)gl; gu = x.b; }
void f6(X& x, long v) { x.c = v; gu = x.b; }
