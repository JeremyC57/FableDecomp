// behaviour test for 00d15a40 CBH_GangGrunt::Main (the same control flow over a mocked engine)
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CCharString {
    char text[64];
    CCharString(const char* t) { std::strcpy(text, t); }
};
static CCharString GFIntToCharString(long v) { char b[16]; std::sprintf(b, "%d", (int)v); return CCharString(b); }
static CCharString operator+(const char* l, const CCharString& r) { CCharString s(l); std::strcat(s.text, r.text); return s; }
struct CScriptThing {
    int id; C3DVector pos; const char* data; int hitFrame;
    CCharString GetDataString() const { return CCharString(data); }
    const C3DVector& GetPos() const { return pos; }
    bool MsgIsHitBy(const char*) const;
    bool MsgIsHitByAnySpecialAbilityFrom(const char*) const { return false; }
    bool MsgIsHitBySpecialAbilityFrom(int, const char*) const { return false; }
};
struct CGame {
    int frame; int terminateAt; int removed; int startTries; int targetOf; int group; int lines;
    char lastLine[64]; float wanderMax; C3DVector wanderCentre; CScriptThing hero, post;
    bool cutsceneAt3;
    void NewScriptFrame() { ++frame; }
    const CScriptThing& GetHero() const { return hero; }
    CScriptThing GetThingWithScriptName(const CCharString&) const { return post; }
    void RemoveThing(const CScriptThing&, bool, bool) { ++removed; }
    bool StartScriptingEntity() { return ++startTries % 2 == 0; }
    long AddNewConversation(const CScriptThing&, bool, bool) { return 7; }
    void AddPersonToConversation(long, const CScriptThing&) { }
    void AddLineToConversation(long c, const CCharString& l, bool, const CScriptThing&, const CScriptThing&) { if (c == 7) { ++lines; std::strcpy(lastLine, l.text); } }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& t) { targetOf = t.id; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) { }
    void SetWanderCentrePoint(CScriptThing, C3DVector c) { wanderCentre = c; }
    void SetWanderMinDistance(CScriptThing, float) { }
    void SetWanderMaxDistance(CScriptThing, float d) { wanderMax = d; }
    void SetScriptingStateGroup(CScriptThing, int g) { group = g; }
};
static CGame* g_game;
bool CScriptThing::MsgIsHitBy(const char*) const { return hitFrame >= 0 && g_game->frame >= hitFrame; }
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing& b, float d) { return d < 10.0f ? g_game->frame >= 6 : (b.id == 1 && g_game->frame >= 12); }
struct CScripted {
    bool scripting;
    bool IsScripting() { return scripting; }
    void Release() { scripting = false; }
    void ClearCommands() { }
    void MoveToPosition(const C3DVector&, float, int, bool, bool) { scripting = true; }
};
struct CQ { bool BanditCutsceneDone; bool BountyHuntIntroComplete; long BanditGruntTextCounter; };
struct CBH_GangGrunt {
    CGame* Game; CScriptThing Me; CQ* ParentClass; float defWander;
    bool IsActiveThreadTerminating() { if (Game->frame >= 3) ParentClass->BanditCutsceneDone = true; return Game->frame >= Game->terminateAt; }
    void Main();
};
void CBH_GangGrunt::Main()
{
    if (!ParentClass->BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me, false, false);
        return;
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass->BanditCutsceneDone) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    CScripted scripted; scripted.scripting = false;
    CCharString data = Me.GetDataString();
    CScriptThing target = Game->GetThingWithScriptName(data);
    if (scripted.IsScripting()) scripted.Release();
    while (!Game->StartScriptingEntity()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    scripted.MoveToPosition(target.GetPos(), 1.0f, 1, false, false);
    while (!IsDistanceBetweenThingsUnder(Me, target, 5.0f)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (scripted.IsScripting()) scripted.Release();
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
    Game->SetWanderCentrePoint(Me, target.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, defWander);
    Game->SetScriptingStateGroup(Me, 4);
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f) && !Me.MsgIsHitBy("SCRIPT_NAME_HERO")
           && !(Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetScriptingStateGroup(Me, 0);
    if (scripted.IsScripting()) scripted.Release();
    while (!Game->StartScriptingEntity()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    scripted.ClearCommands();
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
    CCharString line = "TEXT_QST_017_BANDIT" + GFIntToCharString(ParentClass->BanditGruntTextCounter);
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, line, false, Me, Game->GetHero());
    if (ParentClass->BanditGruntTextCounter + 1 < 4) {
        if (IsActiveThreadTerminating()) return;
        ParentClass->BanditGruntTextCounter++;
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (scripted.IsScripting()) scripted.Release();
    for (;;) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
}
static void init(CGame& g) { std::memset(&g, 0, sizeof(g)); g.hero.id = 1; g.hero.hitFrame = -1; g.post.id = 2; g.post.pos.X = 4.0f; g.terminateAt = 40; }
int main()
{
    int ok = 1;
    CGame g; init(g); g_game = &g;
    CQ q; q.BanditCutsceneDone = false; q.BountyHuntIntroComplete = true; q.BanditGruntTextCounter = 2;
    CBH_GangGrunt s; s.Game = &g; s.Me.id = 3; s.Me.data = "POST"; s.Me.hitFrame = -1; s.ParentClass = &q; s.defWander = 9.0f;
    s.Main();
    if (g.lines != 1 || std::strcmp(g.lastLine, "TEXT_QST_017_BANDIT2") != 0 || q.BanditGruntTextCounter != 3) ok = 0;
    if (g.targetOf != 1 || g.wanderMax != 9.0f || g.wanderCentre.X != 4.0f || g.group != 0 || g.removed != 0) ok = 0;
    q.BanditGruntTextCounter = 3; init(g); s.Main();
    if (q.BanditGruntTextCounter != 3 || std::strcmp(g.lastLine, "TEXT_QST_017_BANDIT3") != 0) ok = 0;
    CQ r; r.BanditCutsceneDone = false; r.BountyHuntIntroComplete = false; r.BanditGruntTextCounter = 0;
    init(g); s.ParentClass = &r; s.Main();
    if (g.removed != 1 || g.lines != 0) ok = 0;
    if (!ok) { std::printf("00d15a40_TEST FAIL\n"); return 1; }
    std::printf("00d15a40_TEST PASS\n");
    return 0;
}
