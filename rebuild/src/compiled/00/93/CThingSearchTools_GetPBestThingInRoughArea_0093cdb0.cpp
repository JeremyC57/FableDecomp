// CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_HasTrackName, CThingCompare_Nearest, CThingTrackNode>(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, const CThingCompare_Nearest& compare, CThingTrackNode** out) const -- the nearest track node in the mapwho cells under the area (each thing once per search id) on the named track, through the out pointer; whether one was found (the scratch map list is cleared)
// Leaf: ??$GetPBestThingInRoughArea@VCThingFilter_HasTrackName@@VCThingCompare_Nearest@@VCThingTrackNode@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_HasTrackName@@ABVCThingCompare_Nearest@@PAPAVCThingTrackNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093CDB0, 560 bytes, thiscall `ret 0x10`; caller 0x0093CC60 (binds a non-template stand-in: NAME_FIXES).
// Identity (the landed row spelled T as a local CThingMask10007): Ego_r.pdb 0x00960510 + FableWin.pdb 0x02B82640 name the <CThingFilter_HasTrackName,
//   CThingCompare_Nearest, CThingTrackNode> instance, the only one of its filter; the body calls 0x006A7F30 (it compares the thing's CCharString at +0xE8 =
//   CThingTrackNode::TrackName: CThingFilter_HasTrackName::operator(), Ego_r.pdb ??RCThingFilter_HasTrackName@@QBE_NABVCThingTrackNode@@@Z 0x005A3D40)
//   and CThingCompare_Nearest::operator()<CThingTrackNode> 0x0093C9A0 out of line; the type test is CThingTrackNode's 0x10007 (NF110 opt-in
//   FABLE_RETAIL_CTHINGTRACKNODE_STATTYPEFLAGS, FableWin.pdb ?StatGetTypeFlags@CThingTrackNode@@SIKXZ).
// NF110 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing classes / the thing map / a local CThingSearchTools,
//   which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf) onto the shared headers as an explicit instantiation of the
//   engine/CThingSearchTools.h GetPBestThingInRoughArea template. It now binds the Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE...
//   (0x0049C6F0) and ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE... (0x0049C700); the self leaf is the Ego_r.pdb instance spelling.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_CTHINGTRACKNODE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CThingTrackNode.h"
// Respelled in place (NF110) from the landed row's local `bool operator()(const CThing*) const` (no shared header defines this filter; class key V,
//   operator() out of line at 0x006A7F30, Ego_r.pdb / FableWin.pdb ??RCThingFilter_HasTrackName@@QBE_NABVCThingTrackNode@@@Z):
class CThingFilter_HasTrackName { public:
    bool operator()(const CThingTrackNode& node) const;   // 0x006A7F30
};
template bool CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_HasTrackName,CThingCompare_Nearest,CThingTrackNode>(const C2DBoxI&,const CThingFilter_HasTrackName&,const CThingCompare_Nearest&,CThingTrackNode**) const;
