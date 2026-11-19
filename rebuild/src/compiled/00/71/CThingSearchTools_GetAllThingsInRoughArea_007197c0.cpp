// CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::vector<CThingCreatureBase*>, CThingFilter_None>(const C2DBoxI& area, const CThingFilter_None& filter, std::vector<CThingCreatureBase*>& things) const -- append every CThingCreatureBase in the mapwho cells under the area to the vector, each thing visited once per search stamp
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$vector@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@VCThingFilter_None@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_None@@AAV?$vector@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007197C0, 570 bytes, thiscall `ret 0Ch`.
// Identity (proven): the type mask 0xF is CThingCreatureBase's; no filter code; the overflow 0x006F2680 is bound to vector<CThingCreatureBase*>-shaped names; Ego_r/FableWin hold one such instance, ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$vector@PAVCThingCreatureBase@@...VCThingFilter_None@@; callers CActionTalkBase::SetUpCamera 0x00715A70, CTCShotDefinitionBase::UpdateCreatureAwarenessOfMe 0x007C04A0 (landed).
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value),
//   vector<CThingCreatureBase*>::_M_insert_overflow 0x006F2680 (push_back's growth path), memmove 0x00BFEAE6 (the AreaMaps clear).
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF106 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / CThingCreatureBase / CThingFilter_None / CThingMap /
//   CMapVector / an STLport vector / a local CThingSearchTools, which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf)
//   onto the shared headers as an explicit instantiation of the header template, on the landed 0x0095C7C0 / 0x007A3CA0 define set (the
//   vector instances of family K016: ORIGINAL_ROUGHAREA + ROUGHAREA_THISEND + ROUGHAREA_CELL_BYTEOFFSET, the overflow out of line). It now
//   binds the Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...,
//   and its self leaf is the PDB QBE spelling the landed caller 0x00715A70 already binds.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_None.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase,std::vector<CThingCreatureBase*>,CThingFilter_None>(const C2DBoxI&,const CThingFilter_None&,std::vector<CThingCreatureBase*>&) const;
