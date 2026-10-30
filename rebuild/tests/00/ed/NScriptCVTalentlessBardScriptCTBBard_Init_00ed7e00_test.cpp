// Behaviour test for NScript::CV_TalentlessBardScript::CTB_Bard::Init (0x00ED7E00): give the bard information, centre his
// wander on his own position with the script-def distances, script his state group (4), mute his AI voice, enable only
// the five listed state groups (sit-outside off) and clear the three last-random-speech strings.
#include <cstdio>
#include <cstring>

class C3DVector { public: float X, Y, Z; };
struct Thing { C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };

static char gLog[512];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }

struct Game {
    C3DVector centre; float minD, maxD; int group; bool info;
    void SetThingHasInformation(const Thing&, bool a, bool b, bool c) { info = a && b && !c; }
    void SetWanderCentrePoint(const Thing&, C3DVector p) { centre = p; }
    void SetWanderMinDistance(const Thing&, float d) { minD = d; }
    void SetWanderMaxDistance(const Thing&, float d) { maxD = d; }
    void SetScriptingStateGroup(const Thing&, int g) { group = g; }
    void EntitySetWillBeUsingNarrator(const Thing&, const char* n) { Log(n); }
    void EntitySetAllStategroupsEnabled(const Thing&, bool on) { Log(on ? "all+" : "all-"); }
    void EntitySetStategroupEnabled(const Thing&, const char* n, bool on) { Log(n); Log(on ? "+" : "-"); }
};

class CScriptDef { public: float BardWanderDistanceMin, BardWanderDistanceMax; };
static CScriptDef gDef = { 3.0f, 9.0f };
static CScriptDef* pGlobalScriptDef = &gDef;

struct CTB_Bard {
    Game* Game_; Thing Me;
    const char* lastRandomSpeech1; const char* lastRandomSpeech2; const char* lastRandomSpeech3;
    void Init()
    {
        Game_->SetThingHasInformation(Me, true, true, false);
        Game_->SetWanderCentrePoint(Me, Me.GetPos());
        Game_->SetWanderMinDistance(Me, pGlobalScriptDef->BardWanderDistanceMin);
        Game_->SetWanderMaxDistance(Me, pGlobalScriptDef->BardWanderDistanceMax);
        Game_->SetScriptingStateGroup(Me, 4);
        Game_->EntitySetWillBeUsingNarrator(Me, "NoAIVoice");
        Game_->EntitySetAllStategroupsEnabled(Me, false);
        Game_->EntitySetStategroupEnabled(Me, "SG_MINION_WANDER", true);
        Game_->EntitySetStategroupEnabled(Me, "SG_MINION_SIT_OUTSIDE", false);
        Game_->EntitySetStategroupEnabled(Me, "SG_SEEK_ENEMY", true);
        Game_->EntitySetStategroupEnabled(Me, "SG_SEEK_SCRIPT_TARGET", true);
        Game_->EntitySetStategroupEnabled(Me, "SG_MELEE_COMBAT_NEARBY2", true);
        lastRandomSpeech1 = "";
        lastRandomSpeech2 = "";
        lastRandomSpeech3 = "";
    }
};

int main()
{
    Game g; g.info = false; g.group = 0;
    CTB_Bard b; b.Game_ = &g; b.Me.Pos.X = 1.0f; b.Me.Pos.Y = 2.0f; b.Me.Pos.Z = 3.0f;
    b.lastRandomSpeech1 = "a"; b.lastRandomSpeech2 = "b"; b.lastRandomSpeech3 = "c";
    gLog[0] = 0;
    b.Init();
    if (!g.info || g.centre.X != 1.0f || g.centre.Y != 2.0f || g.centre.Z != 3.0f || g.minD != 3.0f || g.maxD != 9.0f
        || g.group != 4 || b.lastRandomSpeech1[0] || b.lastRandomSpeech2[0] || b.lastRandomSpeech3[0]
        || std::strcmp(gLog, "NoAIVoice;all-;SG_MINION_WANDER;+;SG_MINION_SIT_OUTSIDE;-;SG_SEEK_ENEMY;+;SG_SEEK_SCRIPT_TARGET;+;SG_MELEE_COMBAT_NEARBY2;+;") != 0) {
        std::printf("00ed7e00_TEST FAIL\n"); return 1;
    }
    std::printf("00ed7e00_TEST PASS\n");
    return 0;
}