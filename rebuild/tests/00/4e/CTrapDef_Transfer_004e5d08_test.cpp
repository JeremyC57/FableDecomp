// behaviour test for 0x004E5D08 CTrapDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x34, +0x38, +0x3C, +0x48, +0x4C, +0x40, +0x44, +0x50, +0x54, +0x58, +0x5C, +0x60),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob4 { unsigned char b[4]; };
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
    Blob12 Sound;
    int TriggerType;
    int TrapType;
    float TriggerRadius;
    Blob4 TriggerHelperName;
    float TrapDamage;
    long TriggeredCollisionMesh;
    long UntriggeredCollisionMesh;
    long ExplosionDefIndex;
    long PhysicalObstructionDefIndex;
    long NumShotsToFireBeforeIdle;
    float SecondsIdle;
    float SecondsBetweenShots;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.Sound);
    c.Transfer(d.TriggerType);
    c.Transfer(d.TrapType);
    c.Transfer(d.TriggerRadius);
    c.Transfer(d.TriggeredCollisionMesh);
    c.Transfer(d.UntriggeredCollisionMesh);
    c.Transfer(d.TriggerHelperName);
    c.Transfer(d.TrapDamage);
    c.Transfer(d.ExplosionDefIndex);
    c.Transfer(d.PhysicalObstructionDefIndex);
    c.Transfer(d.NumShotsToFireBeforeIdle);
    c.Transfer(d.SecondsIdle);
    c.Transfer(d.SecondsBetweenShots);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[13] = { &d.Sound, &d.TriggerType, &d.TrapType, &d.TriggerRadius, &d.TriggeredCollisionMesh, &d.UntriggeredCollisionMesh, &d.TriggerHelperName, &d.TrapDamage, &d.ExplosionDefIndex, &d.PhysicalObstructionDefIndex, &d.NumShotsToFireBeforeIdle, &d.SecondsIdle, &d.SecondsBetweenShots };
    if (save.count != 13u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 13u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.Sound, 0, sizeof(e.Sound));
    std::memset(&e.TriggerType, 0, sizeof(e.TriggerType));
    std::memset(&e.TrapType, 0, sizeof(e.TrapType));
    std::memset(&e.TriggerRadius, 0, sizeof(e.TriggerRadius));
    std::memset(&e.TriggeredCollisionMesh, 0, sizeof(e.TriggeredCollisionMesh));
    std::memset(&e.UntriggeredCollisionMesh, 0, sizeof(e.UntriggeredCollisionMesh));
    std::memset(&e.TriggerHelperName, 0, sizeof(e.TriggerHelperName));
    std::memset(&e.TrapDamage, 0, sizeof(e.TrapDamage));
    std::memset(&e.ExplosionDefIndex, 0, sizeof(e.ExplosionDefIndex));
    std::memset(&e.PhysicalObstructionDefIndex, 0, sizeof(e.PhysicalObstructionDefIndex));
    std::memset(&e.NumShotsToFireBeforeIdle, 0, sizeof(e.NumShotsToFireBeforeIdle));
    std::memset(&e.SecondsIdle, 0, sizeof(e.SecondsIdle));
    std::memset(&e.SecondsBetweenShots, 0, sizeof(e.SecondsBetweenShots));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004e5d08_TEST FAIL %d\n", fail); return 1; }
    std::printf("004e5d08_TEST PASS\n");
    return 0;
}