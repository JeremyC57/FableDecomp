// Behaviour test for 0x006379D0 NThingMap::InitLayerInfo: the row's defines, includes and body verbatim. The layer table's cell sizes
// are not sorted; the largest of the first six (64, at index 4) must win over a smaller last entry, a stale previous maximum (5000)
// must be cleared first, a stale layer count (3) must be replaced by six before the walk (else the maximum would be 8), and a
// seventh entry past the six layers (1000) must be ignored.
#include <stdio.h>
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_CTHINGMAP_INITLAYERINFO
#include "engine/NThingMap_CThingMap.h"

void NThingMap::InitLayerInfo()
{
    MaxLayers = 6;
    MaxLayerCellSize = 0;
    if (false) {} else for (long c1 = 0; c1 < MaxLayers; ++c1)
        MaxLayerCellSize = std::max(LayerInfo[c1].Size, MaxLayerCellSize);
}

namespace NThingMap {
long MaxLayers = 3;
long MaxLayerCellSize = 5000;
extern const CLayerInfo LayerInfo[7] = { {2, 1}, {8, 2}, {4, 3}, {32, 5}, {64, 6}, {-1, 0}, {1000, 9} };
}

static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    NThingMap::InitLayerInfo();
    check(NThingMap::MaxLayers == 6, "six layers");
    check(NThingMap::MaxLayerCellSize == 64, "largest cell size of the six layers");
    NThingMap::MaxLayerCellSize = -7;
    NThingMap::MaxLayers = 1;
    NThingMap::InitLayerInfo();
    check(NThingMap::MaxLayers == 6 && NThingMap::MaxLayerCellSize == 64, "repeatable from any previous state");
    if (!g_fail) printf("006379d0_TEST PASS\n");
    return g_fail;
}