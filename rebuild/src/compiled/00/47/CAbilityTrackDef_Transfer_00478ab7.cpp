// CAbilityTrackDef::Transfer(CPersistContext& context) -- persist the CAbilityTrackDef's fields (3 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CAbilityTrackDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478AB7, 46 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CAbilityTrackDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123649C, whose RTTI type descriptor is .?AVCAbilityTrackDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CAbilityTrackDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CAbilityTrackDef@@UAEXAAVCPersistContext@@@Z" agrees; family H057 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(EHeroExperienceStatCategory&) 0x00469B8A, Transfer(std::vector<CAbilityTrackLevelDef >&) 0x00478AE5, Transfer(long&) 0x00431102.
// Layout: Ego_r.pdb CAbilityTrackDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
class CAbilityTrackLevelDef {                                      // Ego_r.pdb, 76 bytes (opaque here)
public:
    unsigned char _body[76];
};
enum EHeroExperienceStatCategory { };                       // enumerators not needed
class CPersistContext {
public:
    void Transfer(EHeroExperienceStatCategory& value);           // 0x00469B8A
    void Transfer(std::vector<CAbilityTrackLevelDef >& value);   // 0x00478AE5
    void Transfer(long& value);                                  // 0x00431102
};
class CAbilityTrackDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123649C slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    EHeroExperienceStatCategory ExperienceStat;        // +0x3C (Ego_r +0x40)
    std::vector<CAbilityTrackLevelDef > Abilities;     // +0x40 (Ego_r +0x44)
    long MaxedOutDescription;                          // +0x4C (Ego_r +0x54)
};

void CAbilityTrackDef::Transfer(CPersistContext& context)
{
    context.Transfer(ExperienceStat);
    context.Transfer(Abilities);
    context.Transfer(MaxedOutDescription);
}