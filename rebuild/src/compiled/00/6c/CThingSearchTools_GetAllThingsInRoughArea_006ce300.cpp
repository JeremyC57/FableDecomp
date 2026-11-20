// CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::vector<const CThingCreatureBase*>, CThingFilter_IsNot>(const C2DBoxI& area, const CThingFilter_IsNot& filter, std::vector<const CThingCreatureBase*>& result) const -- append every creature in the mapwho cells under the area (each thing once per search id) that is not the excluded thing; the scratch map list is cleared
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$vector@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@std@@VCThingFilter_IsNot@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_IsNot@@AAV?$vector@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CE300, 553 bytes, thiscall `ret 8`; one caller 0x006CDF80 (no catalog row).
// Identity (the catalog label <const CThingGameObject, vector<CIntelligentPointer<const CThing> >, CThingFilter_None> is propagated; the landed row
//   bound a CThingSearchTools_Calls stand-in): FableWin.pdb 0x0247F080 names ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$vector@PBVCThingCreatureBase@@...
//   @VCThingFilter_IsNot@@@CThingSearchTools@@QBE... (the creature-navigation TU, beside its vector<const CThingCreatureBase*> members); the body tests
//   CThingCreatureBase's type flags 0x0F, calls CThingFilter_IsNot::operator() 0x006A76A0 and std::vector<const CThingCreatureBase*>::push_back 0x006CE530
//   out of line (whose own body calls _M_insert_overflow 0x006CE570).
// NF110 (round 10, q11280 / q11283): ported off its local model (RetailVector / ThingVector / CThing / the thing map / a CThingSearchTools_Calls stand-in)
//   onto the shared headers as an explicit instantiation of the engine/CThingSearchTools.h template, on the K016 vector define set (0x0074D710:
//   ORIGINAL_ROUGHAREA + THISEND + CELL_BYTEOFFSET + STLP_VECTOR_OVERFLOW_OUTOFLINE) + ROUGHAREA_TYPEDLOCAL (the NF107 recipe of 0x008786E0) +
//   FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (retail calls push_back 0x006CE530, visible to the TU: the local model's same-TU noinline copy).
//   It now binds the Ego_r.pdb spellings of 0x0049C6F0 / 0x0049C700.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase,std::vector<const CThingCreatureBase*>,CThingFilter_IsNot>(const C2DBoxI&,const CThingFilter_IsNot&,std::vector<const CThingCreatureBase*>&) const;
