// Behaviour test (0x004fc560): CWorldMap::ConvertLocalMapAreaToWorldArea offsets a local box by the map's top-left corner
#include <cstdio>
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) { }
};
class CMapInfo {
public:
    C2DBoxI       Area;
    unsigned char _rest[0x38];
    CMapInfo() : Area(0, 0, 0, 0) { }
};
template <class T> class CArray {
public:
    T* Start; T* Finish; T* EndOfStorage;
    const T& operator[](unsigned long i) const { return Start[i]; }
};
class CWorldMap {
public:
    CArray<CMapInfo> Maps;
    C2DBoxI ConvertLocalMapAreaToWorldArea(long map, const C2DBoxI& box) const
    {
        const C2DBoxI& area = Maps[map].Area;
        C2DBoxI result(box);
        result.TLX += area.TLX;
        result.BRX += area.TLX;
        result.TLY += area.TLY;
        result.BRY += area.TLY;
        return result;
    }
};
int main()
{
    CMapInfo infos[2];
    infos[1].Area = C2DBoxI(100, 200, 300, 400);
    CWorldMap wm; wm.Maps.Start = infos; wm.Maps.Finish = infos + 2; wm.Maps.EndOfStorage = infos + 2;
    C2DBoxI r = wm.ConvertLocalMapAreaToWorldArea(1, C2DBoxI(1, 2, 3, 4));
    if (r.TLX != 101 || r.TLY != 202 || r.BRX != 103 || r.BRY != 204) { std::printf("004fc560_TEST FAIL\n"); return 1; }
    C2DBoxI z = wm.ConvertLocalMapAreaToWorldArea(0, C2DBoxI(-5, 6, 7, 8));
    if (z.TLX != -5 || z.TLY != 6 || z.BRX != 7 || z.BRY != 8) { std::printf("004fc560_TEST FAIL\n"); return 1; }
    std::printf("004fc560_TEST PASS\n");
    return 0;
}