// behaviour test for 0x00452BE2 CDragonActionSwoopDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x60),
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
    float IntoDelay;
    float OutOfDelay;
    float Distance;
    float MidPointsControlOffset;
    float StartHeight;
    float MidHeight;
    float EndHeight;
    float FlightSpeed;
    float MaxSplineProgressForSplineAdjustment;
    float MaxSplineProgressForBurninationEffect;
    float MinSplineProgressForWingFlap;
    float MaxExplosionRange;
    float ExplosionLineProjectionAngle;
    long FramesBetweenExplosions;
    float WindSpeed;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.IntoDelay);
    c.Transfer(d.OutOfDelay);
    c.Transfer(d.Distance);
    c.Transfer(d.MidPointsControlOffset);
    c.Transfer(d.StartHeight);
    c.Transfer(d.MidHeight);
    c.Transfer(d.EndHeight);
    c.Transfer(d.FlightSpeed);
    c.Transfer(d.MaxSplineProgressForSplineAdjustment);
    c.Transfer(d.MaxSplineProgressForBurninationEffect);
    c.Transfer(d.MinSplineProgressForWingFlap);
    c.Transfer(d.MaxExplosionRange);
    c.Transfer(d.ExplosionLineProjectionAngle);
    c.Transfer(d.FramesBetweenExplosions);
    c.Transfer(d.WindSpeed);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[15] = { &d.IntoDelay, &d.OutOfDelay, &d.Distance, &d.MidPointsControlOffset, &d.StartHeight, &d.MidHeight, &d.EndHeight, &d.FlightSpeed, &d.MaxSplineProgressForSplineAdjustment, &d.MaxSplineProgressForBurninationEffect, &d.MinSplineProgressForWingFlap, &d.MaxExplosionRange, &d.ExplosionLineProjectionAngle, &d.FramesBetweenExplosions, &d.WindSpeed };
    if (save.count != 15u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 15u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.IntoDelay, 0, sizeof(e.IntoDelay));
    std::memset(&e.OutOfDelay, 0, sizeof(e.OutOfDelay));
    std::memset(&e.Distance, 0, sizeof(e.Distance));
    std::memset(&e.MidPointsControlOffset, 0, sizeof(e.MidPointsControlOffset));
    std::memset(&e.StartHeight, 0, sizeof(e.StartHeight));
    std::memset(&e.MidHeight, 0, sizeof(e.MidHeight));
    std::memset(&e.EndHeight, 0, sizeof(e.EndHeight));
    std::memset(&e.FlightSpeed, 0, sizeof(e.FlightSpeed));
    std::memset(&e.MaxSplineProgressForSplineAdjustment, 0, sizeof(e.MaxSplineProgressForSplineAdjustment));
    std::memset(&e.MaxSplineProgressForBurninationEffect, 0, sizeof(e.MaxSplineProgressForBurninationEffect));
    std::memset(&e.MinSplineProgressForWingFlap, 0, sizeof(e.MinSplineProgressForWingFlap));
    std::memset(&e.MaxExplosionRange, 0, sizeof(e.MaxExplosionRange));
    std::memset(&e.ExplosionLineProjectionAngle, 0, sizeof(e.ExplosionLineProjectionAngle));
    std::memset(&e.FramesBetweenExplosions, 0, sizeof(e.FramesBetweenExplosions));
    std::memset(&e.WindSpeed, 0, sizeof(e.WindSpeed));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00452be2_TEST FAIL %d\n", fail); return 1; }
    std::printf("00452be2_TEST PASS\n");
    return 0;
}