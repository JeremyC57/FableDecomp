// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_None& filter, std::vector<const CThingObject*>& things) const -- instance <const CThingObject, std::vector<const CThingObject*>, CThingFilter_None>: append every CThingObject (type bits 0x807) in the mapwho cells under the area to the vector, each thing visited once per search stamp.
// Grade: RELOCATION_MATCH -- w24A: ported from the lane's local model to the shared template body (engine/CThingSearchTools.h FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA + ROUGHAREA_THISEND, STLport vector with the overflow kept out of line). The one residue, the SIB operand order of the row-width load (`mov ecx,[ecx+eax+4]`, 0x00959483: layer * 20 as base), needs the FITTED cell expression FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET -- the byte-offset spelling the landed family rows 0x006F3570 / 0x006F37B0 / 0x007197C0 carry in their local models; the natural inline PeekCell gives `[eax+ecx+4]` here (it is retail's order in the std::list instances, 0x008781D3).
// Leaf: ??$GetAllThingsInRoughArea@$$CBVCThingObject@@V?$vector@PBVCThingObject@@V?$allocator@PBVCThingObject@@@std@@@std@@VCThingFilter_None@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_None@@AAV?$vector@PBVCThingObject@@V?$allocator@PBVCThingObject@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00959380, 576 bytes, thiscall `ret 0Ch`.
// Identity: the body tests type mask 0x807 (CThingObject, FableWin.pdb ?StatGetTypeFlags@CThingObject@@SIKXZ) and has no filter code (CThingFilter_None); push_back's growth path is std::vector<const CThingObject*>::_M_insert_overflow 0x0091FFE0; Ego_r.pdb / FableWin.pdb hold this instance name and the landed caller binds exactly this symbol to the address (reloc_bindings.tsv); only caller CAIStateGroup_HoverWanderAroundThing::IsRunnable 0x00959800. The catalog name (GetPBestThingInRoughArea<CThingFilter_HasTrackName,...>) is a propagated label: this body returns void with `ret 0Ch` (three arguments).
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), the vector overflow above, memmove 0x00BFEAE6 (the AreaMaps clear).
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_None.h"
template void CThingSearchTools::GetAllThingsInRoughArea<const CThingObject,std::vector<const CThingObject*>,CThingFilter_None>(const C2DBoxI&,const CThingFilter_None&,std::vector<const CThingObject*>&) const;