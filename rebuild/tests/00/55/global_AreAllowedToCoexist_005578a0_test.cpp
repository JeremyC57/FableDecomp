// Behaviour test for 005578a0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

enum EGameAction {
    GAME_ACTION_SPECIAL_ATTACK = 8,
    GAME_ACTION_ATTRACT_EXPERIENCE_ORBS = 26,
    GAME_ACTION_RUN = 31,
    GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING = 45,
    GAME_ACTION_ACTIVATE_SPELL_MODE = 86,
    GAME_ACTION_COUNT = 115
};
static bool AreAllowedToCoexist(EGameAction action1, EGameAction action2)
{
    switch (action1) {
    case GAME_ACTION_SPECIAL_ATTACK:
    case GAME_ACTION_RUN:
    case GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING:
        if (action2 == GAME_ACTION_SPECIAL_ATTACK || action2 == GAME_ACTION_RUN ||
            action2 == GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING)
            return true;
        break;
    case GAME_ACTION_ATTRACT_EXPERIENCE_ORBS:
    case GAME_ACTION_ACTIVATE_SPELL_MODE:
        if (action2 == GAME_ACTION_ATTRACT_EXPERIENCE_ORBS || action2 == GAME_ACTION_ACTIVATE_SPELL_MODE)
            return true;
        break;
    }
    return false;
}
static int Group(int a)                                   // the expected partition: 1 = movement/attack, 2 = orbs/spell
{
    if (a == 8 || a == 31 || a == 45) return 1;
    if (a == 26 || a == 86) return 2;
    return 0;
}
static void RunTest()
{
    int pairs = 0;
    for (int a = 0; a < GAME_ACTION_COUNT; ++a)
        for (int b = 0; b < GAME_ACTION_COUNT; ++b) {
            bool expected = Group(a) != 0 && Group(a) == Group(b);
            bool got = AreAllowedToCoexist((EGameAction)a, (EGameAction)b);
            CHECK(got == expected);
            if (got) ++pairs;
        }
    CHECK(pairs == 9 + 4);
    CHECK(AreAllowedToCoexist(GAME_ACTION_RUN, GAME_ACTION_SPECIAL_ATTACK));
    CHECK(!AreAllowedToCoexist(GAME_ACTION_RUN, GAME_ACTION_ACTIVATE_SPELL_MODE));
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("005578a0_TEST FAIL\n");
        return 1;
    }
    std::printf("005578a0_TEST PASS\n");
    return 0;
}