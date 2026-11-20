// CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_IsRelatedTo, const CThingDeadCreature>(const C2DBoxI& area, const CThingFilter_IsRelatedTo& filter_func, const CThingDeadCreature** thing) const -- the first dead creature in the mapwho cells under the area (each thing once per search id) that is related to the filter thing, through the out pointer (true; the scratch map list is left as is); else the scratch list is cleared and false
// Leaf: ??$GetPFirstThingInRoughArea@VCThingFilter_IsRelatedTo@@$$CBVCThingDeadCreature@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_IsRelatedTo@@PAPBVCThingDeadCreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096F260, 549 bytes, thiscall `ret 0xc`; caller 0x0096F180 (CAIStateGroup_MinionBehaviourSad::FoundDeadSpouse; binds a stand-in: NAME_FIXES).
// Identity (the landed row spelled <CThingFilter_IsBlocking, CThingDeadCreature>, a propagated label): Ego_r.pdb 0x00993F70 + FableWin.pdb 0x02C04880 name
//   ??$GetPFirstThingInRoughArea@VCThingFilter_IsRelatedTo@@$$CBVCThingDeadCreature@@...; the body calls 0x006A8880, whose landed row is
//   CThingFilter_IsRelatedTo::operator() (??RCThingFilter_IsRelatedTo@@QBE_NABVCThing@@@Z, Ego_r.pdb 0x005A47C0 + FableWin.pdb 0x01F957A0), the type test
//   is CThingDeadCreature's 0x87, and the caller is the dead-spouse search.
// NF110 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing classes / the thing map / a local CThingSearchTools,
//   which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf) onto the shared headers as an explicit instantiation of the
//   engine/CThingSearchTools.h GetPFirstThingInRoughArea template (NF110 opt-in FIRSTROUGH_EGOD: the landed 0x009871C0 body), on the 0x009871C0
//   define / include set. It now binds the Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... (0x0049C6F0) and
//   ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE... (0x0049C700); the self leaf is the Ego_r.pdb instance spelling.
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
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTHINGDEADCREATURE_STATTYPEFLAGS
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
#include "engine/CThingDeadCreature.h"
// Respelled in place (NF110) from the landed row's local `struct CThingFilter_IsBlocking { bool operator()(const CThing*) const; }` stand-in to the
//   Ego_r.pdb / FableWin.pdb class (no shared header defines it; class key V, operator() out of line at 0x006A8880, as that row spells it):
class CThingFilter_IsRelatedTo { public:
    bool operator()(const CThing& thing) const;   // 0x006A8880
};
template bool CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_IsRelatedTo,const CThingDeadCreature>(const C2DBoxI&,const CThingFilter_IsRelatedTo&,const CThingDeadCreature**) const;
