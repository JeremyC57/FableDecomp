// Leaf: ?IsPreventingLookTurning@CCreatureAction_UseObject@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_USEOBJECT
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_UseObject.h"
bool CCreatureAction_UseObject::IsPreventingLookTurning() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_UseObject_IsPreventingLookTurning -> CCreatureAction_UseObject::IsPreventingLookTurning, the RTTI-placed PDB member; class model: engine/CCreatureAction_UseObject.h under FABLE_RETAIL_CCREATUREACTION_USEOBJECT (preamble of CCreatureActionUseObject_Clone_006a0d20.cpp).
