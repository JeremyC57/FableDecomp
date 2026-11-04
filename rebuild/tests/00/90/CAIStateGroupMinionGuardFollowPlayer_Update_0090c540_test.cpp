// Behaviour test for CAIStateGroup_MinionGuardFollowPlayer::Update (0x0090C540), g++ model of the retail logic.
#include <cstdio>
struct Thing { bool dead; };
struct Village { Thing* guardFollowing; };
struct Group { long nextCheck; Thing* creature; Village* village; int moves, looks, terminated; float lastProximity; long lastFrames; };
static long g_frame = 500, g_fps = 30; static Thing* g_hero;
static bool Alive(const Thing* t) { return t != 0 && !t->dead; }
static void Update(Group& g)
{
    g.nextCheck = g_frame + g_fps;
    const Thing* hero = g_hero;
    if (!Alive(hero)) { ++g.terminated; return; }
    if (Alive(g.village->guardFollowing)) return;
    g.village->guardFollowing = g.creature;
    ++g.moves; g.lastProximity = 3.0f;
    ++g.looks; g.lastFrames = 0xb;
    ++g.terminated;
}
int main()
{
    Thing hero = { false }, me = { false }, other = { false };
    Village v = { &other };
    Group g = { 0, &me, &v, 0, 0, 0, 0, 0 };
    g_hero = &hero;
    Update(g);
    int ok = g.nextCheck == 530 && g.moves == 0 && g.terminated == 0 && v.guardFollowing == &other;
    other.dead = true; Update(g);
    ok &= v.guardFollowing == &me && g.moves == 1 && g.looks == 1 && g.terminated == 1 && g.lastFrames == 0xb;
    hero.dead = true; Update(g); ok &= g.terminated == 2 && g.moves == 1;
    g_hero = 0; Update(g); ok &= g.terminated == 3;
    std::printf(ok ? "0090c540_TEST PASS\n" : "0090c540_TEST FAIL\n");
    return ok ? 0 : 1;
}