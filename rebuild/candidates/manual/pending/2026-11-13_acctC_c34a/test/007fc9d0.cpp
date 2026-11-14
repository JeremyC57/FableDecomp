// Behaviour test for 0x007FC9D0 CTCKraken::FrameUpdate: nothing in the editor; the beam-kill countdown kills and forgets a live beam
// effect when it reaches zero; null tentacles are removed; the boss phase is followed; a dead / missing centre marker is re-found and
// a dead / missing target becomes the main player's creature; phases 1 and 2 run the Mode step (1 tentacles only, 2 reset the
// attacking tentacle, 3 head tracking, 4 beam delay then explosion, 5 head tracking + beam); phase 1 then waits for the boss's phase
// change (ReturnHeadToWater), any other phase but 3 dies below 1 health (untargetable, "ON_DIE" head effect, phase 3); the kraken is
// always put at the centre marker raised by the def's height offset.
#include <cstdio>
#include <cstring>
#include <list>
#include <string>
struct Thing { bool dead; float x, y, z; Thing() : dead(false), x(0), y(0), z(0) {} };
struct Kraken {
    bool editor; int killBeamCounter; Thing* beam; int beamKills; std::list<Thing*> tentacles; int phase, bossPhase, setPhaseCalls;
    Thing* centre; Thing* foundCentre; Thing* target; Thing* hero; int mode, delayBeforeBeam; bool ready, returnHead, hasTargeted, targetable;
    float health, heightOffset, px, py, pz; bool positioned; std::string log, effect; bool attackingAtEnd;
    Kraken() : editor(false), killBeamCounter(0), beam(0), beamKills(0), phase(0), bossPhase(0), setPhaseCalls(0), centre(0), foundCentre(0),
               target(0), hero(0), mode(0), delayBeforeBeam(0), ready(false), returnHead(false), hasTargeted(true), targetable(true), health(10),
               heightOffset(2), px(0), py(0), pz(0), positioned(false), attackingAtEnd(false) {}
    void SetPhase(int p) { phase = p; ++setPhaseCalls; }
    void FrameUpdate() {
        if (editor) return;
        if (killBeamCounter > 0 && --killBeamCounter == 0 && beam && !beam->dead) { beam->dead = true; ++beamKills; beam = 0; }
        tentacles.remove(0);
        if (phase != bossPhase) SetPhase(bossPhase);
        if (!centre || centre->dead) centre = foundCentre;
        if (!target || target->dead) target = hero;
        switch (phase) {
        case 1: case 2:
            switch (mode) {
            case 1: log += "T"; break;
            case 2: attackingAtEnd = true; break;
            case 3: log += "H"; break;
            case 5: log += "H";
            case 4: if (delayBeforeBeam == 0) log += "B"; else --delayBeforeBeam; break;
            }
        case 0: default:
            if (phase == 1) { if (ready) returnHead = true; }
            else if (health < 1.0f) { if (hasTargeted) targetable = false; effect = "ON_DIE"; SetPhase(3); }
            break;
        case 3: break;
        }
        px = centre->x; py = centre->y; pz = centre->z + heightOffset; positioned = true;
    }
};
static int fails = 0;
static void check(bool ok, const char* what) { if (!ok) { ++fails; std::printf("FAIL %s\n", what); } }
int main()
{
    Thing marker, marker2, hero, beam, tent; marker.x = 1; marker.y = 2; marker.z = 3; marker2.z = 10;
    { Kraken k; k.editor = true; k.centre = &marker; k.killBeamCounter = 1; k.FrameUpdate(); check(!k.positioned && k.killBeamCounter == 1, "editor"); }
    { Kraken k; k.centre = &marker; k.killBeamCounter = 2; k.beam = &beam; k.FrameUpdate(); check(k.beam == &beam && k.killBeamCounter == 1 && !beam.dead, "countdown");
      k.FrameUpdate(); check(k.beam == 0 && beam.dead && k.beamKills == 1 && k.killBeamCounter == 0, "beam killed");
      k.beam = &beam; k.FrameUpdate(); check(k.beam == &beam && k.beamKills == 1, "no countdown at zero");
      check(k.positioned && k.px == 1 && k.py == 2 && k.pz == 5, "position"); }
    { Kraken k; k.centre = &marker; k.killBeamCounter = 1; beam.dead = true; k.beam = &beam; k.FrameUpdate(); check(k.beam == &beam && k.beamKills == 0, "dead beam kept"); }
    { Kraken k; k.centre = &marker; k.tentacles.push_back(&tent); k.tentacles.push_back(0); k.tentacles.push_back(0); k.FrameUpdate(); check(k.tentacles.size() == 1, "null tentacles removed"); }
    { Kraken k; k.centre = &marker; k.bossPhase = 2; k.mode = 1; k.FrameUpdate(); check(k.phase == 2 && k.setPhaseCalls == 1 && k.log == "T", "phase followed, tentacles only");
      k.FrameUpdate(); check(k.setPhaseCalls == 1, "same phase"); }
    { Kraken k; marker.dead = true; k.centre = &marker; k.foundCentre = &marker2; k.target = 0; k.hero = &hero; k.FrameUpdate();
      check(k.centre == &marker2 && k.pz == 12 && k.target == &hero, "marker and target refreshed"); marker.dead = false;
      Thing t2; k.target = &t2; k.hero = 0; k.FrameUpdate(); check(k.target == &t2, "live target kept"); }
    { Kraken k; k.centre = &marker; k.phase = k.bossPhase = 1; k.mode = 2; k.ready = true; k.health = 0; k.FrameUpdate();
      check(k.attackingAtEnd && k.returnHead && k.phase == 1 && k.effect.empty(), "phase 1: mode 2, ready, no death"); }
    { Kraken k; k.centre = &marker; k.phase = k.bossPhase = 1; k.mode = 3; k.FrameUpdate(); check(k.log == "H" && !k.returnHead, "phase 1 not ready"); }
    { Kraken k; k.centre = &marker; k.phase = k.bossPhase = 2; k.mode = 5; k.delayBeforeBeam = 1; k.FrameUpdate(); check(k.log == "H" && k.delayBeforeBeam == 0, "mode 5 delay");
      k.FrameUpdate(); check(k.log == "HHB", "mode 5 beam"); k.mode = 4; k.FrameUpdate(); check(k.log == "HHBB", "mode 4 beam");
      k.health = 0.5f; k.mode = 0; k.FrameUpdate(); check(!k.targetable && k.effect == "ON_DIE" && k.phase == 3 && k.setPhaseCalls == 1, "death"); }
    { Kraken k; k.centre = &marker; k.health = 0.5f; k.hasTargeted = false; k.FrameUpdate(); check(k.targetable && k.effect == "ON_DIE" && k.phase == 3, "phase 0 death without targeted TC"); }
    { Kraken k; k.centre = &marker; k.health = 1.0f; k.FrameUpdate(); check(k.effect.empty() && k.phase == 0, "health 1 lives"); }
    { Kraken k; k.centre = &marker; k.phase = k.bossPhase = 3; k.health = 0; k.mode = 1; k.FrameUpdate(); check(k.effect.empty() && k.setPhaseCalls == 0 && k.log.empty() && k.positioned, "phase 3 idle"); }
    if (fails == 0) std::printf("007fc9d0_TEST PASS\n");
    return fails;
}
