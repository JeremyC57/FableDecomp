// CMiniMapDisplay::RemoveMarker(const CThing& thing) -- remove the markers bound to the thing (list remove_if with CIsMarkerBoundToThing)
// Leaf: ?RemoveMarker@CMiniMapDisplay@@QAEXABVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082A380, 8 bytes: `add ecx,20h ; jmp 0x0082C770` (the predicate is the caller's argument
//   slot, reused in place).
// Identity (two sources): (1) Ego_r.pdb ?RemoveMarker@CMiniMapDisplay@@QAEXABVCThing@@@Z with the instantiations it pulls in,
//   ??0CIsMarkerBoundToThing@CMiniMapDisplay@@QAE@ABVCThing@@@Z (a 4-byte CThing* PThing) and
//   ??$remove_if@VCIsMarkerBoundToThing@CMiniMapDisplay@@@?$list@VCMarkerInfo@CMiniMapDisplay@@..@std@@QAEXVCIsMarkerBoundToThing@
//   CMiniMapDisplay@@@Z; (2) the target 0x0082C770 walks the list at [ecx], reads each node's PThing (GetPItem 0x00A01B50 on node+8),
//   compares it with the 4-byte argument and tests AutoMarker (+0xc of the value) before unlinking: the inlined
//   CIsMarkerBoundToThing::operator() of the predicate. The landed reloc already names the row RemoveMarker. Catalog
//   "_Cons_val<..CParticleMorphs..>" was propagated.
// Callees: ??$remove_if@VCIsMarkerBoundToThing@CMiniMapDisplay@@@?$list@VCMarkerInfo@CMiniMapDisplay@@V?$allocator@VCMarkerInfo@
//   CMiniMapDisplay@@@std@@@std@@QAEXVCIsMarkerBoundToThing@CMiniMapDisplay@@@Z 0x0082C770 (FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE).
// Header: engine/CMiniMapDisplay.h (FABLE_RETAIL_CMINIMAPDISPLAY new), engine/CIntelligentPointer.h, stlport/fable_containers.h.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#include "engine/CMiniMapDisplay.h"
void CMiniMapDisplay::RemoveMarker(const CThing& thing)
{
    Markers.remove_if(CIsMarkerBoundToThing(thing));
}