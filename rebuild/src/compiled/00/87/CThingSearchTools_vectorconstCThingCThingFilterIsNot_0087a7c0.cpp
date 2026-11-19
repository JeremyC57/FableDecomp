// CThingSearchTools::GetAllThingsInRoughArea<const CThingCreatureBase, std::vector<const CThing*>, CThingFilter_IsNot>(const C2DBoxI& area, const CThingFilter_IsNot& filter_func, std::vector<const CThing*>& things) const -- append every creature in the mapwho cells under the area (each thing once per search id) other than the excluded thing to the vector of things
// Leaf: ??$GetAllThingsInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087A7C0, 590 bytes, thiscall `ret 0xc`; one caller 0x0087A591 (`mov ecx,[eax+20h]`: the world's
//   thing-search tools; result vector at [esp+0xd8]).
// Identity (IDENTIFY row; was the propagated catalog label GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot,
//   CIsVeryCloseMotionlessCreature>,CThingCompare_Nearest,CThingCreatureBase>; the body collects into a vector and compares nothing):
//   Ego_r.pdb 0x00864EC0 (release\cm_projectile_weapon.obj) and FableWin.pdb 0x029CECB0 (debug\cm_projectile_weapon.obj, 1393 bytes /Od)
//   ??$GetAllThingsInRoughArea@$$CBVCThingCreatureBase@@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@VCThingFilter_IsNot@@@
//   CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_IsNot@@AAV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z. Retail
//   proof of the element type: the overflow call goes to 0x0049C370 = vector<const CThing*>::_M_insert_overflow (reloc_bindings; the
//   vector<CThingCreatureBase*> copy is 0x006F2680), and push_back's `const CThing*` conversion temporary is homed only after the filter
//   call returns true (0x0087A933), where a same-type named pointer would be stored before the call (DIFFER 590v590 with
//   <CThingCreatureBase, vector<CThingCreatureBase*>>). The row's caller sits in the camera code (cm_projectile_weapon.obj).
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, CThingFilter_IsNot::operator()
//   0x006A76A0, vector<const CThing*>::_M_insert_overflow 0x0049C370 (out of line), memmove 0x00BFEAE6.
// Shape: the shared Ego_d-named GetAllThingsInRoughArea body (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD, the landed 0x00878430
//   text) instantiated with the PDB's template arguments.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
template void CThingSearchTools::GetAllThingsInRoughArea<const CThingCreatureBase, std::vector<const CThing*>, CThingFilter_IsNot>(const C2DBoxI&, const CThingFilter_IsNot&, std::vector<const CThing*>&) const;