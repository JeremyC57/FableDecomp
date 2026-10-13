#include <cstdio>
struct S; struct Tx; static void* g_t = 0; static void* g_s = 0;
struct D { unsigned char pad[0x2c]; unsigned char s[0x20]; unsigned char t[4]; void Blit(Tx* t, S* s) { g_t = t; g_s = s; }
    Tx* Copy() { Tx* t = reinterpret_cast<Tx*>(&this->t[0]); Blit(t, reinterpret_cast<S*>(&this->s[0])); return t; } };
int main()
{
    D d; if (d.Copy() != (void*)&d.t[0] || g_t != &d.t[0] || g_s != &d.s[0]) { std::printf("009bf5e0_TEST FAIL\n"); return 1; }

    std::printf("009bf5e0_TEST PASS\n");
    return 0;
}