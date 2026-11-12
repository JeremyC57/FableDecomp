// Behaviour test for 0x008A7A70 CGameScriptInterface::TeleportAllFollowersToHeroPosition: a copy of the row's logic over a small
// model (followers list, summoned creature, ghost-sword minions, 2D distance > 2 test, navigation wander position, physics reset).
#include <cstdio>
#include <vector>
struct Vec3 { float X, Y, Z; };
struct Thing {
    Vec3 pos; bool dead; bool hasNav; bool hasPhysics; bool navOk; int resets; Vec3 lastReset;
    Thing(float x, float y) : dead(false), hasNav(true), hasPhysics(true), navOk(true), resets(0)
    { pos.X = x; pos.Y = y; pos.Z = 0; lastReset.X = lastReset.Y = lastReset.Z = 0; }
    const Vec3& GetPos() const { return pos; }
};
static bool GetNearbyWanderPosition(const Thing& self, const Vec3& centre, float radius, Vec3& out)
{
    if (!self.navOk) return false;
    out.X = centre.X + radius; out.Y = centre.Y; out.Z = centre.Z; return true;
}
static void Teleport(Thing* hero, bool hasFollowed, const std::vector<Thing*>& followedList, Thing* summoned,
                     const std::vector<Thing*>* minions)
{
    if (!hero || hero->dead) return;
    if (!hasFollowed) return;
    std::vector<Thing*> followers(followedList);
    if (summoned && !summoned->dead) followers.push_back(summoned);
    if (minions) {
        long i = 0;
        while (i < (long)minions->size()) {
            if ((*minions)[i] && !(*minions)[i]->dead) followers.push_back((*minions)[i]);
            ++i;
        }
    }
    for (unsigned long i = 0; i < followers.size(); ++i) {
        Thing* follower = followers[i];
        float hx = hero->GetPos().X, hy = hero->GetPos().Y;
        float fx = follower->GetPos().X, fy = follower->GetPos().Y;
        float dx = hx - fx; float dy = hy - fy;
        float squared = dy * dy; squared += dx * dx;
        if (squared > 4.0f && follower->hasNav) {
            Vec3 position;
            if (GetNearbyWanderPosition(*follower, hero->GetPos(), 0.8f, position) && follower->hasPhysics) {
                follower->resets++; follower->lastReset = position; follower->pos = position;
            }
        }
    }
}
int main()
{
    Thing hero(10, 10);
    Thing nearF(11, 10);          // distance 1 -> stays
    Thing farF(20, 10);           // distance 10 -> teleported
    Thing edge(12, 10);           // distance exactly 2 -> squared 4, not > 4 -> stays
    Thing noNav(30, 30); noNav.hasNav = false;
    Thing summon(0, 0);
    Thing deadMinion(50, 50); deadMinion.dead = true;
    Thing minion(-5, 10);
    std::vector<Thing*> followed; followed.push_back(&nearF); followed.push_back(&farF); followed.push_back(&edge); followed.push_back(&noNav);
    std::vector<Thing*> minions; minions.push_back(&deadMinion); minions.push_back(0); minions.push_back(&minion);
    Teleport(&hero, true, followed, &summon, &minions);
    bool ok = nearF.resets == 0 && farF.resets == 1 && edge.resets == 0 && noNav.resets == 0 && summon.resets == 1 &&
              deadMinion.resets == 0 && minion.resets == 1 && farF.lastReset.X == 10.8f && farF.lastReset.Y == 10.0f;
    Thing deadHero(0, 0); deadHero.dead = true; Thing far2(100, 0);
    std::vector<Thing*> f2; f2.push_back(&far2);
    Teleport(&deadHero, true, f2, 0, 0);
    Teleport(&hero, false, f2, 0, 0);
    Teleport(0, true, f2, 0, 0);
    ok = ok && far2.resets == 0;
    if (!ok) { std::printf("008a7a70_TEST FAIL\n"); return 1; }
    std::printf("008a7a70_TEST PASS\n");
    return 0;
}