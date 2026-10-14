#include <cstdio>

struct B { unsigned long w[40]; };
B g;
struct P { void Get(B& out) const { out = g; } };
int main()
{
    for (int i = 0; i < 40; ++i) g.w[i] = 1000 + i;
    P p; B b; p.Get(b); if (b.w[0] != 1000 || b.w[39] != 1039) { std::printf("00c33400_TEST FAIL\n"); return 1; }
    std::printf("00c33400_TEST PASS\n");
    return 0;
}