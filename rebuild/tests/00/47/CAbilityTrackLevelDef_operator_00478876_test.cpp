// behaviour test for 0x00478876 CAbilityTrackLevelDef::operator=: the base part is assigned exactly once through the base's operator= (with the source), the three own members are copied, the object's dynamic type is untouched and *this is returned
#include <cstdio>
enum EHeroAbility { EHeroAbility_TestRange = 0xFF };
static int gBaseAssign = 0;
static const void* gBaseFrom = 0;
class Base {
public:
    Base() : Tag(1) { }
    virtual ~Base() { }
    virtual int Kind() const { return 1; }
    Base& operator=(const Base& other) { ++gBaseAssign; gBaseFrom = &other; Tag = other.Tag; return *this; }
    int Tag;
};
class Def : public Base {
public:
    virtual int Kind() const { return 2; }
    long               ExperienceCost;                     // +0x3c
    EHeroAbility       AbilityUnlocked;
    long               Description;
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static bool Same(float a, float b) { return Near(a, b); }
static bool Same(long a, long b) { return a == b; }
static bool Same(unsigned long a, unsigned long b) { return a == b; }
static bool Same(bool a, bool b) { return a == b; }
static bool Same(EHeroAbility a, EHeroAbility b) { return a == b; }
int main()
{
    int fails = 0;
    Def a, b;
    a.Tag = 5;
    a.ExperienceCost = -7;
    a.AbilityUnlocked = (EHeroAbility)4;
    a.Description = -7;
    b.ExperienceCost = 123456;
    b.AbilityUnlocked = (EHeroAbility)3;
    b.Description = 123456;
    Def& r = (b = a);
    if (&r != &b) fails |= 1;
    if (gBaseAssign != 1 || gBaseFrom != &a || b.Tag != 5) fails |= 2;
    if (!Same(b.ExperienceCost, a.ExperienceCost)) fails |= 4;
    if (!Same(b.AbilityUnlocked, a.AbilityUnlocked)) fails |= 8;
    if (!Same(b.Description, a.Description)) fails |= 32;
    Base* pb = &b;
    if (pb->Kind() != 2) fails |= 16;
    if (fails) { std::printf("00478876_TEST FAIL %d\n", fails); return 1; }
    std::printf("00478876_TEST PASS\n");
    return 0;
}