// Behaviour test for 0x0071A730 CTCMapwho::RemoveFromMapwho (NF33 2026-11-16): links the row; the world map and the thing-map
// manager are stubbed. Removes through GetWorldMap().GetThingMapManager() only when InMapwho (bit 0 of +0x50) is set.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_THINGMAPMANAGER
#include "engine/CWorldMap.h"
#define FABLE_RETAIL_CTHINGMAPMANAGER
#include "engine/CThingMapManager.h"
#define FABLE_RETAIL_CTCMAPWHO
#include "engine/CTCMapwho.h"

static unsigned char g_worldMap[0x40];
static unsigned char g_manager[0x40];
static int g_calls = 0;
static void* g_lastThis = 0;
static void* g_lastThing = 0;

CWorldMap& CTCBase::GetWorldMap() { return *reinterpret_cast<CWorldMap*>(g_worldMap); }
void NThingMap::CThingMapManager::RemoveThing(CTCMapwho& thing) { ++g_calls; g_lastThis = this; g_lastThing = &thing; }

int main()
{
    void* manager = g_manager;
    std::memcpy(g_worldMap + 0x10, &manager, sizeof(manager));   // CWorldMap::PThingMapManager (+0x10)
    unsigned char storage[0x60];
    std::memset(storage, 0, sizeof(storage));
    CTCMapwho* m = reinterpret_cast<CTCMapwho*>(storage);

    storage[0x50] = 0; m->RemoveFromMapwho();
    if (g_calls != 0) { std::printf("FAIL: removed when not in the map\n"); return 1; }
    storage[0x50] = 1; m->RemoveFromMapwho();
    if (g_calls != 1 || g_lastThis != g_manager || g_lastThing != m) { std::printf("FAIL: remove call\n"); return 1; }
    storage[0x50] = 2; m->RemoveFromMapwho();
    if (g_calls != 1) { std::printf("FAIL: removed with only bit 1 set\n"); return 1; }
    std::printf("0071a730_TEST PASS\n");
    return 0;
}
