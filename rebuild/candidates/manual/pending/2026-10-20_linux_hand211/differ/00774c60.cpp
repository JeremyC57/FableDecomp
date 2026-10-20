// CTCShop::StockDisplay() -- put the sale items that still need a place onto the free sale-item places, one item per place, as far as both lists go
// Grade: DIFFER(175v188) -- register colouring (retail this=ebx, i=ebp, counts esi/edi; the lane this=esi), the lane keeps places.begin() in edi for the cursor and hoists the items data load; retail aligns the loop head behind a jmp
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00774C60, 188 bytes (plain `ret`).
// Callees: CTCShop::GetFreeSaleItemPlaces(bool) @ 0x00774B40 and CTCShop::GetSaleItemsToPlace() @ 0x00773EE0 (both return an
// STLport vector by value through the hidden pointer), CTCShop::AddSaleItem(long, const GoodsPlace&) @ 0x00772BB0, free
// @ 0x00BFEA14 (the two vectors' storage; both element types are PODs).
// Layout: CTCShop::GoodsPlace is 0x24 bytes (the reciprocal-multiply 0x38E38E39 / sar 3); the vectors are the retail 12-byte
// STLport vectors.
// Shape: both counts are address-taken (STLport `min(const T&, const T&)` = `b < a ? b : a`, re-evaluated every pass), the
// place cursor is its own frame variable stepped after the index, and the items vector's data pointer is re-read per pass.
#include <stddef.h>
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
template <class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; }
struct CTCShop {
    struct GoodsPlace { unsigned char _data[0x24]; };
    struct CGoodsPlaceVector {                          // STLport vector<CTCShop::GoodsPlace>
        GoodsPlace* _M_start; GoodsPlace* _M_finish; GoodsPlace* _M_end_of_storage;
        CGoodsPlaceVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
        ~CGoodsPlaceVector() { if (_M_start) free(_M_start); }
        GoodsPlace* begin() { return _M_start; }
        unsigned long size() const { return _M_finish - _M_start; }
    };
    struct CLongVector {                                // STLport vector<long>
        long* _M_start; long* _M_finish; long* _M_end_of_storage;
        CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
        ~CLongVector() { if (_M_start) free(_M_start); }
        unsigned long size() const { return _M_finish - _M_start; }
        long& operator[](unsigned long i) { return _M_start[i]; }
    };
    virtual void v00();
    void AddSaleItem(long goods, const GoodsPlace& place);       // 0x00772BB0
    CLongVector GetSaleItemsToPlace();                           // 0x00773EE0
    CGoodsPlaceVector GetFreeSaleItemPlaces(bool includeTaken);  // 0x00774B40
    void StockDisplay();
};

void CTCShop::StockDisplay()
{
    CGoodsPlaceVector places = GetFreeSaleItemPlaces(false);
    CLongVector items = GetSaleItemsToPlace();
    unsigned long i;
    unsigned long numItems = items.size();
    unsigned long numPlaces = places.size();
    GoodsPlace* place = places.begin();
    for (i = 0; i < min(numPlaces, numItems); ++i, ++place)
        AddSaleItem(items[i], *place);
}
