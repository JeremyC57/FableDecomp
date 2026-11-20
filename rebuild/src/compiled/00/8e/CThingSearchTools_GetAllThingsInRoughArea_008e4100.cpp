// 008e4100 CThingSearchTools::GetAllThingsInRoughArea, 603 retail bytes.
// Leaf: ??$GetAllThingsInRoughArea@VCThingAICreature@@V?$vector@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_HasDefGlobalIndex@@VCThingFilter_IsHoldingMeleeWeapon@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E4100.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_HasDefGlobalIndex 0x006A70B0, CTCCarrying::IsThingCarryingMeleeWeapon 0x006A15A0 (the inline anonymous-namespace CThingFilter_IsHoldingMeleeWeapon), vector<CThingAICreature*>::_M_insert_overflow 0x00515CC0.
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the filters / Filters / a CThingSearchTools_Calls stand-in, which bound the GetAllThingMapsInArea / ClipAndConvertWorldMapAreaToLocalArea stand-ins of CThingSearchTools_Calls and its own stand-in self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x008AD710 (vector, family K016, CThingAICreature include set) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the PDB-template spelling (Ego_r.pdb 0x00826350, FableWin.pdb; anonymous-namespace filter: the Leaf is cut at ?A0x).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#include <stddef.h>
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingAICreature.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CTCCarrying.h"
// Kept from the landed row (no shared header for this class), moved into the anonymous namespace and spelled `class` as both PDBs
// mangle it (VCThingFilter_IsHoldingMeleeWeapon@?A0x..); its operator() is inline (retail calls CTCCarrying::IsThingCarryingMeleeWeapon
// 0x006A15A0 directly), now declared by engine/CTCCarrying.h (NF107 opt-in FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON):
namespace { class CThingFilter_IsHoldingMeleeWeapon { public: CThingFilter_IsHoldingMeleeWeapon() { } FABLE_FORCEINLINE bool operator()(const CThing& p)const{return CTCCarrying::IsThingCarryingMeleeWeapon(p);}}; }
template void CThingSearchTools::GetAllThingsInRoughArea<CThingAICreature,std::vector<CThingAICreature*>,CFilter_Bind2<CThingFilter_HasDefGlobalIndex,CThingFilter_IsHoldingMeleeWeapon> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_HasDefGlobalIndex,CThingFilter_IsHoldingMeleeWeapon>&,std::vector<CThingAICreature*>&) const;
