#include <cstdio>
#include <cstring>
// behaviour test: CTCTeleporter::FrameUpdate -- tutorial hold, region gate, activation, radius entry, particle upkeep
struct Thing { float X, Y; bool Dead; const char* Name; bool Killed; };
struct Def { float Radius; long Effect; bool GuildMarkerOnly; };
struct Teleporter {
    Thing* Parent; Def* PDef; bool Active, Activated, HeroInsideRadius, Waiting;
    Thing* Particle; int Refreshes, Teleports, Created;
    bool TutorialShowing, Paused, MapLoaded, Enabled, TutorialGiven; long MyRegion, CurRegion; Thing* Hero;
    void CheckActivate(bool marker)
    {
        if (!Activated && PDef->GuildMarkerOnly == marker && std::strcmp(Parent->Name, "HERO_GUILD_TELEPORT_MARKER") != 0 && Active != true) {
            Active = true; Activated = true; ++Refreshes;
        }
    }
    void FrameUpdate()
    {
        if (Waiting) {
            if (TutorialShowing || Paused) return;
            Waiting = false;
            if (Hero && !Hero->Dead) ++Teleports;
            return;
        }
        if (MyRegion != CurRegion) { HeroInsideRadius = true; return; }
        CheckActivate(false);
        if (!PDef || !MapLoaded) return;
        if (!Enabled) { if (Particle && !Particle->Dead) { Particle->Killed = true; Particle = 0; } return; }
        if (Hero && !Hero->Dead) {
            float dx = Hero->X - Parent->X, dy = Hero->Y - Parent->Y, r = PDef->Radius;
            if (dx * dx + dy * dy < r * r) {
                CheckActivate(true);
                if (!Active) return;
                if (!HeroInsideRadius) {
                    HeroInsideRadius = true;
                    if (TutorialGiven) { Waiting = true; return; }
                    ++Teleports;
                }
            } else
                HeroInsideRadius = false;
        }
        if (!Active || Particle) return;
        static Thing p; p.Dead = false; p.Killed = false; Particle = &p; ++Created;
    }
};
int main()
{
    Thing self = { 0.0f, 0.0f, false, "TELEPORTER_A", false };
    Thing hero = { 10.0f, 0.0f, false, "HERO", false };
    Def def = { 2.0f, 7, false };
    Teleporter t; std::memset(&t, 0, sizeof(t));
    t.Parent = &self; t.PDef = &def; t.Hero = &hero; t.MapLoaded = true; t.Enabled = true; t.MyRegion = t.CurRegion = 3;
    t.FrameUpdate();
    bool ok = t.Active && t.Activated && t.Refreshes == 1 && t.Created == 1 && !t.HeroInsideRadius;
    hero.X = 1.0f; t.FrameUpdate();
    ok = ok && t.HeroInsideRadius && t.Teleports == 1;
    t.FrameUpdate();
    ok = ok && t.Teleports == 1;
    hero.X = 5.0f; t.FrameUpdate(); hero.X = 0.5f; t.TutorialGiven = true; t.FrameUpdate();
    ok = ok && t.Waiting && t.Teleports == 1;
    t.TutorialShowing = true; t.FrameUpdate(); ok = ok && t.Waiting;
    t.TutorialShowing = false; t.FrameUpdate(); ok = ok && !t.Waiting && t.Teleports == 2;
    Thing* particle = t.Particle; t.Enabled = false; t.FrameUpdate();
    ok = ok && particle->Killed && t.Particle == 0;
    t.CurRegion = 4; t.HeroInsideRadius = false; t.FrameUpdate(); ok = ok && t.HeroInsideRadius;
    if (!ok) { std::printf("007e00d0_TEST FAIL\n"); return 1; }
    std::printf("007e00d0_TEST PASS\n");
    return 0;
}
