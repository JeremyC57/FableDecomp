#include <cstdio>
#include <new>
struct Rk { long x, y; float Height() const { return (float)(x + y); } }; struct Lk { long x, y; float h; Lk(const Rk* k) : x(k->x), y(k->y), h(k->Height()) {} }; struct Nd { long flag, x, y; float h; Nd(const Rk* k) : flag(0), x(k->x), y(k->y), h(k->Height()) {} }; static Nd* g_found = 0; static Nd* g_ins = 0; static int g_finds = 0;
struct Tb { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void Insert(Nd* n) { g_ins = n; } virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual Nd* Find(const Lk* k) { ++g_finds; return k->h == 5.0f ? g_found : 0; } };
struct X { long m0; Tb* tb; void Mark(const Rk* k, long v) { Lk lk(k); Nd* n = tb->Find(&lk); if (!n) { n = new Nd(k); tb->Insert(n); } if (v) n->flag = 1; } };
static bool Check() { Tb tb; X x; x.tb = &tb; Rk k; k.x = 2; k.y = 3; x.Mark(&k, 0); if (!g_ins || g_ins->flag != 0 || g_ins->x != 2 || g_ins->h != 5.0f) return false; g_found = g_ins; g_ins = 0; x.Mark(&k, 1); bool ok = g_ins == 0 && g_found->flag == 1 && g_finds == 2; delete g_found; return ok; }

int main()
{
    if (!Check()) { std::printf("00c465f0_TEST FAIL\n"); return 1; }
    std::printf("01215560_TEST PASS\n");
    return 0;
}