// Behaviour test for 0x004560E1 (CHeroStatsDef constructor): the base runs first, the vtable is the derived one, and every vector starts empty.
#include <cstdio>
static int g_baseCalls = 0;
struct Base { virtual ~Base() {} virtual int Kind() const { return 0; } unsigned char body[0x38]; Base() { ++g_baseCalls; } };
template <class T> struct Vec { T* first; T* last; T* end; Vec() : first(0), last(0), end(0) {} bool empty() const { return first == last; } };
struct HeroStatsDef : Base {
    long MaximumStamina; float MinAge, MaxAge;
    Vec<int> AppearanceModifierScaling, RenownLevelCosts, RenownLevelMaxFollowers, RenownLevelMaxBoastingCrowd,
             TradeSkillForGuileLevel, StatLevels, MaxHitPointsPerHealthLevel, MaxStaminaPointsPerMagicPowerLevel,
             SoundDistanceMultiplierPerStealthLevel, VisibilityMultiplierPerStealthLevel;
    virtual int Kind() const { return 1; }
    HeroStatsDef() {}
};
int main()
{
    HeroStatsDef d;
    const Vec<int>* v[10] = { &d.AppearanceModifierScaling, &d.RenownLevelCosts, &d.RenownLevelMaxFollowers, &d.RenownLevelMaxBoastingCrowd,
        &d.TradeSkillForGuileLevel, &d.StatLevels, &d.MaxHitPointsPerHealthLevel, &d.MaxStaminaPointsPerMagicPowerLevel,
        &d.SoundDistanceMultiplierPerStealthLevel, &d.VisibilityMultiplierPerStealthLevel };
    int ok = g_baseCalls == 1 && d.Kind() == 1;
    for (int i = 0; i < 10; ++i) if (v[i]->first || v[i]->last || v[i]->end || !v[i]->empty()) ok = 0;
    if (!ok) { std::printf("004560e1_TEST FAIL\n"); return 1; }
    std::printf("004560e1_TEST PASS\n");
    return 0;
}