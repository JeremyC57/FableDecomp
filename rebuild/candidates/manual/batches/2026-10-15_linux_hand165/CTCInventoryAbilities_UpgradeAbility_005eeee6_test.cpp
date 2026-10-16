#include <cstdio>
// Behaviour model of CTCInventoryAbilities::UpgradeAbility (0x005EEEE6): an owned ability (level > -1) rises one level
// while below its maximum (cost-list size - 1, clamped to [0,3]); an unowned one is learnt; then the hero ages.
struct Ability { long Level; long costs; };
struct Inv {
    Ability abilities[4]; int learnt; int morphs; float age; float agePerUpgrade;
    long MaxLevel(long a)
    {
        if (a != 0) { long l = abilities[a].costs - 1; if (l < 0) l = 0; return l < 3 ? l : 3; }
        return 0;
    }
    void Upgrade(long a)
    {
        long level = abilities[a].Level;
        if (level > -1) {
            if (level < MaxLevel(a)) { ++abilities[a].Level; ++morphs; }
        } else {
            ++learnt;
        }
        age += agePerUpgrade;
    }
};
int main()
{
    Inv v = { { { 0, 9 }, { 1, 3 }, { -1, 2 }, { 3, 10 } }, 0, 0, 20.0f, 0.5f };
    v.Upgrade(1);
    if (v.abilities[1].Level != 2 || v.morphs != 1 || v.age != 20.5f) { std::printf("005eeee6_TEST FAIL\n"); return 1; }
    v.Upgrade(1);
    if (v.abilities[1].Level != 2 || v.morphs != 1 || v.age != 21.0f) { std::printf("005eeee6_TEST FAIL\n"); return 1; }
    v.Upgrade(2); v.Upgrade(3);
    if (v.learnt != 1 || v.abilities[3].Level != 3 || v.MaxLevel(0) != 0) { std::printf("005eeee6_TEST FAIL\n"); return 1; }
    std::printf("005eeee6_TEST PASS\n");
    return 0;
}
