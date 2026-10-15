// Behaviour test for 0x00a67ea0: CVolumeTexture::GetVolumeLevel: level fetched, wrapped and returned with one reference.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Vol { int refs; };
struct Tex { Vol levels[3]; long Get(unsigned int l, Vol** out) { *out = &levels[l]; levels[l].refs++; return 0; } };
struct CVolume { Vol* p; CVolume() : p(0) {} CVolume(Vol* v) : p(0) { Init(v); } CVolume(const CVolume& o) : p(o.p) { if (p) p->refs++; } ~CVolume() { Release(); }
  void Init(Vol* v) { Release(); p = v; p->refs++; } void Release() { if (p) { p->refs--; p = 0; } } };
struct VT { Tex* t; CVolume GetVolumeLevel(unsigned int l) { Vol* v; t->Get(l, &v); CVolume r(v); return r; } };
static int run() { Tex t; for (int i = 0; i < 3; ++i) t.levels[i].refs = 0; VT vt = {&t};
  { CVolume v = vt.GetVolumeLevel(2); CHECK(v.p == &t.levels[2] && t.levels[2].refs == 2); }
  CHECK(t.levels[2].refs == 1); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00a67ea0_TEST FAIL\n"); return 1; }
    std::printf("00a67ea0_TEST PASS\n");
    return 0;
}
