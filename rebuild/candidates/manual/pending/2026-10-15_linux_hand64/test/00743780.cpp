#include <cstdio>
#include <cstdlib>
// Behaviour of CModeDragonFlyingBase's scalar deleting destructor (0x00743780): the flight-path block is freed,
// then the CModeBase part (name string, anim filter) is destroyed, then the object's memory is released.
static int g_log[8]; static int g_n = 0; static void* g_freed = 0;
static void TestFree(void* p) { g_freed = p; g_log[g_n++] = 1; std::free(p); }
struct CCharString { void* m_pStringData; ~CCharString() { g_log[g_n++] = 2; } };
struct CAnimFilterBase { virtual void v00() {} ~CAnimFilterBase() { g_log[g_n++] = 3; } };
struct CModeBaseRoot { virtual void v00() {} };
struct CModeBase : CModeBaseRoot {
    long m_type; CAnimFilterBase m_filter; CCharString m_name;
    ~CModeBase() { }
};
struct CModeDragonFlyingBase : CModeBase {
    unsigned char m_pad10[0x0c];
    void* m_pFlightPath;
    ~CModeDragonFlyingBase() { if (m_pFlightPath) TestFree(m_pFlightPath); }
};
static void DeleteDragonFlyingMode(CModeDragonFlyingBase* mode) { delete mode; }
int main()
{
    CModeDragonFlyingBase* m = new CModeDragonFlyingBase;
    void* block = std::malloc(16);
    m->m_pFlightPath = block; m->m_name.m_pStringData = 0;
    DeleteDragonFlyingMode(m);
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_freed != block) { std::printf("00743780_TEST FAIL\n"); return 1; }
    CModeDragonFlyingBase* m2 = new CModeDragonFlyingBase;
    m2->m_pFlightPath = 0;
    DeleteDragonFlyingMode(m2);
    if (g_n != 5 || g_log[3] != 2 || g_log[4] != 3) { std::printf("00743780_TEST FAIL\n"); return 1; }
    std::printf("00743780_TEST PASS\n");
    return 0;
}
