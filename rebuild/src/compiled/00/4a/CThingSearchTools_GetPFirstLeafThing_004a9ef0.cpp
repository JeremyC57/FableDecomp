// CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<CThingFilter_HasTC,CThingFilter_InRegion>,CThing>(const CFilter_Bind2<..>&, CThing**) const -- the first scan-visible thing of type list 15 that passes both bound filters
// Leaf: ??$GetPFirstLeafThing@V?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_InRegion@@@@VCThing@@@CThingSearchTools@@QBE_NABV?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_InRegion@@@@PAPAVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004A9EF0 (`ret 8`).
// Identity: the template instance its only landed caller CWorld::UpdateRegionLoad 0x004A3740 binds (Ego_r.pdb template family
//   ??$GetPFirstLeafThing@..@CThingSearchTools@@QBE_N..; CThingSearchTools.h body); the type list key 15 is CThing::StatGetType.
// Callees: CThingSearchTools::PeekTypeList(long) 0x0049C780, CThingFilter_HasTC::operator() 0x006A80A0 (Ego_r 0x005A3E00 / FableWin
//   0x01F95560 ??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z), CThingFilter_InRegion::operator() 0x006A7960 (Ego_r 0x005A3770 / FableWin
//   0x01F951D0 ??RCThingFilter_InRegion@@QBE_NABVCThing@@@Z), both out of line; CThing::IsIncludedInThingScans expanded in place.
// NF122 (2026-11-20, q6929): re-authored on the shared headers from the free __fastcall stand-in (local Thing / Node / Filter / List
//   models), which bound 0x006A80A0 / 0x006A7960 as ??R..@@QBE_NPAUThing@@@Z. CThingFilter_InRegion has no shared header: modelled as
//   the landed caller 0x004A3740 models it (Ego_r layout: WorldMap reference, Region).
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATGETTYPE
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasTC.h"
class CWorldMap;
class CThingFilter_InRegion : public std::unary_function<CThing, bool> {
public:
    CThingFilter_InRegion(const CWorldMap& world_map, long region) : WorldMap(world_map), Region(region) { }
    bool operator()(const CThing& thing) const;                          // 0x006A7960
private:
    const CWorldMap& WorldMap;
    long             Region;
};
template bool CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InRegion>, CThing>(
    const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InRegion>&, CThing**) const;
