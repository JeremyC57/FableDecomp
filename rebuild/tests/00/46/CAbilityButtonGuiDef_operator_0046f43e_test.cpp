// behaviour test for 0x0046F43E CAbilityButtonGuiDef::operator=: the base part is assigned first (once), then every member (ControllerType, ControllerButton, MouseButton, ButtonGraphic, Position, Offset)
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
    int ControllerType;
    int ControllerButton;
    int MouseButton;
    int ButtonGraphic;
    Vec2 Position;
    Vec2 Offset;
};
int main()
{
    int fail = 0;
    Def a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    a.Tag = 11; b.Tag = 22;
    b.ControllerType = 3;
    b.ControllerButton = 10;
    b.MouseButton = 17;
    b.ButtonGraphic = 24;
    b.Position = Vec2(4.25f, 4.75f);
    b.Offset = Vec2(5.25f, 5.75f);
    Def& r = (a = b);
    if (&r != &a) fail |= 1;
    if (gBaseCalls != 1) fail |= 2;
    if (!(a.ControllerType == b.ControllerType) || !(a.ControllerButton == b.ControllerButton) || !(a.MouseButton == b.MouseButton) || !(a.ButtonGraphic == b.ButtonGraphic) || !(a.Position == b.Position) || !(a.Offset == b.Offset)) fail |= 4;
    if (a.Tag != 11) fail |= 8;
    if (fail) { std::printf("0046f43e_TEST FAIL %d\n", fail); return 1; }
    std::printf("0046f43e_TEST PASS\n");
    return 0;
}