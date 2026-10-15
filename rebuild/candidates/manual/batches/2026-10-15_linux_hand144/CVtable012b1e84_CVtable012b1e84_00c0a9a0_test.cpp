// Behaviour test for 0x00c0a9a0 (CVtable012b1e84::~CVtable012b1e84): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static int g_order[32]; static int g_n = 0;
struct SHandle { int id; ~SHandle() { g_order[g_n++] = id; } };
struct CVtable012b1e84 {
    virtual void v00() {}
    SHandle m_first[16]; SHandle m_second[8];
    CVtable012b1e84() { for (int i = 0; i < 16; ++i) m_first[i].id = i; for (int i = 0; i < 8; ++i) m_second[i].id = 100 + i; }
    ~CVtable012b1e84() {}
};
static void Run()
{
    { CVtable012b1e84 o; o.v00(); }
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
