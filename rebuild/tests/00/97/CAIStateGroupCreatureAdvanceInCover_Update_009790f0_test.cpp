#include <cstdio>
#include <cmath>
// Behaviour copy of CAIStateGroup_CreatureAdvanceInCover::Update (0x009790F0).
struct V { float X, Y, Z; };
struct Thing { bool dead; V pos; };
struct Group {
    Thing* cover; Thing* enemy; Thing* creature; bool hasPos; V nextPos; bool runnable; int listSize;
    int failed, terminated, crouches, moves, ducks, cleared, refreshed, removed;
    void Update() {
        Thing* previous = (cover && !cover->dead) ? cover : 0;
        V coverPos;
        if (!hasPos) { ++failed; ++terminated; return; }
        coverPos = nextPos;
        const V& p = creature->pos; float dx = coverPos.X - p.X, dy = coverPos.Y - p.Y;
        float distance = (float)std::sqrt(dx * dx + dy * dy);
        if (distance > 1.8f) {
            if (previous && !previous->dead) {
                if (!enemy || enemy->dead) { ++terminated; return; }
                ++crouches;
            }
            ++moves; ++ducks;
        }
        if (!runnable) { ++cleared; listSize = 0; ++terminated; return; }
        ++refreshed; ++removed; --listSize;
        if (listSize == 0) ++terminated;
    }
};
int main() {
    Thing cover = { false, {0, 0, 0} }, enemy = { false, {9, 9, 0} }, me = { false, {0, 0, 0} };
    Group g = { &cover, &enemy, &me, true, {5, 0, 0}, true, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
    g.Update();
    if (g.crouches != 1 || g.moves != 1 || g.terminated != 0 || g.listSize != 1) { std::printf("009790f0_TEST FAIL\n"); return 1; }
    g.Update();   // list empties -> terminate
    if (g.terminated != 1) { std::printf("009790f0_TEST FAIL\n"); return 1; }
    Group h = { &cover, &enemy, &me, true, {1, 0, 0}, false, 3, 0, 0, 0, 0, 0, 0, 0, 0 }; h.Update();   // close: no move; not runnable
    if (h.moves != 0 || h.cleared != 1 || h.listSize != 0) { std::printf("009790f0_TEST FAIL\n"); return 1; }
    enemy.dead = true; Group k = { &cover, &enemy, &me, true, {5, 0, 0}, true, 2, 0, 0, 0, 0, 0, 0, 0, 0 }; k.Update();
    if (k.terminated != 1 || k.moves != 0) { std::printf("009790f0_TEST FAIL\n"); return 1; }
    Group m = { &cover, &enemy, &me, false, {5, 0, 0}, true, 2, 0, 0, 0, 0, 0, 0, 0, 0 }; m.Update();
    if (m.failed != 1 || m.terminated != 1) { std::printf("009790f0_TEST FAIL\n"); return 1; }
    std::printf("009790f0_TEST PASS\n"); return 0;
}