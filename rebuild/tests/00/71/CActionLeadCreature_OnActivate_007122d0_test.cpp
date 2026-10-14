#include <cstdio>

static int g_set = -1, g_able = 0, g_removed = 0; static bool g_canMove = false;
struct CMovementController { unsigned char m_locked; CMovementController() : m_locked(5) { }
    unsigned char GetMovementLocked() { return m_locked; } void SetMovementLocked(long locked) { g_set = (int)locked; } };
struct CThing { unsigned char _pad[0x60]; CMovementController* m_pMovement; };
struct CTCScriptedControl { void* __vftable; CThing* m_pCreature; };
bool __fastcall IsAbleToMove(CThing* thing) { ++g_able; return g_canMove; }
void __fastcall RemoveMovementBlockingModes(CThing* thing) { ++g_removed; }
struct CActionLeadCreature { virtual void v00() { } CTCScriptedControl* m_pControl; unsigned char _pad[0x24 - 0x08]; bool m_stateSaved; unsigned char m_savedMovementLocked;
    void OnActivate()
    {
        if (!m_stateSaved) {
            m_savedMovementLocked = m_pControl->m_pCreature->m_pMovement->GetMovementLocked();
            m_pControl->m_pCreature->m_pMovement->SetMovementLocked(0);
        }
        if (!IsAbleToMove(m_pControl->m_pCreature))
            RemoveMovementBlockingModes(m_pControl->m_pCreature);
    } };
int main()
{
    CMovementController mc; CThing t; t.m_pMovement = &mc; CTCScriptedControl c; c.m_pCreature = &t;
    CActionLeadCreature a; a.m_pControl = &c; a.m_stateSaved = false; a.m_savedMovementLocked = 0;
    a.OnActivate();
    if (a.m_savedMovementLocked != 5 || g_set != 0 || g_able != 1 || g_removed != 1) { std::printf("007122d0_TEST FAIL\n"); return 1; }
    a.m_stateSaved = true; g_set = -1; g_canMove = true; a.OnActivate();
    if (g_set != -1 || g_able != 2 || g_removed != 1) { std::printf("007122d0_TEST FAIL\n"); return 1; }
    std::printf("007122d0_TEST PASS\n"); return 0;
}