// Behaviour test for 0x00903970 (CScriptGameResourceObjectScriptedThing::ClearAllActionsIncludingLoopingAnimations): only a present, live
// creature (flag bit 3) gets its action queue cleared and its current action finished.
#include <cstdio>

struct CThing_Retail {
    unsigned char m_flags;
    unsigned char m_dead;
    int m_cleared, m_finished, m_released;
    void ClearQueuedActions() { ++m_cleared; }
    void FinishCurrentAction() { ++m_finished; }
    void ReleaseLoopingAnimation() { ++m_released; }
};
struct CIntelligentPointer_Retail {
    CThing_Retail* m_pItem;
    CThing_Retail* GetPItem() { return m_pItem; }
};
struct CScriptGameResourceObjectScriptedThing {
    CIntelligentPointer_Retail PThing;
    void ClearAllActionsIncludingLoopingAnimations();
};

void CScriptGameResourceObjectScriptedThing::ClearAllActionsIncludingLoopingAnimations()
{
    CIntelligentPointer_Retail* thing = &PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (thing->GetPItem()->m_flags & 8) {
                CThing_Retail* creature = thing->GetPItem();
                creature->ClearQueuedActions();
                creature->FinishCurrentAction();
                creature->FinishCurrentAction();
                creature->ReleaseLoopingAnimation();
            }
        }
    }
}

int main()
{
    CThing_Retail t;
    t.m_flags = 8; t.m_dead = 0; t.m_cleared = t.m_finished = t.m_released = 0;
    CScriptGameResourceObjectScriptedThing s;
    s.PThing.m_pItem = &t;
    s.ClearAllActionsIncludingLoopingAnimations();
    if (t.m_cleared != 1 || t.m_finished != 2 || t.m_released != 1) { std::printf("00903970_TEST FAIL\n"); return 1; }
    t.m_flags = 0;
    s.ClearAllActionsIncludingLoopingAnimations();
    t.m_flags = 8; t.m_dead = 1;
    s.ClearAllActionsIncludingLoopingAnimations();
    s.PThing.m_pItem = 0;
    s.ClearAllActionsIncludingLoopingAnimations();
    if (t.m_cleared != 1) { std::printf("00903970_TEST FAIL\n"); return 1; }
    std::printf("00903970_TEST PASS\n");
    return 0;
}