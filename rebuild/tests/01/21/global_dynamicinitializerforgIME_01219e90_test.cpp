// Behaviour test for 0x01219E90, the dynamic initializer of gIME: CIME's inline ctor runs ResetToDefaultSettings (a static) and its
// inline dtor runs Shutdown.
#include <cstdio>
static int g_resets = 0;
static int g_shutdowns = 0;
class CIMEModel {
public:
    CIMEModel() { ResetToDefaultSettings(); }
    ~CIMEModel() { Shutdown(); }
    static void ResetToDefaultSettings() { ++g_resets; }
    void Shutdown() { ++g_shutdowns; }
};
int main()
{
    {
        CIMEModel ime;
        if (g_resets != 1 || g_shutdowns != 0) { std::printf("01219e90_TEST FAIL\n"); return 1; }
    }
    if (g_resets != 1 || g_shutdowns != 1) { std::printf("01219e90_TEST FAIL\n"); return 1; }
    std::printf("01219e90_TEST PASS\n");
    return 0;
}