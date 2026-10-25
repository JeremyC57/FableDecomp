// Behaviour test (0x004fc5b0): CWorldMap::IsPosInMap is true inside the world area when the map under the position has a CMap
#include <cstdio>
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DCoordI { public: long X, Y; };
class CMap { public: int Dummy; };
class CMapInfo { public: C2DBoxI Area; CMap* PMap; };
template <class T> class CArray {
public:
    T* Start; T* Finish; T* EndOfStorage;
    const T& operator[](unsigned long i) const { return Start[i]; }
};
class CWorldMap {
public:
    CArray<CMapInfo> Maps;
    C2DBoxI          Area;
    virtual long GetMapNumberAt(const C2DCoordI& pos) const { return pos.X < 50 ? 0 : 1; }
    virtual ~CWorldMap() { }
    bool IsPosInMap(const C2DCoordI& pos) const
    {
        if (pos.X >= Area.TLX && pos.X < Area.BRX && pos.Y >= Area.TLY && pos.Y < Area.BRY)
        {
            if (Maps[GetMapNumberAt(pos)].PMap)
                return true;
        }
        return false;
    }
};
int main()
{
    CMap m;
    CMapInfo infos[2]; infos[0].PMap = 0; infos[1].PMap = &m;
    CWorldMap wm; wm.Maps.Start = infos; wm.Maps.Finish = infos + 2; wm.Maps.EndOfStorage = infos + 2;
    wm.Area.TLX = 0; wm.Area.TLY = 0; wm.Area.BRX = 100; wm.Area.BRY = 100;
    C2DCoordI a; a.X = 60; a.Y = 10;            // map 1 has a CMap
    C2DCoordI b; b.X = 10; b.Y = 10;            // map 0 has none
    C2DCoordI c; c.X = 100; c.Y = 10;           // outside
    if (!wm.IsPosInMap(a) || wm.IsPosInMap(b) || wm.IsPosInMap(c)) { std::printf("004fc5b0_TEST FAIL\n"); return 1; }
    std::printf("004fc5b0_TEST PASS\n");
    return 0;
}