// behaviour test for CAIStateGroup_CreatureUseCoverBase cover release (0x00970c10)
#include <cstdio>
struct CCover { void* user; int calls; void SetUser(void* u) { user = u; ++calls; } };
struct CThing { bool dead; bool hasCover; CCover* cover; };
struct Group {
    CThing* PCoverThing; int listSize; bool TimingCritical;
    void ClearCoverThing()
    {
        if (PCoverThing && !PCoverThing->dead) {
            if (PCoverThing->hasCover) PCoverThing->cover->SetUser(0);
            PCoverThing = 0;
        }
    }
    void CleanupLeaf()
    {
        if (PCoverThing && !PCoverThing->dead) { PCoverThing->cover->SetUser(0); }
        listSize = 0; PCoverThing = 0; TimingCritical = false;
    }
};
int main()
{
    CCover cv = { (void*)1, 0 }; CThing t = { false, true, &cv };
    Group g = { &t, 3, true };
    if ("00970c10"[5] == 'b') {
        g.ClearCoverThing();
        if (g.PCoverThing || cv.user || cv.calls != 1) { std::printf("00970c10_TEST FAIL\n"); return 1; }
        t.dead = true; g.PCoverThing = &t; g.ClearCoverThing();
        if (g.PCoverThing != &t || cv.calls != 1) { std::printf("00970c10_TEST FAIL\n"); return 1; }
    } else {
        g.CleanupLeaf();
        if (g.PCoverThing || g.listSize || g.TimingCritical || cv.calls != 1) { std::printf("00970c10_TEST FAIL\n"); return 1; }
        t.dead = true; g.PCoverThing = &t; g.listSize = 2; g.CleanupLeaf();
        if (g.PCoverThing || g.listSize || cv.calls != 1) { std::printf("00970c10_TEST FAIL\n"); return 1; }
    }
    std::printf("00970c10_TEST PASS\n");
    return 0;
}
