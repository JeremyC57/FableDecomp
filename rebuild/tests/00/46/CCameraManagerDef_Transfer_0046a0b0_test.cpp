// behaviour test for 0x0046A0B0 CCameraManagerDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x48, +0x54, +0x58, +0x5C, +0x60, +0x64, +0x68, +0x6C, +0x70),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob12 { unsigned char b[12]; };
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
    Blob12 CameraList;
    Blob12 SpecialityCameraList;
    long InitialMode;
    long ZTargetMode;
    long ProjectileWeaponMode;
    float BaseHeightToKeepAboveFlatLandscape;
    float BaseHeightToKeepAboveWater;
    float MaxHeightRelativeToGround;
    float MaxHeightStepForPointAcceptance;
    float AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.CameraList);
    c.Transfer(d.SpecialityCameraList);
    c.Transfer(d.InitialMode);
    c.Transfer(d.ZTargetMode);
    c.Transfer(d.ProjectileWeaponMode);
    c.Transfer(d.BaseHeightToKeepAboveFlatLandscape);
    c.Transfer(d.BaseHeightToKeepAboveWater);
    c.Transfer(d.MaxHeightRelativeToGround);
    c.Transfer(d.MaxHeightStepForPointAcceptance);
    c.Transfer(d.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[10] = { &d.CameraList, &d.SpecialityCameraList, &d.InitialMode, &d.ZTargetMode, &d.ProjectileWeaponMode, &d.BaseHeightToKeepAboveFlatLandscape, &d.BaseHeightToKeepAboveWater, &d.MaxHeightRelativeToGround, &d.MaxHeightStepForPointAcceptance, &d.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat };
    if (save.count != 10u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 10u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.CameraList, 0, sizeof(e.CameraList));
    std::memset(&e.SpecialityCameraList, 0, sizeof(e.SpecialityCameraList));
    std::memset(&e.InitialMode, 0, sizeof(e.InitialMode));
    std::memset(&e.ZTargetMode, 0, sizeof(e.ZTargetMode));
    std::memset(&e.ProjectileWeaponMode, 0, sizeof(e.ProjectileWeaponMode));
    std::memset(&e.BaseHeightToKeepAboveFlatLandscape, 0, sizeof(e.BaseHeightToKeepAboveFlatLandscape));
    std::memset(&e.BaseHeightToKeepAboveWater, 0, sizeof(e.BaseHeightToKeepAboveWater));
    std::memset(&e.MaxHeightRelativeToGround, 0, sizeof(e.MaxHeightRelativeToGround));
    std::memset(&e.MaxHeightStepForPointAcceptance, 0, sizeof(e.MaxHeightStepForPointAcceptance));
    std::memset(&e.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat, 0, sizeof(e.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("0046a0b0_TEST FAIL %d\n", fail); return 1; }
    std::printf("0046a0b0_TEST PASS\n");
    return 0;
}