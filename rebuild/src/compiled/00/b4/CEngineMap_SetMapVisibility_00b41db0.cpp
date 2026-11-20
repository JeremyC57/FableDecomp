// CEngineMap::SetMapVisibility(list<CEngineMap*>& visibleMaps, bool visible) -- links the map at the front of the manager's visible-map list (or unlinks it), remembers its list position and visibility, then tells its landscape and local-detail parts
// Leaf: ?SetMapVisibility@CEngineMap@@QAEXAAV?$list@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@std@@@std@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B41DB0, length 145 bytes (160 with padding).
// Callees: malloc @ 0x00BFEA0E (the 12-byte list node), free @ 0x00BFEA14, CEngineLandscapeMap::OnMapVisibilityChanged(bool)
// @ 0x00BDC3C0 (on the map's landscape part at +0), CLocalDetailCacheMap::OnMapVisibilityChanged(bool) @ 0x00BDE160 (on its
// local-detail part at +4). Caller: CEngineMapManager::SetMapVisibility @ 0x00B42390 (landed) passes &m_visibleMaps.
// Shape: the list is the retail STLport-style sentinel-only list (node {next, prev, value}, no size word) with malloc/free
// nodes: insert(begin(), this) builds the node and placement-constructs the value (null-tested `lea ecx,[eax+8]`), then
// links it before `where` (prev read after the construction); erase(pos) unlinks next/prev and frees the node. On becoming
// visible the stored position is begin() re-read from the list, on becoming hidden it is end() (the head). Both part calls
// are null-tested and take the new visibility; `ret 8`.
// Naming (NF160, 2026-11-20, q4592 / q4593): the invented CRetailList<CEngineMap*> and the local CEngineMap / CEngineLandscapeMap /
//   CLocalDetailCacheMap models replaced by the shared STLport std::list<CEngineMap*> (stlport/fable_containers.h) and engine/CEngineMap.h
//   (LandscapeMap +0x00, LocalDetailCacheMap +0x04, VisibleMapListIterator +0x58, IsInVisibleMapList +0x64), so the row defines the Ego_r.pdb
//   0x009E2440 / FableWin.pdb 0x02D5B370 spelling ?SetMapVisibility@CEngineMap@@QAEXAAV?$list@PAVCEngineMap@@...@std@@_N@Z that its two
//   callers CEngineWorldMap::SetMapVisibility 0x00B42390 / SetMapVisibilityAllMaps 0x00B423B0 bind (was ...AAU?$CRetailList@PAVCEngineMap@@@@_N@Z).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEMAP_SETMAPVISIBILITY
#define FABLE_RETAIL_CENGINELANDSCAPEMAP_ONMAPVISIBILITYCHANGED
#define FABLE_RETAIL_CLOCALDETAILCACHEMAP_ONMAPVISIBILITYCHANGED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CEngineMap.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"
#include "engine/CLocalDetailCacheMap.h"

void CEngineMap::SetMapVisibility(std::list<CEngineMap*>& visible_maps, bool visible)
{
    if (visible) {
        if (!IsInVisibleMapList) {
            visible_maps.insert(visible_maps.begin(), this);
            VisibleMapListIterator = visible_maps.begin();
            IsInVisibleMapList = true;
        }
    }
    else if (IsInVisibleMapList) {
        visible_maps.erase(VisibleMapListIterator);
        VisibleMapListIterator = visible_maps.end();
        IsInVisibleMapList = false;
    }
    if (LandscapeMap)
        LandscapeMap->OnMapVisibilityChanged(visible);
    if (LocalDetailCacheMap)
        LocalDetailCacheMap->OnMapVisibilityChanged(visible);
}
