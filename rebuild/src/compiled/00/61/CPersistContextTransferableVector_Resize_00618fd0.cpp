// CPersistContext_TransferableVector::Resize(long size) -- resize the transferred vector of 64-bit values (zero fill)
// Leaf: ?Resize@?$CPersistContext_TransferableVector@_K@@EAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00618FD0, 35 bytes, thiscall, `ret 4`.
// Identity: vtable 0x0125718C slot 1 (the only reference, 0x01257190) of CPersistContext_TransferableVector<unsigned
//   __int64> (slot 3 TransferOut formats a 64-bit value; TransVector at +4). The callee 0x00618EF0 is the two-argument
//   vector<unsigned __int64>::resize. Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector<unsigned __int64>::resize(size_type, const value_type&) @ 0x00618EF0.
// Shape: `TransVector->resize(size)` with STLport `resize(n) { resize(n, _Tp()); }` inlined: the zero 64-bit
//   temporary is two eax stores into the 8-byte frame.
namespace std {
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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x00618EF0)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

class CPersistContext;
class CCharString;
class CStringParser;

template <class T>
class CPersistContext_TransferableVector {
public:
    CPersistContext_TransferableVector(std::vector<T>& v);
private:
    virtual long Size();                                                                    // slot 0
    virtual void Resize(long size) { TransVector->resize(size); }                           // slot 1
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);  // slot 2
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);      // slot 3
    std::vector<T>* TransVector;                                                           // +0x04
};

template void CPersistContext_TransferableVector<unsigned __int64>::Resize(long);