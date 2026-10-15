// Behaviour test for 0x008C0B70 CCreatureAction_ThunderLightningStorm::Initialise.
#include <cstdio>
#include <cstring>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
struct CCharString { char m_text[32]; CCharString(const char* t, int) { std::strcpy(m_text, t); } };
struct CTCCreatureModeManager { unsigned int m_mode; bool AddMode(unsigned int mode) { m_mode = mode; Log(3); return true; } };
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
    CBaseIntelligentPointer PCreature; char m_name[32];
    void SetName(const CCharString& n) { std::strcpy(m_name, n.m_text); Log(1); }
    void Initialise()
    {
        { CCharString name("LIGHTNING_STORM_INTO", -1); SetName(name); }
        Log(2);   // CCombatActionBase::Initialise
        CTCCreatureModeManager* pModes = 0;
        PCreature.GetPItem()->GetInterface(&pModes, 0x31);
        pModes->AddMode(0x36);
    }
};
int main()
{
    CTCCreatureModeManager modes; modes.m_mode = 0;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_presentBits[0x31 >> 5] |= 1u << (0x31 & 31); me.m_modes = &modes;
    CAction a; a.PCreature.m_pItem = &me; a.Initialise();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || modes.m_mode != 0x36 || std::strcmp(a.m_name, "LIGHTNING_STORM_INTO") != 0)
    { std::printf("008c0b70_TEST FAIL\n"); return 1; }
    std::printf("008c0b70_TEST PASS\n");
    return 0;
}