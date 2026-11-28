// Leaf: ?EntityUnsheatheWeapons@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Flags:
#define FABLE_RETAIL_ENTITY_UNSHEATHE_OLD_BINDER_PASS30
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_PHYSICS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTCCOMBAT_ISTOBLOCK_PASS30
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CScriptThing.h"
#include "engine/CWorld.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCombatManager.h"
#include "engine/CGameScriptInterface.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/ThingAlive.h"
#include "fable_assert.h"

void CGameScriptInterface::EntityUnsheatheWeapons(const CScriptThing& thing, bool flag) const
{
    FABLE_RETAIL_ASSERT(thing.IsAlive());
    if (thing.IsAlive()) {
        CThing* pthing = thing.GetPThing();
        FABLE_RETAIL_ASSERT(IS_THING_ALIVE(pthing));
        if (pthing->GetTypeFlags() & 8) {
            CThingCreatureBase* pcreature = GFDebugPDynamicCast(pthing,(CThingCreatureBase*)0);
            if (!pcreature->IsPlayerCharacter()) {
                pcreature->FinishCurrentAction();
                if (!flag)
                    pcreature->UnsheatheAllWeapons();
                else
                    World->GetCombatManager().CreatureUnsheathWeapon(*pcreature,UNSHEATHE_ALL_WEAPONS);
            } else {
                pcreature->UnsheathePrimaryWeapon();
            }
        }
    }
}
