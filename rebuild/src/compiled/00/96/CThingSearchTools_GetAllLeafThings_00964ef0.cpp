// CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& things) [TThing = CThingMarker, TContainer = vector<CThingMarker*>, TFilter = CThingFilter_MarkerHasProperty] -- append every scan-visible marker (type list 0x0b) that has the filter's property
// Leaf: ??$GetAllLeafThings@VCThingMarker@@V?$vector@PAVCThingMarker@@V?$allocator@PAVCThingMarker@@@std@@@std@@VCThingFilter_MarkerHasProperty@@@CThingSearchTools@@QBEXABVCThingFilter_MarkerHasProperty@@AAV?$vector@PAVCThingMarker@@V?$allocator@PAVCThingMarker@@@std@@@std@@@Z
// Naming (naming wave 23): the search templates call the filter on a reference (`filter(*p)`); CFilter_Bind2/3 hold their filters by pointer (Ego_r.pdb F1/F2/F3) and forward the reference; each CThingFilter_X::operator() takes its PDB `const T&` (DECISIONS_W22_RESOLVED item 2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964EF0, length 136 bytes (`ret 8`).
// Callees: PeekTypeList(long) 0x0049C780, CThingFilter_MarkerHasProperty::operator() 0x006A75B0, vector::_M_insert_overflow 0x0062E190.
// Model: CThingSearchTools from engine/CThingSearchTools.h (only passed through to the list getter); CThing, the thing list and
//   the result container are local retail-layout models (retail CThing sits 0x10 below Ego_r.pdb from ThingTypeFlags on; the
//   state byte +0x91 = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20).
// Shape: the IsIncludedInThingScans test is inlined (`not cl ; test cl,1`); a vector push_back of an rvalue `(T*)thing` lands the value in the dead container argument slot,
//   and the same-TU noinline _M_insert_overflow stand-in lets VC7.1 home the __true_type tag in the filter argument slot.
// Identity: the catalog label <CThingAICreature, ..., CFilter_Bind2<...>> is propagated; PeekTypeList(0x0b), the single marker filter and
//   the caller CAIStateGroup_StealStealableItem::FindPEscapePoint make this the marker instance.
// NF170 (q11814, 2026-11-20): re-authored on the shared CThingSearchTools::GetAllLeafThings<T,Container,Filter> const member template
//   (Ego_r.pdb 0x009898C0 + FableWin.pdb 0x02C3B370; engine/CThingSearchTools.h LEAFBODY), std::vector<CThingMarker*> (the STLport
//   header's _M_insert_overflow, no row-local specialization) and engine/CThingMarker.h. Its only landed caller 0x00964CA0 binds this
//   spelling. CThingFilter_MarkerHasProperty has no shared header: the one-word local class of that caller. Was a local
//   CThingSearchToolsMethods stand-in (QAE) with a strong vector<CThingMarker*>::_M_insert_overflow specialization.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGMARKER_STATTYPEFLAGS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingMarker.h"
#include "engine/CThingSearchTools.h"

enum EMarkerProperty;                                   // MS forward enum (FableWin ??0CThingFilter_MarkerHasProperty@@QAE@W4EMarkerProperty@@@Z)
class CThingFilter_MarkerHasProperty { public:         // one word: the marker property (no shared header; as the caller 0x00964CA0)
    EMarkerProperty Property;
    CThingFilter_MarkerHasProperty(EMarkerProperty property) : Property(property) {}
    bool operator()(const CThingMarker& thing) const;   // 0x006A75B0
};
namespace {
// Fidelity: scaffolding, not retail: takes the address of the shared GetAllLeafThings<CThingMarker, vector<CThingMarker*>, CThingFilter_MarkerHasProperty> so this object emits it out of line as a pick-any
// COMDAT (retail 0x00964EF0 is that out-of-line copy). Internal to this TU; never called.
void (CThingSearchTools::* ReferenceGetAllLeafThingsMarker())(const CThingFilter_MarkerHasProperty&, std::vector<CThingMarker*>&) const
{
    return &CThingSearchTools::GetAllLeafThings<CThingMarker, std::vector<CThingMarker*>, CThingFilter_MarkerHasProperty>;
}
}
