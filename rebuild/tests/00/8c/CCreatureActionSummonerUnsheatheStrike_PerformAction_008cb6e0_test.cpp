// Behaviour test for 0x008CB6E0 CCreatureAction_SummonerUnsheatheStrike::PerformAction.
#include <cstdio>
#include <cstring>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
struct CCharString { char m_text[40]; CCharString(const char* t, int) { std::strcpy(m_text, t); } ~CCharString() { Log(8); } };
struct CDefinitionManager {
    int GetDefGlobalIndexFromName(const CCharString& name) { return std::strcmp(name.m_text, "CARRY_SLOT_PRIMARY_SHEATHE") == 0 ? 42 : -1; }
};
static CDefinitionManager g_defs;
struct CThing {
    unsigned int m_presentBits[9];
    void* m_carrying;
    unsigned char m_flags91;
    int m_killed;
    CDefinitionManager* GetDefinitionManager() { return &g_defs; }
    void Kill(bool immediately) { m_killed = immediately ? 1 : 2; Log(3); }
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_carrying; return true; }
        return false;
    }
};
struct CTCCarrying {
    int m_slot; CThing* m_inSlot;
    CThing* RemoveThingInCarrySlot(int slot, bool drop) { m_slot = slot; Log(drop ? 9 : 2); CThing* t = m_inSlot; m_inSlot = 0; return t; }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature;
    void PerformAction()
    {
        CTCCarrying* pCarrying;
        if (PCreature.GetPItem()->GetInterface(&pCarrying, 0x46)) {
            int slot;
            {
                CCharString slotName("CARRY_SLOT_PRIMARY_SHEATHE", -1);
                slot = PCreature.GetPItem()->GetDefinitionManager()->GetDefGlobalIndexFromName(slotName);
            }
            CThing* sheathed = pCarrying->RemoveThingInCarrySlot(slot, false);
            if (sheathed != 0 && !(sheathed->m_flags91 & 1)) sheathed->Kill(true);
        }
        Log(1);   // CCreatureAction_UnsheatheWeapons::PerformAction
    }
};
static int Fail() { std::printf("008cb6e0_TEST FAIL\n"); return 1; }
int main()
{
    CThing sword; sword.m_flags91 = 0; sword.m_killed = 0;
    CTCCarrying carrying; carrying.m_slot = 0; carrying.m_inSlot = &sword;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_carrying = &carrying;
    CAction a; a.PCreature.m_pItem = &me;
    a.PerformAction();
    if (g_n != 1 || g_log[0] != 1 || sword.m_killed != 0) return Fail();
    me.m_presentBits[0x46 >> 5] |= 1u << (0x46 & 31);
    g_n = 0; a.PerformAction();
    if (g_n != 4 || g_log[0] != 8 || g_log[1] != 2 || g_log[2] != 3 || g_log[3] != 1 || carrying.m_slot != 42 || sword.m_killed != 1) return Fail();
    g_n = 0; a.PerformAction();   // slot now empty: nothing to kill
    if (g_n != 3 || g_log[2] != 1) return Fail();
    std::printf("008cb6e0_TEST PASS\n");
    return 0;
}