// Behaviour test for CWorldMap::CWorldMap (0x005066E0): granularity 1 << shift, the map-placement grid sized
// (height / granularity) * (width / granularity) and zero-filled, its pitch width / granularity, the fixed defaults.
#include <cstdio>

class C2DBoxI { public: long TLX, TLY, BRX, BRY; long GetWidth() const { return BRX - TLX; } long GetHeight() const { return BRY - TLY; } };
class CWorldMap { public:
    long Granularity; unsigned long GranularityShift; C2DBoxI Area; unsigned short MapPlacement[256]; long MapPlacementSize;
    long MapPlacementPitch; long LoadedRegion; bool UseLevelWADBank; long CurrentMapUIDCount; bool SoundThemesDisabled;
    int Maps, Regions, PlayerPositions;
    CWorldMap(const C2DBoxI& area, unsigned long granularityShift)
        : Granularity(1 << granularityShift), GranularityShift(granularityShift), LoadedRegion(0), UseLevelWADBank(false),
          CurrentMapUIDCount(1), Maps(1), Regions(1), PlayerPositions(4)
    {
        Area = area;
        MapPlacementSize = (Area.GetHeight() / (1 << granularityShift)) * (Area.GetWidth() / (1 << granularityShift));
        for (long i = 0; i < MapPlacementSize; ++i) MapPlacement[i] = 0;
        MapPlacementPitch = Area.GetWidth() / (1 << granularityShift);
        SoundThemesDisabled = true;
    }
};

int main()
{
    C2DBoxI area = { 0, 0, 64, 32 };
    CWorldMap map(area, 3);
    bool ok = map.Granularity == 8 && map.MapPlacementSize == 4 * 8 && map.MapPlacementPitch == 8 && map.MapPlacement[31] == 0
        && map.LoadedRegion == 0 && !map.UseLevelWADBank && map.CurrentMapUIDCount == 1 && map.SoundThemesDisabled
        && map.Maps == 1 && map.Regions == 1 && map.PlayerPositions == 4 && map.Area.BRX == 64;
    C2DBoxI odd = { 16, 8, 50, 30 };
    CWorldMap m2(odd, 2);
    ok = ok && m2.Granularity == 4 && m2.MapPlacementSize == 5 * 8 && m2.MapPlacementPitch == 8;
    if (!ok) { std::printf("005066e0_TEST FAIL\n"); return 1; }
    std::printf("005066e0_TEST PASS\n");
    return 0;
}