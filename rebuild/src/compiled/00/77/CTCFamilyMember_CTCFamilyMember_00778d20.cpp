// CTCFamilyMember::~CTCFamilyMember() -- the compiler-generated destructor: free the FamilyMembers vector block (+0x0C), then ~CTCBase (tail jump)
// Leaf: ??1CTCFamilyMember@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00778D20, 27 bytes (int3 padding 0x00778D3B..F; next function 0x00778D40), thiscall,
//   tail `jmp ~CTCBase`.
// Identity (proven, two sources): (1) the only caller is the scalar deleting destructor 0x00778D00 (call at 0x00778D03), slot 0
//   of vtable 0x0126AADC, RTTI .?AVCTCFamilyMember@@ : CTCBase (rtti-vtables.tsv; the landed ctor 0x00778AE0 stores it);
//   (2) the body is exactly the implicit destruction of Ego_r.pdb's only CTCFamilyMember member, FamilyMembers
//   (vector<const CThing*>, +0x0C), then the base: STLport ~vector (trivial element) -> ~_Vector_base's null-tested free.
//   No vtable store: neither Ego_r.pdb nor FableWin.pdb lists a ??1CTCFamilyMember (FableWin has only the ctor, Alloc,
//   GetInterfaceType, GetClassTypeName), so the destructor is compiler-generated. The catalog label
//   "CTCInventoryItem::OnDie" is propagated and wrong.
// Callees: free 0x00BFEA14 (STLport __malloc_alloc<0>::deallocate), CTCBase::~CTCBase 0x00686830 (??1CTCBase@@UAE@XZ, tail).
// Layout: engine/CTCBase.h retail branch (FABLE_RETAIL_CTCBASE: vptr, ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9,
//   0xC); FamilyMembers +0x0C, the 12-byte STLport vector (Ego_r.pdb Dinkumware 0x10); retail sizeof 0x18.
// Shape: no user destructor. Defining retail's own constructor (landed separately at 0x00778AE0) emits the vtable, whose
//   slot 0 ??_G makes VC7.1 emit the out-of-line implicit ??1 (chunk 658 / 705 pattern; no emission helper needed).
//   STLport 4.6 text: allocator::deallocate -> __sgi_alloc (= __malloc_alloc<0>, _STLP_USE_MALLOC) -> free; the two null
//   tests (_Vector_base and allocator) fold into one.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; ~CTCBase 0x00686830

typedef unsigned int size_t;
extern "C" void __cdecl free(void* block);                // 0x00BFEA14

namespace std {
template <int __inst> class __malloc_alloc { public:     // _STLP_USE_MALLOC: STLport frees straight to the CRT
    static void deallocate(void* __p, size_t) { free(__p); }
};
typedef __malloc_alloc<0> __sgi_alloc;
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, size_t __n) { if (__p != 0) __sgi_alloc::deallocate((void*)__p, __n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, allocator_type> _M_end_of_storage;   // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CTCFamilyMember : public CTCBase {                  // vtable 0x0126AADC (RTTI .?AVCTCFamilyMember@@ : CTCBase)
public:
    static CTCBase* __fastcall Alloc(CThing& thing);      // 0x004D41AF
    CTCFamilyMember(CThing& thing);                       // 0x00778AE0 (landed)
    virtual void Copy(const CTCBase& other);              // slot 12  0x00778B10
    virtual ETCInterfaceType GetInterfaceType() const;    // slot 21  0x00778BD0
    virtual CCharString GetClassTypeName() const;         // slot 22  0x00778BE0
    bool IsRelatedTo(const CThing& thing) const;          // 0x00778AB0
    void AddRelative(const CThing& thing);
private:
    std::vector<const CThing*> FamilyMembers;             // +0x0C
};
FABLE_STATIC_ASSERT(sizeof(CTCFamilyMember) == 0x18);

CTCFamilyMember::CTCFamilyMember(CThing& thing) : CTCBase(thing) { }