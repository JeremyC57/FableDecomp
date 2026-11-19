// CThingSearchTools::ForEachInRoughArea<CThingPhysical, CThingFilter_None, bool (__fastcall*)(CThingPhysical&)>(const C2DBoxI& area,
//   const CThingFilter_None& filter_func, bool (__fastcall* const& op_func)(CThingPhysical&)) const -- calls op_func on every physical
//   thing linked into a mapwho cell overlapping the area (each thing once per search: the search stamp), over every overlapping thing map
//   and every mapwho level; true when any call returned true
// Leaf: ??$ForEachInRoughArea@VCThingPhysical@@VCThingFilter_None@@P6I_NAAV1@@Z@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_None@@ABQ6I_NAAVCThingPhysical@@@Z@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00518130, 526 bytes, __thiscall, `ret 0Ch`. Name: Ego_r.pdb 0x004E3E00 / FableWin.pdb 0x01CC9960
//   ??$ForEachInRoughArea@VCThingPhysical@@VCThingFilter_None@@P6I_NAAV1@@Z@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_None@@ABQ6I_NAAVCThingPhysical@@@Z@Z
//   (const member, the filter by const reference); the landed caller 0x004FF920 CWorldMap::Update binds this spelling.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0 (protected const), ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   memmove 0x00BFEAE6 (the AreaMaps clear).
// Port (NF101, 2026-11-19, q11280 / q11283): the landed row's dozen-class local CThingSearchTools model (a GetMapsInArea stand-in for
//   0x0049C6F0, a non-const by-value-filter self leaf) replaced by an explicit instantiation of the shared engine/CThingSearchTools.h
//   ForEachInRoughArea body (FOREACHINROUGHAREA_EGOD), the define set of the landed 0x0066A1E0 instance.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "fable_compiler.h"
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_None.h"

template bool CThingSearchTools::ForEachInRoughArea<CThingPhysical,CThingFilter_None,bool (__fastcall*)(CThingPhysical&)>(const C2DBoxI&,const CThingFilter_None&,bool (__fastcall* const&)(CThingPhysical&)) const;
