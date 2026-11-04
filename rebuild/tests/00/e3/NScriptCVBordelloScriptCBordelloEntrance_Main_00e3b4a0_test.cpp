#include <cstdio>
#include <cstring>
static char g_log[8192];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CBordelloEntrance::Main keeps the door locked and shows "closed" on use until the escort quest is done, then sets the sign text and unlocks
static int g_questAt = 6, g_usedAt = 2, g_clickAt = 4;
class CScriptThing { public:
    const char* Name;
    CScriptThing(const char* n = "") : Name(n) { }
    bool MsgIsUsedBy(const char* who) const { (void)who; return g_frame == g_usedAt; }
};
class CGame { public:
    void NewScriptFrame() const { ++g_frame; }
    bool MsgIsGameInfoClickedPast() const { return g_frame >= g_clickAt; }
    CScriptThing GetThingWithScriptName(const char* n) const { return CScriptThing(n); }
    void DisplayGameInfo(const char* t) const { Log(t); }
    bool IsQuestCompleted(const char*) const { return g_frame >= g_questAt; }
    void EntitySetAsLocked(const CScriptThing& t, bool l) const { char x[64]; std::sprintf(x, "lock %s %d", t.Name, (int)l); Log(x); }
    void SetReadableObjectTextTag(const CScriptThing& t, const char* tag) const { char x[96]; std::sprintf(x, "tag %s %s", t.Name, tag); Log(x); }
};
struct CV_BordelloScript { bool BecomeNunnery; };
struct CBordelloEntrance {
    CGame* Game; CScriptThing Me; CV_BordelloScript* ParentClass;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void Main();
};
void CBordelloEntrance::Main()
{
    Game->EntitySetAsLocked(Me, true);
    while (!Game->IsQuestCompleted("Q_TraderEscort")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_B13_BORDELLO_CLOSED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass->BecomeNunnery) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BordelloSign"), "TEXT_QST_B13_BORDELLO_OPEN");
        Game->EntitySetAsLocked(Me, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BordelloSign"), "TEXT_QST_B13_BORDELLO_REFUGE");
        Game->EntitySetAsLocked(Me, false);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
int main()
{
    CGame game; CV_BordelloScript parent; CBordelloEntrance e; e.Game = &game; e.Me = CScriptThing("DOOR"); e.ParentClass = &parent;
    Reset(10); parent.BecomeNunnery = false;
    e.Main();
    CHECK("00e3b4a0", std::strcmp(g_log, "lock DOOR 1@0;TEXT_QST_B13_BORDELLO_CLOSED@2;tag BordelloSign TEXT_QST_B13_BORDELLO_OPEN@6;lock DOOR 0@6;") == 0 && g_frame == 10);
    Reset(10); parent.BecomeNunnery = true;
    e.Main();
    CHECK("00e3b4a0", std::strcmp(g_log, "lock DOOR 1@0;TEXT_QST_B13_BORDELLO_CLOSED@2;tag BordelloSign TEXT_QST_B13_BORDELLO_REFUGE@6;lock DOOR 0@6;") == 0);
    Reset(3);
    e.Main();
    CHECK("00e3b4a0", std::strcmp(g_log, "lock DOOR 1@0;TEXT_QST_B13_BORDELLO_CLOSED@2;") == 0 && g_frame == 3);
    std::printf("00e3b4a0_TEST PASS\n");
    return 0;
}