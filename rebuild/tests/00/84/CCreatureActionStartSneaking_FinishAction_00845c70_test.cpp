// Behaviour test for 0x00845C70 CCreatureAction_StartSneaking::FinishAction: mode 7 added, then the base FinishAction.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
class CTCCreatureModeManager { public: unsigned int m_mode; bool AddMode(unsigned int mode) { m_mode = mode; Log(1); return true; } };
class CThing { public:
    unsigned int m_presentBits[9];
    CTCCreatureModeManager* m_modes;
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_modes; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature;
    void FinishAction()
    {
        CTCCreatureModeManager* pModes = 0;
        PCreature.GetPItem()->GetInterface(&pModes, 0x31);
        pModes->AddMode(7);
        Log(2);
    }
};
int main()
{
    CTCCreatureModeManager modes; modes.m_mode = 0;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_presentBits[0x31 >> 5] |= 1u << (0x31 & 31); me.m_modes = &modes;
    CAction a; a.PCreature.m_pItem = &me; a.FinishAction();
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 2 || modes.m_mode != 7) { std::printf("00845c70_TEST FAIL\n"); return 1; }
    std::printf("00845c70_TEST PASS\n");
    return 0;
}