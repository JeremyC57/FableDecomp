#include <cstdio>
#include <new>
#include <cstdlib>
static int g_st = 0, g_sd = 0, g_vr = 0, g_vd = 0; static float g_val = 0; static bool g_fail = false; static float g_v = 0; static void* g_ow = 0;
struct St { unsigned char b[0x3c]; St(float v, void* o) { ++g_st; g_v = v; g_ow = o; } ~St() { ++g_sd; } }; struct Vr { unsigned char b[0xc]; Vr(const St* s) { ++g_vr; } ~Vr() { ++g_vd; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct X { void* m0; void* ow; float val; Vr* var; float Get() { return g_val; } void Upd() { float v = Get(); if (v != val) { delete var; val = v; var = 0; if (v != 0.0f) { St s(v, ow); var = new Vr(&s); } } } };
static bool Check() { int o; X x; x.ow = &o; x.val = 0; x.var = 0; g_val = 0; x.Upd(); if (g_st || x.var) return false; g_val = 1.5f; x.Upd(); if (x.var == 0 || g_st != 1 || g_sd != 1 || g_vr != 1 || g_v != 1.5f || g_ow != &o || x.val != 1.5f) return false; x.Upd(); if (g_vr != 1) return false; g_val = 0; x.Upd(); if (x.var != 0 || g_vd != 1 || x.val != 0) return false; g_val = 2; g_fail = true; x.Upd(); return x.var == 0 && g_st == 2 && g_sd == 2 && g_vr == 1; }

int main()
{
    if (!Check()) { std::printf("00c37050_TEST FAIL\n"); return 1; }
    std::printf("00c37050_TEST PASS\n");
    return 0;
}