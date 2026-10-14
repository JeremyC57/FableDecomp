#include <cstdio>
static float g_v = 0; static unsigned g_i = 0;
struct Src { unsigned char pad[0x7c0]; float v; };
struct Tgt { void Apply(float v, unsigned short i) { g_v = v; g_i = i; } };
struct Own { unsigned char pad[0x34]; Tgt* t; Src* s; };
struct U { unsigned char p0[4]; Own* o; unsigned char p1[4]; unsigned short idx; unsigned char p2[2]; float scale;
    void Go() { o->t->Apply(scale * o->s->v, idx); } };
int main()
{
    Src s; s.v = 2.5f; Tgt t; Own o; o.t = &t; o.s = &s; U u; u.o = &o; u.idx = 7; u.scale = 4.0f; u.Go();
    if (g_v != 10.0f || g_i != 7) { std::printf("00c15a70_TEST FAIL\n"); return 1; }
    std::printf("00c15a70_TEST PASS\n");
    return 0;
}