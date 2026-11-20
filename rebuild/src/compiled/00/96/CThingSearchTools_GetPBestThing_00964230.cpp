// CThingSearchTools::GetPBestThing<CFilter_Bind2<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint >, CThingCompare_Nearest, CThingObject>(const CFilter_Bind2<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint >&, const CThingCompare_Nearest&, CThingObject**) -- nearest scan-visible object passing both bound filters
// Leaf: ??$GetPBestThing@V?$CFilter_Bind2@VCThingFilter_ObjectHasProperty@@VCThingFilter_HasFreeActionPoint@@@@VCThingCompare_Nearest@@VCThingObject@@@CThingSearchTools@@QBE_NABV?$CFilter_Bind2@VCThingFilter_ObjectHasProperty@@VCThingFilter_HasFreeActionPoint@@@@ABVCThingCompare_Nearest@@PAPAVCThingObject@@@Z
// Naming (naming wave 23): the search templates call the filter on a reference (`filter(*p)`); CFilter_Bind2/3 hold their filters by pointer (Ego_r.pdb F1/F2/F3) and forward the reference; each CThingFilter_X::operator() takes its PDB `const T&` (DECISIONS_W22_RESOLVED item 2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964230, length 160 bytes (`ret 0Ch`).
// Callees: PeekAllThings() 0x0049C770, CThingFilter_ObjectHasProperty::operator() 0x006A7590 (manifest: CThingFilter_MarkerHasProperty), CThingFilter_HasFreeActionPoint::operator() 0x006A8B40, CThingCompare_Nearest::operator()<CThingObject> 0x00916B10.
// Model: CThingSearchTools from engine/CThingSearchTools.h (this is only passed through to the list
// getter); CThing is a local retail-layout model (see below; retail sits 0x10 below Ego_r.pdb from
// ThingTypeFlags on). The thing type is selected by its THING_TYPE mask (TThing::ThingTypeMask).
// NF170 (q11782, 2026-11-20): re-authored on the shared CThingSearchTools::GetPBestThing<Filter,Compare,T> const member template
//   (Ego_r.pdb 0x00988B00 + FableWin.pdb 0x02C3B050; engine/CThingSearchTools.h BESTTHING_BODY), engine/CFilter_Bind2.h,
//   engine/CThingCompare_Nearest.h and engine/CThingObject.h (StatGetTypeFlags). Its only landed caller 0x00964360 binds this spelling
//   and emits the same pick-any COMDAT. The two filters have no shared header: the one-word / empty local classes of that caller.
//   Was a local CThingSearchToolsMethods stand-in (QAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingCompare_Nearest.h"

class CThingFilter_ObjectHasProperty { public:         // one word: the property mask (no shared header; as the caller 0x00964360)
    unsigned long Property;
    CThingFilter_ObjectHasProperty(unsigned long property) : Property(property) {}
    bool operator()(const CThingObject& thing) const;   // 0x006A7590
};
class CThingFilter_HasFreeActionPoint { public:        // empty filter (no shared header; as the caller 0x00964360)
    CThingFilter_HasFreeActionPoint() {}
    bool operator()(const CThing& thing) const;         // 0x006A8B40
};
typedef CFilter_Bind2<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint> CStealableFilter;
namespace {
// Fidelity: scaffolding, not retail: takes the address of the shared GetPBestThing<CFilter_Bind2<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint>, CThingCompare_Nearest, CThingObject> so this object emits it out of line as a pick-any
// COMDAT (retail 0x00964230 is that out-of-line copy). Internal to this TU; never called.
bool (CThingSearchTools::* ReferenceGetPBestThingStealable())(const CStealableFilter&, const CThingCompare_Nearest&, CThingObject**) const
{
    return &CThingSearchTools::GetPBestThing<CStealableFilter, CThingCompare_Nearest, CThingObject>;
}
}
