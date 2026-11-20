// Behaviour test for 0x0071A710 CTCMapwho::PlaceInMapwho (NF171 2026-11-20, on the shared headers; the 0x0071A730 test form): links the
// row; the world map and the thing-map manager are stubbed. Places through GetWorldMap().GetThingMapManager() only when InMapwho (bit 0
// of +0x50) is clear, passing the component and the position by reference. Keeps the catalog PassPattern PLACEMAPWHO_OK.
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
static int g_placed = 0;
static void* g_lastThis = 0;
static void* g_lastThing = 0;
static const void* g_lastPos = 0;

CWorldMap& CTCBase::GetWorldMap() { return *reinterpret_cast<CWorldMap*>(g_worldMap); }
void NThingMap::CThingMapManager::PlaceThing(CTCMapwho& thing, const C3DVector& pos)
{
    ++g_placed; g_lastThis = this; g_lastThing = &thing; g_lastPos = &pos;
}

int main()
{
    void* manager = g_manager;
    std::memcpy(g_worldMap + 0x10, &manager, sizeof(manager));   // CWorldMap::PThingMapManager (+0x10)
    unsigned char storage[0x60];
    std::memset(storage, 0, sizeof(storage));
    CTCMapwho* m = reinterpret_cast<CTCMapwho*>(storage);
    float v[3] = { 1.0f, 2.0f, 3.0f };
    const C3DVector& pos = *reinterpret_cast<const C3DVector*>(v);

    storage[0x50] = 0; m->PlaceInMapwho(pos);
    if (g_placed != 1 || g_lastThis != g_manager || g_lastThing != m || g_lastPos != v) { std::printf("FAIL1\n"); return 1; }
    storage[0x50] = 1; m->PlaceInMapwho(pos);
    if (g_placed != 1) { std::printf("FAIL2\n"); return 1; }
    storage[0x50] = 3; m->PlaceInMapwho(pos);
    if (g_placed != 1) { std::printf("FAIL3\n"); return 1; }
    storage[0x50] = 2; m->PlaceInMapwho(pos);
    if (g_placed != 2) { std::printf("FAIL4\n"); return 1; }
    std::printf("PLACEMAPWHO_OK\n");
    return 0;
}
