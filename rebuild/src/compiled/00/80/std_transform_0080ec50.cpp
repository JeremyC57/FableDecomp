// Leaf: ??$transform@PAV?$CIntelligentPointer@VCThing@@@@V?$back_insert_iterator@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@std@@VCGetThingFromIntelligentPointer@?A0x
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
#define FABLE_RETAIL_EXPLOSION_HITS_PASS29
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_TRANSFORM_PASS30
#include "engine/CThingCreatureBase.h"
#include "engine/tcd_explosion_filters.h"
#include "stlport/fable_algorithm.h"
namespace std {
template back_insert_iterator<vector<const CThing*> > FABLE_FASTCALL transform(
    CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*,
    back_insert_iterator<vector<const CThing*> >, CGetThingFromIntelligentPointer);
}
