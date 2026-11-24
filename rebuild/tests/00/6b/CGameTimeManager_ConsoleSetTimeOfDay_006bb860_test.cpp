#include <stdio.h>
#include <vector>
#include "engine/GFLimit.h"

class CGenericVar {
public:
    explicit CGenericVar(float value) : ValFloat(value) {}
    float GetFloat() const { return ValFloat; }
private:
    float ValFloat;
};

class CGameTimeManager {
public:
    float Time, PrevTime;
    bool UsingVillageTime, VillageDay;
    static void FABLE_FASTCALL ConsoleSetTimeOfDay(CGameTimeManager&, const std::vector<CGenericVar>&);
};

void FABLE_FASTCALL CGameTimeManager::ConsoleSetTimeOfDay(
    CGameTimeManager& owner, const std::vector<CGenericVar>& params)
{
    owner.Time = owner.PrevTime = params[0].GetFloat() / 24.0f;
    if (!owner.UsingVillageTime) {
        GFLimit(owner.Time, 0.0f, 1.0f);
    } else if (owner.VillageDay) {
        GFLimit(owner.Time, 9.0f / 24.0f, 17.0f / 24.0f);
    } else if (owner.Time > 5.5f / 24.0f && owner.Time < 19.5f / 24.0f) {
        owner.Time = 19.5f / 24.0f;
    }
}

int main()
{
    struct Case { bool village, day; float hours, expected; };
    const Case cases[] = {
        {false, false, -6, 0}, {false, false, 0, 0}, {false, true, 12, 0.5f},
        {false, true, 24, 1}, {false, false, 30, 1},
        {true, true, 3, 0.375f}, {true, true, 9, 0.375f}, {true, true, 12, 0.5f},
        {true, true, 17, 17.0f/24.0f}, {true, true, 24, 17.0f/24.0f},
        {true, false, 3, 0.125f}, {true, false, 5.5f, 5.5f/24.0f},
        {true, false, 12, 0.8125f}, {true, false, 19.5f, 0.8125f}, {true, false, 24, 1}
    };
    for (unsigned i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i) {
        const Case& c = cases[i];
        CGameTimeManager manager;
        manager.Time = manager.PrevTime = -2;
        manager.UsingVillageTime = c.village;
        manager.VillageDay = c.day;
        std::vector<CGenericVar> params(1, CGenericVar(c.hours));
        CGameTimeManager::ConsoleSetTimeOfDay(manager, params);
        const float previous = c.hours / 24.0f;
        if (manager.Time != c.expected || manager.PrevTime != previous ||
            manager.UsingVillageTime != c.village || manager.VillageDay != c.day)
            return i + 1;
    }
    puts("006bb860_TEST PASS");
}