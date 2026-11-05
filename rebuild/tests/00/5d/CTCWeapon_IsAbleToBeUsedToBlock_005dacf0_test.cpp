// Behaviour test for 0x005dacf0: CTCWeapon::IsAbleToBeUsedToBlock -- blocks exactly when the blocker can use the weapon
// Host model: IsUsableBy answers from a settable flag and records the thing it was asked about.
#include <cstdio>
#include <cstddef>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CThing { int Id; };
struct CTCWeapon {
    bool Usable; mutable const CThing* Asked; mutable int Calls;
    bool IsUsableBy(const CThing& thing) const { Asked = &thing; ++Calls; return Usable; }
    bool IsAbleToBeUsedToBlock(const CThing& blocker, const CThing* attacker, const CThing* attackerWeapon) const
    {
        if (!IsUsableBy(blocker))
            return false;
        if (attacker == NULL)
            return true;
        return true;
    }
};
int main()
{
    CThing hero = { 1 }, bandit = { 2 }, sword = { 3 };
    CTCWeapon w; w.Usable = true; w.Asked = 0; w.Calls = 0;
    CHECK(w.IsAbleToBeUsedToBlock(hero, &bandit, &sword) == true);
    CHECK(w.Asked == &hero && w.Calls == 1);
    CHECK(w.IsAbleToBeUsedToBlock(hero, NULL, NULL) == true);
    w.Usable = false;
    CHECK(w.IsAbleToBeUsedToBlock(hero, &bandit, &sword) == false);
    CHECK(w.IsAbleToBeUsedToBlock(bandit, NULL, &sword) == false);
    CHECK(w.Asked == &bandit && w.Calls == 4);
    if (g_fail) { std::printf("005dacf0_TEST FAIL\n"); return 1; }
    std::printf("005dacf0_TEST PASS\n");
    return 0;
}
