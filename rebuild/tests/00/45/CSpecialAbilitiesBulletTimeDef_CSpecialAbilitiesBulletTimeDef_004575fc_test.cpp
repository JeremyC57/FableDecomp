// behaviour test for 0x004575FC def destructor: the 7 float vectors freed in reverse declaration order, then the base part
#include <cstdio>
#include <cstdlib>
static int g_log[16]; static int g_n = 0;
struct CParentDefClassBase { ~CParentDefClassBase() { g_log[g_n++] = 100; } };
struct CFloatVector { int id; float* first; ~CFloatVector() { if (first) { std::free(first); g_log[g_n++] = id; } } };
struct CDef : CParentDefClassBase {
    CFloatVector v[7];
    CDef() { for (int i = 0; i < 7; ++i) { v[i].id = i; v[i].first = (i == 2) ? 0 : (float*)std::malloc(4); } }
    ~CDef() {}
};
int main()
{
    { CDef d; }
    bool ok = g_n == 7 && g_log[0] == 7 - 1 && g_log[7 - 2] == 0 && g_log[7 - 1] == 100;
    for (int i = 0; i + 1 < g_n - 1; ++i) if (g_log[i] <= g_log[i + 1]) ok = false;
    if (!ok) { std::printf("004575fc_TEST FAIL\n"); return 1; }
    std::printf("004575fc_TEST PASS\n");
    return 0;
}