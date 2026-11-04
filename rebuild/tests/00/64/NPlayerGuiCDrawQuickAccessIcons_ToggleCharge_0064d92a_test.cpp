// Behaviour test for 0x0064D92A NPlayerGui::CDrawQuickAccessIcons::ToggleCharge(long, bool): a model of the row over a stub hero;
// checks stopping (sound cleared, button -1, not charging), nothing when the hero is dead, the inventory check (no quick-access
// item def -> no change; -2 skips it), the charge-up sound started only when none is playing and the hero has a sound player,
// and the charge state set (time 0, button, charging, not fully charged).
#include <cstdio>
struct Hero { bool alive, hasItem, hasSound; };
struct Icons {
    unsigned long ChargingSoundID; long ChargingButtonIndex; float ChargingButtonTime; bool Charging, FullyCharged; int clears, plays;
    void ToggleCharge(const Hero& hero, long buttonIndex, bool start)
    {
        if (start) {
            if (!hero.alive) return;
            if (buttonIndex != -2 && !hero.hasItem) return;
            if (ChargingSoundID == 0 && hero.hasSound) { ++plays; ChargingSoundID = 77; }
            ChargingButtonTime = 0.0f; ChargingButtonIndex = buttonIndex; Charging = true; FullyCharged = false;
        } else {
            ++clears; ChargingSoundID = 0; ChargingButtonIndex = -1; Charging = false;
        }
    }
};
int main()
{
    int ok = 1;
    Icons i = { 0, -1, 5.0f, false, true, 0, 0 };
    Hero dead = { false, true, true }, noItem = { true, false, true }, hero = { true, true, true };
    i.ToggleCharge(dead, 3, true); i.ToggleCharge(noItem, 3, true);
    ok &= !i.Charging && i.ChargingButtonIndex == -1 && i.plays == 0;
    i.ToggleCharge(noItem, -2, true);
    ok &= i.Charging && i.ChargingButtonIndex == -2 && i.ChargingButtonTime == 0.0f && !i.FullyCharged && i.plays == 1;
    i.ToggleCharge(hero, 2, true); ok &= i.plays == 1 && i.ChargingButtonIndex == 2;   // sound already playing
    i.ToggleCharge(hero, 2, false); ok &= i.clears == 1 && !i.Charging && i.ChargingButtonIndex == -1 && i.ChargingSoundID == 0;
    std::printf(ok ? "0064D92A_TEST PASS\n" : "0064D92A_TEST FAIL\n");
    return ok ? 0 : 1;
}