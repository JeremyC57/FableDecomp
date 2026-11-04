// Behaviour test for CSoundEvent::StopListening (0x00c20810): a model of the source's logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; std::printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
struct CSoundEvent;
static int g_removes = 0, g_checkArg = 99;
static CSoundEvent* g_removed = 0;
struct CEventList { void Remove(CSoundEvent* e) { ++g_removes; g_removed = e; } };   // stub for slot 8
struct CSoundEventManager { CEventList m_behaviourEvents; };
struct CSoundEvent {
    bool m_listening;
    void CheckState(long expected) { g_checkArg = (int)expected; }
    void StopListening(CSoundEventManager* manager)
    {
        CheckState(-1);
        m_listening = false;
        manager->m_behaviourEvents.Remove(this);
    }
};
static void RunTest()
{
    CSoundEventManager manager;
    CSoundEvent e; e.m_listening = true;
    e.StopListening(&manager);
    CHECK(!e.m_listening && g_removes == 1 && g_removed == &e && g_checkArg == -1);
}
int main()
{
    RunTest();
    if (g_fail) { std::printf("00c20810_TEST FAIL\n"); return 1; }
    std::printf("00c20810_TEST PASS\n");
    return 0;
}