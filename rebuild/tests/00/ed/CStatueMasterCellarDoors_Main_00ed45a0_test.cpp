// Behaviour test for CStatueMasterCellarDoors::Main (0x00ED45A0): statue at 1 with the trader dead unlocks the door;
// otherwise it locks it and shows the locked message each time the hero uses it, until the script is done.
#include <cstdio>
#include <cstring>
struct CCharString {
    char Text[48];
    CCharString(const char* text) { std::strncpy(Text, text, 47); Text[47] = 0; }
};
static int g_frames = 0;
static int g_thingsAlive = 0;
struct CScriptThing {
    bool alive; int usedAt;
    CScriptThing(bool a) : alive(a), usedAt(-1) { ++g_thingsAlive; }
    CScriptThing(const CScriptThing& o) : alive(o.alive), usedAt(o.usedAt) { ++g_thingsAlive; }
    ~CScriptThing() { --g_thingsAlive; }
    bool IsAlive() const { return alive; }
    bool MsgIsUsedBy(const CCharString& user) const { return std::strcmp(user.Text, "SCRIPT_NAME_HERO") == 0 && g_frames == usedAt; }
};
struct CGameScriptInterface {
    bool traderAlive; int locks, unlocks, infos, clickAt;
    void NewScriptFrame() { ++g_frames; }
    bool MsgIsGameInfoClickedPast() { return g_frames >= clickAt; }
    CScriptThing GetThingWithScriptName(const CCharString& name) { return CScriptThing(std::strcmp(name.Text, "TraderToEscort") == 0 && traderAlive); }
    void DisplayGameInfo(const CCharString& text) { if (std::strcmp(text.Text, "TEXT_QST_061_CELLAR_LOCKED") == 0) ++infos; }
    void EntitySetAsLocked(const CScriptThing&, bool locked) { if (locked) ++locks; else ++unlocks; }
};
struct CV_StatueMasterScript { long pos; long GetStatuePointingPosition() { return pos; } };
struct CStatueMasterCellarDoors {
    CGameScriptInterface* Game; CScriptThing Me; CV_StatueMasterScript* ParentClass; int doneAt;
    CStatueMasterCellarDoors() : Me(true) { }
    bool IsDone() { return g_frames >= doneAt; }
    void Main();
};
void CStatueMasterCellarDoors::Main()
{
    bool unlocked = ParentClass->GetStatuePointingPosition() == 1
                 && !Game->GetThingWithScriptName("TraderToEscort").IsAlive();
    if (unlocked) {
        if (IsDone())
            return;
        Game->EntitySetAsLocked(Me, false);
        return;
    }
    if (IsDone())
        return;
    Game->EntitySetAsLocked(Me, true);
    if (IsDone())
        return;
    for (;;) {
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsDone())
                return;
            Game->DisplayGameInfo("TEXT_QST_061_CELLAR_LOCKED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsDone())
                    return;
            }
            if (IsDone())
                return;
        }
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
}
static bool Run(long pos, bool traderAlive, int doneAt, int locks, int unlocks, int infos, int frames)
{
    g_frames = 0;
    CGameScriptInterface g; std::memset(&g, 0, sizeof(g)); g.traderAlive = traderAlive; g.clickAt = 6;
    CV_StatueMasterScript p; p.pos = pos;
    CStatueMasterCellarDoors d; d.Game = &g; d.ParentClass = &p; d.doneAt = doneAt; d.Me.usedAt = 3;
    d.Main();
    return g.locks == locks && g.unlocks == unlocks && g.infos == infos && g_frames == frames && g_thingsAlive == 1;
}
int main()
{
    bool ok = Run(1, false, 100, 0, 1, 0, 0) && Run(1, true, 10, 1, 0, 1, 10) && Run(2, false, 10, 1, 0, 1, 10)
           && Run(2, false, 2, 1, 0, 0, 2);
    if (!ok) { std::printf("00ed45a0_TEST FAIL\n"); return 1; }
    std::printf("00ed45a0_TEST PASS\n");
    return 0;
}