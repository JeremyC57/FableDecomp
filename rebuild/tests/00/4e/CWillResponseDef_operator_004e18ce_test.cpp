// behaviour test for 0x004E18CE CWillResponseDef::operator=: the base part is assigned first (once), then every member (IsForcePushable, IsLifeDrainable, IsLightningable, IsNotBloodyWillable, IsAffectedBySlowTime, IsAbleToBeLifted, IsEpicPowerExplodable, IsEpicSpellable)
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
    bool IsForcePushable;
    bool IsLifeDrainable;
    bool IsLightningable;
    bool IsNotBloodyWillable;
    bool IsAffectedBySlowTime;
    bool IsAbleToBeLifted;
    bool IsEpicPowerExplodable;
    bool IsEpicSpellable;
};
int main()
{
    int fail = 0;
    Def a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    a.Tag = 11; b.Tag = 22;
    b.IsForcePushable = true;
    b.IsLifeDrainable = true;
    b.IsLightningable = true;
    b.IsNotBloodyWillable = true;
    b.IsAffectedBySlowTime = true;
    b.IsAbleToBeLifted = true;
    b.IsEpicPowerExplodable = true;
    b.IsEpicSpellable = true;
    Def& r = (a = b);
    if (&r != &a) fail |= 1;
    if (gBaseCalls != 1) fail |= 2;
    if (!(a.IsForcePushable == b.IsForcePushable) || !(a.IsLifeDrainable == b.IsLifeDrainable) || !(a.IsLightningable == b.IsLightningable) || !(a.IsNotBloodyWillable == b.IsNotBloodyWillable) || !(a.IsAffectedBySlowTime == b.IsAffectedBySlowTime) || !(a.IsAbleToBeLifted == b.IsAbleToBeLifted) || !(a.IsEpicPowerExplodable == b.IsEpicPowerExplodable) || !(a.IsEpicSpellable == b.IsEpicSpellable)) fail |= 4;
    if (a.Tag != 11) fail |= 8;
    if (fail) { std::printf("004e18ce_TEST FAIL %d\n", fail); return 1; }
    std::printf("004e18ce_TEST PASS\n");
    return 0;
}