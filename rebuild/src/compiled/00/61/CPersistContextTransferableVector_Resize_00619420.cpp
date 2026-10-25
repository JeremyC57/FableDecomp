// CPersistContext_TransferableVector::Resize(long size) -- resize the transferred inventory map region vector (zeroed fill)
// Leaf: ?Resize@?$CPersistContext_TransferableVector@VCUIRegionInfo@CTCInventoryMap@@@@EAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00619420, 67 bytes, thiscall, `ret 4`.
// Identity: vtable 0x01257178 slot 1 (the only reference, 0x0125717C) of CPersistContext_TransferableVector<
//   CTCInventoryMap::CUIRegionInfo>; TransVector at +4 (`mov ecx,[ecx+4]`). The callee 0x006190C0 is the two-argument
//   resize shared with 0x00619290. Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector<CTCInventoryMap::CUIRegionInfo>::resize(size_type, const value_type&) @ 0x006190C0.
// Shape: `TransVector->resize(size)` with the STLport one-argument resize inlined (ten zero stores of CUIRegionInfo()).
class CTCInventoryMap {
public:
    class CUIRegionInfo {
    public:
        long Region;
        long Shops;
        long BarberShops;
        long TitleShops;
        long TatooShops;
        long ShopsForSale;
        long HousesForSale;
        long ShopsOwned;
        long HousesOwned;
        long Taverns;
        CUIRegionInfo() : Region(0), Shops(0), BarberShops(0), TitleShops(0), TatooShops(0), ShopsForSale(0),
                          HousesForSale(0), ShopsOwned(0), HousesOwned(0), Taverns(0) { }
    };
};
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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x006190C0)
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

template void CPersistContext_TransferableVector<CTCInventoryMap::CUIRegionInfo>::Resize(long);