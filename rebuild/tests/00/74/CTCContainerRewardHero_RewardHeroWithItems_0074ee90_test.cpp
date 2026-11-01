// behaviour test: CTCContainerRewardHero::RewardHeroWithItems -- no live hero with a receive-items component: false;
// no gold and no items: TXT_SIGN_NOTHING subtitle (unless the chest def says not to) and false; gold goes to the
// hero's stats (when present); items go to the receiver and the result is true only when there were items.
#include <cstdio>
struct Outcome { bool result; bool message; long money; int given; };
static Outcome Reward(bool hero, bool dead, bool hasReceiver, long gold, int items, bool hasStats,
                      bool hasChestDef, bool displayMessageOnEmpty)
{
    Outcome o; o.result = false; o.message = false; o.money = 0; o.given = 0;
    if (hero && !dead && hasReceiver) {
        if (gold <= 0 && items == 0) {
            if (!hasChestDef || displayMessageOnEmpty)
                o.message = true;
            return o;
        }
        if (gold > 0 && hasStats)
            o.money += gold;
        if (items != 0) {
            o.given = items;
            o.result = true;
            return o;
        }
    }
    return o;
}
int main()
{
    Outcome a = Reward(false, false, true, 5, 1, true, true, true);
    Outcome b = Reward(true, false, true, 0, 0, true, false, false);
    Outcome c = Reward(true, false, true, 0, 0, true, true, false);
    Outcome d = Reward(true, false, true, 25, 0, true, true, true);
    Outcome e = Reward(true, false, true, 25, 2, true, true, true);
    Outcome f = Reward(true, true, true, 25, 2, true, true, true);
    if (a.result || a.message || !b.message || b.result || c.message || d.result || d.money != 25
        || !e.result || e.money != 25 || e.given != 2 || f.result || f.money != 0) {
        std::printf("0074ee90_TEST FAIL\n"); return 1;
    }
    std::printf("0074ee90_TEST PASS\n");
    return 0;
}