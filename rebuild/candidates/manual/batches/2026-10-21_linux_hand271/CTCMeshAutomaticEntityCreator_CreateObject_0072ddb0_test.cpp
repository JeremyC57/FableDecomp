// Behaviour test for CTCMeshAutomaticEntityCreator::CreateObject (0x0072DDB0): for each dummy, when the dummy position is
// in the map and the object name resolves, create the object, copy a non-unit creator scale, mark it created-by-owner,
// set its creator and register it as owned.
#include <cstdio>

struct Obj { float scale; unsigned char flags; int creatorSet; int owned; };
struct World { int dummies; float dummyX[4]; long defIndex; float creatorScale; Obj objs[4]; int created; };
static bool InMap(float x) { return x >= 0.0f && x < 100.0f; }
static void CreateObject(World& w)
{
    for (int i = 0; i < w.dummies; ++i) {
        if (InMap(w.dummyX[i])) {
            long defIndex = w.defIndex;
            if (defIndex) {
                Obj* o = &w.objs[w.created++];
                float scale = w.creatorScale;
                if (scale != 1.0f)
                    o->scale = scale;
                o->flags |= 2;
                o->creatorSet = 1;
                o->owned = 1;
            }
        }
    }
}
int main()
{
    World w;
    w.dummies = 3; w.dummyX[0] = 5.0f; w.dummyX[1] = -3.0f; w.dummyX[2] = 50.0f; w.defIndex = 7; w.creatorScale = 2.0f;
    w.created = 0;
    for (int i = 0; i < 4; ++i) { w.objs[i].scale = 1.0f; w.objs[i].flags = 0; w.objs[i].creatorSet = 0; w.objs[i].owned = 0; }
    CreateObject(w);
    if (w.created != 2 || w.objs[0].scale != 2.0f || w.objs[1].flags != 2 || !w.objs[1].owned || !w.objs[0].creatorSet) {
        std::printf("0072ddb0_TEST FAIL\n"); return 1;
    }
    w.created = 0; w.defIndex = 0; CreateObject(w);
    if (w.created != 0) { std::printf("0072ddb0_TEST FAIL\n"); return 1; }
    w.defIndex = 3; w.creatorScale = 1.0f; w.objs[0].scale = 1.0f; CreateObject(w);
    if (w.created != 2 || w.objs[0].scale != 1.0f) { std::printf("0072ddb0_TEST FAIL\n"); return 1; }
    std::printf("0072ddb0_TEST PASS\n");
    return 0;
}
