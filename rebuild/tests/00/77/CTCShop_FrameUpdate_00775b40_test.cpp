// Behaviour test for CTCShop::FrameUpdate (0x00775B40), g++ model of the retail logic.
#include <cstdio>
struct Thing { bool dead; int type; bool inside; bool hasVillageMember; bool criminal; bool hasVillage; };
struct Shop {
    Thing* parent; Thing* keeper; unsigned long alarm; long dayLastChecked; bool permanent;
    int crimeShown, alarmOff, removed, restocked, displayed;
};
static Thing* g_hero; static long g_day;
static bool Alive(const Thing* t) { return t != 0 && !t->dead; }
static void FrameUpdate(Shop& s)
{
    const Thing* hero = g_hero;
    if (Alive(hero) && s.parent->type == 3 && s.parent->inside) ++s.crimeShown;
    if (s.alarm && s.parent->hasVillageMember && s.parent->hasVillage && !s.parent->criminal) { ++s.alarmOff; s.alarm = 0; }
    if (Alive(hero) && Alive(s.keeper) && s.dayLastChecked != g_day) {
        ++s.removed; ++s.restocked; s.dayLastChecked = g_day;
        if (s.permanent && Alive(s.keeper)) ++s.displayed;
    }
}
int main()
{
    Thing hero = { false, 1, false, false, false, false };
    Thing building = { false, 3, true, true, false, true };
    Thing keeper = { false, 1, false, false, false, false };
    Shop s = { &building, &keeper, 1, 4, true, 0, 0, 0, 0, 0 };
    g_hero = &hero; g_day = 5;
    FrameUpdate(s);
    int ok = s.crimeShown == 1 && s.alarmOff == 1 && s.alarm == 0 && s.removed == 1 && s.restocked == 1 && s.displayed == 1;
    FrameUpdate(s);                       // same day: no restock
    ok &= s.removed == 1 && s.crimeShown == 2;
    hero.dead = true; g_day = 6; FrameUpdate(s);
    ok &= s.removed == 1 && s.crimeShown == 2;
    hero.dead = false; keeper.dead = true; FrameUpdate(s);
    ok &= s.removed == 1;
    keeper.dead = false; building.criminal = true; s.alarm = 1; s.permanent = false; FrameUpdate(s);
    ok &= s.alarm == 1 && s.removed == 2 && s.displayed == 1;
    std::printf(ok ? "00775b40_TEST PASS\n" : "00775b40_TEST FAIL\n");
    return ok ? 0 : 1;
}