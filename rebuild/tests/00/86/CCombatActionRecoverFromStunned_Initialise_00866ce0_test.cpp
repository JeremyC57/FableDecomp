// Behaviour test for 0x00866CE0 CCombatAction_RecoverFromStunned::Initialise.
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
static int g_recoverBaseInit = 0;
class CCombatActionBase { public:
    CBaseIntelligentPointer PCreature;
};
struct CKnockdownRecoverBase : CCombatActionBase {
    void Initialise() { ++g_recoverBaseInit; }
};
struct CRecoverFromStunned : CKnockdownRecoverBase {
    void Initialise()
    {
        if (PCreature.GetPItem()->m_type == 1) {
            CThing* pCreature = PCreature.GetPItem();
            pCreature->SetSuspendAnimation(false, 0);
        }
        ++PCreature.GetPItem()->physicsResets;           // GetPhysicsTC()->ResetMovement() (slot 44)
        CTCCreatureModeManager* pModes = 0;
        PCreature.GetPItem()->GetInterface(&pModes, 0x31);
        pModes->RemoveMode(29);
        CKnockdownRecoverBase::Initialise();
    }
};
int main()
{
    CTCCreatureModeManager modes; CThing creature; creature.modes = &modes; creature.m_type = 1;
    CRecoverFromStunned a; a.PCreature.m_pItem = &creature;
    a.Initialise();
    if (creature.suspendCalls != 1 || creature.lastSuspend || creature.physicsResets != 1 || modes.count != 1 ||
        modes.removed[0] != 29 || g_recoverBaseInit != 1) { std::printf("00866ce0_TEST FAIL\n"); return 1; }
    std::printf("00866ce0_TEST PASS\n");
    return 0;
}