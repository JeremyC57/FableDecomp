// behaviour test for 0x00C21560 CSoundEvent::ReturnToIdle: leave the current state through its exit transition, then check state 1
#include <cstdio>

struct CSoundEventManager { int dummy; };
static int g_calls[8];
static long g_lastCheck = 0;
static int g_checks = 0;

struct CSoundEvent {
    unsigned short m_state;
    void  ChangeState(long state) { ++g_checks; g_lastCheck = state; }        // stand-in for 0x00C20720
    void* Leave(int n) { ++g_calls[n]; m_state = 1; return 0; }
    void* LeaveState2(CSoundEventManager*) { return Leave(2); }                      // 0x00C20920
    void* LeaveState3(CSoundEventManager*) { return Leave(3); }                      // 0x00C209C0
    void* LeaveState4(CSoundEventManager*) { return Leave(4); }                      // 0x00C208E0
    void* LeaveState5(CSoundEventManager*) { return Leave(5); }                      // 0x00C20BB0
    void* LeaveState6(CSoundEventManager*) { return Leave(6); }                      // 0x00C20B50
    void  ReturnToIdle(CSoundEventManager* owner);
};

void CSoundEvent::ReturnToIdle(CSoundEventManager* owner)
{
    ChangeState(-1);
    switch (m_state) {
    case 2: LeaveState2(owner); break;
    case 6: LeaveState6(owner); break;
    case 5: LeaveState5(owner); break;
    case 3: LeaveState3(owner); break;
    case 4: LeaveState4(owner); break;
    }
    ChangeState(1);
}

int main()
{
    CSoundEventManager owner;
    CSoundEvent event;
    for (unsigned short s = 0; s < 9; ++s) {
        for (int i = 0; i < 8; ++i) g_calls[i] = 0;
        g_checks = 0;
        event.m_state = s;
        event.ReturnToIdle(&owner);
        int total = 0;
        for (int i = 0; i < 8; ++i) total += g_calls[i];
        bool active = s >= 2 && s <= 6;
        if (g_checks != 2 || g_lastCheck != 1 || total != (active ? 1 : 0) || (active && g_calls[s] != 1)) { std::printf("00c21560_TEST FAIL\n"); return 1; }
        if (active && event.m_state != 1) { std::printf("00c21560_TEST FAIL\n"); return 1; }
    }
    std::printf("00c21560_TEST PASS\n");
    return 0;
}