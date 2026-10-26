// behaviour test for 0x0055A340 NUISystem::CDraggable::SetPositionBeforeDrag: the argument's X and Y are copied into PositionBeforeDrag, nothing else changes
#include <cstdio>
struct Vec2 { float X; float Y; };
struct Comp {
    Vec2 before; Vec2 member; Vec2 after;
    void Set(Vec2& v) { member = v; }                        // the function under test
};
int main()
{
    int fail = 0;
    Comp c; c.before.X = 1.0f; c.before.Y = 2.0f; c.member.X = 0.0f; c.member.Y = 0.0f; c.after.X = 5.0f; c.after.Y = 6.0f;
    Vec2 v; v.X = 3.5f; v.Y = -4.25f;
    c.Set(v);
    if (c.member.X != 3.5f || c.member.Y != -4.25f) fail |= 1;
    if (c.before.X != 1.0f || c.before.Y != 2.0f || c.after.X != 5.0f || c.after.Y != 6.0f) fail |= 2;
    if (v.X != 3.5f || v.Y != -4.25f) fail |= 4;
    if (sizeof(Vec2) != (unsigned int)8) fail |= 8;
    if (fail) { std::printf("0055a340_TEST FAIL %d\n", fail); return 1; }
    std::printf("0055a340_TEST PASS\n");
    return 0;
}