extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x007332F0 CCombatAbility::IsValid: an active ability (ActiveTimer >= 0) is valid without touching the
// seed; an idle usable one (RepeatTimer == -1) steps the seed (seed * 0x24A1 + 0x24DF, then GFROR13) and passes when the new
// seed mod 100 is below PercentageChance; otherwise false.
static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
class CCombatAbilityData { public: long PercentageChance; };
class CCombatAbility { public:
    long RepeatTimer, ActiveTimer;
    bool IsValid(const CCombatAbilityData& d, unsigned long* seed) const
    {
        if (ActiveTimer >= 0) return true;
        if (RepeatTimer == -1) {
            *seed = *seed * 0x24A1 + 0x24DF;
            *seed = GFROR13(*seed);
            if (*seed % 100 < (unsigned long)d.PercentageChance) return true;
        }
        return false;
    }
};
int main()
{
    CCombatAbility a; CCombatAbilityData d; unsigned long seed = 5;
    a.ActiveTimer = 0; a.RepeatTimer = 3; d.PercentageChance = 0;
    CHECK(a.IsValid(d, &seed)); CHECK(seed == 5);
    a.ActiveTimer = -1; a.RepeatTimer = 3;
    CHECK(!a.IsValid(d, &seed)); CHECK(seed == 5);
    a.RepeatTimer = -1; d.PercentageChance = 100;
    CHECK(a.IsValid(d, &seed));
    unsigned long expect = GFROR13(5UL * 0x24A1 + 0x24DF); CHECK(seed == expect);
    d.PercentageChance = 0; CHECK(!a.IsValid(d, &seed));
    unsigned long s2 = 1; d.PercentageChance = (long)(GFROR13(1UL * 0x24A1 + 0x24DF) % 100);
    CHECK(!a.IsValid(d, &s2));                                   // equal is not below
    s2 = 1; d.PercentageChance += 1; CHECK(a.IsValid(d, &s2));
    if (fails == 0) printf("007332f0_TEST PASS\n"); else printf("007332f0_TEST FAIL\n");
    return fails;
}