#include <cstdio>
// Behaviour copy of CAIStateGroup_ChickenPanic::Update (0x00911D90).
struct V { float x, y, z; };
struct Thing { V pos; Thing* attacker; bool hasNav; int fleeQueries; };
struct Group {
    Thing* creature; int moves; int idles; int idleLimit; int terminated; V lastFlee;
    bool GetNearestFleePosition(Thing* nav, const V& centre, float radius, V* out) { ++nav->fleeQueries; out->x = centre.x + radius; out->y = centre.y; out->z = 0; return true; }
    long DoNothing() { return ++idles < idleLimit ? 4 : 2; }
    void Update() {
        do {
            if (!creature->attacker) break;
            const V& a = creature->attacker->pos; const V& p = creature->pos;
            float dx = p.x - a.x, dy = p.y - a.y; float d2 = dx * dx + dy * dy;
            if (!(d2 < 64.0f)) break;
            V flee;
            if (GetNearestFleePosition(creature, creature->attacker->pos, 8.0f, &flee)) { ++moves; lastFlee = flee; creature->pos = flee; }
        } while (DoNothing() == 4);
        ++terminated;
    }
};
int main() {
    Thing attacker = { {0, 0, 0}, 0, false, 0 };
    Thing chicken = { {1, 1, 0}, &attacker, true, 0 };
    Group g = { &chicken, 0, 0, 10, 0, {0, 0, 0} };
    g.Update();   // first pass flees to x=8 (d2 = 64, not < 64) -> stop
    if (g.moves != 1 || g.terminated != 1 || g.lastFlee.x != 8.0f || g.idles != 1) { std::printf("00911d90_TEST FAIL\n"); return 1; }
    Thing calm = { {1, 1, 0}, 0, true, 0 }; Group h = { &calm, 0, 0, 10, 0, {0, 0, 0} }; h.Update();
    if (h.moves != 0 || h.terminated != 1 || h.idles != 0) { std::printf("00911d90_TEST FAIL\n"); return 1; }
    std::printf("00911d90_TEST PASS\n"); return 0;
}