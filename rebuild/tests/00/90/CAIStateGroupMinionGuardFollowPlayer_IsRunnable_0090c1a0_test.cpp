// 0090c1a0 behaviour test: CAIStateGroup_MinionGuardFollowPlayer::IsRunnable (final.cpp body, plain models of the world)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class C3DVector { public: float x, y, z; };
class CThing { public: bool dead; bool isVillager; bool isMinion; C3DVector pos; struct CVillage* village; };
struct CVillage { CThing* guardFollower; };
static long g_frame = 100;
static long g_framesPerSecond = 30;
static CThing* g_hero = 0;
static CThing* g_candidates[4];
static int g_candidateCount = 0;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
long CWorld::GetFrame() { return g_frame; }
static CVillage* GFGetPVillageInterface(CThing* villager) { return villager->village; }
// nearest candidate carrying the minion interface (the GetPBestThing stand-in)
static bool GetPBestThing(const C3DVector& from, CThing** out)
{
    CThing* best = 0; float bestDistance = 0.0f;
    for (int i = 0; i < g_candidateCount; ++i) {
        CThing* t = g_candidates[i];
        if (!t->isMinion) continue;
        float dx = t->pos.x - from.x, dy = t->pos.y - from.y, dz = t->pos.z - from.z;
        float d = dx * dx + dy * dy + dz * dz;
        if (!best || d < bestDistance) { best = t; bestDistance = d; }
    }
    *out = best;
    return best != 0;
}
struct CStateGroup {
    CThing* creature;
    unsigned long NextCheckFrame;
    bool stillRunnable;
    int stillRunnableCalls;
    bool IsStillRunnable(bool) { ++stillRunnableCalls; return stillRunnable; }
    bool IsRunnable();
};
bool CStateGroup::IsRunnable()
{
    if (NextCheckFrame > (unsigned long)CWorld::GetFrame())
        return false;
    CThing* hero = g_hero;
    if (hero && !hero->dead && creature->isVillager) {
        if (GFGetPVillageInterface(creature)) {
            CVillage* village = GFGetPVillageInterface(creature);
            CThing* follower = village->guardFollower;
            if (!follower || follower->dead) {
                CThing* nearest;
                GetPBestThing(hero->pos, &nearest);
                if (nearest == creature && IsStillRunnable(true))
                    return true;
            }
        }
    }
    long oneSecond = g_framesPerSecond;
    NextCheckFrame = CWorld::GetFrame() + oneSecond;
    return false;
}
int main()
{
    CVillage village = { 0 };
    CThing hero = { false, false, false, { 0, 0, 0 }, 0 };
    CThing me = { false, true, true, { 1, 0, 0 }, &village };
    CThing other = { false, true, true, { 5, 0, 0 }, &village };
    g_hero = &hero; g_candidates[0] = &other; g_candidates[1] = &me; g_candidateCount = 2;
    CStateGroup g = { &me, 0, true, 0 };
    // nearest minion to the hero, no living follower: runnable, frame untouched
    CHECK(g.IsRunnable() && g.NextCheckFrame == 0 && g.stillRunnableCalls == 1);
    // throttled before the next check frame
    g.NextCheckFrame = 101;
    CHECK(!g.IsRunnable() && g.NextCheckFrame == 101);
    // another creature is nearer: not runnable, next check one second later
    g.NextCheckFrame = 0; other.pos.x = 0.5f;
    CHECK(!g.IsRunnable() && g.NextCheckFrame == 130 && g.stillRunnableCalls == 1);
    // a living follower already exists
    other.pos.x = 5; g.NextCheckFrame = 0; village.guardFollower = &other;
    CHECK(!g.IsRunnable() && g.NextCheckFrame == 130);
    // a dead follower is replaced
    other.dead = true; g.NextCheckFrame = 0;
    CHECK(g.IsRunnable());
    // the state refuses to start
    g.stillRunnable = false; g.NextCheckFrame = 0;
    CHECK(!g.IsRunnable() && g.NextCheckFrame == 130);
    // dead hero
    g.stillRunnable = true; hero.dead = true; g.NextCheckFrame = 0;
    CHECK(!g.IsRunnable());
    if (!g_fail) printf("0090c1a0_TEST PASS\n");
    return g_fail;
}