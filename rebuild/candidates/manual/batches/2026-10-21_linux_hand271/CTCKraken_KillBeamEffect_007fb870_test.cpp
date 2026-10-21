// Behaviour test for CTCKraken::KillBeamEffect (0x007FB870): only when the kill counter is idle and the beam effect lives,
// start the counter at 5 and spawn BEAM_ATTACK_FADE, attaching it when it has an emitter.
#include <cstdio>

struct Effect { bool dead, hasEmitter, attached; };
struct Kraken { long killBeamCounter; Effect* beam; bool hasSubDef, knowsFade; Effect fade; int spawned; };
static void KillBeamEffect(Kraken& k)
{
    if (k.killBeamCounter == 0 && k.beam != 0 && !k.beam->dead) {
        k.killBeamCounter = 5;
        if (k.hasSubDef && k.knowsFade) {
            Effect* fade = &k.fade;
            ++k.spawned;
            if (!fade->dead && fade->hasEmitter)
                fade->attached = true;
        }
    }
}
int main()
{
    Effect beam; beam.dead = false;
    Kraken k; k.killBeamCounter = 0; k.beam = &beam; k.hasSubDef = true; k.knowsFade = true; k.spawned = 0;
    k.fade.dead = false; k.fade.hasEmitter = true; k.fade.attached = false;
    KillBeamEffect(k);
    if (k.killBeamCounter != 5 || k.spawned != 1 || !k.fade.attached) { std::printf("007fb870_TEST FAIL\n"); return 1; }
    KillBeamEffect(k);                                  // counter busy: nothing
    if (k.spawned != 1) { std::printf("007fb870_TEST FAIL\n"); return 1; }
    k.killBeamCounter = 0; beam.dead = true; KillBeamEffect(k);
    if (k.killBeamCounter != 0 || k.spawned != 1) { std::printf("007fb870_TEST FAIL\n"); return 1; }
    beam.dead = false; k.knowsFade = false; KillBeamEffect(k);
    if (k.killBeamCounter != 5 || k.spawned != 1) { std::printf("007fb870_TEST FAIL\n"); return 1; }
    std::printf("007fb870_TEST PASS\n");
    return 0;
}
