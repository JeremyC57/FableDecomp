// behaviour test for 0x004E485C CBalverineBattleDef::operator=: the base part is assigned exactly once through the base's operator= (with the source), the three own members are copied, the object's dynamic type is untouched and *this is returned
#include <cstdio>
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
    float              SecondsBetweenLunges;                     // +0x3c
    float              LungeAttackDamage;
    float              TimeInTrees;
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static bool Same(float a, float b) { return Near(a, b); }
static bool Same(long a, long b) { return a == b; }
static bool Same(unsigned long a, unsigned long b) { return a == b; }
static bool Same(bool a, bool b) { return a == b; }
int main()
{
    int fails = 0;
    Def a, b;
    a.Tag = 5;
    a.SecondsBetweenLunges = 1.5f;
    a.LungeAttackDamage = -2.25f;
    a.TimeInTrees = 1.5f;
    b.SecondsBetweenLunges = -2.25f;
    b.LungeAttackDamage = 1.5f;
    b.TimeInTrees = -2.25f;
    Def& r = (b = a);
    if (&r != &b) fails |= 1;
    if (gBaseAssign != 1 || gBaseFrom != &a || b.Tag != 5) fails |= 2;
    if (!Same(b.SecondsBetweenLunges, a.SecondsBetweenLunges)) fails |= 4;
    if (!Same(b.LungeAttackDamage, a.LungeAttackDamage)) fails |= 8;
    if (!Same(b.TimeInTrees, a.TimeInTrees)) fails |= 32;
    Base* pb = &b;
    if (pb->Kind() != 2) fails |= 16;
    if (fails) { std::printf("004e485c_TEST FAIL %d\n", fails); return 1; }
    std::printf("004e485c_TEST PASS\n");
    return 0;
}