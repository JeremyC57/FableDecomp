// Behaviour test for 0x00774B40 CTCShop::GetFreeSaleItemPlaces (copy of the source logic over a small vector model).
#include <cstdio>
#include <cstdlib>
#include <cstring>
class CThing { public: int id; };
struct C3DVector { float X, Y, Z; };
struct GoodsPlace { C3DVector Position; long extra[6]; };
struct CGoodsPlaceVector {
    GoodsPlace* _M_start; GoodsPlace* _M_finish; GoodsPlace* _M_end_of_storage;
    CGoodsPlaceVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CGoodsPlaceVector(const CGoodsPlaceVector& o) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    { for (GoodsPlace* p = o._M_start; p != o._M_finish; ++p) push_back(*p); }
    ~CGoodsPlaceVector() { if (_M_start) std::free(_M_start); }
    GoodsPlace* begin() const { return _M_start; }
    GoodsPlace* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    GoodsPlace& operator[](unsigned int i) { return *(_M_start + i); }
    GoodsPlace* erase(GoodsPlace* first, GoodsPlace* last)
    { std::memmove(first, last, (char*)_M_finish - (char*)last); _M_finish -= (last - first); return first; }
    void push_back(const GoodsPlace& x)
    {
        if (_M_finish == _M_end_of_storage) {
            unsigned int n = size(), cap = n ? n * 2 : 4;
            GoodsPlace* s = (GoodsPlace*)std::malloc(cap * sizeof(GoodsPlace));
            if (n) std::memcpy(s, _M_start, n * sizeof(GoodsPlace));
            if (_M_start) std::free(_M_start);
            _M_start = s; _M_finish = s + n; _M_end_of_storage = s + cap;
        }
        *_M_finish++ = x;
    }
};
struct CThingFilter_IsNot { CThing* Thing; CThingFilter_IsNot(CThing* t) : Thing(t) {} };
struct CThingFilter_In2DRange {
    C3DVector Position; float Range;
    CThingFilter_In2DRange(const C3DVector& p, float r) : Position(p), Range(r) {}
};
template <class A, class B> struct CFilter_Bind2 {
    const A* PA; const B* PB;
    CFilter_Bind2(const A& a, const B& b) : PA(&a), PB(&b) {}
};
static CThing g_other, g_self;
static float g_occupiedX = 2.0f;
static CThing* g_seenNot = 0; static float g_seenRange = 0.0f;
struct CThingSearchTools {
    bool GetPFirstThing(const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>& filter, CThing** out)
    {
        g_seenNot = filter.PA->Thing; g_seenRange = filter.PB->Range;
        if (filter.PB->Position.X == g_occupiedX) { *out = &g_other; return true; }
        return false;
    }
};
static CThingSearchTools g_tools;
struct CWorld {
    CThingSearchTools* PThingSearchTools;
    CThingSearchTools* GetThingSearchTools() const { return PThingSearchTools; }
};
static CWorld g_world = { &g_tools };
static int g_inits = 0;
struct CTCShop {
    CThing*           ParentThing;
    CGoodsPlaceVector GoodsPlaces;
    CWorld* GetWorld() const { return &g_world; }
    void InitGoodsPlaces(CThing*)
    {
        ++g_inits;
        for (int i = 1; i <= 3; ++i) { GoodsPlace p; std::memset(&p, 0, sizeof p); p.Position.X = (float)i; GoodsPlaces.push_back(p); }
    }
    CGoodsPlaceVector GetFreeSaleItemPlaces(bool includeOccupied);
};
CGoodsPlaceVector CTCShop::GetFreeSaleItemPlaces(bool includeOccupied)
{
    GoodsPlaces.erase(GoodsPlaces.begin(), GoodsPlaces.end());
    InitGoodsPlaces(ParentThing);
    CGoodsPlaceVector result;
    for (unsigned int i = 0; i < GoodsPlaces.size(); ++i) {
        if (!includeOccupied) {
            CThing* found;
            if (GetWorld()->GetThingSearchTools()->GetPFirstThing(CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>(
                    CThingFilter_IsNot(ParentThing), CThingFilter_In2DRange(GoodsPlaces[i].Position, 0.2f)), &found))
                continue;
        }
        result.push_back(GoodsPlaces[i]);
    }
    return result;
}
static int fail() { std::printf("00774b40_TEST FAIL\n"); return 1; }
int main()
{
    CTCShop shop; shop.ParentThing = &g_self;
    CGoodsPlaceVector free1 = shop.GetFreeSaleItemPlaces(false);
    if (free1.size() != 2 || free1[0].Position.X != 1.0f || free1[1].Position.X != 3.0f) return fail();
    if (g_seenNot != &g_self || g_seenRange != 0.2f || shop.GoodsPlaces.size() != 3) return fail();
    CGoodsPlaceVector all = shop.GetFreeSaleItemPlaces(true);      // rebuilt, not appended
    if (all.size() != 3 || shop.GoodsPlaces.size() != 3 || g_inits != 2) return fail();
    std::printf("00774b40_TEST PASS\n");
    return 0;
}
