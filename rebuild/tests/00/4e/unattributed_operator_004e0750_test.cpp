#include <cstdio>
static int g_b = 0; struct B { long x; B& operator=(const B& o) { x = o.x; ++g_b; return *this; } };
struct D : B { long a; unsigned char c; long d; D& operator=(const D& o); }; D& D::operator=(const D& o) { B::operator=(o); a = o.a; c = o.c; d = o.d; return *this; }
int main()
{
    D s, t; s.x = 1; s.a = 2; s.c = 3; s.d = 4; t = s; if (g_b != 1 || t.x != 1 || t.a != 2 || t.c != 3 || t.d != 4) { std::printf("004e0750_TEST FAIL\n"); return 1; }

    std::printf("004e0750_TEST PASS\n");
    return 0;
}