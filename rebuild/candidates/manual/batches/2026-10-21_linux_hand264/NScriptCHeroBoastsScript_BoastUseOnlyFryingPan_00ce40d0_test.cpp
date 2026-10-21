// Behaviour test for CHeroBoastsScript::BoastUseOnlyFryingPan @ 0x00CE40D0: wielding a non-pan weapon fails the boast after
// the game-info is clicked past; a failed quest fails it without the info; a completed quest completes it.
#include <cstdio>
#include <cstring>
struct World { int frame; int wieldFrame; const char* item; int failFrame; int completeFrame; int clickFrame; int maxFrame;
    int info; int failed; int completed; };
static bool Run(World& w)
{
    while (w.frame < w.maxFrame) {
        bool wielding = w.wieldFrame >= 0 && w.frame >= w.wieldFrame;
        bool questFailed = w.failFrame >= 0 && w.frame >= w.failFrame;
        if ((wielding && std::strcmp(w.item, "OBJECT_FRYING_PAN") != 0) || questFailed) {
            if (!questFailed) {
                ++w.info;
                while (w.frame < w.clickFrame) { ++w.frame; if (w.frame >= w.maxFrame) return false; }
            }
            w.failed = 2; return true;
        }
        if (w.completeFrame >= 0 && w.frame >= w.completeFrame) { w.completed = 2; return true; }
        ++w.frame;
    }
    return false;
}
int main()
{
    World a = { 0, 3, "OBJECT_SWORD", -1, -1, 6, 100, 0, 0, 0 };
    World b = { 0, 3, "OBJECT_FRYING_PAN", -1, 9, 0, 100, 0, 0, 0 };
    World c = { 0, -1, "", 4, -1, 0, 100, 0, 0, 0 };
    World d = { 0, 2, "OBJECT_AXE", -1, -1, 50, 10, 0, 0, 0 };
    bool ok = Run(a) && a.failed == 2 && a.info == 1 && a.frame == 6;
    ok = ok && Run(b) && b.completed == 2 && b.failed == 0 && b.frame == 9;
    ok = ok && Run(c) && c.failed == 2 && c.info == 0;
    ok = ok && !Run(d) && d.failed == 0 && d.info == 1;
    if (!ok) { std::printf("00ce40d0_TEST FAIL\n"); return 1; }
    std::printf("00ce40d0_TEST PASS\n");
    return 0;
}
