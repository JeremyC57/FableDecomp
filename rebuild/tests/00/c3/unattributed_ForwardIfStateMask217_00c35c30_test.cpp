// Behaviour test for ForwardIfStateMask217 (0x00C35C30): the two values reach the handler only when a 0x217 state bit is set.
#include <cstdio>

static int g_calls = 0;
static long g_first = 0, g_second = 0;

struct CStateObject {
    unsigned long m_state;
    void Handle(long first, long second) { ++g_calls; g_first = first; g_second = second; }
};

void ForwardIfStateMask217(CStateObject* object, long first, long second)
{
    if (object->m_state & 0x217)
        object->Handle(first, second);
}

int main()
{
    CStateObject o; o.m_state = 0x1E8;                      // no bit of 0x217
    ForwardIfStateMask217(&o, 1, 2);
    if (g_calls != 0) { std::printf("00c35c30_TEST FAIL\n"); return 1; }
    o.m_state = 0x200;
    ForwardIfStateMask217(&o, 3, 4);
    o.m_state = 0x1;
    ForwardIfStateMask217(&o, 5, 6);
    if (g_calls != 2 || g_first != 5 || g_second != 6) { std::printf("00c35c30_TEST FAIL\n"); return 1; }
    std::printf("00c35c30_TEST PASS\n");
    return 0;
}