// behaviour test for 0x00C21700 CPanelItem::Restart: states 3 and 6 go back to idle first, then the item enters state 4
#include <cstdio>

struct CPanel { int dummy; };
static int g_leave3 = 0, g_leave6 = 0, g_enter4 = 0, g_checks = 0;

struct CPanelItem {
    unsigned short m_state;
    void  ChangeState(long) { ++g_checks; }                                   // stand-in for 0x00C20720
    void* EnterState4(CPanel*) { ++g_enter4; m_state = 4; return 0; }    // 0x00C20880
    void* LeaveState3(CPanel*) { ++g_leave3; m_state = 1; return 0; }    // 0x00C209C0
    void* LeaveState6(CPanel*) { ++g_leave6; m_state = 1; return 0; }    // 0x00C20B50
    void  Restart(CPanel* owner);
};

void CPanelItem::Restart(CPanel* owner)
{
    ChangeState(-1);
    if (m_state == 3)
        LeaveState3(owner);
    else if (m_state == 6)
        LeaveState6(owner);
    EnterState4(owner);
}

int main()
{
    CPanel owner;
    CPanelItem item;
    item.m_state = 3; item.Restart(&owner);
    if (g_leave3 != 1 || g_leave6 != 0 || g_enter4 != 1 || item.m_state != 4) { std::printf("00c21700_TEST FAIL\n"); return 1; }
    item.m_state = 6; item.Restart(&owner);
    if (g_leave3 != 1 || g_leave6 != 1 || g_enter4 != 2 || item.m_state != 4) { std::printf("00c21700_TEST FAIL\n"); return 1; }
    item.m_state = 1; item.Restart(&owner);
    if (g_leave3 != 1 || g_leave6 != 1 || g_enter4 != 3 || g_checks != 3) { std::printf("00c21700_TEST FAIL\n"); return 1; }
    std::printf("00c21700_TEST PASS\n");
    return 0;
}