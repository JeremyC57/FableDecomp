#include <cstdio>
// Behaviour test for CQ_TraderEscortScript::WatchForPickpocketing (0x00E04F10): each pickpocket of the watched trader
// bumps the count and ends the thread (count > 2 sets the master-data state to 2); the trader's death resets it to 0.
struct CScriptThing {
    int Id; bool* Killed;
    CScriptThing() : Id(0), Killed(0) {}
    bool operator==(const CScriptThing& o) const { return Id == o.Id; }
    bool MsgIsKilledBy(const char*) const { return Killed && *Killed; }
};
struct CGSI {
    int Frames; int PickFrame; int PickId; CScriptThing* Trader; int KillFrame;
    void NewScriptFrame() { ++Frames; if (Frames == KillFrame) *Trader->Killed = true; }
    bool MsgOnHeroPickedPocket(CScriptThing& victim) { if (Frames == PickFrame) { victim.Id = PickId; return true; } return false; }
};
struct CData { long DarkwoodPickpocketedAllTraders; };
struct CQ_TraderEscortScript {
    CGSI* GSI; CData* PMasterData; long TraderPickpocketedCount;
    bool IsActiveThreadTerminating() { return GSI->Frames > 50; }
    void WatchForPickpocketing(CScriptThing trader)
    {
        CScriptThing victim;
        for (;;) {
            if (IsActiveThreadTerminating())
                return;
            if (GSI->MsgOnHeroPickedPocket(victim)) {
                if (IsActiveThreadTerminating())
                    return;
                if (victim == trader) {
                    if (IsActiveThreadTerminating())
                        return;
                    ++TraderPickpocketedCount;
                    if (IsActiveThreadTerminating())
                        return;
                    if (TraderPickpocketedCount > 2) {
                        if (IsActiveThreadTerminating())
                            return;
                        PMasterData->DarkwoodPickpocketedAllTraders = 2;
                    }
                    return;
                }
            }
            if (trader.MsgIsKilledBy("")) {
                if (IsActiveThreadTerminating())
                    return;
                PMasterData->DarkwoodPickpocketedAllTraders = 0;
                return;
            }
            GSI->NewScriptFrame();
        }
    }
};
int main()
{
    bool dead = false; CScriptThing trader; trader.Id = 7; trader.Killed = &dead;
    CGSI gsi; gsi.Frames = 0; gsi.PickFrame = 3; gsi.PickId = 7; gsi.Trader = &trader; gsi.KillFrame = 100;
    CData data; data.DarkwoodPickpocketedAllTraders = 1;
    CQ_TraderEscortScript s; s.GSI = &gsi; s.PMasterData = &data; s.TraderPickpocketedCount = 2;
    s.WatchForPickpocketing(trader);
    bool ok = s.TraderPickpocketedCount == 3 && data.DarkwoodPickpocketedAllTraders == 2 && gsi.Frames == 3;
    gsi.Frames = 0; gsi.PickId = 8; gsi.KillFrame = 5; data.DarkwoodPickpocketedAllTraders = 1;
    s.WatchForPickpocketing(trader);
    ok = ok && s.TraderPickpocketedCount == 3 && data.DarkwoodPickpocketedAllTraders == 0 && gsi.Frames == 5;
    if (!ok) { std::printf("00e04f10_TEST FAIL\n"); return 1; }
    std::printf("00e04f10_TEST PASS\n");
    return 0;
}
