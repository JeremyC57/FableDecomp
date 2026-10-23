#include <cstdio>
#include <cstring>
// Behaviour test for CQ_UndeadRisingScript::LookAfterHealth (0x00E0DBB0): each frame while the undead lives, near the
// centre (< 6 units) it is made engageable/targetable/damageable/opaque, away from it evasive and translucent (0.6).
struct CScriptThing { int Id; float X; int LifeFrames; };
struct CGSI {
    int Frames; int NearFrames; int FarFrames; float Alpha; bool Targetable; bool Evade; int HealthReads; int EpicSets;
    CScriptThing* Undead;
    void NewScriptFrame() { ++Frames; Undead->X -= 2.0f; }
    float GetHealth(const CScriptThing&) { ++HealthReads; return 10.0f; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.Id = std::strcmp(n, "UndeadRisingCentre") ? 0 : 9; t.X = 0.0f; t.LifeFrames = 0; return t; }
    void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) {}
    void EntitySetTargetable(const CScriptThing&, bool t) { Targetable = t; }
    void EntitySetAsDamageable(const CScriptThing&, bool) {}
    void EntitySetAsKillable(const CScriptThing&, bool, bool) {}
    void EntitySetAlpha(const CScriptThing&, float a, bool) { Alpha = a; }
    void EntitySetEvadeAllHits(const CScriptThing&, bool e) { Evade = e; }
    void SetIsPushableByHero(CScriptThing, bool) {}
    void EntitySetAsCollidableToThings(const CScriptThing&, bool) {}
    void SetIsThingForcePushable(const CScriptThing&, bool) {}
    void SetIsThingEpicSpellable(const CScriptThing&, bool) { ++EpicSets; }
};
static bool IsAlive(const CScriptThing& t, int frames) { return frames < t.LifeFrames; }
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { float x = a.X - b.X; if (x < 0) x = -x; return x < d; }
struct CQ_UndeadRisingScript {
    CGSI* GSI;
    bool IsActiveThreadTerminating() { return GSI->Frames > 50; }
    void LookAfterHealth(CScriptThing undead)
    {
        GSI->GetHealth(undead);
        CScriptThing centre = GSI->GetThingWithScriptName("UndeadRisingCentre");
        while (IsAlive(*GSI->Undead, GSI->Frames)) {
            if (IsActiveThreadTerminating()) return;
            if (IsDistanceBetweenThingsUnder(*GSI->Undead, centre, 6.0f)) {
                if (IsActiveThreadTerminating()) return;
                GSI->EntitySetAbleToBeEngagedInCombat(undead, true); GSI->EntitySetTargetable(undead, true);
                GSI->EntitySetAsDamageable(undead, true); GSI->EntitySetAsKillable(undead, true, true);
                GSI->EntitySetAlpha(undead, 1.0f, true); GSI->EntitySetEvadeAllHits(undead, false);
                GSI->SetIsPushableByHero(undead, true); GSI->EntitySetAsCollidableToThings(undead, true);
                GSI->SetIsThingForcePushable(undead, true); GSI->SetIsThingEpicSpellable(undead, true);
                ++GSI->NearFrames;
            } else {
                if (IsActiveThreadTerminating()) return;
                GSI->EntitySetAbleToBeEngagedInCombat(undead, false); GSI->EntitySetTargetable(undead, false);
                GSI->EntitySetAsDamageable(undead, false); GSI->EntitySetAsKillable(undead, false, true);
                GSI->EntitySetAlpha(undead, 0.6f, true); GSI->EntitySetEvadeAllHits(undead, true);
                GSI->SetIsPushableByHero(undead, false); GSI->EntitySetAsCollidableToThings(undead, false);
                GSI->SetIsThingForcePushable(undead, false); GSI->SetIsThingEpicSpellable(undead, false);
                ++GSI->FarFrames;
            }
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    CScriptThing undead; undead.Id = 1; undead.X = 11.0f; undead.LifeFrames = 6;
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi); gsi.Undead = &undead;
    CQ_UndeadRisingScript s; s.GSI = &gsi;
    s.LookAfterHealth(undead);
    // X: 11, 9, 7 (far), 5, 3, 1 (near); the last frame was near
    bool ok = gsi.Frames == 6 && gsi.FarFrames == 3 && gsi.NearFrames == 3 && gsi.Alpha == 1.0f && gsi.Targetable && !gsi.Evade
              && gsi.HealthReads == 1 && gsi.EpicSets == 6;
    if (!ok) { std::printf("00e0dbb0_TEST FAIL %d %d %d\n", gsi.Frames, gsi.FarFrames, gsi.NearFrames); return 1; }
    std::printf("00e0dbb0_TEST PASS\n");
    return 0;
}
