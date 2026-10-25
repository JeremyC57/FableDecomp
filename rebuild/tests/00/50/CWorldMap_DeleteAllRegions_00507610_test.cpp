// Behaviour test (0x00507610): CWorldMap::DeleteAllRegions empties the region vector and leaves one default region
#include <cstdio>
#include <new>
static int g_live = 0, g_tag = 0;
class CRegion {
public:
    int Tag;
    CRegion() : Tag(0) { ++g_live; }
    CRegion(const CRegion& o) : Tag(o.Tag) { ++g_live; }
    ~CRegion() { --g_live; }
};
class CRegionVector {                           // minimal model of the STLport vector<CRegion> calls used
public:
    CRegion* _M_start; CRegion* _M_finish; CRegion* _M_end_of_storage;
    unsigned char Storage[16 * sizeof(CRegion)];
    CRegionVector() { _M_start = _M_finish = (CRegion*)Storage; _M_end_of_storage = _M_start + 16; }
    CRegion* begin() { return _M_start; }
    CRegion* end() { return _M_finish; }
    CRegion* erase(CRegion* first, CRegion* last)
    {
        CRegion* out = first;
        for (CRegion* in = last; in != _M_finish; ++in, ++out) *out = *in;
        for (CRegion* p = out; p != _M_finish; ++p) p->~CRegion();
        _M_finish = out;
        return first;
    }
    void resize(unsigned int n, const CRegion& x)
    {
        while ((unsigned int)(_M_finish - _M_start) < n) { new (_M_finish) CRegion(x); ++_M_finish; }
        if ((unsigned int)(_M_finish - _M_start) > n) erase(_M_start + n, _M_finish);
    }
    void resize(unsigned int n) { resize(n, CRegion()); }
    void clear() { erase(begin(), end()); }
};
class CWorldMap {
public:
    CRegionVector Regions;
    virtual ~CWorldMap() { }
    virtual void DeleteAllRegions()
    {
        Regions.clear();
        Regions.resize(1);
    }
};
int main()
{
    {
        CWorldMap wm;
        CRegion r; r.Tag = 5;
        wm.Regions.resize(4, r);
        if (g_live != 5) { std::printf("00507610_TEST FAIL\n"); return 1; }
        wm.DeleteAllRegions();
        if (wm.Regions.end() - wm.Regions.begin() != 1 || wm.Regions.begin()->Tag != 0 || g_live != 2)
            { std::printf("00507610_TEST FAIL\n"); return 1; }
        wm.Regions.clear();
    }
    if (g_live != 0) { std::printf("00507610_TEST FAIL\n"); return 1; }
    std::printf("00507610_TEST PASS\n");
    return 0;
}