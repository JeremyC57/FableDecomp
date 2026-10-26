// behaviour test for 0x00453014 CSnowTrollDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C),
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
    long IcicleAttackIndex;
    float SecondsIcicleAttackTargetPositionPredictionTime;
    float DistancePoundGroundRange;
    float SecondsForIcicleAssault;
    float SecondsLeadIntoIcicles;
    float SecondsBetweenIcicles;
    float SecondsBetweenWarningFXAndTrigger;
    float SecondsForTrapToRemain;
    float DistanceRange;
    float DistanceAroundTarget;
    float DistanceExclusionAroundIcicles;
    long IcicleAssaultWarningFXIndex;
    long IcicleAssaultTrapIndex;
    float IceTrailDistanceLimit;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.IcicleAttackIndex);
    c.Transfer(d.SecondsIcicleAttackTargetPositionPredictionTime);
    c.Transfer(d.DistancePoundGroundRange);
    c.Transfer(d.SecondsForIcicleAssault);
    c.Transfer(d.SecondsLeadIntoIcicles);
    c.Transfer(d.SecondsBetweenIcicles);
    c.Transfer(d.SecondsBetweenWarningFXAndTrigger);
    c.Transfer(d.SecondsForTrapToRemain);
    c.Transfer(d.DistanceRange);
    c.Transfer(d.DistanceAroundTarget);
    c.Transfer(d.DistanceExclusionAroundIcicles);
    c.Transfer(d.IcicleAssaultWarningFXIndex);
    c.Transfer(d.IcicleAssaultTrapIndex);
    c.Transfer(d.IceTrailDistanceLimit);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[14] = { &d.IcicleAttackIndex, &d.SecondsIcicleAttackTargetPositionPredictionTime, &d.DistancePoundGroundRange, &d.SecondsForIcicleAssault, &d.SecondsLeadIntoIcicles, &d.SecondsBetweenIcicles, &d.SecondsBetweenWarningFXAndTrigger, &d.SecondsForTrapToRemain, &d.DistanceRange, &d.DistanceAroundTarget, &d.DistanceExclusionAroundIcicles, &d.IcicleAssaultWarningFXIndex, &d.IcicleAssaultTrapIndex, &d.IceTrailDistanceLimit };
    if (save.count != 14u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 14u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.IcicleAttackIndex, 0, sizeof(e.IcicleAttackIndex));
    std::memset(&e.SecondsIcicleAttackTargetPositionPredictionTime, 0, sizeof(e.SecondsIcicleAttackTargetPositionPredictionTime));
    std::memset(&e.DistancePoundGroundRange, 0, sizeof(e.DistancePoundGroundRange));
    std::memset(&e.SecondsForIcicleAssault, 0, sizeof(e.SecondsForIcicleAssault));
    std::memset(&e.SecondsLeadIntoIcicles, 0, sizeof(e.SecondsLeadIntoIcicles));
    std::memset(&e.SecondsBetweenIcicles, 0, sizeof(e.SecondsBetweenIcicles));
    std::memset(&e.SecondsBetweenWarningFXAndTrigger, 0, sizeof(e.SecondsBetweenWarningFXAndTrigger));
    std::memset(&e.SecondsForTrapToRemain, 0, sizeof(e.SecondsForTrapToRemain));
    std::memset(&e.DistanceRange, 0, sizeof(e.DistanceRange));
    std::memset(&e.DistanceAroundTarget, 0, sizeof(e.DistanceAroundTarget));
    std::memset(&e.DistanceExclusionAroundIcicles, 0, sizeof(e.DistanceExclusionAroundIcicles));
    std::memset(&e.IcicleAssaultWarningFXIndex, 0, sizeof(e.IcicleAssaultWarningFXIndex));
    std::memset(&e.IcicleAssaultTrapIndex, 0, sizeof(e.IcicleAssaultTrapIndex));
    std::memset(&e.IceTrailDistanceLimit, 0, sizeof(e.IceTrailDistanceLimit));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00453014_TEST FAIL %d\n", fail); return 1; }
    std::printf("00453014_TEST PASS\n");
    return 0;
}