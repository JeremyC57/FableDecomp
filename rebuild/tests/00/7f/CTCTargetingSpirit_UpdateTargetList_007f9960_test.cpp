#include <cstdio>
#include <vector>
#include <algorithm>
struct Thing { float x, y, z; bool targetable; float score; };
struct Box { long l, t, r, b; };
static Thing* g_players[4];
struct Cmp { bool operator()(const Thing* a, const Thing* b) const { return a->score > b->score; } };
// Model of UpdateTargetList: box, exclusion list, 3D range filter, sort by score, pick first.
static Thing* UpdateTargetList(const Thing& parent, float maxdist, std::vector<Thing>& world, Box& box)
{
    long max_dist = (long)maxdist;
    long half = (long)(maxdist + 1.0f);
    long x = (long)parent.x, y = (long)parent.y;
    box.l = x - half; box.t = y - half; box.r = x + half; box.b = y + half;
    std::vector<const Thing*> players;
    for (long i = 0; i < 4; ++i) if (g_players[i]) players.push_back(g_players[i]);
    std::vector<Thing*> targets;
    for (size_t i = 0; i < world.size(); ++i) {
        Thing& t = world[i];
        if ((long)t.x < box.l || (long)t.x > box.r || (long)t.y < box.t || (long)t.y > box.b) continue;
        if (std::find(players.begin(), players.end(), &t) != players.end()) continue;
        if (!t.targetable) continue;
        float dx = t.x - parent.x, dy = t.y - parent.y, dz = t.z - parent.z;
        if (dx*dx + dy*dy + dz*dz > (float)max_dist * (float)max_dist) continue;
        targets.push_back(&t);
    }
    std::sort(targets.begin(), targets.end(), Cmp());
    Thing* best = 0;
    if (!targets.empty()) best = targets[0];
    return best;
}
int main()
{
    Thing parent = { 10.f, 10.f, 0.f, false, 0.f };
    std::vector<Thing> world;
    Thing a = { 12.f, 10.f, 0.f, true, 1.f }; world.push_back(a);
    Thing b = { 11.f, 11.f, 0.f, true, 5.f }; world.push_back(b);   // player creature: excluded
    Thing c = { 30.f, 30.f, 0.f, true, 9.f }; world.push_back(c);   // out of range
    Thing d = { 9.f, 9.f, 0.f, false, 8.f }; world.push_back(d);    // not targetable
    Thing e = { 10.f, 13.f, 0.f, true, 3.f }; world.push_back(e);
    g_players[0] = &world[1];
    Box box;
    Thing* best = UpdateTargetList(parent, 5.5f, world, box);
    if (best != &world[4] || box.l != 4 || box.r != 16 || box.t != 4 || box.b != 16) { std::printf("007f9960_TEST FAIL\n"); return 1; }
    g_players[0] = 0;
    best = UpdateTargetList(parent, 5.5f, world, box);
    if (best != &world[1]) { std::printf("007f9960_TEST FAIL\n"); return 1; }
    std::vector<Thing> empty;
    if (UpdateTargetList(parent, 5.5f, empty, box) != 0) { std::printf("007f9960_TEST FAIL\n"); return 1; }
    std::printf("007f9960_TEST PASS\n");
    return 0;
}