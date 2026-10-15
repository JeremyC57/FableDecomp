#include <cstdio>
// Behaviour test for CTyler::CleanupAfterFight (0x00E83680): release, flag cleared, thing re-enabled, fight ended.
static int g_log[16]; static int g_n = 0;
struct CScriptThing {
    bool alive; bool available;
    virtual bool IsAlive() { return alive; }
    virtual void SetAvailable(bool a) { available = a; g_log[g_n++] = 67; }
};
struct CGameScriptInterface {
    long released; const CScriptThing* fightThing; long fightTarget; const CScriptThing* ended;
    virtual long GetHero() { g_log[g_n++] = 70; return 5; }
    virtual void ReleaseCreature(long c) { released = c; g_log[g_n++] = 338; }
    virtual void SetFightTarget(const CScriptThing& t, long target) { fightThing = &t; fightTarget = target; g_log[g_n++] = 451; }
    virtual void EndFight(const CScriptThing& t) { ended = &t; g_log[g_n++] = 452; }
};
struct CV_FisticuffsClubScript { bool FightInProgress; long Opponent; };
struct CTyler {
    CGameScriptInterface* m_pGSI; CScriptThing Me; CV_FisticuffsClubScript* ParentClass;
    void CleanupAfterFight()
    {
        m_pGSI->ReleaseCreature(ParentClass->Opponent);
        ParentClass->FightInProgress = false;
        Me.SetAvailable(true);
        m_pGSI->EndFight(Me);
    }
    void OnPredicateFail()
    {
        if (Me.IsAlive())
            m_pGSI->SetFightTarget(Me, m_pGSI->GetHero());
        if (ParentClass->FightInProgress)
            CleanupAfterFight();
        else
            m_pGSI->ReleaseCreature(ParentClass->Opponent);
    }
};
int main()
{
    CGameScriptInterface gsi; gsi.released = 0; gsi.ended = 0;
    CV_FisticuffsClubScript club; club.FightInProgress = true; club.Opponent = 33;
    CTyler t; t.m_pGSI = &gsi; t.ParentClass = &club; t.Me.alive = true; t.Me.available = false;
    t.CleanupAfterFight();
    if (gsi.released != 33 || club.FightInProgress || !t.Me.available || gsi.ended != &t.Me) { std::printf("00e83680_TEST FAIL\n"); return 1; }
    if (g_n != 3 || g_log[0] != 338 || g_log[1] != 67 || g_log[2] != 452) { std::printf("00e83680_TEST FAIL\n"); return 1; }
    std::printf("00e83680_TEST PASS\n");
    return 0;
}