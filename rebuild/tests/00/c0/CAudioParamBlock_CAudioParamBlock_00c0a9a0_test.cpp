// Behaviour test for 0x00c0a9a0 (CAudioParamBlock::~CAudioParamBlock): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static int g_order[32]; static int g_n = 0;
struct CString { int id; ~CString() { g_order[g_n++] = id; } };
struct CAudioParamBlock {
    virtual void v00() {}
    CString m_gainNames[16]; CString m_pitchNames[8];
    CAudioParamBlock() { for (int i = 0; i < 16; ++i) m_gainNames[i].id = i; for (int i = 0; i < 8; ++i) m_pitchNames[i].id = 100 + i; }
    ~CAudioParamBlock() {}
};
static void Run()
{
    { CAudioParamBlock o; o.v00(); }
    CHECK(g_n == 24);
    CHECK(g_order[0] == 107 && g_order[7] == 100 && g_order[8] == 15 && g_order[23] == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00c0a9a0_TEST FAIL\n"); return 1; }
    std::printf("00c0a9a0_TEST PASS\n");
    return 0;
}