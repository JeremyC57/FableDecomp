// Behaviour test for CObjectAugmentationsDef::operator= (0x004EC174): copies the model, spies the base,
// map, vector and CCharString assignments (order and count), checks every scalar lands and *this returns.
#include <cstdio>

static int g_order[12];
static int g_calls = 0;

struct CDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x24];
    CDefClassBase& operator=(const CDefClassBase& other)
    {
        for (int i = 0; i < 0x24; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CLongVector {
    long* m_first; long* m_last; long* m_end;
    CLongVector& operator=(const CLongVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 2; return *this; }
};

struct CRetailTree_004ec3b1 {
    struct node;
    node* head; unsigned long size; unsigned long _alloc;
    CRetailTree_004ec3b1& operator=(const CRetailTree_004ec3b1& o)
    { head = o.head; size = o.size; _alloc = o._alloc; g_order[g_calls++] = 3; return *this; }
};

struct CRetailTree_004ec28c {
    struct node;
    node* head; unsigned long size; unsigned long _alloc;
    CRetailTree_004ec28c& operator=(const CRetailTree_004ec28c& o)
    { head = o.head; size = o.size; _alloc = o._alloc; g_order[g_calls++] = 4; return *this; }
};

struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& o) { m_pData = o.m_pData; g_order[g_calls++] = 5; return *this; }
};

struct CObjectAugmentationsDef : CDefClassBase {
    long                 NumberOfSlots;
    long                 MaximumNumberOfSlots;
    CRetailTree_004ec3b1 AugmentationParticles;
    unsigned long        EmptySlotName;
    CLongVector          InitialAugmentations;
    CLongVector          SlotUpgradeCosts;
    CRetailTree_004ec28c InitSounds;
    CRetailTree_004ec28c LoopingSounds;
    long                 FireProjectileTrailEffect;
    long                 LightningProjectileTrailEffect;
    long                 FireAndLightningProjectileTrailEffect;
    long                 FireProjectileOnHitEffect;
    long                 LightningProjectileOnHitEffect;
    long                 FireAndLightningProjectileOnHitEffect;
    CCharString          WeaponTrailAttachmentPoint;
    CObjectAugmentationsDef& operator=(const CObjectAugmentationsDef& other);
};

CObjectAugmentationsDef& CObjectAugmentationsDef::operator=(const CObjectAugmentationsDef& other)
{
    CDefClassBase::operator=(other);
    NumberOfSlots = other.NumberOfSlots;
    MaximumNumberOfSlots = other.MaximumNumberOfSlots;
    AugmentationParticles = other.AugmentationParticles;
    EmptySlotName = other.EmptySlotName;
    InitialAugmentations = other.InitialAugmentations;
    SlotUpgradeCosts = other.SlotUpgradeCosts;
    InitSounds = other.InitSounds;
    LoopingSounds = other.LoopingSounds;
    FireProjectileTrailEffect = other.FireProjectileTrailEffect;
    LightningProjectileTrailEffect = other.LightningProjectileTrailEffect;
    FireAndLightningProjectileTrailEffect = other.FireAndLightningProjectileTrailEffect;
    FireProjectileOnHitEffect = other.FireProjectileOnHitEffect;
    LightningProjectileOnHitEffect = other.LightningProjectileOnHitEffect;
    FireAndLightningProjectileOnHitEffect = other.FireAndLightningProjectileOnHitEffect;
    WeaponTrailAttachmentPoint = other.WeaponTrailAttachmentPoint;
    return *this;
}

int main()
{
    static long v1[2], v2[3];
    static char text[] = "hand_1";
    CObjectAugmentationsDef a, b;
    int i;
    for (i = 0; i < 0x24; ++i) { a._body[i] = 0; b._body[i] = (unsigned char)(i + 5); }
    long* la[] = { &a.NumberOfSlots, &a.MaximumNumberOfSlots, &a.FireProjectileTrailEffect, &a.LightningProjectileTrailEffect,
        &a.FireAndLightningProjectileTrailEffect, &a.FireProjectileOnHitEffect, &a.LightningProjectileOnHitEffect,
        &a.FireAndLightningProjectileOnHitEffect };
    long* lb[] = { &b.NumberOfSlots, &b.MaximumNumberOfSlots, &b.FireProjectileTrailEffect, &b.LightningProjectileTrailEffect,
        &b.FireAndLightningProjectileTrailEffect, &b.FireProjectileOnHitEffect, &b.LightningProjectileOnHitEffect,
        &b.FireAndLightningProjectileOnHitEffect };
    for (i = 0; i < 8; ++i) { *la[i] = 0; *lb[i] = 100 + i; }
    a.EmptySlotName = 0; b.EmptySlotName = 0xDEADBEEF;
    a.AugmentationParticles.head = 0; a.AugmentationParticles.size = 0; a.AugmentationParticles._alloc = 0;
    b.AugmentationParticles.head = (CRetailTree_004ec3b1::node*)&b; b.AugmentationParticles.size = 2; b.AugmentationParticles._alloc = 1;
    a.InitSounds.head = 0; a.InitSounds.size = 0; a.InitSounds._alloc = 0; a.LoopingSounds = a.InitSounds; g_calls = 0;
    b.InitSounds.head = (CRetailTree_004ec28c::node*)v1; b.InitSounds.size = 3; b.InitSounds._alloc = 0;
    b.LoopingSounds.head = (CRetailTree_004ec28c::node*)v2; b.LoopingSounds.size = 4; b.LoopingSounds._alloc = 0;
    a.InitialAugmentations.m_first = a.InitialAugmentations.m_last = a.InitialAugmentations.m_end = 0;
    a.SlotUpgradeCosts = a.InitialAugmentations; g_calls = 0;
    b.InitialAugmentations.m_first = v1; b.InitialAugmentations.m_last = v1 + 2; b.InitialAugmentations.m_end = v1 + 2;
    b.SlotUpgradeCosts.m_first = v2; b.SlotUpgradeCosts.m_last = v2 + 3; b.SlotUpgradeCosts.m_end = v2 + 3;
    a.WeaponTrailAttachmentPoint.m_pData = 0; b.WeaponTrailAttachmentPoint.m_pData = text;
    CObjectAugmentationsDef& r = (a = b);
    static const int expect[] = { 1, 3, 2, 2, 4, 4, 5 };
    bool ok = (&r == &a) && g_calls == 7;
    for (i = 0; i < 7; ++i) if (g_order[i] != expect[i]) ok = false;
    for (i = 0; i < 0x24; ++i) if (a._body[i] != (unsigned char)(i + 5)) ok = false;
    for (i = 0; i < 8; ++i) if (*la[i] != 100 + i) ok = false;
    if (a.EmptySlotName != 0xDEADBEEF || a.AugmentationParticles.size != 2 || a.AugmentationParticles.head != (CRetailTree_004ec3b1::node*)&b
        || a.InitSounds.size != 3 || a.LoopingSounds.size != 4 || a.LoopingSounds.head != (CRetailTree_004ec28c::node*)v2
        || a.InitialAugmentations.m_last != v1 + 2 || a.SlotUpgradeCosts.m_first != v2
        || a.WeaponTrailAttachmentPoint.m_pData != text)
        ok = false;
    if (!ok) { std::printf("004ec174_TEST FAIL\n"); return 1; }
    std::printf("004ec174_TEST PASS\n");
    return 0;
}