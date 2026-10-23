#include <cstdio>
#include <new>
static int g_lk = 0; static unsigned char g_sec[8]; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
static int g_get = 0; static unsigned long g_groups = 0; static unsigned long g_cog[2];
struct Dm { float d; float GetDamage() { ++g_get; return d; } }; struct An { unsigned char pad[8]; Dm dm; }; static An g_an[2][2];
struct Gl { virtual void s0() {} virtual void s1() {} virtual void s2() {} virtual void s3() {} virtual unsigned long CountOfGroup(unsigned long g) { return g_cog[g]; } virtual unsigned long Count() { return g_groups; } virtual An* AnimationOfGroup(unsigned long g, unsigned long i) { return &g_an[g][i]; } }; static Gl g_gl;
struct Set { virtual void s0() {} virtual void s1() {} virtual void s2() {} virtual void s3() {} virtual void s4() {} virtual Gl* Groups() { return &g_gl; } }; static Set g_set;
struct Tb { Set* Find(void* k) { return k ? &g_set : 0; } float Max(void* k) { Pl l(g_sec); Set* s = Find(k); if (!s) return -1.0f; float best = 0.0f; for (unsigned long g = 0; g < s->Groups()->Count(); ++g) { for (unsigned long i = 0; i < s->Groups()->CountOfGroup(g); ++i) { An* a = s->Groups()->AnimationOfGroup(g, i); if (a->dm.GetDamage() > best) best = a->dm.GetDamage(); } } return best; } };
static bool Check() { g_groups = 2; g_cog[0] = 2; g_cog[1] = 1; g_an[0][0].dm.d = 3; g_an[0][1].dm.d = 9; g_an[1][0].dm.d = 4; Tb t; int k; if (t.Max(&k) != 9.0f || g_lk) return false; if (t.Max(0) != -1.0f) return false; g_an[0][1].dm.d = -5; return t.Max(&k) == 4.0f && g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c199a0_TEST FAIL\n"); return 1; }
    std::printf("01212148_TEST PASS\n");
    return 0;
}