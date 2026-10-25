// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the inventory map region records
// Leaf: ?resize@?$vector@VCUIRegionInfo@CTCInventoryMap@@V?$allocator@VCUIRegionInfo@CTCInventoryMap@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00619290, 64 bytes, thiscall, `ret 4`.
// Identity: vector<CTCInventoryMap::CUIRegionInfo> (40 bytes: Region, Shops, BarberShops, TitleShops, TatooShops,
//   ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns -- Ego_r.pdb): the callee 0x006190C0 is the two-argument
//   resize (also called by the TransferableVector Resize 0x00619420). No direct reference (xref 0). Catalog
//   "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x006190C0.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`; CUIRegionInfo() zeroes all ten counters (ten eax stores).
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

template void vector<CTCInventoryMap::CUIRegionInfo>::resize(size_type);
}