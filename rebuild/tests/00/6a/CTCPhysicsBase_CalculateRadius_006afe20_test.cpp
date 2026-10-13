#include <cstdio>

struct Shape { unsigned char pad[0x18]; float Radius; };
struct Def { unsigned char pad[0x28]; float Diameter; };
struct Ph { Shape* shape; Def* def; virtual Shape* GetCollisionShape() { return shape; }
    float Calc() { Shape* s = GetCollisionShape(); if (s) return s->Radius; Def* d = def; if (d) return d->Diameter * 0.5f; return 1.0f; } };
int main()
{
    Shape s; s.Radius = 2.5f; Def d; d.Diameter = 3.0f; Ph p; p.shape = &s; p.def = &d; if (p.Calc() != 2.5f) { std::printf("006afe20_TEST FAIL\n"); return 1; }
    p.shape = 0; if (p.Calc() != 1.5f) { std::printf("006afe20_TEST FAIL\n"); return 1; }
    p.def = 0; if (p.Calc() != 1.0f) { std::printf("006afe20_TEST FAIL\n"); return 1; }
    std::printf("006afe20_TEST PASS\n");
    return 0;
}