// 006c1ed0 std::copy_if
// Leaf: ??$copy_if@U?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Const_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@V?$back_insert_iterator@V?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@@2@VHasBuildingAsHomeOrWork@@@std@@YI?AV?$back_insert_iterator@V?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@@0@U?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Const_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@0@0V10@VHasBuildingAsHomeOrWork@@@Z
// Flags: /Gr
#define FABLE_RETAIL_HASBUILDINGASHOMEORWORK_PASS26
#define FABLE_RETAIL_HASBUILDINGASHOMEORWORK_SIZE_INSTANCE_PASS26
#define FABLE_RETAIL_STLP_CREATURE_LIST_PUSHBACK_SIZE_INSTANCE_PASS26
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_BUILDING_GETTERS_PASS26
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#include "fable_string.h"
#include "engine/HasBuildingAsHomeOrWork.h"
#include "stlport/fable_creature_pointer_list.h"


namespace std {
template<class Input, class Output, class Predicate>
Output FABLE_FASTCALL copy_if(Input first, Input last, Output result, Predicate pred)
{
    for (; first != last; ++first) {
        if (pred(*first)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}
typedef list<CIntelligentPointer<CThingAICreature> > CreatureList;
template back_insert_iterator<CreatureList> FABLE_FASTCALL copy_if(CreatureList::const_iterator,
    CreatureList::const_iterator, back_insert_iterator<CreatureList>, HasBuildingAsHomeOrWork);
}