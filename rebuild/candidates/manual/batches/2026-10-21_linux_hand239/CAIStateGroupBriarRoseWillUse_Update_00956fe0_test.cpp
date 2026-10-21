#include <cstdio>
// Behaviour copy of CAIStateGroup_BriarRoseWillUse::Update (0x00956FE0).
struct V { float x, y, z; };
struct Def { long refs; float SecondsBetweenWillUse; float DistanceForcePushThreshold; int deleted; };
struct Thing { V pos; bool dead; unsigned char flags6c; Def* def; };
struct Group {
    Thing* creature; Thing* enemy; long NextWillUseFrame; int pushes, shots, terminated; long frame; long fps; Def* sub;
    void Update() {
        if (!enemy || enemy->dead) { ++terminated; return; }
        Def* def = creature->def; if (def) ++def->refs;
        Def* out = sub; ++out->refs;                          // GetSubDef add-refs the sub-def
        if (def) { if (--def->refs == 0) ++def->deleted; }
        def = out;
        float threshold = def->DistanceForcePushThreshold, interval = def->SecondsBetweenWillUse;
        if (enemy->flags6c & 8) {
            const V& a = creature->pos; const V& p = enemy->pos; float dx = p.x - a.x, dy = p.y - a.y;
            if (dx * dx + dy * dy < threshold * threshold) ++pushes; else ++shots;
        }
        NextWillUseFrame = (long)((float)frame + (float)fps * interval);
        ++terminated;
        if (--def->refs == 0) ++def->deleted;
    }
};
int main() {
    Def creatureDef = { 1, 0, 0, 0 }, sub = { 0, 2.0f, 5.0f, 0 };
    Thing me = { {0, 0, 0}, false, 0, &creatureDef }, foe = { {3, 0, 0}, false, 8, 0 };
    Group g = { &me, &foe, 0, 0, 0, 0, 100, 15, &sub };
    g.Update();
    if (g.pushes != 1 || g.shots != 0 || g.NextWillUseFrame != 130 || g.terminated != 1 || sub.refs != 0 || sub.deleted != 1 || creatureDef.refs != 1) {
        std::printf("00956fe0_TEST FAIL\n"); return 1; }
    foe.pos.x = 10; sub.refs = 0; sub.deleted = 0; g.Update();
    if (g.shots != 1) { std::printf("00956fe0_TEST FAIL\n"); return 1; }
    foe.dead = true; g.Update();
    if (g.terminated != 3 || g.shots != 1) { std::printf("00956fe0_TEST FAIL\n"); return 1; }
    std::printf("00956fe0_TEST PASS\n"); return 0;
}
