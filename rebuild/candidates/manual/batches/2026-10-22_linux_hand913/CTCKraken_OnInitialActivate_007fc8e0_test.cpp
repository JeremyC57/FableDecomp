#include <cstdio>

struct CTCBoss { long phase; long GetCurrentPhase() const { return phase; } };
struct CKrakenDef { long id; };
struct CTCTargeted { bool Targetable; };
struct CThing {
    CTCBoss* boss; CKrakenDef* def; CTCTargeted targeted; bool LeaveExperienceOrbsOverride;
    void GetInterface(CTCBoss** out, long type) { if (type == 0xad) *out = boss; }
    void GetSubDef(CKrakenDef** out) { *out = def; }
};
struct CTCKraken {
    CThing* ParentThing; CTCBoss* PTCBoss; CKrakenDef* PKrakenDef; int explosions; int attackIt; int headIt;
    long KillBeamCounter; bool TentacleReadyToStrike, ReturnHeadToWater; long phaseSet; int markers;
    void GetCentreMarker() { ++markers; }
    void SetPhase(long p) { phaseSet = p; }
    void OnInitialActivate()
    {
        {
            CThing* thing = ParentThing;
            thing->GetInterface(&PTCBoss, 0xad);
            thing->GetSubDef(&PKrakenDef);
        }
        GetCentreMarker();
        explosions = 0;
        attackIt = headIt;
        KillBeamCounter = 0;
        TentacleReadyToStrike = false;
        ReturnHeadToWater = false;
        SetPhase(PTCBoss->GetCurrentPhase());
        ParentThing->targeted.Targetable = false;
        ParentThing->LeaveExperienceOrbsOverride = false;
    }
};
static bool Run()
{
    CTCBoss boss; boss.phase = 2; CKrakenDef def; def.id = 3;
    CThing t; t.boss = &boss; t.def = &def; t.targeted.Targetable = true; t.LeaveExperienceOrbsOverride = true;
    CTCKraken k; k.ParentThing = &t; k.PTCBoss = 0; k.PKrakenDef = 0; k.explosions = 5; k.attackIt = 1; k.headIt = 7;
    k.KillBeamCounter = 4; k.TentacleReadyToStrike = true; k.ReturnHeadToWater = true; k.phaseSet = -1; k.markers = 0;
    k.OnInitialActivate();
    return k.PTCBoss == &boss && k.PKrakenDef == &def && k.markers == 1 && k.explosions == 0 && k.attackIt == 7 &&
           k.KillBeamCounter == 0 && !k.TentacleReadyToStrike && !k.ReturnHeadToWater && k.phaseSet == 2 &&
           !t.targeted.Targetable && !t.LeaveExperienceOrbsOverride;
}

int main()
{
    if (!Run()) { std::printf("007fc8e0_TEST FAIL\n"); return 1; }
    std::printf("007fc8e0_TEST PASS\n");
    return 0;
}
