// CSpecialAbilitiesForcePushDef::Transfer(CPersistContext& context) -- persist the CSpecialAbilitiesForcePushDef's fields (20 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CSpecialAbilitiesForcePushDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465163, 245 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CSpecialAbilitiesForcePushDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012342CC, whose RTTI type descriptor is .?AVCSpecialAbilitiesForcePushDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CSpecialAbilitiesForcePushDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CSpecialAbilitiesForcePushDef@@UAEXAAVCPersistContext@@@Z" agrees; family H053 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(std::vector<float >&) 0x00464CBF.
// Layout: Ego_r.pdb CSpecialAbilitiesForcePushDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
//   engine/CSpecialAbilitiesForcePushDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
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
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(std::vector<float >& value);                   // 0x00464CBF
};
class CSpecialAbilitiesForcePushDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012342CC slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    float MaxDistanceToPush;                           // +0x3C (Ego_r +0x40)
    float MinDistanceToPush;                           // +0x40 (Ego_r +0x44)
    float DustBoxWidth;                                // +0x44 (Ego_r +0x48)
    long MaxNumOfExtraEffects;                         // +0x48 (Ego_r +0x4C)
    long MinNumOfExtraEffects;                         // +0x4C (Ego_r +0x50)
    float LevelUpTimings[3];                           // +0x50 (Ego_r +0x54)
    float ReleaseLevel1Delay;                          // +0x5C (Ego_r +0x60)
    float ReleaseLevel2Delay;                          // +0x60 (Ego_r +0x64)
    float ReleaseLevel3DelayA;                         // +0x64 (Ego_r +0x68)
    float ReleaseLevel3DelayB;                         // +0x68 (Ego_r +0x6C)
    float ReleaseLevel4Delay;                          // +0x6C (Ego_r +0x70)
    float HeightOffGround;                             // +0x70 (Ego_r +0x74)
    float Level4OffsetMagnitude;                       // +0x74 (Ego_r +0x78)
    std::vector<float > RadiusOfEffect;                // +0x78 (Ego_r +0x7C)
    float ExtraPush;                                   // +0x84 (Ego_r +0x8C)
    float PushSpeed;                                   // +0x88 (Ego_r +0x90)
    std::vector<float > ObjectDamage;                  // +0x8C (Ego_r +0x94)
    std::vector<float > TimeDelayBetweenCastsSec;      // +0x98 (Ego_r +0xA4)
};

void CSpecialAbilitiesForcePushDef::Transfer(CPersistContext& context)
{
    context.Transfer(MaxDistanceToPush);
    context.Transfer(MinDistanceToPush);
    context.Transfer(DustBoxWidth);
    context.Transfer(MaxNumOfExtraEffects);
    context.Transfer(MinNumOfExtraEffects);
    context.Transfer(LevelUpTimings[0]);
    context.Transfer(LevelUpTimings[1]);
    context.Transfer(LevelUpTimings[2]);
    context.Transfer(ReleaseLevel1Delay);
    context.Transfer(ReleaseLevel2Delay);
    context.Transfer(ReleaseLevel3DelayA);
    context.Transfer(ReleaseLevel3DelayB);
    context.Transfer(ReleaseLevel4Delay);
    context.Transfer(HeightOffGround);
    context.Transfer(Level4OffsetMagnitude);
    context.Transfer(RadiusOfEffect);
    context.Transfer(ExtraPush);
    context.Transfer(PushSpeed);
    context.Transfer(ObjectDamage);
    context.Transfer(TimeDelayBetweenCastsSec);
}