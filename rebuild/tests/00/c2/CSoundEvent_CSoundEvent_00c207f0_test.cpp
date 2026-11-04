// Behaviour test for CSoundEvent::~CSoundEvent (0x00c207f0): a model of the source's logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; std::printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static int g_checkArg = 99, g_checks = 0, g_keyDtors = 0;
struct CSampleKey { ~CSampleKey() { ++g_keyDtors; } };               // stub for 0x00C25CA0 (empty)
struct CSoundEvent {
    unsigned short m_state;
    CSampleKey m_sample;
    void CheckState(long expected) { ++g_checks; g_checkArg = (int)expected; }   // stub for 0x00C20720
    ~CSoundEvent() { CheckState(0); }
};
static void RunTest()
{
    { CSoundEvent e; e.m_state = 0; }
    CHECK(g_checks == 1 && g_checkArg == 0 && g_keyDtors == 1);   // state check with 0 (idle), then the member
    CSoundEvent* a = new CSoundEvent[3];
    delete[] a;
    CHECK(g_checks == 4 && g_keyDtors == 4);                       // the vector deleting destructor runs it per element
}
int main()
{
    RunTest();
    if (g_fail) { std::printf("00c207f0_TEST FAIL\n"); return 1; }
    std::printf("00c207f0_TEST PASS\n");
    return 0;
}