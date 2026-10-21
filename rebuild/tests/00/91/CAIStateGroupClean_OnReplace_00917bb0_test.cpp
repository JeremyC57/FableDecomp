// Behaviour test for CAIStateGroup_Clean::OnReplace (0x00917BB0): kneeling -> finish action only; cleaning -> finish the
// cleaning action when it is the current one, post the get-up animation only when free; always reset the timers,
// choose an idle when the scheduler exists and destroy the right-hand item.
#include <cstdio>
#include <cstring>
struct Creature { bool busy; int currentId; bool free; bool hasScheduler; int finished; int posted; int idles; int destroyed; };
struct Group {
    Creature* c; bool cleaning; bool kneeling; int frameToClean; int cleanId; int frame; int fps; char lastItem[32];
    void OnReplace()
    {
        if (kneeling) { ++c->finished; kneeling = false; }
        else if (cleaning) {
            if (c->busy && c->currentId == cleanId) ++c->finished;
            cleaning = false;
            if (c->free) ++c->posted;
        }
        cleanId = -1;
        int f = fps;
        frameToClean = frame + f * 100;
        if (c->hasScheduler) ++c->idles;
        std::strcpy(lastItem, "CARRY_SLOT_RIGHT_HAND"); ++c->destroyed;
    }
};
int main()
{
    int ok = 1;
    Creature c = { true, 7, true, true, 0, 0, 0, 0 };
    Group g = { &c, true, true, 0, 7, 500, 30, "" };
    g.OnReplace();
    ok &= c.finished == 1 && c.posted == 0 && !g.kneeling && g.cleaning && g.cleanId == -1 && g.frameToClean == 3500;
    g.cleanId = 7; g.OnReplace();
    ok &= c.finished == 2 && c.posted == 1 && !g.cleaning && c.idles == 2 && c.destroyed == 2;
    Creature d = { true, 3, false, false, 0, 0, 0, 0 };
    Group h = { &d, true, false, 0, 7, 0, 25, "" };
    h.OnReplace();
    ok &= d.finished == 0 && d.posted == 0 && d.idles == 0 && h.frameToClean == 2500 && std::strcmp(h.lastItem, "CARRY_SLOT_RIGHT_HAND") == 0;
    std::printf(ok ? "00917bb0_TEST PASS\n" : "00917bb0_TEST FAIL\n");
    return ok ? 0 : 1;
}