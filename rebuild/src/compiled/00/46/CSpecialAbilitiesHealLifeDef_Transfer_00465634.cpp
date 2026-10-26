// CSpecialAbilitiesHealLifeDef::Transfer(CPersistContext& context) -- persist the CSpecialAbilitiesHealLifeDef's fields (3 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CSpecialAbilitiesHealLifeDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465634, 46 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CSpecialAbilitiesHealLifeDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123446C, whose RTTI type descriptor is .?AVCSpecialAbilitiesHealLifeDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CSpecialAbilitiesHealLifeDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CSpecialAbilitiesHealLifeDef@@UAEXAAVCPersistContext@@@Z" agrees; family H057 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(std::vector<float >&) 0x00464CBF.
// Layout: Ego_r.pdb CSpecialAbilitiesHealLifeDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
    void Transfer(std::vector<float >& value);                   // 0x00464CBF
};
class CSpecialAbilitiesHealLifeDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123446C slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    std::vector<float > HealthIncreasePerFrame;        // +0x3C (Ego_r +0x40)
    std::vector<float > TotalHealthIncrease;           // +0x48 (Ego_r +0x50)
    std::vector<float > RadiusOfEffect;                // +0x54 (Ego_r +0x60)
};

void CSpecialAbilitiesHealLifeDef::Transfer(CPersistContext& context)
{
    context.Transfer(HealthIncreasePerFrame);
    context.Transfer(TotalHealthIncrease);
    context.Transfer(RadiusOfEffect);
}