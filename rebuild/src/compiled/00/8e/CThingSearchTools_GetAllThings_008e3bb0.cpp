// CThingSearchTools::GetAllThings(const TFilter& filter, TContainer& result) [TThing = CThingAICreature, TContainer = std::vector<CThingAICreature*>, TFilter = CThingFilter_IsNot] -- append every scan-visible AI creature other than the filter's thing to the result vector
// Leaf: ??$GetAllThings@VCThingAICreature@@V?$vector@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@VCThingFilter_IsNot@@@CThingSearchTools@@QBEXABVCThingFilter_IsNot@@AAV?$vector@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3BB0, 147 bytes (+ int3 padding to 160) (`ret 8`).
// Callees: PeekAllThings 0x0049C770, CThingFilter_IsNot::operator() 0x006A76A0, vector::_M_insert_overflow 0x00515CC0.
// Model: CThingSearchTools from engine/CThingSearchTools.h (`this` is only passed through to the list getter);
// CThing, the thing lists, the result container and the filters are local retail-layout models.
// Shape: scan test, then the type-mask test (mask copied into a local), then the filter on the thing reference.
// A vector push takes a by-value ThingCast<T> temporary (homed in the dead argument slot and shared with the
// __true_type tag: the same-TU noinline overflow stand-in shows VC7.1 the tag is never read); a list push
// inserts before end() reading end()->prev once; an intelligent-pointer push builds a named temporary.
// NF170 (q11742, 2026-11-20): re-authored on the shared CThingSearchTools::GetAllThings<T,Container,Filter> const member template
//   (Ego_r.pdb 0x00825CE0 + FableWin.pdb 0x0286CD30 ??$GetAllThings@VCThingAICreature@@V?$vector@...@std@@VCThingFilter_IsNot@@@CThingSearchTools@@QBEX...;
//   engine/CThingSearchTools.h ALLTHINGSBODY with the Ego_d.pdb text ALLTHINGSBODY_EGOD, the body of the same-T / same-container instance
//   0x0052B270 GetAllThings<CThingAICreature, vector<CThingAICreature*>, CThingFilter_None>), std::vector<CThingAICreature*> and
//   engine/CThingFilter_IsNot.h. Its only landed caller 0x008DCB60 binds this spelling. Was a local CThingSearchToolsMethods / CResultVector
//   stand-in (QAE). The row takes the instance's address in an anonymous-namespace helper, so it emits the one pick-any COMDAT its
//   header includers emit too (GOTCHAS nf86 / nf88; landing nf135).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
namespace {
// Fidelity: scaffolding, not retail: takes the address of the shared GetAllThings<CThingAICreature, vector<CThingAICreature*>,
// CThingFilter_IsNot> so this object emits it out of line as a pick-any COMDAT (retail 0x008E3BB0 is that out-of-line copy).
// Internal to this TU; never called.
void (CThingSearchTools::* ReferenceGetAllThingsCThingAICreatureIsNot())(const CThingFilter_IsNot&, std::vector<CThingAICreature*>&) const
{
    return &CThingSearchTools::GetAllThings<CThingAICreature, std::vector<CThingAICreature*>, CThingFilter_IsNot>;
}
}
