// Behaviour test for 00691560: body copied from src/00691560.cpp (CInputProcessBetting::ProcessInput), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CInputEvent {};
class CProcessedInput {};
class CInputProcessBetting;
static int g_procCalls = 0; static const void* g_procEvent = 0; static const void* g_procProcessed = 0; static CInputProcessBetting* g_procProcess = 0;
class CTCBase { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual void v26() {} virtual void v27() {}
    virtual void ProcessBettingInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessBetting* process) { ++g_procCalls; g_procEvent = &event; g_procProcessed = &processed; g_procProcess = process; }
};
struct STCEntry { long m_kind; CTCBase* m_pTC; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) { STCEntry* it = m_pFirst; while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it; return it; }
    __forceinline CTCBase* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];
    unsigned char _pad40[4];
    CTCMap        m_tcs;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CPlayer { public: CThing* hero; CThing* GetHeroCreature() { return hero; } };
class CPlayerManager { public: CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
class CMainGameComponent { public: unsigned char _pad[0x1c]; CPlayerManager* PlayerManager; };
static CMainGameComponent g_component; CMainGameComponent* g_pMainGameComponent = &g_component;
bool g_bBettingInputEnabled = false;
class CTCBetting : public CTCBase { public:
    void ProcessInputIfEnabled(const CInputEvent& event, CProcessedInput& processed, CInputProcessBetting* process)
    {
        if (g_bBettingInputEnabled)
            ProcessBettingInput(event, processed, process);
    }
};
class CInputProcessBetting {
public:
    virtual void v00() {}
    unsigned char _base04[0x24];
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};
void CInputProcessBetting::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroCreature();
    if (hero->HasTC(0x102)) {
        static_cast<CTCBetting*>(hero->GetTC(0x102))->ProcessInputIfEnabled(event, processed, this);
    }
}
int main()
{
    CThing hero; memset(&hero, 0, sizeof(hero)); CTCBetting betting;
    STCEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x102, &betting } };
    hero.m_tcs.m_pFirst = entries; hero.m_tcs.m_pLast = entries + 2;
    CPlayerManager pm; pm.player.hero = &hero; g_component.PlayerManager = &pm;
    CInputProcessBetting ipb; CInputEvent ev; CProcessedInput pi;
    // 1. hero without the betting component: nothing
    ipb.ProcessInput(ev, pi); CHECK(g_procCalls == 0);
    // 2. component present but betting input disabled: nothing
    hero.m_tcMask[0x102 >> 5] = 1UL << (0x102 & 31); ipb.ProcessInput(ev, pi); CHECK(g_procCalls == 0);
    // 3. enabled: forwarded with the event, the processed input and this process
    g_bBettingInputEnabled = true; ipb.ProcessInput(ev, pi); CHECK(g_procCalls == 1); CHECK(g_procEvent == &ev); CHECK(g_procProcessed == &pi); CHECK(g_procProcess == &ipb);
    if (fails == 0) printf("00691560_TEST PASS\n"); else printf("00691560_TEST FAIL\n");
    return fails;
}