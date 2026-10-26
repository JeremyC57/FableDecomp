// CTurncoatDef::Transfer(CPersistContext& context) -- persist the CTurncoatDef's fields (10 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CTurncoatDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7F24, 123 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CTurncoatDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0124193C, whose RTTI type descriptor is .?AVCTurncoatDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CTurncoatDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CTurncoatDef@@UAEXAAVCPersistContext@@@Z" agrees; family H058 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(bool&) 0x0043314A, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061, Transfer(std::vector<long >&) 0x00466A47.
// Layout: Ego_r.pdb CTurncoatDef members rebased to retail (def base 0 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
    template <class T> void Transfer(T& value);                                  // 0x0043314A
    void Transfer(std::vector<long >& value);                    // 0x00466A47
};
class CTurncoatDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0124193C slot 18
    unsigned char _base[0x25 - 4];                        // def base body
    bool Turncoatable;                                 // +0x25
    long TurncoatedEffect;                             // +0x28
    long TurncoatActivateEffect;                       // +0x2C
    long TurncoatDeactivateEffect;                     // +0x30
    float MinimumResistance;                           // +0x34
    float TimeDelayBeforeResistanceRecoverySecs;       // +0x38
    float ResistanceRecoveryPerSec;                    // +0x3C
    std::vector<long > TurncoatStageEffects;           // +0x40
    float PercentageOfHealthAsResistance;              // +0x4C (Ego_r +0x50)
    float ExtraResistance;                             // +0x50 (Ego_r +0x54)
};

void CTurncoatDef::Transfer(CPersistContext& context)
{
    context.Transfer(Turncoatable);
    context.Transfer(TurncoatedEffect);
    context.Transfer(TurncoatActivateEffect);
    context.Transfer(TurncoatDeactivateEffect);
    context.Transfer(MinimumResistance);
    context.Transfer(TimeDelayBeforeResistanceRecoverySecs);
    context.Transfer(ResistanceRecoveryPerSec);
    context.Transfer(TurncoatStageEffects);
    context.Transfer(PercentageOfHealthAsResistance);
    context.Transfer(ExtraResistance);
}