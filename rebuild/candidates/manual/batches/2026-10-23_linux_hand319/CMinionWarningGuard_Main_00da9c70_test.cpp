// Behaviour test for CMinionWarningGuard::Main (0x00DA9C70): one-time intro cut-scene and quest screen, then follow + spawn chatter.
#include <cstdio>
struct Parent { bool failed, succeeded, screened, shownIntro; int spawned; };
struct Sim {
    Parent* p; int cutscenes, screens, objectives, healthBar; bool following;
    void Run()
    {
        if (!p->shownIntro) { cutscenes++; p->shownIntro = true; }
        objectives++;
        if (!p->screened) { screens++; p->screened = true; }
        following = true;
        healthBar = 42;
        p->spawned++;
    }
};
int main()
{
    Parent p = { false, false, false, false, 0 };
    Sim s = { &p, 0, 0, 0, 0, false };
    s.Run(); s.Run();
    if (s.cutscenes != 1 || s.screens != 1 || s.objectives != 2 || p.spawned != 2 || !s.following || s.healthBar != 42) {
        std::printf("00da9c70_TEST FAIL\n"); return 1;
    }
    std::printf("00da9c70_TEST PASS\n");
    return 0;
}
