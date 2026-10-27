// CTCShopKeeper::~CTCShopKeeper() -- compiler-generated destructor: destroy the previous-customers vector, the shop and customer pointers, then CTCBase
// Leaf: ??1CTCShopKeeper@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079A4D0, 73 bytes, tail jump to ~CTCBase.
// Identity: called only by CTCShopKeeper's scalar deleting destructor 0x0079A4B0 (vtable 0x0126C154 slot 0, RTTI
//   .?AVCTCShopKeeper@@); Ego_r.pdb `??1CTCShopKeeper@@UAE@XZ`. It stores no vtable (VC7.1's implicit destructor leaves the
//   vptr alone; a user `~T() {}` would reset it), so the class declares no destructor. The catalog's `Catch@0047e8cc` label
//   is propagated: this is a plain thiscall function with its own prologue, not a catch block.
// Layout: Ego_r.pdb CTCShopKeeper with the retail 12-byte vector: Customer +0x0c (CIntelligentPointer<CThingCreatureBase>),
//   Shop +0x14 (CIntelligentPointer<CThing>), PreviousCustomers +0x1c (STLport vector<CIntelligentPointer<CThingCreatureBase> >,
//   8-byte polymorphic elements), CustomerIndex +0x28.
// Callees: the elements' virtual destructor (slot 0, flag 0), free @ 0x00BFEA14 (the STLport allocator's deallocate),
//   CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (twice), CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Shape: members in reverse order: the vector's `_Destroy(_M_start, _M_finish)` loop (one-byte `nop` loop alignment: speed
//   TU) then `if (_M_start) free(_M_start)` with _M_start re-read, then Shop and Customer, then the base. Emitted through a
//   qualified destructor call in a helper (GOTCHAS: an implicit dtor is only emitted out of line that way).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14 (CRT import thunk)

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)        // STLport __destroy_aux(first, last, __false_type)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

class CThing;
class CThingCreatureBase;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

class CTCBase {
public:
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};

class CTCShopKeeper : public CTCBase {
public:
    CIntelligentPointer<CThingCreatureBase> Customer;                    // +0x0c
    CIntelligentPointer<CThing>             Shop;                        // +0x14
    std::vector<CIntelligentPointer<CThingCreatureBase> > PreviousCustomers;   // +0x1c
    long                                    CustomerIndex;               // +0x28
    bool                                    OtherPeopleInShop;           // +0x2c
    long                                    FrameLastCalcedOtherPeopleInShop;   // +0x30
};

void DestroyShopKeeper_0079a4d0(CTCShopKeeper* keeper)
{
    keeper->CTCShopKeeper::~CTCShopKeeper();
}