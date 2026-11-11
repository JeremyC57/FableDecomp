// CTCFlashOnHit::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFlashOnHit@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D62CE, thiscall, `ret`.
// Identity: slot 8 of CTCFlashOnHit's vtable 0x0123E034 (defined here: the slot holds this body in 1 vtable(s), all of CTCFlashOnHit or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFlashOnHit@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCFlashOnHit.h (opt-ins as in the landed row CThingCreatureBase_OnHit_00667d30.cpp).
#define FABLE_RETAIL_CTCFLASHONHIT_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERMANAGER
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CHitParameters.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCFlashOnHit.h"
#include "engine/CTCTalk.h"
#include "engine/CTCScriptedControl.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHaste.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCCombat.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CDefinitionManager.h"
bool CTCFlashOnHit::HasFrameUpdate() const
{
    return true;
}
