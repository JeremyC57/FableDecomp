#include <cstdio>
// Behaviour model of CTCCreatureOpinionOfHero::IsHeroOpinionEnemy (0x006827A0).
struct Village { bool enemyOfGuards; bool GetIsHeroEnemyOfGuards() { return enemyOfGuards; } };
struct Member { Village* village; Village* GetVillage() { return village; } };
struct Thing { bool hasB4; Member* member; };
struct Op {
    bool HeroOpinionEnemy; Thing* parent;
    bool IsHeroOpinionEnemy()
    {
        if (HeroOpinionEnemy) return true;
        if (parent->hasB4) {
            if (parent->member) {
                Village* v = parent->member->GetVillage();
                if (v && v->GetIsHeroEnemyOfGuards()) return true;
            }
        }
        return false;
    }
};
int main()
{
    Village v = { true }; Member m = { &v }; Thing t = { true, &m }; Op o = { false, &t };
    if (!o.IsHeroOpinionEnemy()) { std::printf("006827a0_TEST FAIL\n"); return 1; }
    v.enemyOfGuards = false;
    if (o.IsHeroOpinionEnemy()) { std::printf("006827a0_TEST FAIL\n"); return 1; }
    o.HeroOpinionEnemy = true;
    if (!o.IsHeroOpinionEnemy()) { std::printf("006827a0_TEST FAIL\n"); return 1; }
    o.HeroOpinionEnemy = false; v.enemyOfGuards = true; t.hasB4 = false;
    if (o.IsHeroOpinionEnemy()) { std::printf("006827a0_TEST FAIL\n"); return 1; }
    t.hasB4 = true; m.village = 0;
    if (o.IsHeroOpinionEnemy()) { std::printf("006827a0_TEST FAIL\n"); return 1; }
    std::printf("006827a0_TEST PASS\n");
    return 0;
}