// Behaviour test for 0x00BEE710 std::__uninitialized_fill_n over the decal-map entries: uninitialized fill of n
// ((long, long), CEngineTextureDecal) entries, each a deep copy (key pair, two decal words, its own copy of the blend vector).
#include <cstdio>
#include <cstdlib>
#include <new>
class CCompositeBlendDesc { public: unsigned int w[4]; };
struct Vec { CCompositeBlendDesc* s; CCompositeBlendDesc* f; CCompositeBlendDesc* e;
  Vec(const Vec& o) { unsigned int n = (unsigned int)(o.f - o.s); s = n ? (CCompositeBlendDesc*)std::malloc(n * sizeof(CCompositeBlendDesc)) : 0; f = s; e = s + n; for (CCompositeBlendDesc* p = o.s; p != o.f; ++p, ++f) new (f) CCompositeBlendDesc(*p); }
  Vec() : s(0), f(0), e(0) {} };
struct CDecalEntry { unsigned int a, b, c, d; Vec v; };   // key.first, key.second, OriginalTextureIndex, DecalID, OverlaidBlends
static CDecalEntry* Fill(CDecalEntry* first, unsigned int n, const CDecalEntry& x) { CDecalEntry* cur = first; for (; n > 0; --n, ++cur) new (cur) CDecalEntry(x); return cur; }
int main()
{
    CCompositeBlendDesc es[2] = {{{1, 2, 3, 4}}, {{5, 6, 7, 8}}};
    CDecalEntry x; x.a = 10; x.b = 11; x.c = 12; x.d = 13; x.v.s = es; x.v.f = es + 2; x.v.e = es + 2;
    void* mem = std::malloc(3 * sizeof(CDecalEntry)); CDecalEntry* r = (CDecalEntry*)mem;
    CDecalEntry* end = Fill(r, 3, x);
    bool ok = end == r + 3 && r[2].a == 10 && r[2].d == 13 && r[1].v.f - r[1].v.s == 2 && r[1].v.s != es && r[1].v.s[1].w[3] == 8 && r[0].v.s != r[1].v.s;
    ok = ok && Fill(r, 0, x) == r;
    if (!ok) { std::printf("00bee710_TEST FAIL\n"); return 1; }
    std::printf("00bee710_TEST PASS\n");
    return 0;
}