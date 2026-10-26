// behaviour test for 0x00453B4D CCreatureStatsDef::operator=: the base part is assigned first (once), then every member (MoralityForKill, MoralityForPunch, MoralityForWeaponHit, ExperienceWorth, RenownWorth, Speed, Dexterity, StrengthDamageMultiplier)
// is copied, and *this is returned; the base's own bytes are left to the base operator=
#include <cstdio>
#include <cstring>
static int gBaseCalls = 0;
struct Vec2 {
    float X, Y;
    Vec2() : X(0.0f), Y(0.0f) { }
    Vec2(float x, float y) : X(x), Y(y) { }
    bool operator==(const Vec2& o) const { return X == o.X && Y == o.Y; }
};
struct Base {
    int Tag;
    Base() : Tag(0) { }
    Base& operator=(const Base&) { ++gBaseCalls; return *this; }
};
struct Def : public Base {
    int MoralityForKill;
    int MoralityForPunch;
    int MoralityForWeaponHit;
    int ExperienceWorth;
    int RenownWorth;
    float Speed;
    float Dexterity;
    float StrengthDamageMultiplier;
};
int main()
{
    int fail = 0;
    Def a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    a.Tag = 11; b.Tag = 22;
    b.MoralityForKill = 3;
    b.MoralityForPunch = 10;
    b.MoralityForWeaponHit = 17;
    b.ExperienceWorth = 24;
    b.RenownWorth = 31;
    b.Speed = 1.5f + 5;
    b.Dexterity = 1.5f + 6;
    b.StrengthDamageMultiplier = 1.5f + 7;
    Def& r = (a = b);
    if (&r != &a) fail |= 1;
    if (gBaseCalls != 1) fail |= 2;
    if (!(a.MoralityForKill == b.MoralityForKill) || !(a.MoralityForPunch == b.MoralityForPunch) || !(a.MoralityForWeaponHit == b.MoralityForWeaponHit) || !(a.ExperienceWorth == b.ExperienceWorth) || !(a.RenownWorth == b.RenownWorth) || !(a.Speed == b.Speed) || !(a.Dexterity == b.Dexterity) || !(a.StrengthDamageMultiplier == b.StrengthDamageMultiplier)) fail |= 4;
    if (a.Tag != 11) fail |= 8;
    if (fail) { std::printf("00453b4d_TEST FAIL %d\n", fail); return 1; }
    std::printf("00453b4d_TEST PASS\n");
    return 0;
}