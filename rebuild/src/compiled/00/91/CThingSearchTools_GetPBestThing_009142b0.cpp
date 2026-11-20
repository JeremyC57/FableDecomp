// CThingSearchTools::GetPBestThing<CThingFilter_None, CThingCompare_Nearest, CThingDeadCreature>(const CThingFilter_None&, const CThingCompare_Nearest&, CThingDeadCreature**) -- nearest scan-visible dead creature (no filter)
// Leaf: ??$GetPBestThing@VCThingFilter_None@@VCThingCompare_Nearest@@VCThingDeadCreature@@@CThingSearchTools@@QBE_NABVCThingFilter_None@@ABVCThingCompare_Nearest@@PAPAVCThingDeadCreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009142B0, length 128 bytes (`ret 0Ch`).
// Callees: PeekAllThings() 0x0049C770, CThingCompare_Nearest::operator()<CThingDeadCreature> 0x00914130.
// Model: CThingSearchTools from engine/CThingSearchTools.h (this is only passed through to the list
// getter); CThing is a local retail-layout model (see below; retail sits 0x10 below Ego_r.pdb from
// ThingTypeFlags on). The thing type is selected by its THING_TYPE mask (TThing::ThingTypeMask).
// NF170 (q11813, 2026-11-20): re-authored on the shared CThingSearchTools::GetPBestThing<Filter,Compare,T> const member template
//   (Ego_r.pdb 0x00960DD0 + FableWin.pdb 0x02B8CA30; engine/CThingSearchTools.h BESTTHING_BODY), engine/CThingFilter_None.h,
//   engine/CThingCompare_Nearest.h and engine/CThingDeadCreature.h (StatGetTypeFlags). Its only landed caller 0x00913F30 binds this
//   spelling. Was a local CThingSearchToolsMethods stand-in (QAE) over a local CThing model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGDEADCREATURE_STATTYPEFLAGS
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
#include "engine/CThingCreatureBase.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CThingCompare_Nearest.h"
namespace {
// Fidelity: scaffolding, not retail: takes the address of the shared GetPBestThing<CThingFilter_None, CThingCompare_Nearest, CThingDeadCreature> so this object emits it out of line as a pick-any
// COMDAT (retail 0x009142B0 is that out-of-line copy). Internal to this TU; never called.
bool (CThingSearchTools::* ReferenceGetPBestThingDeadCreature())(const CThingFilter_None&, const CThingCompare_Nearest&, CThingDeadCreature**) const
{
    return &CThingSearchTools::GetPBestThing<CThingFilter_None, CThingCompare_Nearest, CThingDeadCreature>;
}
}
