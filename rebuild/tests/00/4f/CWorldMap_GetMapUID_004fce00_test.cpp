// Behaviour test for 0x004FCE00 CWorldMap::GetMapUID(long) const: links the row object; raw CWorldMap storage whose Maps vector (+0x20)
// points at three 0x48-byte CMapInfo records; checks the UID word (+0x20) of the indexed record.
// (NF108 2026-11-19: replaces the self-contained copy of the old S::GetEditMap stand-in.)
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"

int main()
{
    static unsigned char maps[3 * 0x48];
    static unsigned char world[0x100];
    std::memset(maps, 0, sizeof(maps));
    std::memset(world, 0, sizeof(world));
    unsigned long uid = 42;
    std::memcpy(maps + 2 * 0x48 + 0x20, &uid, sizeof(uid));
    unsigned char* start = maps;
    unsigned char* finish = maps + sizeof(maps);
    std::memcpy(world + 0x20, &start, sizeof(start));
    std::memcpy(world + 0x24, &finish, sizeof(finish));
    std::memcpy(world + 0x28, &finish, sizeof(finish));
    const CWorldMap* world_map = reinterpret_cast<const CWorldMap*>(world);
    if (world_map->GetMapUID(2) != 42 || world_map->GetMapUID(1) != 0) { std::printf("004fce00_TEST FAIL\n"); return 1; }
    std::printf("004fce00_TEST PASS\n");
    return 0;
}
