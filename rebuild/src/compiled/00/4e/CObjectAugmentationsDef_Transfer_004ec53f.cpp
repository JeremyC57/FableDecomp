// CObjectAugmentationsDef::Transfer(CPersistContext& context) -- persist the CObjectAugmentationsDef's fields (15 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CObjectAugmentationsDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EC53F, 187 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CObjectAugmentationsDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01243974, whose RTTI type descriptor is .?AVCObjectAugmentationsDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CObjectAugmentationsDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CObjectAugmentationsDef@@UAEXAAVCPersistContext@@@Z" agrees; family H054 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102, Transfer(std::map<EObjectAugmentationType, CObjectAugmentationParticleSet >&) 0x004EC825, Transfer(unsigned long&) 0x00431020, Transfer(std::vector<long >&) 0x00466A47, Transfer(std::map<EObjectAugmentationType, CDefString >&) 0x004EC5FA, Transfer(CCharString&) 0x00431143.
// Layout: Ego_r.pdb CObjectAugmentationsDef members rebased to retail (def base 0 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {      // STLport, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                              // STLport, 12 bytes
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}
class CCharString {                                        // 4 bytes: the shared string data pointer
public:
    void* PStringData;
};
class CDefString {                                         // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
class CObjectAugmentationParticleSet {                             // Ego_r.pdb, 52 bytes (opaque here)
public:
    unsigned char _body[52];
};
enum EObjectAugmentationType { };                           // enumerators not needed
class CPersistContext {
public:
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(std::map<EObjectAugmentationType, CObjectAugmentationParticleSet >& value); // 0x004EC825
    void Transfer(unsigned long& value);                         // 0x00431020
    void Transfer(std::vector<long >& value);                    // 0x00466A47
    void Transfer(std::map<EObjectAugmentationType, CDefString >& value); // 0x004EC5FA
    void Transfer(CCharString& value);                           // 0x00431143
};
class CObjectAugmentationsDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01243974 slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    long NumberOfSlots;                                // +0x28
    long MaximumNumberOfSlots;                         // +0x2C
    std::map<EObjectAugmentationType, CObjectAugmentationParticleSet > AugmentationParticles; // +0x30
    unsigned long EmptySlotName;                       // +0x3C (Ego_r +0x40)
    std::vector<long > InitialAugmentations;           // +0x40 (Ego_r +0x44)
    std::vector<long > SlotUpgradeCosts;               // +0x4C (Ego_r +0x54)
    std::map<EObjectAugmentationType, CDefString > InitSounds; // +0x58 (Ego_r +0x64)
    std::map<EObjectAugmentationType, CDefString > LoopingSounds; // +0x64 (Ego_r +0x74)
    long FireProjectileTrailEffect;                    // +0x70 (Ego_r +0x84)
    long LightningProjectileTrailEffect;               // +0x74 (Ego_r +0x88)
    long FireAndLightningProjectileTrailEffect;        // +0x78 (Ego_r +0x8C)
    long FireProjectileOnHitEffect;                    // +0x7C (Ego_r +0x90)
    long LightningProjectileOnHitEffect;               // +0x80 (Ego_r +0x94)
    long FireAndLightningProjectileOnHitEffect;        // +0x84 (Ego_r +0x98)
    CCharString WeaponTrailAttachmentPoint;            // +0x88 (Ego_r +0x9C)
};

void CObjectAugmentationsDef::Transfer(CPersistContext& context)
{
    context.Transfer(NumberOfSlots);
    context.Transfer(MaximumNumberOfSlots);
    context.Transfer(AugmentationParticles);
    context.Transfer(EmptySlotName);
    context.Transfer(InitialAugmentations);
    context.Transfer(SlotUpgradeCosts);
    context.Transfer(InitSounds);
    context.Transfer(LoopingSounds);
    context.Transfer(FireProjectileTrailEffect);
    context.Transfer(LightningProjectileTrailEffect);
    context.Transfer(FireAndLightningProjectileTrailEffect);
    context.Transfer(FireProjectileOnHitEffect);
    context.Transfer(LightningProjectileOnHitEffect);
    context.Transfer(FireAndLightningProjectileOnHitEffect);
    context.Transfer(WeaponTrailAttachmentPoint);
}