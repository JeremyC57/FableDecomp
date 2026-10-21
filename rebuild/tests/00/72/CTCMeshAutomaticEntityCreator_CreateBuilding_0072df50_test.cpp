// Behaviour test for CTCMeshAutomaticEntityCreator::CreateBuilding (0x0072DF50): like CreateObject, but a dead/missing
// building or an unknown name raises the thing manager's failure flag, and a building with a physics mesh gets its type.
#include <cstdio>

struct Building { bool dead; bool hasMesh; unsigned short meshType; unsigned short appliedType; unsigned char flags; };
struct World { int dummies; float dummyX[4]; long defIndex; Building b[4]; int created; bool failed; };
static bool InMap(float x) { return x >= 0.0f && x < 100.0f; }
static void CreateBuilding(World& w)
{
    for (int i = 0; i < w.dummies; ++i) {
        if (InMap(w.dummyX[i])) {
            if (w.defIndex) {
                Building* b = &w.b[w.created++];
                if (!b->dead) {
                    if (b->hasMesh)
                        b->appliedType = b->meshType;
                    b->flags |= 2;
                    continue;
                }
            }
            w.failed = true;
        }
    }
}
int main()
{
    World w;
    w.dummies = 3; w.dummyX[0] = 1.0f; w.dummyX[1] = 2.0f; w.dummyX[2] = 500.0f; w.defIndex = 4; w.created = 0; w.failed = false;
    for (int i = 0; i < 4; ++i) { w.b[i].dead = false; w.b[i].hasMesh = false; w.b[i].meshType = 9; w.b[i].appliedType = 0; w.b[i].flags = 0; }
    w.b[0].hasMesh = true; w.b[1].dead = true;
    CreateBuilding(w);
    if (w.created != 2 || w.b[0].appliedType != 9 || w.b[0].flags != 2 || w.b[1].flags != 0 || !w.failed) {
        std::printf("0072df50_TEST FAIL\n"); return 1;
    }
    w.created = 0; w.failed = false; w.b[1].dead = false; CreateBuilding(w);
    if (w.failed || w.b[1].flags != 2) { std::printf("0072df50_TEST FAIL\n"); return 1; }
    w.created = 0; w.defIndex = 0; CreateBuilding(w);
    if (!w.failed || w.created != 0) { std::printf("0072df50_TEST FAIL\n"); return 1; }
    std::printf("0072df50_TEST PASS\n");
    return 0;
}