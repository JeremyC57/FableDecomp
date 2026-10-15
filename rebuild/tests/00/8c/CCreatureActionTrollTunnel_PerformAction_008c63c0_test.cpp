// Behaviour test for 0x008C63C0 CCreatureAction_TrollTunnel::PerformAction.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
struct CTCCreatureModeManager { unsigned int m_mode; bool AddMode(unsigned int mode) { m_mode = mode; Log(2); return true; } };
struct CPlayerGui { int m_flag; void Refresh(int flag) { m_flag = flag; Log(3); } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
struct CThing {
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
    void WhackGround() { Log(1); }
    void PerformAction()
    {
        WhackGround();
        CTCCreatureModeManager* pModes = 0;
        PCreature.GetPItem()->GetInterface(&pModes, 0x31);
        pModes->AddMode(0x1e);
        g_pPlayerGui->Refresh(0);
    }
};
int main()
{
    CTCCreatureModeManager modes; modes.m_mode = 0; g_gui.m_flag = 5;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_presentBits[0x31 >> 5] |= 1u << (0x31 & 31); me.m_modes = &modes;
    CAction a; a.PCreature.m_pItem = &me; a.PerformAction();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || modes.m_mode != 0x1e || g_gui.m_flag != 0) { std::printf("008c63c0_TEST FAIL\n"); return 1; }
    std::printf("008c63c0_TEST PASS\n");
    return 0;
}