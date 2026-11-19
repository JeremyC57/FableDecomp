// NThingMap::InitLayerInfo() -- set up the thing-map layer globals: the layer count (six) and the largest layer cell size of the static layer table
// Leaf: ?InitLayerInfo@NThingMap@@YAXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006379D0, 53 bytes, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?InitLayerInfo@NThingMap@@YIXXZ (thing_map_manager.obj #3, Ego_r 53 bytes like retail); the only
//   caller is NThingMap::CThingMapManager::Init 0x00638540. Globals: NThingMap::MaxLayers 0x013BA848, NThingMap::MaxLayerCellSize
//   0x013BA84C (FableWin public ?MaxLayerCellSize@NThingMap@@3JA), NThingMap::LayerInfo 0x0137DAD8 (six CLayerInfo, cell size first).
// Shape: FableWin 0x0221F3A0 (/Od): MaxLayers = 6; MaxLayerCellSize = 0; then a VC6 for-scope loop (`xor eax,eax ; je`) over
//   c1 < MaxLayers storing *std::max(&LayerInfo[c1].Size, &MaxLayerCellSize) each pass (Ego_d / FableWin local `c1`, long).
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