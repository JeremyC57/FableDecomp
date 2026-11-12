// std::_List_base<CBaseIntelligentPointer>::clear() -- destroy and free every node, leave the sentinel linked to itself
// Leaf: ?clear@?$_List_base@VCMarkerInfo@CMiniMapDisplay@@V?$allocator@VCMarkerInfo@CMiniMapDisplay@@@std@@@std@@QAEXXZ
// Retail: Fable.exe @ 0x00440730, 56 bytes, plain `ret`; callers 0x00440E43, 0x00441673, 0x00442663, 0x0082A313 and a
// tail jump from 0x0082A303. Catalog label CGuiControlTreePane::SortTreeRecursively is wrong: STLport _List_base::clear
// (as 0x0043FE40) over intelligent-pointer elements -- each node's value (+8) is destroyed by the out-of-line
// CBaseIntelligentPointer destructor (0x00A01C10, non-virtual: called directly), then free(node) (0x00BFEA14). The
// exact CIntelligentPointer<T> instantiation is not visible in the bytes, so the element is modelled as the base.
// Layout: STLport list -- one sentinel-node pointer; node {_M_next +0, _M_prev +4, _M_data +8}; CBaseIntelligentPointer
// 8 bytes (vptr, info pointer).
// Shape: /O2 (`lea ecx,[ecx]` loop-head pad, end re-read from the sentinel pointer every pass).
// Name fix (agentNF23): the element is CMiniMapDisplay::CMarkerInfo (catalog name; Ego_r.pdb CMiniMapDisplay::Markers is
//   list<CMiniMapDisplay::CMarkerInfo>, CMarkerInfo {CIntelligentPointer<CThing const> PThing +0, GraphicIndex +8, AutoMarker +0xc}),
//   whose implicit destructor runs the PThing member's base destructor directly -- so the call binds the PDB spelling
//   ??1CBaseIntelligentPointer@@UAE@XZ (virtual) through engine/CIntelligentPointer.h instead of a local non-virtual model.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMINIMAPDISPLAY
#include "stlport/fable_containers.h"
#include "engine/CMiniMapDisplay.h"
template void std::_List_base<CMiniMapDisplay::CMarkerInfo, std::allocator<CMiniMapDisplay::CMarkerInfo> >::clear();
