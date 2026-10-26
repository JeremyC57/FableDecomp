#include <cstdio>

// Model of CCombatActionBase::SetUpCombatAnimationNotes @ 0x00859EC0
static int g_events = 0, g_interrupts = 0;
struct CStrikeEventNode { CStrikeEventNode* m_pNext; CStrikeEventNode* m_pPrev; };
struct CStrikeEventList { CStrikeEventNode* m_pHead; long m_size; CStrikeEventNode* begin() const { return m_pHead->m_pNext; } CStrikeEventNode* end() const { return m_pHead; } };
class CCombatActionBase { public:
    virtual void v00() {} unsigned char _pad04[0x14 - 4]; long ActionTime; unsigned char _pad18[0xbc - 0x18]; CStrikeEventList StrikeEvents; unsigned char _padc4[0xde - 0xc4]; bool m_keepActionTime; unsigned char _paddf[0x114 - 0xdf];
    void SetupCombatStrikeEvents() { ++g_events; }
    void SetupCombatInterrupts() { ++g_interrupts; }
    void SetUpCombatAnimationNotes() { SetupCombatStrikeEvents(); SetupCombatInterrupts(); if (StrikeEvents.begin() != StrikeEvents.end() && !m_keepActionTime) ActionTime = -1; }
};
int main()
{
    CStrikeEventNode head, node; head.m_pNext = &head; head.m_pPrev = &head;
    CCombatActionBase a; a.StrikeEvents.m_pHead = &head; a.StrikeEvents.m_size = 0; a.m_keepActionTime = false; a.ActionTime = 50;
    a.SetUpCombatAnimationNotes();
    if (a.ActionTime != 50 || g_events != 1 || g_interrupts != 1) { std::printf("00859ec0_TEST FAIL\n"); return 1; }
    head.m_pNext = &node; node.m_pNext = &head; node.m_pPrev = &head; head.m_pPrev = &node; a.m_keepActionTime = true;
    a.SetUpCombatAnimationNotes();
    if (a.ActionTime != 50) { std::printf("00859ec0_TEST FAIL\n"); return 1; }
    a.m_keepActionTime = false; a.SetUpCombatAnimationNotes();
    if (a.ActionTime != -1 || g_events != 3 || (sizeof(void*) == 4 && (char*)&a.StrikeEvents - (char*)&a != 0xbc)) { std::printf("00859ec0_TEST FAIL\n"); return 1; }
    std::printf("00859ec0_TEST PASS\n"); return 0;
}