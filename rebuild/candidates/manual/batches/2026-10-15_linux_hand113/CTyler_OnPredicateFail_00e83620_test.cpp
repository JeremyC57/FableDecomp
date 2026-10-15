#include <cstdio>
// Behaviour test for CTyler::OnPredicateFail (0x00E83620): optional fight hand-over, then cleanup or release.
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
    CGameScriptInterface gsi; gsi.released = 0; gsi.ended = 0; gsi.fightThing = 0; gsi.fightTarget = 0;
    CV_FisticuffsClubScript club; club.FightInProgress = false; club.Opponent = 12;
    CTyler t; t.m_pGSI = &gsi; t.ParentClass = &club; t.Me.alive = false; t.Me.available = false;
    t.OnPredicateFail();                       // dead thing, no fight: only the release
    if (g_n != 1 || g_log[0] != 338 || gsi.released != 12 || gsi.fightThing != 0) { std::printf("00e83620_TEST FAIL\n"); return 1; }
    g_n = 0; t.Me.alive = true; club.FightInProgress = true; gsi.released = 0;
    t.OnPredicateFail();                       // live thing, fight running: hand-over then full cleanup
    if (gsi.fightThing != &t.Me || gsi.fightTarget != 5 || club.FightInProgress || gsi.ended != &t.Me || gsi.released != 12) { std::printf("00e83620_TEST FAIL\n"); return 1; }
    if (g_n != 5 || g_log[0] != 70 || g_log[1] != 451 || g_log[2] != 338 || g_log[3] != 67 || g_log[4] != 452) { std::printf("00e83620_TEST FAIL\n"); return 1; }
    std::printf("00e83620_TEST PASS\n");
    return 0;
}
