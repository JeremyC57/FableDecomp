// Leaf: ??$__find@PAPBVCThing@@PAVCThingCreatureBase@@@std@@YIPAPBVCThing@@PAPBV1@0ABQAVCThingCreatureBase@@ABUrandom_access_iterator_tag@0@@Z
// Flags: /Gr
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include "engine/CThingCreatureBase.h"
#include "stlport/fable_algorithm.h"
namespace std {
template const CThing** FABLE_FASTCALL __find<const CThing**, CThingCreatureBase*>(
    const CThing**, const CThing**, CThingCreatureBase* const&, const random_access_iterator_tag&);
}
