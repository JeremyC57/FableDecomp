#include <cstdio>
struct T { unsigned char p[0x91]; unsigned char f; bool D() const { return (f & 1) != 0; } };
struct P { T* t; T* G() { return t; } };
static bool __fastcall A(P& p) { if (p.G() != 0) { if (!p.G()->D()) return true; } return false; }
int main()
{
    T t; t.f = 0; P p; p.t = &t; if (!A(p)) { std::printf("0091dda0_TEST FAIL\n"); return 1; }
    t.f = 1; if (A(p)) { std::printf("0091dda0_TEST FAIL\n"); return 1; }
    p.t = 0; if (A(p)) { std::printf("0091dda0_TEST FAIL\n"); return 1; }

    std::printf("0091dda0_TEST PASS\n");
    return 0;
}