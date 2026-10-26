// CSpecialAbilitiesUnholyPowerDef::Transfer(CPersistContext& context) -- persist the CSpecialAbilitiesUnholyPowerDef's fields (20 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CSpecialAbilitiesUnholyPowerDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465B41, 275 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CSpecialAbilitiesUnholyPowerDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012348E4, whose RTTI type descriptor is .?AVCSpecialAbilitiesUnholyPowerDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CSpecialAbilitiesUnholyPowerDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CSpecialAbilitiesUnholyPowerDef@@UAEXAAVCPersistContext@@@Z" agrees; family H053 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(std::vector<CDefString >&) 0x004657F2, Transfer(float&) 0x00431061, Transfer(std::vector<float >&) 0x00464CBF, Transfer(bool&) 0x0043314A.
// Layout: Ego_r.pdb CSpecialAbilitiesUnholyPowerDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
class CDefString {                                         // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
class CPersistContext {
public:
    void Transfer(std::vector<CDefString >& value);              // 0x004657F2
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(std::vector<float >& value);                   // 0x00464CBF
    void Transfer(bool& value);                                  // 0x0043314A
};
class CSpecialAbilitiesUnholyPowerDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012348E4 slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    std::vector<CDefString > StartPowerupSound;        // +0x3C (Ego_r +0x40)
    std::vector<CDefString > LoopingPowerupSound;      // +0x48 (Ego_r +0x50)
    std::vector<CDefString > ReleaseSound;             // +0x54 (Ego_r +0x60)
    std::vector<CDefString > ReleaseLoopSound;         // +0x60 (Ego_r +0x70)
    std::vector<CDefString > FinishSound;              // +0x6C (Ego_r +0x80)
    std::vector<CDefString > CancelSound;              // +0x78 (Ego_r +0x90)
    float RandomFactorForNumSecsBetweenDamageHit;      // +0x84 (Ego_r +0xA0)
    std::vector<float > NumSecsBetweenDamageHit;       // +0x88 (Ego_r +0xA4)
    std::vector<float > DamagePerHit;                  // +0x94 (Ego_r +0xB4)
    std::vector<float > DurationOfSpellSecs;           // +0xA0 (Ego_r +0xC4)
    std::vector<float > RadiusOfEffect;                // +0xAC (Ego_r +0xD4)
    std::vector<float > EnvChangeSecs;                 // +0xB8 (Ego_r +0xE4)
    std::vector<float > EnvReturnSecs;                 // +0xC4 (Ego_r +0xF4)
    bool DoLifting;                                    // +0xD0 (Ego_r +0x104)
    float VelocityOfLift;                              // +0xD4 (Ego_r +0x108)
    float DistanceToLift;                              // +0xD8 (Ego_r +0x10C)
    std::vector<float > StaminaCostPerSec;             // +0xDC (Ego_r +0x110)
    std::vector<float > DamageTakenMultiplier;         // +0xE8 (Ego_r +0x120)
    float SmallRingRadius;                             // +0xF4 (Ego_r +0x130)
    float LargeRingRadius;                             // +0xF8 (Ego_r +0x134)
};

void CSpecialAbilitiesUnholyPowerDef::Transfer(CPersistContext& context)
{
    context.Transfer(StartPowerupSound);
    context.Transfer(LoopingPowerupSound);
    context.Transfer(ReleaseSound);
    context.Transfer(ReleaseLoopSound);
    context.Transfer(FinishSound);
    context.Transfer(CancelSound);
    context.Transfer(RandomFactorForNumSecsBetweenDamageHit);
    context.Transfer(NumSecsBetweenDamageHit);
    context.Transfer(DamagePerHit);
    context.Transfer(DurationOfSpellSecs);
    context.Transfer(RadiusOfEffect);
    context.Transfer(EnvChangeSecs);
    context.Transfer(EnvReturnSecs);
    context.Transfer(DoLifting);
    context.Transfer(VelocityOfLift);
    context.Transfer(DistanceToLift);
    context.Transfer(StaminaCostPerSec);
    context.Transfer(DamageTakenMultiplier);
    context.Transfer(SmallRingRadius);
    context.Transfer(LargeRingRadius);
}