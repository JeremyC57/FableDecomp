#include <cstdio>
static int g_dtor = 0;
struct Str { const char* p; ~Str() { ++g_dtor; } };
struct Bank { unsigned long Get(const Str& n) { return n.p[0] == 'x' ? 0 : (unsigned long)n.p[0]; } };
struct G { unsigned char t[0x34]; Str name; unsigned long idx; bool local; };
struct Tag { };
static G* __fastcall Copy(G* f, G* l, G* r, const Tag&, long*) { for (long n = l - f; n > 0; --n) { r->name = f->name; r->idx = f->idx; ++f; ++r; } return r; }
struct Arr { G* f; G* l; G* e; G* Erase(G* p) { if (p + 1 != l) Copy(p + 1, l, p, Tag(), (long*)0); --l; l->~G(); return p; } };
struct M { virtual void v00() {} unsigned char pad[0x64]; Arr gens;
    void Init(Bank* b, const Str&) { G* it = gens.f; while (it != gens.l) { it->idx = b->Get(it->name); if (it->idx) ++it; else it = gens.Erase(it); } } };
int main()
{
    G gs[4]; gs[0].name.p = "a"; gs[1].name.p = "x"; gs[2].name.p = "x"; gs[3].name.p = "b";
    M m; m.gens.f = gs; m.gens.l = gs + 4; m.gens.e = gs + 4; Bank b; Str n = { "" };
    m.Init(&b, n);
    if (m.gens.l != gs + 2 || gs[0].idx != 'a' || gs[1].name.p[0] != 'b' || gs[1].idx != 'b' || g_dtor != 2) { std::printf("00a2ec40_TEST FAIL\n"); return 1; }
    std::printf("00a2ec40_TEST PASS\n");
    return 0;
}