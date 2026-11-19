// CMiniMapDisplay::GetRelativePosOnMap(const C3DVector& worldPos, C2DVector& relativePos) const -- a world position as a fraction of the region map
// Leaf: ?GetRelativePosOnMap@CMiniMapDisplay@@QBEXABVC3DVector@@AAVC2DVector@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00829CE0, 60 bytes, no frame pointer, `ret 8`. Callee: CRegionMinimap::GetRegionMapExtents 0x0066C6A0
//   (`add ecx,54h ; call`), then (worldPos - mapOrigin) / mapExtents per axis (`fild ; fsubr ; fidiv`).
// Re-port (hand3c b10): the header body engine/CMiniMapDisplay.h FABLE_RETAIL_CMINIMAPDISPLAY_GETDRAWPOSITION GetRelativePosOnMap (FableWin.pdb
//   inline 0x027D3920; Ego_d locals mapOrigin / mapExtents), the same text 0x0082A020 GetDrawPosition expands. Retail emits the inline
//   member out of line as a COMDAT; the anonymous-namespace scaffolding below takes its address so this TU emits it too (the 0x00B9C110
//   pattern).
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_DRAWMARKERS
#define FABLE_RETAIL_CMINIMAPDISPLAY_GETDRAWPOSITION
#define FABLE_RETAIL_CMINIMAPDISPLAY_RELATIVEMAPSIZE_BODY
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_EXTENTS_NOINLINE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXI
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/CRegionMinimap.h"
#include "engine/CMiniMapDisplay.h"

namespace {
// Fidelity: scaffolding, not retail: takes the header member's address so this TU emits its out-of-line COMDAT. Internal to this TU.
void (CMiniMapDisplay::*ReferenceGetRelativePosOnMap())(const C3DVector&, C2DVector&) const
{
    return &CMiniMapDisplay::GetRelativePosOnMap;
}
}
