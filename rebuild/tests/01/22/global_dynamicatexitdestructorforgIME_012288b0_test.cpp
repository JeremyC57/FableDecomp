// Behaviour test for 0x012288B0, the dynamic atexit destructor of gIME: CIME's inline destructor runs Shutdown on the global exactly once.
#include <cstdio>
#include <new>
static int g_shutdowns = 0;
class CIMEModel {
public:
    ~CIMEModel() { Shutdown(); }
    void Shutdown() { ++g_shutdowns; }
};
static CIMEModel* g_pIME = 0;
static void AtexitDestructor() { g_pIME->~CIMEModel(); }
int main()
{
    static char storage[sizeof(CIMEModel)];
    g_pIME = new (storage) CIMEModel;
    if (g_shutdowns != 0) { std::printf("AUTO_TINY_012288b0_TEST FAIL\n"); return 1; }
    AtexitDestructor();
    if (g_shutdowns == 1) { std::printf("AUTO_TINY_012288b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_012288b0_TEST FAIL\n");
    return 1;
}
