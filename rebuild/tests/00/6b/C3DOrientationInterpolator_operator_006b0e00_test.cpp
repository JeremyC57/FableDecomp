#include <cstdio>
struct V { float X, Y, Z; };
struct R { V Up, Forward; };
struct Base { virtual void v00() {} };
struct O : Base { R s, e; O& operator=(const O& o) { s = o.s; e = o.e; return *this; } };
int main()
{
    O a, b; a.s.Up.X = 1; a.s.Forward.Z = 2; a.e.Up.Y = 3; a.e.Forward.Z = 4;
    b.s.Up.X = 0; b.s.Forward.Z = 0; b.e.Up.Y = 0; b.e.Forward.Z = 0;
    O& r = (b = a);
    if (&r != &b || b.s.Up.X != 1 || b.s.Forward.Z != 2 || b.e.Up.Y != 3 || b.e.Forward.Z != 4) { std::printf("006b0e00_TEST FAIL\n"); return 1; }
    std::printf("006b0e00_TEST PASS\n");
    return 0;
}