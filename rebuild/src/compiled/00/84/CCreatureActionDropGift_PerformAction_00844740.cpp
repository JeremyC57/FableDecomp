// 00844740 CCreatureAction_DropGift::PerformAction
// Leaf: ?PerformAction@CCreatureAction_DropGift@@UAEXXZ
// Flags:
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_PBASEDEF
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCBASE
// Inline-budget: original 00844764 calls GetSubDef; its visible shared body also matches 00685C00.
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD
#define FABLE_RETAIL_CCREATUREACTION_DROPGIFT_PASS27
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_COPINIONOFHERODEF_TYPENAME_PASS27
#include "engine/GFDebugPDynamicCast.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureAction_DropGift.h"
#include "engine/CThingObject.h"
#include "engine/CDefinitionManager.h"
#include "engine/COpinionOfHeroDef.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/GFSin.h"
#include "engine/GFFloatRandom.h"

void CCreatureAction_DropGift::PerformAction()
{
    CDefPointer<const COpinionOfHeroDef> pop_def;
    if (PCreature->PeekBaseDef().GetSubDef(&pop_def)) {
        long def_index = pop_def->ThreatenGift;
        C3DVector position = PCreature->GetPos();
        float angle = GFFloatRandom(1.0f, GetWorld().GetWorldSeed());
        float xy_ampli = 0.25f;
        C3DVector acceleration((float)(xy_ampli * GFSin(angle)), (float)(xy_ampli * GFSin(angle)), 1.0f);
        CThingObject* new_thing = CThingObject::Create(def_index, position, 5, 0, 0, "");
        if (new_thing) {
            new_thing->GetPhysicsTC()->ApplyAcceleration(acceleration);
            float xy = GFFloatRandom(1.0f, PeekWorld().GetWorldSeed());
            new_thing->GetPhysicsTC()->SetFacingAngleXY(xy);
            CTCSolidToThing* tc_solid_to_thing = GFDebugPDynamicCast(new_thing->AddTC("CTCSolidToThing", false, 0), (CTCSolidToThing*)0);
            tc_solid_to_thing->AddThingNotSolidTo(PCreature.Get());
        }
    }
}