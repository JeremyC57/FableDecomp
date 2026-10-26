// behaviour test for 0x00452EA5 CJackDragonDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x2C, +0x28, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x5C, +0x58, +0x60, +0x64, +0x68, +0x6C, +0x70, +0x74),
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
    float MinTimeBetweenSlams;
    long MaxHitsBeforeFlyAway;
    float MaxFirebeamDistance;
    float MinSnapDistance;
    float MaxSnapDistance;
    float MaxTimeAtLongRangeBeforeFlyAway;
    float MaxSlamDistance;
    long MaxHoverLoops;
    float PercentChanceOfFakeLandingSwoop;
    float MaxTimeOnGround;
    float DragonGroundHeightOffsetBodge;
    float HoverBackOffOffset;
    long FramesBetweenBurninateUpdate;
    long FramesBetweenFireBeamUpdate;
    float HoverObstructionRadius;
    float StrafeTriggerDistance;
    float GetHitSnapBuildupTime;
    float NotHitSnapBuildupTime;
    float MaxTimeBeforeSnap;
    float MinRoarDistance;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.MaxHitsBeforeFlyAway);
    c.Transfer(d.MinTimeBetweenSlams);
    c.Transfer(d.MaxFirebeamDistance);
    c.Transfer(d.MinSnapDistance);
    c.Transfer(d.MaxSnapDistance);
    c.Transfer(d.MaxTimeAtLongRangeBeforeFlyAway);
    c.Transfer(d.MaxSlamDistance);
    c.Transfer(d.MaxHoverLoops);
    c.Transfer(d.PercentChanceOfFakeLandingSwoop);
    c.Transfer(d.MaxTimeOnGround);
    c.Transfer(d.DragonGroundHeightOffsetBodge);
    c.Transfer(d.HoverBackOffOffset);
    c.Transfer(d.FramesBetweenFireBeamUpdate);
    c.Transfer(d.FramesBetweenBurninateUpdate);
    c.Transfer(d.HoverObstructionRadius);
    c.Transfer(d.StrafeTriggerDistance);
    c.Transfer(d.GetHitSnapBuildupTime);
    c.Transfer(d.NotHitSnapBuildupTime);
    c.Transfer(d.MaxTimeBeforeSnap);
    c.Transfer(d.MinRoarDistance);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[20] = { &d.MaxHitsBeforeFlyAway, &d.MinTimeBetweenSlams, &d.MaxFirebeamDistance, &d.MinSnapDistance, &d.MaxSnapDistance, &d.MaxTimeAtLongRangeBeforeFlyAway, &d.MaxSlamDistance, &d.MaxHoverLoops, &d.PercentChanceOfFakeLandingSwoop, &d.MaxTimeOnGround, &d.DragonGroundHeightOffsetBodge, &d.HoverBackOffOffset, &d.FramesBetweenFireBeamUpdate, &d.FramesBetweenBurninateUpdate, &d.HoverObstructionRadius, &d.StrafeTriggerDistance, &d.GetHitSnapBuildupTime, &d.NotHitSnapBuildupTime, &d.MaxTimeBeforeSnap, &d.MinRoarDistance };
    if (save.count != 20u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 20u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.MaxHitsBeforeFlyAway, 0, sizeof(e.MaxHitsBeforeFlyAway));
    std::memset(&e.MinTimeBetweenSlams, 0, sizeof(e.MinTimeBetweenSlams));
    std::memset(&e.MaxFirebeamDistance, 0, sizeof(e.MaxFirebeamDistance));
    std::memset(&e.MinSnapDistance, 0, sizeof(e.MinSnapDistance));
    std::memset(&e.MaxSnapDistance, 0, sizeof(e.MaxSnapDistance));
    std::memset(&e.MaxTimeAtLongRangeBeforeFlyAway, 0, sizeof(e.MaxTimeAtLongRangeBeforeFlyAway));
    std::memset(&e.MaxSlamDistance, 0, sizeof(e.MaxSlamDistance));
    std::memset(&e.MaxHoverLoops, 0, sizeof(e.MaxHoverLoops));
    std::memset(&e.PercentChanceOfFakeLandingSwoop, 0, sizeof(e.PercentChanceOfFakeLandingSwoop));
    std::memset(&e.MaxTimeOnGround, 0, sizeof(e.MaxTimeOnGround));
    std::memset(&e.DragonGroundHeightOffsetBodge, 0, sizeof(e.DragonGroundHeightOffsetBodge));
    std::memset(&e.HoverBackOffOffset, 0, sizeof(e.HoverBackOffOffset));
    std::memset(&e.FramesBetweenFireBeamUpdate, 0, sizeof(e.FramesBetweenFireBeamUpdate));
    std::memset(&e.FramesBetweenBurninateUpdate, 0, sizeof(e.FramesBetweenBurninateUpdate));
    std::memset(&e.HoverObstructionRadius, 0, sizeof(e.HoverObstructionRadius));
    std::memset(&e.StrafeTriggerDistance, 0, sizeof(e.StrafeTriggerDistance));
    std::memset(&e.GetHitSnapBuildupTime, 0, sizeof(e.GetHitSnapBuildupTime));
    std::memset(&e.NotHitSnapBuildupTime, 0, sizeof(e.NotHitSnapBuildupTime));
    std::memset(&e.MaxTimeBeforeSnap, 0, sizeof(e.MaxTimeBeforeSnap));
    std::memset(&e.MinRoarDistance, 0, sizeof(e.MinRoarDistance));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00452ea5_TEST FAIL %d\n", fail); return 1; }
    std::printf("00452ea5_TEST PASS\n");
    return 0;
}