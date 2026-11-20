// Behaviour test for 0x0071A750 CTCMapwho::MoveInMapwho (NF171 2026-11-20, on the shared headers; the 0x0071A730 test form): links the
// row; the world map and the thing-map manager are stubbed. When in the map (bit 0 of +0x50) the component is removed first; it is then
// placed at the new position only if the removal left it out (the bit is re-read after the call).
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
static unsigned char g_storage[0x60];
static int g_removed = 0, g_placed = 0;
static bool g_staysIn = false;
static float g_lastY = 0.0f;

CWorldMap& CTCBase::GetWorldMap() { return *reinterpret_cast<CWorldMap*>(g_worldMap); }
void NThingMap::CThingMapManager::RemoveThing(CTCMapwho&) { ++g_removed; if (!g_staysIn) g_storage[0x50] &= 0xfe; }
void NThingMap::CThingMapManager::PlaceThing(CTCMapwho&, const C3DVector& pos)
{
    ++g_placed; g_lastY = reinterpret_cast<const float*>(&pos)[1]; g_storage[0x50] |= 1;
}

int main()
{
    void* manager = g_manager;
    std::memcpy(g_worldMap + 0x10, &manager, sizeof(manager));   // CWorldMap::PThingMapManager (+0x10)
    CTCMapwho* m = reinterpret_cast<CTCMapwho*>(g_storage);
    float v[3] = { 1.0f, 2.0f, 3.0f };
    const C3DVector& pos = *reinterpret_cast<const C3DVector*>(v);

    m->MoveInMapwho(pos);
    if (g_removed != 0 || g_placed != 1 || g_lastY != 2.0f || !(g_storage[0x50] & 1)) { std::printf("0071a750_TEST FAIL\n"); return 1; }
    v[1] = 7.0f; m->MoveInMapwho(pos);
    if (g_removed != 1 || g_placed != 2 || g_lastY != 7.0f) { std::printf("0071a750_TEST FAIL\n"); return 1; }
    g_staysIn = true; m->MoveInMapwho(pos);
    if (g_removed != 2 || g_placed != 2) { std::printf("0071a750_TEST FAIL\n"); return 1; }
    std::printf("0071a750_TEST PASS\n");
    return 0;
}
