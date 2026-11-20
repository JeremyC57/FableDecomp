// 0080e670 CThingSearchTools::GetAllThingsInRoughArea, 622 retail bytes.
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$list@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@V?$CFilter_Bind4@VCThingFilter_IsNot@@VCThingFilter_IsNotList@@VCThingFilter_IsWithinArea@@V?$unary_negate@VCThingFilter_IsEvadeExplosion@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080E670.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_IsNot 0x006A76A0, CThingFilter_IsNotList 0x006A7EB0, CThingFilter_IsWithinArea 0x007FD830, the anonymous-namespace CThingFilter_IsEvadeExplosion 0x0080DC90 (negated), the list node malloc 0x00BFEA0E.
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the filters / Filters / a CThingSearchTools_Calls stand-in, which bound the GetAllThingMapsInArea / ClipAndConvertWorldMapAreaToLocalArea stand-ins of CThingSearchTools_Calls and its own stand-in self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0080E8E0 (std::list, ROUGHAREA_EGOD; 0x0080E8E0 is the same TU) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the PDB-template spelling (Ego_r.pdb 0x0080A0C0, FableWin.pdb; anonymous-namespace filter: the Leaf is cut at ?A0x).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#include <stddef.h>
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsNotList.h"
#include "stlport/fable_containers.h"
// Kept from the landed rows (no shared header for these classes): CThingFilter_IsWithinArea as the landed TU-mate 0x0080E8E0 declares it;
// CThingFilter_IsEvadeExplosion stays in the anonymous namespace (as this row had it), spelled `class` over std::unary_function<CThing,bool>
// as its own row 0x0080DC90 declares it (both PDBs mangle VCThingFilter_IsEvadeExplosion@?A0x..; unary_negate needs argument_type):
class CThingFilter_IsWithinArea {
public:
    bool operator()(const CThing& thing) const;   // 0x007FD830 (PDB ??RCThingFilter_IsWithinArea@@QBE_NABVCThing@@@Z)
};
namespace { class CThingFilter_IsEvadeExplosion : public std::unary_function<CThing,bool> { public:C3DVector* StartPos;C3DVector* EndPos;bool operator()(const CThing&)const;}; } // 0x0080dc90
template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase,std::list<CThingCreatureBase*>,CFilter_Bind4<CThingFilter_IsNot,CThingFilter_IsNotList,CThingFilter_IsWithinArea,std::unary_negate<CThingFilter_IsEvadeExplosion> > >(const C2DBoxI&,const CFilter_Bind4<CThingFilter_IsNot,CThingFilter_IsNotList,CThingFilter_IsWithinArea,std::unary_negate<CThingFilter_IsEvadeExplosion> >&,std::list<CThingCreatureBase*>&) const;
