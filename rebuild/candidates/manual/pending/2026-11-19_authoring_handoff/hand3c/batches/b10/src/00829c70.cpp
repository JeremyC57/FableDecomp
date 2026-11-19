// CMiniMapDisplay::GetRelativeMapSize(C2DVector& size) const -- the region map's aspect as a size whose long side is 1
// Leaf: ?GetRelativeMapSize@CMiniMapDisplay@@QBEXAAVC2DVector@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00829C70, 112 bytes, no frame pointer, `ret 4`. Callee: CRegionMinimap::GetRegionMapExtents 0x0066C6A0
//   (`add ecx,54h ; call`), then three `ret 4` arms: equal extents (1, 1), wider (1, Y / X) and taller (X / Y, 1) by `fild ; fidiv`.
// Re-port (hand3c b10, coordinator-approved): the landed row was a free __fastcall stand-in over a local model. This is the header body
//   engine/CMiniMapDisplay.h FABLE_RETAIL_CMINIMAPDISPLAY_RELATIVEMAPSIZE_BODY (FableWin.pdb 0x027D3850; Ego_d locals size / mapOrigin /
//   mapExtents), the same text 0x0082A020 GetDrawPosition sees. Retail emits the inline member out of line as a COMDAT; the anonymous-
//   namespace scaffolding below takes its address so this TU emits it too (the 0x00B9C110 pattern).
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
void (CMiniMapDisplay::*ReferenceGetRelativeMapSize())(C2DVector&) const
{
    return &CMiniMapDisplay::GetRelativeMapSize;
}
}
