// Exercise the shared classes with the real STLport vector. The fixture supplies
// storage for the world fields this member reads without constructing engine services.
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_OVERLAPPING_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHREADEDFILE_ULONG_OVERFLOW_PASS30
#define FABLE_RETAIL_QUICK_MENU_PAIR_ALLOCATE_PASS27
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"
#include <stdio.h>
#include <string.h>

#ifndef FABLE_TEST_LINK_PRODUCTION
void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI& box, std::vector<unsigned long>& map_list) const
{
    for (CArray<long>::const_iterator it = LoadedMaps.begin(); it != LoadedMaps.end(); ++it) {
        long index = *it;
        if (Maps[index].Area.Overlapping(box))
            map_list.push_back(index);
    }
}
#endif

namespace {
    struct WorldStorage : CWorldMap {
        static void Init(WorldStorage& world, CMapInfo* maps, long* start, long* finish)
        {
            CMapInfo* map_fields[] = {maps, maps + 4, maps + 4};
            long* loaded_fields[] = {start, finish, finish};
            memcpy(&world.Maps, map_fields, sizeof map_fields);
            memcpy(&world.LoadedMaps, loaded_fields, sizeof loaded_fields);
        }
    };

    void InitBox(C2DBoxI& box, long x0, long y0, long x1, long y1)
    {
        box.TLX = x0;
        box.TLY = y0;
        box.BRX = x1;
        box.BRY = y1;
    }
}

int main()
{
    union { double alignment; unsigned char bytes[sizeof(CWorldMap)]; } world_bytes;
    union { double alignment; unsigned char bytes[4 * sizeof(CMapInfo)]; } map_bytes;
    WorldStorage& world = *reinterpret_cast<WorldStorage*>(world_bytes.bytes);
    CMapInfo* maps = reinterpret_cast<CMapInfo*>(map_bytes.bytes);
    long loaded[] = {2, 1, 0, 2, 3};
    InitBox(maps[0].Area, 2, 0, 8, 8);
    InitBox(maps[1].Area, 20, 20, 28, 28);
    InitBox(maps[2].Area, -8, 0, 0, 8);
    InitBox(maps[3].Area, 0, 10, 8, 20);

    C2DBoxI query;
    InitBox(query, 0, 0, 6, 6);
    // Exercise the existing shared overlap body used as the out-of-scope link boundary.
    if (!maps[2].Area.Overlapping(query) || maps[1].Area.Overlapping(query))
        return 1;

    for (unsigned n = 0; n <= sizeof loaded / sizeof loaded[0]; ++n) {
        for (unsigned seeded = 0; seeded != 2; ++seeded) {
            std::vector<unsigned long> found;
            if (seeded)
                found.push_back(99);
            unsigned long expected[6] = {99};
            unsigned end = seeded;
            for (unsigned j = 0; j != n; ++j) {
                if (loaded[j] == 2 || loaded[j] == 0)
                    expected[end++] = loaded[j];
            }
            WorldStorage::Init(world, maps, loaded, loaded + n);
            world.CWorldMap::GetAllLoadedMapsInArea(query, found);
            if (found.size() != end)
                return 2;
            for (unsigned j = 0; j != end; ++j) {
                if (found[j] != expected[j])
                    return 3;
            }
        }
    }
    puts("004ff310_TEST PASS");
    return 0;
}
