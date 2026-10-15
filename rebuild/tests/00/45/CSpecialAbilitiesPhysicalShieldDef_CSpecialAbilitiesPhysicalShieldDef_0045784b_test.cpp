// behaviour test for 0x0045784B def destructor: the 8 float vectors freed in reverse declaration order, then the base part
#include <cstdio>
#include <cstdlib>
static int g_log[16]; static int g_n = 0;
struct CParentDefClassBase { ~CParentDefClassBase() { g_log[g_n++] = 100; } };
struct CFloatVector { int id; float* first; ~CFloatVector() { if (first) { std::free(first); g_log[g_n++] = id; } } };
struct CDef : CParentDefClassBase {
    CFloatVector v[8];
    CDef() { for (int i = 0; i < 8; ++i) { v[i].id = i; v[i].first = (i == 2) ? 0 : (float*)std::malloc(4); } }
    ~CDef() {}
};
int main()
{
    { CDef d; }
    bool ok = g_n == 8 && g_log[0] == 8 - 1 && g_log[8 - 2] == 0 && g_log[8 - 1] == 100;
    for (int i = 0; i + 1 < g_n - 1; ++i) if (g_log[i] <= g_log[i + 1]) ok = false;
    if (!ok) { std::printf("0045784b_TEST FAIL\n"); return 1; }
    std::printf("0045784b_TEST PASS\n");
    return 0;
}