// behaviour test for 0x004DE28C CLookDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x4D, +0x4E, +0x4F, +0x50, +0x54),
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
    float EyesTurnRangeXY;
    float EyesTurnRangeYZ;
    float HeadTurnRangeXY;
    float HeadTurnRangeYZUp;
    float HeadTurnRangeYZDown;
    float MaxTurnSpeed;
    bool AllowTurningViaAnimation;
    float AnimationTurnSpeedMultiplier;
    float CombatMaxTurnSpeed;
    bool CombatAllowTurningViaAnimation;
    bool AllowOversteer;
    bool AllowUndersteer;
    bool DrawEyePrimitives;
    float CutOffNoLooking;
    bool AlwaysUpdateHeadPos;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.EyesTurnRangeXY);
    c.Transfer(d.EyesTurnRangeYZ);
    c.Transfer(d.HeadTurnRangeXY);
    c.Transfer(d.HeadTurnRangeYZUp);
    c.Transfer(d.HeadTurnRangeYZDown);
    c.Transfer(d.MaxTurnSpeed);
    c.Transfer(d.AllowTurningViaAnimation);
    c.Transfer(d.AnimationTurnSpeedMultiplier);
    c.Transfer(d.CombatMaxTurnSpeed);
    c.Transfer(d.CombatAllowTurningViaAnimation);
    c.Transfer(d.AllowOversteer);
    c.Transfer(d.AllowUndersteer);
    c.Transfer(d.DrawEyePrimitives);
    c.Transfer(d.CutOffNoLooking);
    c.Transfer(d.AlwaysUpdateHeadPos);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.AllowTurningViaAnimation = true;
    d.CombatAllowTurningViaAnimation = true;
    d.AllowOversteer = true;
    d.AllowUndersteer = true;
    d.DrawEyePrimitives = true;
    d.AlwaysUpdateHeadPos = true;
    Context save(0); Persist(d, save);
    const void* want[15] = { &d.EyesTurnRangeXY, &d.EyesTurnRangeYZ, &d.HeadTurnRangeXY, &d.HeadTurnRangeYZUp, &d.HeadTurnRangeYZDown, &d.MaxTurnSpeed, &d.AllowTurningViaAnimation, &d.AnimationTurnSpeedMultiplier, &d.CombatMaxTurnSpeed, &d.CombatAllowTurningViaAnimation, &d.AllowOversteer, &d.AllowUndersteer, &d.DrawEyePrimitives, &d.CutOffNoLooking, &d.AlwaysUpdateHeadPos };
    if (save.count != 15u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 15u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.EyesTurnRangeXY, 0, sizeof(e.EyesTurnRangeXY));
    std::memset(&e.EyesTurnRangeYZ, 0, sizeof(e.EyesTurnRangeYZ));
    std::memset(&e.HeadTurnRangeXY, 0, sizeof(e.HeadTurnRangeXY));
    std::memset(&e.HeadTurnRangeYZUp, 0, sizeof(e.HeadTurnRangeYZUp));
    std::memset(&e.HeadTurnRangeYZDown, 0, sizeof(e.HeadTurnRangeYZDown));
    std::memset(&e.MaxTurnSpeed, 0, sizeof(e.MaxTurnSpeed));
    std::memset(&e.AllowTurningViaAnimation, 0, sizeof(e.AllowTurningViaAnimation));
    std::memset(&e.AnimationTurnSpeedMultiplier, 0, sizeof(e.AnimationTurnSpeedMultiplier));
    std::memset(&e.CombatMaxTurnSpeed, 0, sizeof(e.CombatMaxTurnSpeed));
    std::memset(&e.CombatAllowTurningViaAnimation, 0, sizeof(e.CombatAllowTurningViaAnimation));
    std::memset(&e.AllowOversteer, 0, sizeof(e.AllowOversteer));
    std::memset(&e.AllowUndersteer, 0, sizeof(e.AllowUndersteer));
    std::memset(&e.DrawEyePrimitives, 0, sizeof(e.DrawEyePrimitives));
    std::memset(&e.CutOffNoLooking, 0, sizeof(e.CutOffNoLooking));
    std::memset(&e.AlwaysUpdateHeadPos, 0, sizeof(e.AlwaysUpdateHeadPos));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004de28c_TEST FAIL %d\n", fail); return 1; }
    std::printf("004de28c_TEST PASS\n");
    return 0;
}