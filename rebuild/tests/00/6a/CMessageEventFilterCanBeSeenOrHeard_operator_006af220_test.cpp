#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static bool g_d = false, g_s = false; struct CChecker { bool DistanceCheck(int*) { std::strcat(g_seq, "D"); return g_d; } bool SurfaceCheck(int*) { std::strcat(g_seq, "S"); return g_s; } };
struct CFilter { CChecker* m_pChecker; long operator()(int* t) { CChecker* c = m_pChecker; if (c->DistanceCheck(t) || c->SurfaceCheck(t)) return 1; return 0; } };

int main() {
    CChecker c; CFilter f; f.m_pChecker = &c; int t; g_d = true; g_s = false; if (f(&t) != 1 || std::strcmp(g_seq, "D")) { std::printf("006af220_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_d = false; g_s = true; if (f(&t) != 1 || std::strcmp(g_seq, "DS")) { std::printf("006af220_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_s = false; if (f(&t) != 0 || std::strcmp(g_seq, "DS")) { std::printf("006af220_TEST FAIL\n"); return 1; }
    std::printf("006af220_TEST PASS\n"); return 0;
}