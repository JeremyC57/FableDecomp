// Behaviour test for 0x0069F670 CCreatureAction_SheatheItemToInventory::FinishAction.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
class CTCCreatureModeManager { public: void UpdateFrontMode() { Log(6); } };
struct CAction;
class CThing { public:
    unsigned int m_presentBits[9];
    CTCCreatureModeManager* m_modes;
    bool m_accept;
    bool SetCurrentAction(CAction*) { Log(4); return m_accept; }
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_modes; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature;
    bool PerformedAction;
    CAction* m_pNextAction;
    virtual ~CAction() { }
    virtual void PerformAction() { PerformedAction = true; Log(1); }
    virtual void Cancel() { Log(5); }
    bool GetPerformedAction() const { return PerformedAction; }
    void FinishAction()
    {
        if (!GetPerformedAction()) PerformAction();
        Log(2);   // CCreatureActionBase::FinishAction
        if (m_pNextAction) {
            if (!PCreature.GetPItem()->SetCurrentAction(m_pNextAction)) m_pNextAction->Cancel();
        }
        CTCCreatureModeManager* pModes;
        if (PCreature.GetPItem()->GetInterface(&pModes, 0x31)) pModes->UpdateFrontMode();
    }
};
static int Fail() { std::printf("0069f670_TEST FAIL\n"); return 1; }
int main()
{
    CTCCreatureModeManager modes;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_modes = &modes; me.m_accept = false;
    CAction next; next.PerformedAction = true; next.m_pNextAction = 0;
    CAction a; a.PCreature.m_pItem = &me; a.PerformedAction = false; a.m_pNextAction = &next;
    a.FinishAction();   // performs, refused follow-up gets cancelled, no mode manager
    if (g_n != 4 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 4 || g_log[3] != 5) return Fail();
    g_n = 0; me.m_accept = true; me.m_presentBits[0x31 >> 5] |= 1u << (0x31 & 31);
    a.FinishAction();   // already performed, follow-up accepted, front mode run
    if (g_n != 3 || g_log[0] != 2 || g_log[1] != 4 || g_log[2] != 6) return Fail();
    g_n = 0; a.m_pNextAction = 0; a.FinishAction();
    if (g_n != 2 || g_log[0] != 2 || g_log[1] != 6) return Fail();
    std::printf("0069f670_TEST PASS\n");
    return 0;
}