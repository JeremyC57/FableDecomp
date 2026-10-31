// CTCShop::StockDisplay() -- put the sale items that still need a place onto the free sale-item places, one item per place, as far as both lists go
// Grade: DIFFER(188v188) -- register colouring only (26 lines in the sweep udiff count, unchanged): retail this=ebx, i=ebp, counts esi/edi; ours this=esi, i=edi, counts ebx/ebp. This pass: rewritten over real STLport 4.6 std::vector (_Vector_base + _STLP_alloc_proxy, deallocate -> free) and the PDB spellings (GetSaleItemsToPlace const, GoodsPlace struct) -- same residue; for-init index and while-loop forms: no change. Lane gap: register allocation priority
// Leaf: ?StockDisplay@CTCShop@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00774C60, 188 bytes (plain `ret`). Ego_r.pdb ?StockDisplay@CTCShop@@QAEXXZ.
// Callees (Ego_r.pdb spellings): CTCShop::GetFreeSaleItemPlaces(bool) 0x00774B40 and CTCShop::GetSaleItemsToPlace() const
//   0x00773EE0 (STLport vectors by value, hidden result pointer), CTCShop::AddSaleItem(long, const GoodsPlace&) 0x00772BB0,
//   free 0x00BFEA14 (STLport allocator<T>::deallocate of both vectors; PODs, no element dtors).
// Layout: CTCShop::GoodsPlace 0x24 bytes (reciprocal multiply 0x38E38E39 / sar 3).
// Shape: both counts address-taken by STLport min (`b < a ? b : a`, re-evaluated per pass), place[i] strength-reduced to a
//   frame cursor stepped by 0x24, the items data pointer re-read per pass, items destroyed before places.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef _Alloc allocator_type;
    vector(const vector<_Tp, _Alloc>& __x);
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
};
template <class _Tp> inline const _Tp& (min)(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }
}

class CTCShop {
public:
    struct GoodsPlace { unsigned char _data[0x24]; };   // Ego_r.pdb CTCShop::GoodsPlace (0x24, `U`)
    virtual void v00();
    void AddSaleItem(long goods, const GoodsPlace& place);                      // 0x00772BB0
    std::vector<long> GetSaleItemsToPlace() const;                              // 0x00773EE0
    std::vector<GoodsPlace> GetFreeSaleItemPlaces(bool includeTaken);           // 0x00774B40
    void StockDisplay();
};

void CTCShop::StockDisplay()
{
    std::vector<GoodsPlace> places = GetFreeSaleItemPlaces(false);
    std::vector<long> items = GetSaleItemsToPlace();
    unsigned int i;
    unsigned int numItems = items.size();
    unsigned int numPlaces = places.size();
    GoodsPlace* place = places.begin();
    for (i = 0; i < std::min(numPlaces, numItems); ++i)
        AddSaleItem(items[i], place[i]);
}
