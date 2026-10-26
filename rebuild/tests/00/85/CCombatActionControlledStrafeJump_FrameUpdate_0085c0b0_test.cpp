// Behaviour test for 0x0085C0B0 CCombatAction_ControlledStrafeJump::FrameUpdate.
#include <cstdio>
class CTCCreatureModeManager { public:
    long removed[8]; int count;
    CTCCreatureModeManager() : count(0) { }
    void RemoveMode(unsigned long mode) { removed[count++] = (long)mode; }
};
class CThing { public:
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
static int g_baseUpdate = 0;
class CCombatActionBase { public:
    CBaseIntelligentPointer PCreature;
    bool ready;
    virtual ~CCombatActionBase() { }
    virtual bool IsActionReadyForInterruption() const { return ready; }
    void FrameUpdate() { ++g_baseUpdate; }
};
struct CStrafeJump : CCombatActionBase {
    bool AddedRollMode;
    void FrameUpdate()
    {
        CCombatActionBase::FrameUpdate();
        if (IsActionReadyForInterruption() && AddedRollMode) {
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
    CStrafeJump a; a.PCreature.m_pItem = &creature; a.AddedRollMode = true; a.ready = false;
    a.FrameUpdate();
    if (modes.count != 0 || !a.AddedRollMode) { std::printf("0085c0b0_TEST FAIL\n"); return 1; }
    a.ready = true;
    a.FrameUpdate();
    if (g_baseUpdate != 2 || modes.count != 1 || modes.removed[0] != 70 || a.AddedRollMode) { std::printf("0085c0b0_TEST FAIL\n"); return 1; }
    std::printf("0085c0b0_TEST PASS\n");
    return 0;
}