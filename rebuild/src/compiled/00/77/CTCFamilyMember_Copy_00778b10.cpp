// CTCFamilyMember::Copy(const CTCBase& other) -- copy another family-member component's FamilyMembers vector (STLport vector<const CThing*>::operator=)
// Leaf: ?Copy@CTCFamilyMember@@UAEXABVCTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00778B10, 19 bytes, thiscall, tail `jmp` to the vector assignment (its result is dropped).
// Identity (proven): slot 12 of the CTCFamilyMember vtable 0x0126AADC (rtti-vtables.tsv); slot 12 of the CTCBase vtables is
//   Copy(const CTCBase&) (landed CTCQuestCompletionUI::Copy 0x005E49E0, CTCHeroMorph::Copy 0x0071CD90, CTCDoor::Copy 0x007305A0);
//   Ego_r.pdb ?Copy@CTCFamilyMember@@UAEXABVCTCBase@@@Z (donor 0x007844F0, between the ctor 0x007844C0 and AddRelative 0x00784510).
//   The catalog label "CTCFamilyMember::vfunc_12" is a placeholder.
// Callee: std::vector<const CThing*>::operator= 0x00778C00 (STLport 4.6 _vector.c, out of line: `if (&__x != this)` head).
// Layout: Ego_r.pdb CTCFamilyMember = CTCBase (0xC) + FamilyMembers (vector<const CThing*>) at +0xC (retail 12-byte STLport vector).
// Shape: the other component is read as a CTCFamilyMember; no CTCBase::Copy call in retail.
class CThing;

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);    // 0x00778C00 for <const CThing*>
};
}

class CTCBase {                                             // engine/CTCBase.h is flattened (vptr as a data member)
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCFamilyMember : public CTCBase {                    // vtable 0x0126AADC
public:
    virtual void Copy(const CTCBase& other);
    std::vector<const CThing*> FamilyMembers;               // +0x0C
};

void CTCFamilyMember::Copy(const CTCBase& other)
{
    const CTCFamilyMember& o = (const CTCFamilyMember&)other;
    FamilyMembers = o.FamilyMembers;
}