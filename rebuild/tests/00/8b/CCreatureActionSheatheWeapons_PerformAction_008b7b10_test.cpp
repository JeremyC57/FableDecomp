// Behaviour test for 0x008B7B10 CCreatureAction_SheatheWeapons::PerformAction.
#include <cstdio>
#include <cstring>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
class CCharString { public:
    const char* m_text;
    CCharString(const char* text, long) : m_text(text) { }
    ~CCharString() { Log(9); }
};
class CTCCreatureModeManager { public: unsigned int m_removed; void RemoveMode(unsigned int mode) { m_removed = mode; Log(2); } };
class CThing { public:
    unsigned int m_presentBits[9];
    CTCCreatureModeManager* m_modes;
    char m_noise[32];
    void SheatheAllWeapons() { Log(1); }
    void MakeNoise(const CCharString& noise) { std::strcpy(m_noise, noise.m_text); Log(3); }
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_modes; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature;
    void PerformAction()
    {
        PCreature.GetPItem()->SheatheAllWeapons();
        CTCCreatureModeManager* pModes;
        if (PCreature.GetPItem()->GetInterface(&pModes, 0x31)) pModes->RemoveMode(4);
        {
            CCharString noise("SIMPLE_NOISE", -1);
            PCreature.GetPItem()->MakeNoise(noise);
        }
    }
};
static int Fail() { std::printf("008b7b10_TEST FAIL\n"); return 1; }
int main()
{
    CTCCreatureModeManager modes; modes.m_removed = 0;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_presentBits[0x31 >> 5] |= 1u << (0x31 & 31); me.m_modes = &modes; me.m_noise[0] = 0;
    CAction a; a.PCreature.m_pItem = &me;
    a.PerformAction();
    if (g_n != 4 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_log[3] != 9 || modes.m_removed != 4
        || std::strcmp(me.m_noise, "SIMPLE_NOISE") != 0) return Fail();
    g_n = 0; me.m_presentBits[0x31 >> 5] = 0; a.PerformAction();
    if (g_n != 3 || g_log[1] != 3) return Fail();
    std::printf("008b7b10_TEST PASS\n");
    return 0;
}