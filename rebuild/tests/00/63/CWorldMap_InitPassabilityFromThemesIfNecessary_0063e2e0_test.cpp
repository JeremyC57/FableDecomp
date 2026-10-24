#include <cstdio>
#include <cstdlib>
// Behaviour test: CWorldMap::InitPassabilityFromThemesIfNecessary -- clip the area to the world, collect the maps under
// it, and let each map whose clipped local box is non-empty initialise its passability over that box.
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
    void ClipTo(const C2DBoxI& o)
    {
        if (TLX < o.TLX) TLX = o.TLX; else if (TLX > o.BRX) TLX = o.BRX;
        if (TLY < o.TLY) TLY = o.TLY; else if (TLY > o.BRY) TLY = o.BRY;
        if (BRX < o.TLX) BRX = o.TLX; else if (BRX > o.BRX) BRX = o.BRX;
        if (BRY < o.TLY) BRY = o.TLY; else if (BRY > o.BRY) BRY = o.BRY;
    }
};
struct ULongVector {
    unsigned long* _M_start; unsigned long* _M_finish; unsigned long* _M_end_of_storage;
    ULongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~ULongVector() { if (_M_start) std::free(_M_start); }
    unsigned long* begin() { return _M_start; }
    unsigned long* end() { return _M_finish; }
};
struct CMap {
    int Calls; C2DBoxI Last;
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area) { ++Calls; Last = area; }
};
struct CWorldMap {
    C2DBoxI Dims;
    C2DBoxI MapArea[3];
    CMap Maps[3];
    const C2DBoxI& GetDimensions() const { return Dims; }
    CMap& GetMap(long map) { return Maps[map]; }
    void GetAllMapsInArea(const C2DBoxI& area, ULongVector& maps) const
    {
        maps._M_start = (unsigned long*)std::malloc(3 * sizeof(unsigned long));
        maps._M_finish = maps._M_start; maps._M_end_of_storage = maps._M_start + 3;
        for (unsigned long m = 0; m < 3; ++m)
            if (MapArea[m].TLX < area.BRX && area.TLX < MapArea[m].BRX) *maps._M_finish++ = m;
    }
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(long map, const C2DBoxI& area) const
    {
        C2DBoxI b = area;
        b.ClipTo(MapArea[map]);
        b.TLX -= MapArea[map].TLX; b.BRX -= MapArea[map].TLX; b.TLY -= MapArea[map].TLY; b.BRY -= MapArea[map].TLY;
        return b;
    }
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area)
    {
        C2DBoxI box = area;
        box.ClipTo(GetDimensions());
        ULongVector maps;
        GetAllMapsInArea(box, maps);
        for (unsigned long* it = maps.begin(); it < maps.end(); ++it) {
            C2DBoxI local = ClipAndConvertWorldMapAreaToLocalArea(*it, box);
            if (local.GetWidth() != 0 && local.GetHeight() != 0) {
                CMap& map = GetMap(*it);
                map.InitPassabilityFromThemesIfNecessary(local);
            }
        }
    }
};
int main()
{
    CWorldMap w;
    C2DBoxI dims = { 0, 0, 300, 100 }; w.Dims = dims;
    C2DBoxI a0 = { 0, 0, 100, 100 }, a1 = { 100, 0, 200, 100 }, a2 = { 200, 0, 300, 100 };
    w.MapArea[0] = a0; w.MapArea[1] = a1; w.MapArea[2] = a2;
    for (int i = 0; i < 3; ++i) w.Maps[i].Calls = 0;
    C2DBoxI area = { 50, -20, 150, 40 };
    w.InitPassabilityFromThemesIfNecessary(area);
    if (w.Maps[0].Calls != 1 || w.Maps[1].Calls != 1 || w.Maps[2].Calls != 0) { std::printf("0063e2e0_TEST FAIL\n"); return 1; }
    if (w.Maps[0].Last.TLX != 50 || w.Maps[0].Last.BRX != 100 || w.Maps[0].Last.TLY != 0 || w.Maps[0].Last.BRY != 40) { std::printf("0063e2e0_TEST FAIL\n"); return 1; }
    if (w.Maps[1].Last.TLX != 0 || w.Maps[1].Last.BRX != 50) { std::printf("0063e2e0_TEST FAIL\n"); return 1; }
    C2DBoxI flat = { 120, 10, 180, 10 };   // zero height: no map is touched
    w.InitPassabilityFromThemesIfNecessary(flat);
    if (w.Maps[1].Calls != 1) { std::printf("0063e2e0_TEST FAIL\n"); return 1; }
    std::printf("0063e2e0_TEST PASS\n");
    return 0;
}