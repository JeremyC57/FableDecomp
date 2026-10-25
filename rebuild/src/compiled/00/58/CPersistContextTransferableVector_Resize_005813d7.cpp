// CPersistContext_TransferableVector::Resize(long size) -- resize the transferred hero marriage vector (default-constructed fill)
// Leaf: ?Resize@?$CPersistContext_TransferableVector@VCHeroMarriageInfo@@@@EAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005813D7, 45 bytes, thiscall, `ret 4`; size-optimised TU.
// Identity: vtable 0x0124F350 slot 1 (the only reference, 0x0124F354) of CPersistContext_TransferableVector<
//   CHeroMarriageInfo>, the adaptor CPersistContext uses to transfer a vector (slots: Size, Resize, TransferIn,
//   TransferOut; TransVector at +4). `mov ecx,[ecx+4]` loads TransVector, then the inlined vector::resize(n) calls the
//   two-argument resize 0x005811F1 (also called by 0x00581284). Catalog "vector<EWeaponClass>::_Umove" is propagated.
// Callees: vector<CHeroMarriageInfo>::resize(size_type, const value_type&) @ 0x005811F1.
// Pragma: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
// Shape: `TransVector->resize(size)`; under /Os VC7.1 would call the one-argument resize out of line, so the STLport
//   forwarder is `__forceinline` (retail inlines it: the CHeroMarriageInfo() stores are in this body).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CHeroMarriageInfo {
public:
    unsigned __int64 WifeUID;
    unsigned __int64 MaritalHomeUID;
    long State;
    bool GayMarriage;
    CHeroMarriageInfo() : WifeUID(0), MaritalHomeUID(0), State(0), GayMarriage(false) { }
};

namespace _STL {
template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x005811F1)
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

class CPersistContext;
class CCharString;
class CStringParser;

template <class T>
class CPersistContext_TransferableVector {
public:
    CPersistContext_TransferableVector(_STL::vector<T>& v);
private:
    virtual long Size();                                                                    // slot 0
    virtual void Resize(long size) { TransVector->resize(size); }                           // slot 1
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);  // slot 2
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);      // slot 3
    _STL::vector<T>* TransVector;                                                           // +0x04
};

template void CPersistContext_TransferableVector<CHeroMarriageInfo>::Resize(long);