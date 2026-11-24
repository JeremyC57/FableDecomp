// Leaf: ?IsToEvadeAllHits@CCreatureActionBase@@UBE_NXZ
// Switch route (flatten pilot --switch): the class model is engine/CCreatureActionBase.h under FABLE_RETAIL_CCREATUREACTIONBASE; preamble copied from the landed precedent CCreatureActionBase_RegisterActionEventTypes_006932b0.cpp.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
#include "engine/NCreatureActionEvents.h"

bool CCreatureActionBase::IsToEvadeAllHits() const
{
    return false;
}
