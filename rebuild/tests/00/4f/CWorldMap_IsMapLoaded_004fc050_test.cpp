// Behaviour test (0x004fc050): CWorldMap::IsMapLoaded is true only for a used, activated map that has a CMap
#include <cstdio>
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CMap { public: int Dummy; };
class CMapInfo {
public:
    C2DBoxI       Area;
    CMap*         PMap;
    bool          Used;
    bool          Placed;
    bool          Activated;
};
template <class T> class CArray {
public:
    T* Start; T* Finish; T* EndOfStorage;
    const T& operator[](unsigned long i) const { return Start[i]; }
};
class CWorldMap {
public:
    CArray<CMapInfo> Maps;
    bool IsMapLoaded(long map) const
    {
        const CMapInfo& info = Maps[map];
        return info.Used && info.Activated && info.PMap;
    }
};
int main()
{
    CMap m;
    CMapInfo infos[5];
    for (int i = 0; i < 5; ++i) { infos[i].PMap = &m; infos[i].Used = true; infos[i].Activated = true; infos[i].Placed = false; }
    infos[1].Used = false; infos[2].Activated = false; infos[3].PMap = 0;
    CWorldMap wm; wm.Maps.Start = infos; wm.Maps.Finish = infos + 5; wm.Maps.EndOfStorage = infos + 5;
    if (!wm.IsMapLoaded(0) || wm.IsMapLoaded(1) || wm.IsMapLoaded(2) || wm.IsMapLoaded(3) || !wm.IsMapLoaded(4))
        { std::printf("004fc050_TEST FAIL\n"); return 1; }
    std::printf("004fc050_TEST PASS\n");
    return 0;
}