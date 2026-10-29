// Behaviour test for 0x00F088E0 NScript::CQ_AmbushTradersScript::EntityCountDecrease: a CREATURE_TRADER_01 death lowers
// TraderAliveCount, the four Bowerstone guard defs lower GuardAliveCount, every call (unless the thread terminates inside a
// matching arm) raises AmbushTradersKillCount; a null (empty) name only counts the kill.
#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* PStringData;   // 0 = the empty string
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return std::strcmp(text, "") == 0;
        return std::strcmp(PStringData, text) == 0;
    }
};
struct CQ_SunnyvaleMasterData { long AmbushTradersKillCount; };
struct CQ_AmbushTradersScript {
    CQ_SunnyvaleMasterData* MasterScriptData; long TraderAliveCount; long GuardAliveCount; bool Terminating;
    bool IsActiveThreadTerminating() { return Terminating; }
    void EntityCountDecrease(CCharString defName)
    {
        if (defName == "CREATURE_TRADER_01") {
            if (IsActiveThreadTerminating())
                return;
            --TraderAliveCount;
        } else if (defName == "CREATURE_BS_GUARD" || defName == "CREATURE_BS_GUARD_RED" || defName == "CREATURE_BS_GUARD_BLACK"
                   || defName == "CREATURE_BS_GUARD_CROSSBOW") {
            if (IsActiveThreadTerminating())
                return;
            --GuardAliveCount;
        }
        ++MasterScriptData->AmbushTradersKillCount;
    }
};

static CCharString Name(const char* s) { CCharString c; c.PStringData = s; return c; }

int main()
{
    CQ_SunnyvaleMasterData data; data.AmbushTradersKillCount = 0;
    CQ_AmbushTradersScript s; s.MasterScriptData = &data; s.TraderAliveCount = 5; s.GuardAliveCount = 9; s.Terminating = false;
    s.EntityCountDecrease(Name("CREATURE_TRADER_01"));
    s.EntityCountDecrease(Name("CREATURE_BS_GUARD"));
    s.EntityCountDecrease(Name("CREATURE_BS_GUARD_RED"));
    s.EntityCountDecrease(Name("CREATURE_BS_GUARD_BLACK"));
    s.EntityCountDecrease(Name("CREATURE_BS_GUARD_CROSSBOW"));
    s.EntityCountDecrease(Name("CREATURE_BANDIT"));
    s.EntityCountDecrease(Name(0));
    int ok = s.TraderAliveCount == 4 && s.GuardAliveCount == 5 && data.AmbushTradersKillCount == 7;
    s.Terminating = true;
    s.EntityCountDecrease(Name("CREATURE_TRADER_01"));   // returns before any count
    s.EntityCountDecrease(Name("CREATURE_BS_GUARD"));
    s.EntityCountDecrease(Name("CREATURE_BANDIT"));      // no arm: the kill still counts
    ok = ok && s.TraderAliveCount == 4 && s.GuardAliveCount == 5 && data.AmbushTradersKillCount == 8;
    if (!ok) { std::printf("00f088e0_TEST FAIL\n"); return 1; }
    std::printf("00f088e0_TEST PASS\n");
    return 0;
}