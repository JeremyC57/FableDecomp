// CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex, CThing>(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter_func, CThing** thing) const -- the first thing in the mapwho cells under the area (each thing once per search id) that has the def global index, through the out pointer (true; the scratch map list is left as is); else the scratch list is cleared and false
// Leaf: ??$GetPFirstThingInRoughArea@VCThingFilter_HasDefGlobalIndex@@VCThing@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_HasDefGlobalIndex@@PAPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FCEF0, 542 bytes, thiscall `ret 0xc`; callers 0x007FBC90 (CTCKraken::GetCentreMarker), 0x00972990.
// Identity: Ego_r.pdb 0x007F81E0 names this instance; the body calls CThingFilter_HasDefGlobalIndex::operator() 0x006A70B0 (engine/CThingFilter_HasDefGlobalIndex.h).
// NF110 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing map / a local CThingSearchTools, which bound the
//   GetMapsInArea / GetCellExtents stand-ins) onto the shared headers as an explicit instantiation of the engine/CThingSearchTools.h
//   GetPFirstThingInRoughArea template (NF110 opt-in FIRSTROUGH_EGOD: the 0x009871C0 body), on the 0x009871C0 define / include set. It now
//   binds the Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... (0x0049C6F0) / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE... (0x0049C700).
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
template bool CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex,CThing>(const C2DBoxI&,const CThingFilter_HasDefGlobalIndex&,CThing**) const;
