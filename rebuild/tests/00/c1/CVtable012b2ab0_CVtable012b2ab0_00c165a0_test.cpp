// Behaviour test for 0x00c165a0 (CVtable012b2ab0::CVtable012b2ab0): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CHelper { void* source; CHelper(void* s) : source(s) {} };
struct CVtable012b2ab0 {
    virtual ~CVtable012b2ab0() { delete m_pHelper; }
    void* m_pSource; CHelper* m_pHelper;
    CVtable012b2ab0(void* source) : m_pSource(source), m_pHelper(0) { m_pHelper = new CHelper(m_pSource); }
};
static void Run()
{
    int token = 5;
    CVtable012b2ab0 a(&token);
    CHECK(a.m_pSource == &token && a.m_pHelper != 0 && a.m_pHelper->source == &token);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00c165a0_TEST FAIL\n"); return 1; }
    std::printf("00c165a0_TEST PASS\n");
    return 0;
}