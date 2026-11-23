// Leaf: ?AllowPriorityOverlap@CAIStateGroupBase@@UAE_NXZ
// Switch route (flatten pilot --switch): the class model is engine/CAIStateGroupBase.h under FABLE_RETAIL_CAISTATEGROUPBASE, FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS; preamble copied from the landed precedent CAIStateGroupBase_Terminate_008fcf40.cpp.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"

bool CAIStateGroupBase::AllowPriorityOverlap()
{
    return false;
}
