// Behaviour test for 0x008CF3D0 CCreatureAction_TentacleStrikeInto::Initialise.
#include <cstdio>
#include <cstring>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
class CCharString { public: char m_text[32]; CCharString(const char* t, int) { std::strcpy(m_text, t); } };
class CTCKrakenTentacle { public: int m_striking; void SetIsStriking(bool s) { m_striking = s ? 1 : 0; Log(2); } };
class CThing { public:
    unsigned int m_presentBits[9];
    CTCKrakenTentacle* m_tentacle;
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_tentacle; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature; char m_name[32]; CTCKrakenTentacle* PTentacle;
    void SetName(const CCharString& n) { std::strcpy(m_name, n.m_text); Log(1); }
    void Initialise()
    {
        PCreature.GetPItem()->GetInterface(&PTentacle, 0xe3);
        { CCharString name("TENTACLE_SMASH_INTO", -1); SetName(name); }
        PTentacle->SetIsStriking(true);
        Log(3);   // CCombatActionBase::Initialise
    }
};
int main()
{
    CTCKrakenTentacle tentacle; tentacle.m_striking = 0;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_presentBits[0xe3 >> 5] |= 1u << (0xe3 & 31); me.m_tentacle = &tentacle;
    CAction a; a.PCreature.m_pItem = &me; a.PTentacle = 0;
    a.Initialise();
    if (a.PTentacle != &tentacle || tentacle.m_striking != 1 || std::strcmp(a.m_name, "TENTACLE_SMASH_INTO") != 0
        || g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3) { std::printf("008cf3d0_TEST FAIL\n"); return 1; }
    std::printf("008cf3d0_TEST PASS\n");
    return 0;
}