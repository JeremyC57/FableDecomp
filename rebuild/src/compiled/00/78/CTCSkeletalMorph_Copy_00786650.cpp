// CTCSkeletalMorph::Copy(const CTCBase& other) -- copy another skeletal-morph component's Morphs array and mark the morph Changed
// Leaf: ?Copy@CTCSkeletalMorph@@UAEXABVCTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00786650, 23 bytes, thiscall, tail `jmp` to the STLport vector assignment (result dropped).
// Identity (proven): slot 12 of the CTCSkeletalMorph vtable 0x0126B6F4 (the only reference, 0x0126B724); CTCBase slot 12 is
//   Copy(const CTCBase&) (landed CTCFamilyMember::Copy 0x00778B10, CTCSmokeGenerator::Copy 0x00722570); Ego_r.pdb name set has
//   ?Copy@CTCSkeletalMorph@@UAEXABVCTCBase@@@Z (donor 0x0078DD30, after AddSkeletalMorph, before CalculateSkeletalMorph).
//   The catalog label "CTCSkeletalMorph::vfunc_12" is a placeholder.
// Callee: std::vector<CTCSkeletalMorph::CSkeletalMorph>::operator= 0x007871D0 (landed, out of line; CArray's implicit operator=
//   inlines to it).
// Layout: Ego_r.pdb CTCSkeletalMorph (0x28) = CTCBase (0xC) + bool Changed +0xC + CArray<CSkeletalMorph> Morphs +0x10 (retail
//   12-byte STLport vector) + CCountedPointer<C3DSkeletalMorph> PSkeletalMorph +0x1C.
class CThing;

class CCharString {
public:
    void* PStringData;
};

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
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);    // 0x007871D0 for <CTCSkeletalMorph::CSkeletalMorph>
};
}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Lionhead CArray over the STLport vector

class CTCBase {                                             // engine/CTCBase.h is flattened (vptr as a data member)
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCSkeletalMorph : public CTCBase {                   // vtable 0x0126B6F4
public:
    class CSkeletalMorph {                                  // Ego_r.pdb, 12 bytes
    public:
        long        ID;                                     // +0x0
        CCharString Name;                                   // +0x4
        float       Strength;                               // +0x8
    };
    virtual void Copy(const CTCBase& other);
    bool                   Changed;                         // +0x0C
    CArray<CSkeletalMorph> Morphs;                          // +0x10
};

void CTCSkeletalMorph::Copy(const CTCBase& other)
{
    const CTCSkeletalMorph& o = (const CTCSkeletalMorph&)other;
    Changed = true;
    Morphs = o.Morphs;
}