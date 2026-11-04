// CMiniMapDisplay::RemoveAllMarkers() -- drop every map marker (clear the marker list)
// Leaf: ?RemoveAllMarkers@CMiniMapDisplay@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082A300, 8 bytes: `add ecx,20h ; jmp 0x00440730` (Markers.clear(), tail call).
// Identity (two sources): (1) Ego_r.pdb ?RemoveAllMarkers@CMiniMapDisplay@@QAEXXZ, its body a clear of the Markers list
//   (?clear@?$list@VCMarkerInfo@CMiniMapDisplay@@..), Markers the Ego_r member after MiniMapGraphic; (2) retail places it in the
//   CMiniMapDisplay TU between RemoveAllDeadMarkers 0x0082A2A0 and Initialise 0x0082A310 (landed), which runs the same
//   `add ecx,20h ; call 0x00440730` before clearing its flag, and the landed GetNonAutoMarkers 0x0082A390 walks [this+0x20] as the
//   marker list. The catalog's "_Cons_val<..CParticleMorphs..>" label was propagated.
// Callees: ?clear@?$_List_base@VCMarkerInfo@CMiniMapDisplay@@V?$allocator@VCMarkerInfo@CMiniMapDisplay@@@std@@@std@@QAEXXZ 0x00440730
//   (STLport _List_base::clear; the landed body is spelled for CBaseIntelligentPointer: NAME_FIXES proposal).
// Header: engine/CMiniMapDisplay.h (FABLE_RETAIL_CMINIMAPDISPLAY new), engine/CIntelligentPointer.h, stlport/fable_containers.h.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMINIMAPDISPLAY
#include "engine/CMiniMapDisplay.h"
void CMiniMapDisplay::RemoveAllMarkers()
{
    Markers.clear();
}