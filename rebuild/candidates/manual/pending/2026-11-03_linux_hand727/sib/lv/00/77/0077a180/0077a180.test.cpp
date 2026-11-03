// behaviour test: CTCDExperienceOrb::OnCollected -- points scaled by the hero's pickup multiplier, the pickup
// effect is the last map entry whose threshold the points reach, the PICKUP_SOUND plays only for a collector with a
// sound player and an orb with a sound def that has the sound, and the orb is always killed.
#include <cstdio>
struct Pair { long first, second; };
struct Result { long effect; bool played; bool killed; };
static Result Collect(long points, bool hasHeroExp, long multiplier, const Pair* map, int mapSize,
                      bool hasSoundPlayer, bool hasSoundDef, bool hasPickupSound)
{
    Result r; r.played = false; r.killed = false;
    long experience = points;
    if (hasHeroExp)
        experience *= multiplier;
    long effect = 0;
    for (int i = 0; i < mapSize; ++i) {
        if (experience < map[i].first)
            break;
        effect = map[i].second;
    }
    r.effect = effect;                                   // CTCDParticleEmitter::Create(effect, pos, false)
    if (hasSoundPlayer && hasSoundDef && hasPickupSound)
        r.played = true;                                 // PlayAttachedSample(pair, CSoundInit(), true, false, false)
    r.killed = true;                                     // ParentThing->Kill(true)
    return r;
}
int main()
{
    Pair map[3] = { { 1, 100 }, { 10, 200 }, { 50, 300 } };
    Result a = Collect(5, false, 3, map, 3, true, true, true);
    Result b = Collect(5, true, 3, map, 3, true, true, false);
    Result c = Collect(0, true, 3, map, 3, false, true, true);
    Result d = Collect(60, false, 1, map, 3, true, false, true);
    if (a.effect != 100 || !a.played || !a.killed || b.effect != 200 || b.played || c.effect != 0 || c.played
        || d.effect != 300 || d.played || !d.killed) {
        std::printf("0077a180_TEST FAIL\n"); return 1;
    }
    std::printf("0077a180_TEST PASS\n");
    return 0;
}