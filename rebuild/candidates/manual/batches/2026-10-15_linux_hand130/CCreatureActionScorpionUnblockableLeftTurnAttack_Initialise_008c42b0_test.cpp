// Behaviour test for 0x008c42b0: the scorpion unblockable-attack hook notifies the live target's shield spell (type 0x93).
#include <cstdio>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
struct CThing;
struct CTCPhysicalShieldSpell {
    CThing* m_attacker; int m_starting;
    void NotifyUnblockableAttack(CThing* attacker, bool starting) { m_attacker = attacker; m_starting = starting ? 1 : 0; Log(10 + m_starting); }
};
struct CThing {
    unsigned int m_presentBits[9];
    CTCPhysicalShieldSpell* m_shield;
    unsigned char m_flags91;
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_shield; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; }
    return false;
}
struct CAction {
    CBaseIntelligentPointer PCreature, PTarget;
    void Hook()
    {
#if 1
        if (IS_THING_ALIVE(PTarget)) {
            CTCPhysicalShieldSpell* pShield;
            if (PTarget.GetPItem()->GetInterface(&pShield, 0x93)) pShield->NotifyUnblockableAttack(PCreature.GetPItem(), true);
        }
        Log(1);   // base Initialise
#else
        Log(1);   // base FinishAction
        if (IS_THING_ALIVE(PTarget)) {
            CTCPhysicalShieldSpell* pShield;
            if (PTarget.GetPItem()->GetInterface(&pShield, 0x93)) pShield->NotifyUnblockableAttack(PCreature.GetPItem(), false);
        }
#endif
    }
};
static int Fail() { std::printf("008c42b0_TEST FAIL\n"); return 1; }
int main()
{
    CTCPhysicalShieldSpell shield; shield.m_attacker = 0; shield.m_starting = -1;
    CThing me, target;
    for (int i = 0; i < 9; ++i) target.m_presentBits[i] = 0;
    target.m_presentBits[0x93 >> 5] |= 1u << (0x93 & 31);
    target.m_shield = &shield; target.m_flags91 = 0;
    CAction a; a.PCreature.m_pItem = &me; a.PTarget.m_pItem = &target;
    a.Hook();
    if (g_n != 2 || shield.m_attacker != &me || shield.m_starting != 1) return Fail();
    if (((1) && (g_log[0] != 11 || g_log[1] != 1)) || ((!1) && (g_log[0] != 1 || g_log[1] != 10))) return Fail();
    g_n = 0; target.m_flags91 = 1; a.Hook();                  // dead target: base call only
    if (g_n != 1 || g_log[0] != 1) return Fail();
    g_n = 0; target.m_flags91 = 0; target.m_presentBits[0x93 >> 5] = 0; a.Hook();   // no shield component
    if (g_n != 1) return Fail();
    g_n = 0; a.PTarget.m_pItem = 0; a.Hook();                 // no target
    if (g_n != 1) return Fail();
    std::printf("008c42b0_TEST PASS\n");
    return 0;
}
