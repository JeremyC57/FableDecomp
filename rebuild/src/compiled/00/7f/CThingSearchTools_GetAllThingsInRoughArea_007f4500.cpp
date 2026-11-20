// 007f4500 CThingSearchTools::GetAllThingsInRoughArea, 634 retail bytes.
// Leaf: ??$GetAllThingsInRoughArea@VCThingObject@@V?$vector@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@V?$CFilter_Bind3@VCThingFilter_In3DRange@@V?$unary_negate@VCThingFilter_HasTC@@@std@@VCThingFilter_IsElectrocutable@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F4500.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_In3DRange 0x006A7780, CThingFilter_HasTC 0x006A80A0 (negated), the anonymous-namespace CThingFilter_IsElectrocutable 0x007F3D60, vector<CThingGameObject*>::_M_insert_overflow 0x00788FFC.
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the filters / Filters / a CThingSearchTools_Calls stand-in, which bound the GetAllThingMapsInArea / ClipAndConvertWorldMapAreaToLocalArea stand-ins of CThingSearchTools_Calls and its own stand-in self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0074D710 / 0x007F4020 (vector, family K016; 0x007F4020 is the same TU) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the PDB-template spelling (Ego_r.pdb 0x007ED8F0, FableWin.pdb, anonymous-namespace filter: the Leaf is cut at ?A0x) the landed caller 0x007F2DC0 already binds.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
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
#define FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_In3DRange.h"
#include "engine/CThingFilter_HasTC.h"
// Kept from the landed row (no shared header for this class), moved into the anonymous namespace and spelled `class` as both PDBs
// mangle it (VCThingFilter_IsElectrocutable@?A0x..; its own row 0x007F3D60 defines it there):
namespace { class CThingFilter_IsElectrocutable { public: bool operator()(const CThing&)const;}; } // 0x007f3d60
template void CThingSearchTools::GetAllThingsInRoughArea<CThingObject,std::vector<CThingGameObject*>,CFilter_Bind3<CThingFilter_In3DRange,std::unary_negate<CThingFilter_HasTC>,CThingFilter_IsElectrocutable> >(const C2DBoxI&,const CFilter_Bind3<CThingFilter_In3DRange,std::unary_negate<CThingFilter_HasTC>,CThingFilter_IsElectrocutable>&,std::vector<CThingGameObject*>&) const;
