// std::vector::resize(unsigned int new_size) -- STLport vector<CLipSyncDataBankEntry>::resize(n): resize the lip-sync entry list to n entries, filling with an empty entry (no data, zero duration)
// Leaf: ?resize@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A43FF0, 39 bytes, thiscall on the vector, `ret 4`.
// Identity: FableWin.pdb ?resize@?$vector@VCLipSyncDataBankEntry@@...@@QAEXI@Z; no direct call site in the image (xref:
//   0 calls / 0 references); its callee is the two-argument resize that CLipSyncDataBank::NotifyEntryAdded 0x00A42DF0 /
//   NotifyBankSize 0x00A42E70 call on CLipSyncDataBank::Entries (CArray<CLipSyncDataBankEntry>).
// Callees: vector<CLipSyncDataBankEntry>::resize(n, const CLipSyncDataBankEntry&) 0x00A43F20 (out of line).
// Layout: Ego_r.pdb CLipSyncDataBankEntry (8 bytes): Data CIVCountedPointer<CLipSyncDataBankEntry::CData> +0,
//   Duration float +4; FableWin.pdb lists its user default constructor.
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument resize out of line: the
//   temporary's null pointer and 0.0f duration are immediate stores after the pushes. The CIVCountedPointer release
//   (the 0x00A43A00 form) folds away as in retail because the two-argument resize has a same-TU noinline stand-in that
//   never writes through the value reference (the retail TU holds its body); declaration-only, VC7.1 re-tests the
//   pointer after the call (66 v 39 bytes).
// Label correction: catalogued as `vector<EWeaponClass>::_Umove<EWeaponClass*>` (propagated); the body is
//   vector<CLipSyncDataBankEntry>::resize(n).
class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);                                // slot 0
    virtual void Delete();                                          // slot 1
    unsigned long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer {
public:
    T* Object;                                                      // +0x00
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer()
    {
        T* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
};

class CLipSyncDataBankEntry {                                       // 8 bytes
public:
    class CData : public CIVCountedPointeeBase { };
    CIVCountedPointer<CData> Data;                                  // +0x00
    float                    Duration;                              // +0x04
    CLipSyncDataBankEntry() : Duration(0.0f) { }
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    void resize(size_type __new_size, const _Tp& __x);              // 0x00A43F20
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

// 0x00A43F20 resize(n, x) (out of line in retail): a same-TU noinline stand-in that, like the retail body, never
// writes through the value reference (not graded)
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::resize(unsigned int __new_size, const _Tp& __x)
{
    _Tp* __last = this->_M_start + __new_size;
    for (_Tp* __p = this->_M_finish; __p < __last; ++__p)
        __p->Duration = __x.Duration;
    this->_M_finish = __last;
}
#pragma auto_inline(on)

template void std::vector<CLipSyncDataBankEntry>::resize(unsigned int);