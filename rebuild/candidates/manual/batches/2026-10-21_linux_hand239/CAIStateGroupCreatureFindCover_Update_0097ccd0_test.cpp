#include <cstdio>
#include <cmath>
// Behaviour copy of CAIStateGroup_CreatureFindCover::Update (0x0097CCD0).
struct V { float X, Y, Z; };
struct Thing { bool dead; V pos; };
struct Group {
    Thing* enemy; Thing* cover; Thing* creature; Thing* perceived; bool hasPerception;
    bool hasCover; V coverPos; bool moved; int runnableCount; V hidden;
    int aware, failed, terminated, ducks;
    void Update() {
        if (!enemy || enemy->dead) { ++terminated; return; }
        if (hasPerception && perceived == enemy) ++aware;
        if (!hasCover) { ++failed; ++terminated; return; }
        if (moved) {
            do {
                ++ducks;
                if (!cover || cover->dead) break;
                if (!enemy || enemy->dead) break;
                const V& p = creature->pos; float dx = hidden.X - p.X, dy = hidden.Y - p.Y;
                float distance = (float)std::sqrt(dx * dx + dy * dy);
                if (distance > 1.0f) break;
            } while (runnableCount-- > 0);
        }
        ++terminated;
    }
};
int main() {
    Thing enemy = { false, {9, 9, 0} }, cover = { false, {0, 0, 0} }, me = { false, {0, 0, 0} };
    Group g = { &enemy, &cover, &me, &enemy, true, true, {1, 0, 0}, true, 2, {0.5f, 0, 0}, 0, 0, 0, 0 };
    g.Update();
    if (g.aware != 1 || g.ducks != 3 || g.terminated != 1 || g.failed != 0) { std::printf("0097ccd0_TEST FAIL\n"); return 1; }
    Group h = { &enemy, &cover, &me, &cover, true, true, {1, 0, 0}, true, 5, {3, 0, 0}, 0, 0, 0, 0 }; h.Update();
    if (h.aware != 0 || h.ducks != 1 || h.terminated != 1) { std::printf("0097ccd0_TEST FAIL\n"); return 1; }
    Group k = { &enemy, &cover, &me, &enemy, false, false, {1, 0, 0}, true, 5, {0, 0, 0}, 0, 0, 0, 0 }; k.Update();
    if (k.failed != 1 || k.terminated != 1 || k.ducks != 0) { std::printf("0097ccd0_TEST FAIL\n"); return 1; }
    enemy.dead = true; Group m = g; m.aware = m.terminated = m.ducks = 0; m.Update();
    if (m.terminated != 1 || m.aware != 0 || m.ducks != 0) { std::printf("0097ccd0_TEST FAIL\n"); return 1; }
    std::printf("0097ccd0_TEST PASS\n"); return 0;
}
