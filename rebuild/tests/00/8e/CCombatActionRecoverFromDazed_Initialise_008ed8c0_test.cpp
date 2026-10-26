// Behaviour test for 0x008ED8C0 CCombatAction_RecoverFromDazed::Initialise.
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
static int g_baseInit = 0;
class CCombatActionBase { public:
    CBaseIntelligentPointer PCreature;
    void Initialise() { ++g_baseInit; }
};
struct CRecoverFromDazed : CCombatActionBase {
    void Initialise()
    {
        if (PCreature.GetPItem()->m_type == 1) {
            CThing* pCreature = PCreature.GetPItem();
            pCreature->SetSuspendAnimation(false, 0);
        }
        CTCCreatureModeManager* pModes = 0;
        PCreature.GetPItem()->GetInterface(&pModes, 0x31);
        pModes->RemoveMode(67);
        CCombatActionBase::Initialise();
    }
};
int main()
{
    CTCCreatureModeManager modes; CThing creature; creature.modes = &modes;
    CRecoverFromDazed a; a.PCreature.m_pItem = &creature;
    a.Initialise();                                      // not the player: animation untouched
    if (creature.suspendCalls != 0 || modes.removed[0] != 67 || g_baseInit != 1) { std::printf("008ed8c0_TEST FAIL\n"); return 1; }
    creature.m_type = 1;
    a.Initialise();
    if (creature.suspendCalls != 1 || creature.lastSuspend || modes.count != 2 || g_baseInit != 2) { std::printf("008ed8c0_TEST FAIL\n"); return 1; }
    std::printf("008ed8c0_TEST PASS\n");
    return 0;
}