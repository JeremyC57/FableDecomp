// Behaviour test for CAC_DummyStatic::Main @ 0x00E369B0: archery dummy waits for the contest, wobbles, scores hits, spins and pauses.
#include <cstdio>
#include <cstring>
#include <string>
class CCharString { public:
    std::string text;
    CCharString(const char* t) : text(t) { }
};
static int g_frames = 0;
static int g_terminateAtFrame = 0;
static int g_hitEvery = 0;
static int g_startFrame = 3;
static float g_hitDistance = 0.0f;
struct CV_ArcheryCompetitionScript;
static CV_ArcheryCompetitionScript* g_contest = 0;
struct CV_ArcheryCompetitionScript {
    long ContestState;
    long score, hits;
    float lastDistance;
    void CalculateScoreForHit(long points, float distance) { score += points; ++hits; lastDistance = distance; }
};
class CScriptThing { public:
    float angle;
    int framesSinceHit;
    float GetAngleXY() const { return angle; }
    bool MsgIsHitByWithProjectileWeapon(const CCharString& name, float& distance) const
    {
        if (name.text != "SCRIPT_NAME_HERO" || g_hitEvery == 0)
            return false;
        if (g_frames % g_hitEvery == 0) { distance = g_hitDistance; return true; }
        return false;
    }
};
class CGameScriptInterface { public:
    CScriptThing hero;
    float lastAngle; int facings, sounds, anims; float paused; int pauses;
    std::string lastSound, lastAnim;
    // Script: contest state goes 3 (running) at frame 3, 2 (over) after 2 hits.
    void NewScriptFrame() const
    {
        CGameScriptInterface* self = (CGameScriptInterface*)this;
        ++g_frames;
        if (g_startFrame && g_frames == g_startFrame) g_contest->ContestState = 3;
        (void)self;
    }
    CScriptThing& GetHero() { return hero; }
    void Pause(float seconds) const { CGameScriptInterface* s = (CGameScriptInterface*)this; s->paused = seconds; ++s->pauses; if (g_contest->hits >= 2) g_contest->ContestState = 2; }
    void EntitySetFacingAngle(const CScriptThing&, float angle, bool) const { CGameScriptInterface* s = (CGameScriptInterface*)this; s->lastAngle = angle; ++s->facings; }
    void EntityPlayObjectAnimation(const CScriptThing&, const CCharString& anim, bool) const { CGameScriptInterface* s = (CGameScriptInterface*)this; s->lastAnim = anim.text; ++s->anims; }
    unsigned long PlaySoundOnThing(const CScriptThing& thing, const CCharString& sound) const
    {
        CGameScriptInterface* s = (CGameScriptInterface*)this;
        if (&thing == &hero) { s->lastSound = sound.text; ++s->sounds; }
        return 0;
    }
};
struct CQuestGlobals { long ArcheryDummyStaticPoints; };
static CQuestGlobals g_globals;
static CQuestGlobals* g_pQuestGlobals = &g_globals;
class CAC_DummyStatic {
public:
    CGameScriptInterface*        Game;
    CScriptThing                 Me;
    CV_ArcheryCompetitionScript* ParentClass;
    long                         PointsValue;
    bool IsActiveThreadTerminating() const { return g_terminateAtFrame && g_frames >= g_terminateAtFrame; }
    void Main();
};

void CAC_DummyStatic::Main()
{
    float angle = Me.GetAngleXY();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    PointsValue = g_pQuestGlobals->ArcheryDummyStaticPoints;
    float distance = 0.0f;
    while (!IsActiveThreadTerminating()) {
        while (ParentClass->ContestState != 3) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (ParentClass->ContestState != 2 && ParentClass->ContestState != 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngle(Me, angle + 0.25, true);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngle(Me, angle, true);
            while (!Me.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO", distance)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            ParentClass->CalculateScoreForHit(PointsValue, distance);
            Game->PlaySoundOnThing(Game->GetHero(), "SND_ARROWIMPACT_03");
            Game->EntityPlayObjectAnimation(Me, "GET_HIT_SPIN", false);
            Game->EntitySetFacingAngle(Me, angle + 0.25, true);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngle(Me, angle + 0.5, true);
            Game->Pause((float)PointsValue);
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++failures; } }

int main()
{
    CV_ArcheryCompetitionScript contest; contest.ContestState = 1; contest.score = 0; contest.hits = 0; contest.lastDistance = 0;
    g_contest = &contest;
    CGameScriptInterface game; game.facings = game.sounds = game.anims = game.pauses = 0; game.paused = 0; game.lastAngle = 0;
    g_globals.ArcheryDummyStaticPoints = 5;
    CAC_DummyStatic d; d.Game = &game; d.Me.angle = 1.0f; d.ParentClass = &contest; d.PointsValue = 0;

    // Terminate during the start-up frames: nothing scored, points not yet set.
    g_frames = 0; g_terminateAtFrame = 2;
    d.Main();
    Check(d.PointsValue == 0 && contest.hits == 0, "early terminate");

    // Full run: contest starts at frame 3, hits every 2 frames, ends after 2 hits; terminate at frame 60.
    g_frames = 0; g_terminateAtFrame = 60; g_hitEvery = 2; g_hitDistance = 12.5f;
    d.Main();
    Check(d.PointsValue == 5, "points from quest globals");
    Check(contest.hits == 2 && contest.score == 10 && contest.lastDistance == 12.5f, "two hits scored");
    Check(game.sounds == 2 && game.lastSound == "SND_ARROWIMPACT_03", "impact sound on hero");
    Check(game.anims == 2 && game.lastAnim == "GET_HIT_SPIN", "spin animation");
    Check(game.pauses == 2 && game.paused == 5.0f, "pause by points value");
    Check(game.facings == 8 && game.lastAngle == 1.5f, "facing wobble ends at angle + 0.5");
    Check(g_frames >= 60, "ran until terminating");

    // Contest never starts: waits until terminated without facing changes.
    contest.ContestState = 1; contest.hits = 0; game.facings = 0; g_frames = 0; g_terminateAtFrame = 20; g_startFrame = 0;
    g_contest = &contest;
    d.Main();
    Check(game.facings == 0 && contest.hits == 0 && g_frames == 20, "idle until terminate");

    if (failures == 0)
        std::printf("00e369b0_TEST PASS\n");
    return failures;
}