// Behaviour test for 00898c40 CGameScriptInterface::GiveHeroItemsFromContainer: inventory move or reward-container reward with two frames.
#include <cstdio>
#include <cstring>
struct Container { bool has; const char* cls; bool rewardOk; bool addOk; int rewarded; int added; };
static int g_frames;
static bool Give(bool heroAlive, bool valid, Container* c, bool addToInventory, bool runner)
{
    if (heroAlive && valid) {
        if (!addToInventory) {
            if (c->has) {
                bool isReward = !std::strcmp(c->cls, "CTCContainerRewardHero");
                if (isReward && (++c->rewarded, c->rewardOk)) { if (runner) g_frames += 2; return true; }
            }
        } else if (c->has) { ++c->added; return c->addOk; }
    }
    return false;
}
int main()
{
    Container r = { true, "CTCContainerRewardHero", true, false, 0, 0 }, p = { true, "CTCContainer", true, true, 0, 0 };
    bool a = Give(true, true, &r, false, true);
    bool b = Give(true, true, &p, false, true);
    bool c = Give(true, true, &p, true, true);
    bool d = Give(false, true, &r, false, true);
    if (!a || b || !c || d || g_frames != 2 || r.rewarded != 1 || p.added != 1) { std::printf("00898c40_TEST FAIL\n"); return 1; }
    std::printf("00898c40_TEST PASS\n");
    return 0;
}