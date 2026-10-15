// Behaviour test for 0x0085C120 CCombatAction_ControlledStrafeJump::FinishAction.
#include <cstdio>
struct CTCCreatureModeManager {
    long removed[8]; int count;
    CTCCreatureModeManager() : count(0) { }
    void RemoveMode(unsigned long mode) { removed[count++] = (long)mode; }
};
struct CThing {
    unsigned char m_type;
    bool hasModes;
    CTCCreatureModeManager* modes;
    int suspendCalls; bool lastSuspend;
    int physicsResets;
    CThing() : m_type(0), hasModes(true), modes(0), suspendCalls(0), lastSuspend(true), physicsResets(0) { }
    void SetSuspendAnimation(bool suspend, long) { ++suspendCalls; lastSuspend = suspend; }
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        if (type == 0x31 && hasModes) { *ppOut = (T*)modes; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
static int g_baseFinish = 0;
struct CCombatActionBase {
    CBaseIntelligentPointer PCreature;
    void FinishAction() { ++g_baseFinish; }
};
struct CStrafeJump : CCombatActionBase {
    bool AddedRollMode;
    void FinishAction()
    {
        CCombatActionBase::FinishAction();
        if (AddedRollMode) {
            CTCCreatureModeManager* pModes;
            if (PCreature.GetPItem()->GetInterface(&pModes, 0x31))
                pModes->RemoveMode(70);
            AddedRollMode = false;
        }
    }
};
int main()
{
    CTCCreatureModeManager modes; CThing creature; creature.modes = &modes;
    CStrafeJump a; a.PCreature.m_pItem = &creature; a.AddedRollMode = true;
    a.FinishAction();
    if (g_baseFinish != 1 || modes.count != 1 || modes.removed[0] != 70 || a.AddedRollMode) { std::printf("0085c120_TEST FAIL\n"); return 1; }
    a.FinishAction();                                    // flag already clear: nothing removed
    creature.hasModes = false; a.AddedRollMode = true;
    a.FinishAction();                                    // no mode manager: flag still cleared
    if (g_baseFinish != 3 || modes.count != 1 || a.AddedRollMode) { std::printf("0085c120_TEST FAIL\n"); return 1; }
    std::printf("0085c120_TEST PASS\n");
    return 0;
}