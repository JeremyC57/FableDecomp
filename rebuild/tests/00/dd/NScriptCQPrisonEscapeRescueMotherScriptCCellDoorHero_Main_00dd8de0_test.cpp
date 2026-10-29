// Behaviour test for 0x00DD8DE0 NScript::CQ_PrisonEscapeRescueMotherScript::CCellDoorHero::Main: the cell door is untargetable
// until the prison key is stolen, then targetable; when the hero uses it, HeroEscapedCell is set, the door opens and
// Q_PrisonEscapeFromCell is activated. A terminating thread returns.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
struct CQ_SunnyvaleMasterData { bool PrisonKeyStolenByHero; };
class CScriptThing { public:
    int UsedAt; int* Frames;
    bool MsgIsUsedBy(const CCharString& n) const { return !std::strcmp(n.Text, "SCRIPT_NAME_HERO") && *Frames >= UsedAt; }
};
class CGameScriptInterface { public:
    int Frames; int KeyAt; CQ_SunnyvaleMasterData* Data;
    void NewScriptFrame() { ++Frames; if (Frames >= KeyAt) Data->PrisonKeyStolenByHero = true; }
    void ActivateQuest(const CCharString& q) const { char x[64]; std::sprintf(x, "activate(%s)", q.Text); Log(x); }
    void OpenDoor(const CScriptThing&) const { Log("open"); }
    void EntitySetTargetable(const CScriptThing&, bool t) const { Log(t ? "targetable" : "untargetable"); }
};
struct CQ_PrisonEscapeRescueMotherScript { bool HeroEscapedCell; };
struct CCellDoorHero {
    CGameScriptInterface* Game; CScriptThing Me; CQ_PrisonEscapeRescueMotherScript* ParentClass; CQ_SunnyvaleMasterData* PMasterData;
    int TermAt, TermCalls;
    bool IsActiveThreadTerminating() { return TermAt && ++TermCalls >= TermAt; }
    void Main()
    {
        Game->EntitySetTargetable(Me, false);
        while (!PMasterData->PrisonKeyStolenByHero) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetTargetable(Me, true);
        if (IsActiveThreadTerminating())
            return;
        do {
            if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->HeroEscapedCell = true;
                Game->OpenDoor(Me);
                Game->ActivateQuest("Q_PrisonEscapeFromCell");
                return;
            }
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
};

int main()
{
    CQ_SunnyvaleMasterData data; data.PrisonKeyStolenByHero = false;
    CGameScriptInterface game; game.Frames = 0; game.KeyAt = 3; game.Data = &data;
    CQ_PrisonEscapeRescueMotherScript quest; quest.HeroEscapedCell = false;
    CCellDoorHero d; d.Game = &game; d.Me.UsedAt = 6; d.Me.Frames = &game.Frames; d.ParentClass = &quest; d.PMasterData = &data;
    d.TermAt = 0; d.TermCalls = 0;
    d.Main();
    int ok = !std::strcmp(g_log, "untargetable;targetable;open;activate(Q_PrisonEscapeFromCell);") && quest.HeroEscapedCell && game.Frames == 6;
    // the thread terminates while waiting for the hero: nothing after the targetable switch
    g_log[0] = 0; data.PrisonKeyStolenByHero = false; quest.HeroEscapedCell = false; game.Frames = 0; d.Me.UsedAt = 100;
    d.TermAt = 6; d.TermCalls = 0;
    d.Main();
    ok = ok && !std::strcmp(g_log, "untargetable;targetable;") && !quest.HeroEscapedCell && game.Frames == 4;
    if (!ok) { std::printf("00dd8de0_TEST FAIL\n"); return 1; }
    std::printf("00dd8de0_TEST PASS\n");
    return 0;
}