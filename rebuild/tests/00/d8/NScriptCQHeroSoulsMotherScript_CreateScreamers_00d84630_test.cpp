#include <cstdio>
#include <cstring>
// behaviour test: CQ_HeroSoulsMotherScript::CreateScreamers -- start the super-screamer timer, gather the
// ScreamerHere points, show a green/red quest bar sized by the screamer count; until the bar is full, top the live
// screamers up to the at-once limit at a random point each frame and update the bar; then remove the bar and record
// the mother's soul as collected
struct SScriptDef { float HS_SecondsBeforeNewSuperScreamer; long HS_ScreamerCount, HS_ScreamersAtOnce; };
static SScriptDef g_def;
static int g_frames = 0, g_endAt = 1000, g_created = 0, g_updates = 0, g_removedBar = 0, g_seed = 0;
static float g_timer = 0, g_lastValue = -1;
static int NextRandom() { return g_seed++; }
struct CScriptThing { int id; };
struct CThings {
    CScriptThing Items[4]; unsigned int Count;
    CThings() : Count(0) { }
    void push_back(int id) { CScriptThing t; t.id = id; Items[Count++] = t; }
    unsigned int size() const { return Count; }
    const CScriptThing& operator[](unsigned int i) const { return Items[i]; }
};
struct SMasterData { bool CollectedSoulFromMother; };
struct CQ_HeroSoulsMotherScript;
struct CGameScriptInterface {
    CQ_HeroSoulsMotherScript* Owner;
    void NewScriptFrame() const;
    void GetAllThingsWithScriptName(const char* n, CThings& v) const { if (!std::strcmp(n, "ScreamerHere")) { v.push_back(30); v.push_back(31); } }
    void DisplayQuestInfo(bool) const { }
    long AddQuestInfoBar(float maxValue, const char* icon) const { return (maxValue == 4.0f && !std::strcmp(icon, "HUD_QUEST_ICON_UNDEAD")) ? 77 : 0; }
    void UpdateQuestInfoBar(long id, float v) const { if (id == 77) { ++g_updates; g_lastValue = v; } }
    void CreateCreature(const char* def, const CScriptThing& at, const char* name) const;
    void RemoveQuestInfoElement(long id) const { if (id == 77) ++g_removedBar; }
};
struct CQ_HeroSoulsMotherScript {
    CGameScriptInterface* Game; SMasterData* PMasterData; long BarID, ScreamersAlive, BarToFill; int LastPoint;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void CreateScreamers();
};
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (Owner->ScreamersAlive > 0) { --Owner->ScreamersAlive; ++Owner->BarToFill; } }
void CGameScriptInterface::CreateCreature(const char* def, const CScriptThing& at, const char* name) const
{ if (!std::strcmp(def, "CREATURE_NW_SCREAMER_01") && !std::strcmp(name, "HS_Screamer")) { ++g_created; ++Owner->ScreamersAlive; Owner->LastPoint = at.id; } }
void CQ_HeroSoulsMotherScript::CreateScreamers()
{
    g_timer = g_def.HS_SecondsBeforeNewSuperScreamer;
    CThings screamerPoints;
    Game->GetAllThingsWithScriptName("ScreamerHere", screamerPoints);
    Game->DisplayQuestInfo(true);
    BarID = Game->AddQuestInfoBar((float)g_def.HS_ScreamerCount, "HUD_QUEST_ICON_UNDEAD");
    Game->UpdateQuestInfoBar(BarID, (float)ScreamersAlive);
    while (BarToFill < g_def.HS_ScreamerCount) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (ScreamersAlive < g_def.HS_ScreamersAtOnce)
            Game->CreateCreature("CREATURE_NW_SCREAMER_01", screamerPoints[NextRandom() % screamerPoints.size()], "HS_Screamer");
        Game->UpdateQuestInfoBar(BarID, (float)BarToFill);
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveQuestInfoElement(BarID);
    PMasterData->CollectedSoulFromMother = true;
}
int main()
{
    g_def.HS_SecondsBeforeNewSuperScreamer = 20.0f; g_def.HS_ScreamerCount = 4; g_def.HS_ScreamersAtOnce = 2;
    CGameScriptInterface game; SMasterData md; md.CollectedSoulFromMother = false;
    CQ_HeroSoulsMotherScript s; game.Owner = &s; s.Game = &game; s.PMasterData = &md; s.ScreamersAlive = 0; s.BarToFill = 0;
    s.CreateScreamers();
    bool ok = g_timer == 20.0f && s.BarID == 77 && s.BarToFill == 4 && g_removedBar == 1 && md.CollectedSoulFromMother
           && g_created == g_frames && g_lastValue == 4.0f && g_updates == g_frames + 1;
    md.CollectedSoulFromMother = false; s.BarToFill = 0; g_endAt = g_frames + 1;
    s.CreateScreamers();                                // thread ends: no soul
    ok = ok && !md.CollectedSoulFromMother && g_removedBar == 1;
    if (!ok) { std::printf("00d84630_TEST FAIL\n"); return 1; }
    std::printf("00d84630_TEST PASS\n");
    return 0;
}