// Behaviour test for NProfileTimer::EndProfile(long) 0x009D8250: compiled empty in retail, any id is accepted and nothing changes
#include <cstdio>
namespace NProfileTimer {
    void EndProfile(long id) { (void)id; }
}
static int g_state = 7;
int main()
{
    NProfileTimer::EndProfile(0);
    NProfileTimer::EndProfile(1);
    NProfileTimer::EndProfile(-5);
    if (g_state != 7) { std::printf("009d8250_TEST FAIL\n"); return 1; }
    std::printf("009d8250_TEST PASS\n");
    return 0;
}