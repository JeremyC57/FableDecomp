// Behaviour test for the trader entity script's HandleTrader(long state) (0x00E0A510): the state is always stored;
// state 2 makes the trader follow the hero and picks the health-bar icon by the data string (INFECTED also starts the
// infection timer, long or short by the parent flag; SCARED also sets scared); other strings add nothing.
#include <cstdio>
#include <cstring>

struct Trader {
    long state; long mode; long timerTime; const char* icon; bool following; bool scared; bool interrupted;
    const char* data; bool longInfection;
};

static void HandleTrader(Trader* t, long state)
{
    t->state = state;
    if (state != 2 || t->interrupted)
        return;
    t->following = true;
    if (std::strcmp(t->data, "INFECTED") == 0) {
        t->icon = "HUD_QUEST_ICON_TRADER";
        t->mode = 1;
        t->timerTime = t->longInfection ? 200 : 100;
    } else if (std::strcmp(t->data, "SCARED") == 0) {
        t->icon = "HUD_QUEST_ICON_TRADER_HAT_01";
        t->scared = true;
    } else if (std::strcmp(t->data, "FRIENDLY") == 0) {
        t->icon = "HUD_QUEST_ICON_TRADER_HAT_02";
    }
}

int main()
{
    Trader a = { 0, 0, 0, 0, false, false, false, "INFECTED", true };
    HandleTrader(&a, 2);
    if (a.state != 2 || a.mode != 1 || a.timerTime != 200 || std::strcmp(a.icon, "HUD_QUEST_ICON_TRADER") != 0 || !a.following) {
        std::printf("00e0a510_TEST FAIL\n"); return 1;
    }
    Trader b = { 0, 0, 0, 0, false, false, false, "SCARED", false };
    HandleTrader(&b, 2);
    if (!b.scared || b.mode != 0 || std::strcmp(b.icon, "HUD_QUEST_ICON_TRADER_HAT_01") != 0) { std::printf("00e0a510_TEST FAIL\n"); return 1; }
    Trader c = { 0, 0, 0, 0, false, false, false, "FRIENDLY", false };
    HandleTrader(&c, 1);
    if (c.state != 1 || c.following || c.icon != 0) { std::printf("00e0a510_TEST FAIL\n"); return 1; }
    HandleTrader(&c, 2);
    if (std::strcmp(c.icon, "HUD_QUEST_ICON_TRADER_HAT_02") != 0 || c.scared) { std::printf("00e0a510_TEST FAIL\n"); return 1; }
    Trader d = { 0, 0, 0, 0, false, false, false, "OTHER", false };
    HandleTrader(&d, 2);
    if (d.icon != 0 || !d.following) { std::printf("00e0a510_TEST FAIL\n"); return 1; }
    std::printf("00e0a510_TEST PASS\n");
    return 0;
}