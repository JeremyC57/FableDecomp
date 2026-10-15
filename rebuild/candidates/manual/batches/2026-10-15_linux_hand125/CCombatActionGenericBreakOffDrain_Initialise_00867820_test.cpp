// Behaviour test for 0x00867820 CCombatAction_GenericBreakOffDrain::Initialise: BREAK_OUT animation, mode 69 removed, base Initialise.
#include <cstdio>
#include <cstring>
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
struct CCharString {
    const char* text;
    CCharString(const char* t, long) : text(t) { }
};
static char g_anim[32]; static int g_baseInit = 0;
struct CCombatActionBase {
    CBaseIntelligentPointer PCreature;
    void SetCombatAnimation(const CCharString& name) { std::strcpy(g_anim, name.text); }
    void Initialise() { ++g_baseInit; }
};
struct CBreakOffDrain : CCombatActionBase {
    void Initialise()
    {
        {
            CCharString name("BREAK_OUT", -1);
            SetCombatAnimation(name);
        }
        CTCCreatureModeManager* pModes = 0;
        PCreature.GetPItem()->GetInterface(&pModes, 0x31);
        pModes->RemoveMode(69);                          // retail does not test the lookup (the creature always has one)
        CCombatActionBase::Initialise();
    }
};
int main()
{
    CTCCreatureModeManager modes; CThing creature; creature.modes = &modes;
    CBreakOffDrain a; a.PCreature.m_pItem = &creature;
    a.Initialise();
    if (std::strcmp(g_anim, "BREAK_OUT") != 0 || modes.count != 1 || modes.removed[0] != 69 || g_baseInit != 1) {
        std::printf("00867820_TEST FAIL\n"); return 1;
    }
    std::printf("00867820_TEST PASS\n");
    return 0;
}
