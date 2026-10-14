#include <cstdio>
#include <new>
#include <cstring>
struct Cs { char* p; Cs(const char* t) : p((char*)t) {} ~Cs() {} long Hash() const { return (long)std::strlen(p); } long Eq(const char* t) const { return std::strcmp(p, t) == 0; } }; inline bool operator==(const Cs& a, const Cs& b) { return a.Eq(b.p) != 0; }
static Cs g_n0(""), g_n1("walk"); struct En { virtual void v0() {} virtual void v1() {} virtual Cs* Name() { return nm; } Cs* nm; }; static En g_e0, g_e1;
struct Mp { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual En* Lookup(long* k) { return *k == 0 ? &g_e0 : (*k == 4 ? &g_e1 : 0); } };
struct X { unsigned char p[0x10]; Mp m; En* Find(const Cs& n, long* c) { long k = n.Hash(); En* e = m.Lookup(&k); bool un = e && *e->Name() == Cs(""); if (un) { if (*e->Name() == n) { if (c) ++*c; } } return e; } };
static bool Check() { g_e0.nm = &g_n0; g_e1.nm = &g_n1; X x; long c = 0; if (x.Find(Cs("walk"), &c) != &g_e1 || c != 0) return false; if (x.Find(Cs(""), &c) != &g_e0 || c != 1) return false; if (x.Find(Cs(""), 0) != &g_e0 || c != 1) return false; return x.Find(Cs("abcdefg"), &c) == 0 && c == 1; }

int main()
{
    if (!Check()) { std::printf("00c101d0_TEST FAIL\n"); return 1; }
    std::printf("00c101d0_TEST PASS\n");
    return 0;
}
