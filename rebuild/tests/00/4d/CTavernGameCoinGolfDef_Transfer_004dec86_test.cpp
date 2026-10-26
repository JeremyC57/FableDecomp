// behaviour test for 0x004DEC86 CTavernGameCoinGolfDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
    float TargetDistanceScale;
    float JoystickScaleXboxX;
    float JoystickScaleXboxY;
    float JoystickScaleX;
    float JoystickScaleY;
    float JoystickMagBelow;
    float AverageJoystickMagAbove;
    float AverageDrift;
    float MaxPower;
    float RestSpeed;
    long Courses;
    float GoalRadius;
    long StartingCourse;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.TargetDistanceScale);
    c.Transfer(d.JoystickScaleXboxX);
    c.Transfer(d.JoystickScaleXboxY);
    c.Transfer(d.JoystickScaleX);
    c.Transfer(d.JoystickScaleY);
    c.Transfer(d.JoystickMagBelow);
    c.Transfer(d.AverageJoystickMagAbove);
    c.Transfer(d.AverageDrift);
    c.Transfer(d.MaxPower);
    c.Transfer(d.RestSpeed);
    c.Transfer(d.Courses);
    c.Transfer(d.GoalRadius);
    c.Transfer(d.StartingCourse);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[13] = { &d.TargetDistanceScale, &d.JoystickScaleXboxX, &d.JoystickScaleXboxY, &d.JoystickScaleX, &d.JoystickScaleY, &d.JoystickMagBelow, &d.AverageJoystickMagAbove, &d.AverageDrift, &d.MaxPower, &d.RestSpeed, &d.Courses, &d.GoalRadius, &d.StartingCourse };
    if (save.count != 13u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 13u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.TargetDistanceScale, 0, sizeof(e.TargetDistanceScale));
    std::memset(&e.JoystickScaleXboxX, 0, sizeof(e.JoystickScaleXboxX));
    std::memset(&e.JoystickScaleXboxY, 0, sizeof(e.JoystickScaleXboxY));
    std::memset(&e.JoystickScaleX, 0, sizeof(e.JoystickScaleX));
    std::memset(&e.JoystickScaleY, 0, sizeof(e.JoystickScaleY));
    std::memset(&e.JoystickMagBelow, 0, sizeof(e.JoystickMagBelow));
    std::memset(&e.AverageJoystickMagAbove, 0, sizeof(e.AverageJoystickMagAbove));
    std::memset(&e.AverageDrift, 0, sizeof(e.AverageDrift));
    std::memset(&e.MaxPower, 0, sizeof(e.MaxPower));
    std::memset(&e.RestSpeed, 0, sizeof(e.RestSpeed));
    std::memset(&e.Courses, 0, sizeof(e.Courses));
    std::memset(&e.GoalRadius, 0, sizeof(e.GoalRadius));
    std::memset(&e.StartingCourse, 0, sizeof(e.StartingCourse));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004dec86_TEST FAIL %d\n", fail); return 1; }
    std::printf("004dec86_TEST PASS\n");
    return 0;
}