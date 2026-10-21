// Behaviour test for CThingManager::AllocateNewThingUID (0x0051E100): global / plain counters with their high tags,
// level-less zero, and the map-specific id built from the map prefix and the map counter.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct CWorldMap {
    u64 MapCounter;
    unsigned long GetMapUIDPrefix(long mapNumber) { return (unsigned long)mapNumber * 3; }
    u64 AllocateMapThingUID(long) { return MapCounter++; }
};

struct CThingManager {
    CWorldMap* WorldMap;
    u64 CurrentUniqueID;
    u64 CurrentLocalThingUniqueID;
    bool UseMapSpecificUIDs;
    u64 AllocateNewThingUID(long mapNumber, bool global, bool levelLess)
    {
        u64 uid = 0;
        if (UseMapSpecificUIDs) {
            if (global)
                { uid = CurrentLocalThingUniqueID++; uid += (u64)0xFFFFFD00 << 32; }
            if (levelLess)
                return 0;
            if (mapNumber > 0) {
                u64 prefix = WorldMap->GetMapUIDPrefix(mapNumber);
                CWorldMap* worldMap = WorldMap;
                uid = worldMap->AllocateMapThingUID(mapNumber) + prefix * ((u64)0x100 << 32);
            }
        } else {
            if (levelLess)
                return 0;
            if (global)
                { uid = CurrentLocalThingUniqueID++; uid += (u64)0xFFFFFD00 << 32; }
            else
                { uid = CurrentUniqueID++; uid += (u64)0xFFFFFF00 << 32; }
        }
        return uid;
    }
};

int main()
{
    CWorldMap map; map.MapCounter = 7;
    CThingManager m; m.WorldMap = &map; m.CurrentUniqueID = 10; m.CurrentLocalThingUniqueID = 20; m.UseMapSpecificUIDs = false;
    bool ok = true;
    ok = ok && m.AllocateNewThingUID(1, false, false) == (((u64)0xFFFFFF00 << 32) + 10) && m.CurrentUniqueID == 11;
    ok = ok && m.AllocateNewThingUID(1, true, false) == (((u64)0xFFFFFD00 << 32) + 20) && m.CurrentLocalThingUniqueID == 21;
    ok = ok && m.AllocateNewThingUID(1, true, true) == 0 && m.CurrentLocalThingUniqueID == 21;
    m.UseMapSpecificUIDs = true;
    ok = ok && m.AllocateNewThingUID(2, false, false) == (((u64)6 << 40) + 7) && map.MapCounter == 8;
    ok = ok && m.AllocateNewThingUID(0, true, false) == (((u64)0xFFFFFD00 << 32) + 21) && m.CurrentLocalThingUniqueID == 22;
    ok = ok && m.AllocateNewThingUID(0, false, false) == 0;
    ok = ok && m.AllocateNewThingUID(3, true, true) == 0 && m.CurrentLocalThingUniqueID == 23;
    if (!ok) { std::printf("0051e100_TEST FAIL\n"); return 1; }
    std::printf("0051e100_TEST PASS\n");
    return 0;
}