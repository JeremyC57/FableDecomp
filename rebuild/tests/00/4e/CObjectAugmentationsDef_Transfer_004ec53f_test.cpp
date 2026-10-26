// behaviour test for 0x004EC53F CObjectAugmentationsDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x3C, +0x40, +0x4C, +0x58, +0x64, +0x70, +0x74, +0x78, +0x7C, +0x80, +0x84, +0x88),
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
    long NumberOfSlots;
    long MaximumNumberOfSlots;
    Blob12 AugmentationParticles;
    unsigned long EmptySlotName;
    Blob12 InitialAugmentations;
    Blob12 SlotUpgradeCosts;
    Blob12 InitSounds;
    Blob12 LoopingSounds;
    long FireProjectileTrailEffect;
    long LightningProjectileTrailEffect;
    long FireAndLightningProjectileTrailEffect;
    long FireProjectileOnHitEffect;
    long LightningProjectileOnHitEffect;
    long FireAndLightningProjectileOnHitEffect;
    Blob4 WeaponTrailAttachmentPoint;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.NumberOfSlots);
    c.Transfer(d.MaximumNumberOfSlots);
    c.Transfer(d.AugmentationParticles);
    c.Transfer(d.EmptySlotName);
    c.Transfer(d.InitialAugmentations);
    c.Transfer(d.SlotUpgradeCosts);
    c.Transfer(d.InitSounds);
    c.Transfer(d.LoopingSounds);
    c.Transfer(d.FireProjectileTrailEffect);
    c.Transfer(d.LightningProjectileTrailEffect);
    c.Transfer(d.FireAndLightningProjectileTrailEffect);
    c.Transfer(d.FireProjectileOnHitEffect);
    c.Transfer(d.LightningProjectileOnHitEffect);
    c.Transfer(d.FireAndLightningProjectileOnHitEffect);
    c.Transfer(d.WeaponTrailAttachmentPoint);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[15] = { &d.NumberOfSlots, &d.MaximumNumberOfSlots, &d.AugmentationParticles, &d.EmptySlotName, &d.InitialAugmentations, &d.SlotUpgradeCosts, &d.InitSounds, &d.LoopingSounds, &d.FireProjectileTrailEffect, &d.LightningProjectileTrailEffect, &d.FireAndLightningProjectileTrailEffect, &d.FireProjectileOnHitEffect, &d.LightningProjectileOnHitEffect, &d.FireAndLightningProjectileOnHitEffect, &d.WeaponTrailAttachmentPoint };
    if (save.count != 15u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 15u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.NumberOfSlots, 0, sizeof(e.NumberOfSlots));
    std::memset(&e.MaximumNumberOfSlots, 0, sizeof(e.MaximumNumberOfSlots));
    std::memset(&e.AugmentationParticles, 0, sizeof(e.AugmentationParticles));
    std::memset(&e.EmptySlotName, 0, sizeof(e.EmptySlotName));
    std::memset(&e.InitialAugmentations, 0, sizeof(e.InitialAugmentations));
    std::memset(&e.SlotUpgradeCosts, 0, sizeof(e.SlotUpgradeCosts));
    std::memset(&e.InitSounds, 0, sizeof(e.InitSounds));
    std::memset(&e.LoopingSounds, 0, sizeof(e.LoopingSounds));
    std::memset(&e.FireProjectileTrailEffect, 0, sizeof(e.FireProjectileTrailEffect));
    std::memset(&e.LightningProjectileTrailEffect, 0, sizeof(e.LightningProjectileTrailEffect));
    std::memset(&e.FireAndLightningProjectileTrailEffect, 0, sizeof(e.FireAndLightningProjectileTrailEffect));
    std::memset(&e.FireProjectileOnHitEffect, 0, sizeof(e.FireProjectileOnHitEffect));
    std::memset(&e.LightningProjectileOnHitEffect, 0, sizeof(e.LightningProjectileOnHitEffect));
    std::memset(&e.FireAndLightningProjectileOnHitEffect, 0, sizeof(e.FireAndLightningProjectileOnHitEffect));
    std::memset(&e.WeaponTrailAttachmentPoint, 0, sizeof(e.WeaponTrailAttachmentPoint));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004ec53f_TEST FAIL %d\n", fail); return 1; }
    std::printf("004ec53f_TEST PASS\n");
    return 0;
}