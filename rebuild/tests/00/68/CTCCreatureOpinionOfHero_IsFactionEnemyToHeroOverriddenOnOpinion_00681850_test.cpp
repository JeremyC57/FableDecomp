// Behaviour test for 0x00681850 CTCCreatureOpinionOfHero::IsFactionEnemyToHeroOverriddenOnOpinion(): false while the hero is
// this creature's opinion enemy; otherwise true exactly for attitudes 6 and 12..17 (retail byte table 00 01 01 01 01 01 00 x6
// over 6..17, entry 0 = true).
#include <cstdio>

static long g_attitude = 0;
static int g_calls = 0;
class COpinionReactionManager {
public:
    long GetThingAttitudeToPlayer(const int&) const { ++g_calls; return g_attitude; }
};
class CTCCreatureOpinionOfHero {
public:
    CTCCreatureOpinionOfHero() : ParentThing(&Thing), Thing(0), HeroOpinionEnemy(false) { }
    bool IsFactionEnemyToHeroOverriddenOnOpinion() const
    {
        if (HeroOpinionEnemy)
            return false;
        const int& thing = *ParentThing;
        const COpinionReactionManager* manager = &Manager;
        unsigned long index = manager->GetThingAttitudeToPlayer(thing) - 6;
        if (index > 11)
            return false;
        switch (index) {
        case 0: case 6: case 7: case 8: case 9: case 10: case 11:
            return true;
        case 1: case 2: case 3: case 4: case 5:
            return false;
        }
        return false;
    }
    int* ParentThing;
    int Thing;
    bool HeroOpinionEnemy;
    COpinionReactionManager Manager;
};

int main()
{
    CTCCreatureOpinionOfHero c;
    for (long a = -2; a < 24; ++a) {
        g_attitude = a;
        bool expected = (a == 6) || (a >= 12 && a <= 17);
        if (c.IsFactionEnemyToHeroOverriddenOnOpinion() != expected) {
            std::printf("00681850_TEST FAIL (attitude %ld)\n", a);
            return 1;
        }
    }
    c.HeroOpinionEnemy = true;
    g_attitude = 6;
    int before = g_calls;
    if (c.IsFactionEnemyToHeroOverriddenOnOpinion() || g_calls != before) {
        std::printf("00681850_TEST FAIL (enemy)\n");
        return 1;
    }
    std::printf("00681850_TEST PASS\n");
    return 0;
}