// 00661f90 CThingCreatureBase::GetCombatRadius
// Leaf: ?GetCombatRadius@CThingCreatureBase@@QBEMXZ
// Flags: /O2 /Oy
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATRADIUS
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCPhysicsBase.h"

float CThingCreatureBase::GetCombatRadius() const
{
    float combat_radius = PeekDefinition().CombatDiameter * 0.5f;
    if (combat_radius > 0.01f)
        return combat_radius;
    return GetPhysicsTC()->GetRadius();
}
