// COpinionDeedReactionDef::Transfer(CPersistContext& context) -- persist the COpinionDeedReactionDef's fields (3 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@COpinionDeedReactionDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00472095, 46 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): COpinionDeedReactionDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012361DC, whose RTTI type descriptor is .?AVCOpinionDeedReactionDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@COpinionDeedReactionDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@COpinionDeedReactionDef@@UAEXAAVCPersistContext@@@Z" agrees; family H057 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(std::vector<EOpinionAttitudeType >&) 0x004720C3, Transfer(CDefString&) 0x0045228F, Transfer(unsigned long&) 0x00431020.
// Layout: Ego_r.pdb COpinionDeedReactionDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
enum EOpinionAttitudeType { };                              // enumerators not needed
class CPersistContext {
public:
    void Transfer(std::vector<EOpinionAttitudeType >& value);    // 0x004720C3
    template <class T> void Transfer(T& value);                            // 0x0045228F
};
class COpinionDeedReactionDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012361DC slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    std::vector<EOpinionAttitudeType > Attitudes;      // +0x3C (Ego_r +0x40)
    CDefString Animation;                              // +0x48 (Ego_r +0x50)
    unsigned long DialogueTag;                         // +0x4C (Ego_r +0x54)
};

void COpinionDeedReactionDef::Transfer(CPersistContext& context)
{
    context.Transfer(Attitudes);
    context.Transfer(Animation);
    context.Transfer(DialogueTag);
}