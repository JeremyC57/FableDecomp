// CObjectAugmentationsDef::operator=(const CObjectAugmentationsDef&) -- memberwise def copy: CDefClassBase::operator=, the slot counts, the particle map, EmptySlotName, two vector<long>, the two sound maps, six effect ids and the CCharString attachment point; returns *this
// Leaf: ??4CObjectAugmentationsDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EC174, length 165 bytes.
// Identity: the manifest label "CThingBuildingDef::operator=" is propagated. The body sits behind the
//   5-byte `jmp` thunk at 0x004EC16F = slot 19 of vtable 0x01243974 = CObjectAugmentationsDef
//   (rtti-vtables.tsv); the touched offsets are the PDB fields with 12-byte retail STLport containers
//   (NumberOfSlots +0x28 .. WeaponTrailAttachmentPoint +0x88).
// Callees: CDefClassBase::operator= @ 0x00431F10,
//   map<EObjectAugmentationType,CObjectAugmentationParticleSet>::operator= @ 0x004EC33E (landed CRetailTree_004ec3b1),
//   std::vector<long>::operator= @ 0x00454886 (InitialAugmentations, SlotUpgradeCosts),
//   map<EObjectAugmentationType,CDefString>::operator= @ 0x004EC219 (landed CRetailTree_004ec28c; InitSounds, LoopingSounds),
//   CCharString::operator= @ 0x0099EFB0.
// Shape: compiler-generated memberwise assignment in declaration order, scalars through eax alone
//   (size-optimised def module); the last member's address is formed with `add edi,88h` -- the natural
//   `&other.WeaponTrailAttachmentPoint` of the final call.
// No engine header exists for the class: modelled locally from struct_layouts_egor.tsv.
#pragma optimize("s", on)

class CDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x24];                                            // +0x04..+0x28
    CDefClassBase& operator=(const CDefClassBase& other);                 // 0x00431F10
};

struct CLongVector {                                                      // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);                     // 0x00454886
};

struct CRetailTree_004ec3b1 {                                             // retail map<EObjectAugmentationType,CObjectAugmentationParticleSet> (12 bytes)
    struct node;
    node*         head;
    unsigned long size;
    unsigned long _alloc;
    CRetailTree_004ec3b1& operator=(const CRetailTree_004ec3b1& other);   // 0x004EC33E
};

struct CRetailTree_004ec28c {                                             // retail map<EObjectAugmentationType,CDefString> (12 bytes)
    struct node;
    node*         head;
    unsigned long size;
    unsigned long _alloc;
    CRetailTree_004ec28c& operator=(const CRetailTree_004ec28c& other);   // 0x004EC219
};

class CCharString { public:
    void* m_pData;
    CCharString& operator=(const CCharString& other);                     // 0x0099EFB0
};

class CObjectAugmentationsDef : public CDefClassBase { public:                          // vtable 0x01243974
    long                 NumberOfSlots;                                   // +0x28
    long                 MaximumNumberOfSlots;                            // +0x2c
    CRetailTree_004ec3b1 AugmentationParticles;                           // +0x30
    unsigned long        EmptySlotName;                                   // +0x3c
    CLongVector          InitialAugmentations;                            // +0x40
    CLongVector          SlotUpgradeCosts;                                // +0x4c
    CRetailTree_004ec28c InitSounds;                                      // +0x58
    CRetailTree_004ec28c LoopingSounds;                                   // +0x64
    long                 FireProjectileTrailEffect;                       // +0x70
    long                 LightningProjectileTrailEffect;                  // +0x74
    long                 FireAndLightningProjectileTrailEffect;           // +0x78
    long                 FireProjectileOnHitEffect;                       // +0x7c
    long                 LightningProjectileOnHitEffect;                  // +0x80
    long                 FireAndLightningProjectileOnHitEffect;           // +0x84
    CCharString          WeaponTrailAttachmentPoint;                      // +0x88
    virtual void v00();
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