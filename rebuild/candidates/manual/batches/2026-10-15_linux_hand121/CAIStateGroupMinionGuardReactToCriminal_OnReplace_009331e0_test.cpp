#include <cstdio>
// CAIStateGroup_MinionGuardReactToCriminal::OnReplace: clear the village's criminal handler when it is this creature
struct Village { void* criminalHandler; void SetCriminalHandler(void* p) { criminalHandler = p; } };
struct Group {
    void* creature; Village* village; bool hasVillager;
    void OnReplace()
    {
        if (hasVillager) {
            Village* v = village;
            if (v != 0) {
                void* c = creature;
                if (v->criminalHandler == c) v->SetCriminalHandler(0);
            }
        }
    }
};
int main()
{
    int me, other; Village v; v.criminalHandler = &me;
    Group g; g.creature = &me; g.village = &v; g.hasVillager = true;
    g.OnReplace(); if (v.criminalHandler != 0) { std::printf("009331e0_TEST FAIL\n"); return 1; }
    v.criminalHandler = &other; g.OnReplace(); if (v.criminalHandler != &other) { std::printf("009331e0_TEST FAIL\n"); return 1; }
    v.criminalHandler = &me; g.hasVillager = false; g.OnReplace(); if (v.criminalHandler != &me) { std::printf("009331e0_TEST FAIL\n"); return 1; }
    g.hasVillager = true; g.village = 0; g.OnReplace(); if (v.criminalHandler != &me) { std::printf("009331e0_TEST FAIL\n"); return 1; }
    std::printf("009331e0_TEST PASS\n"); return 0;
}
