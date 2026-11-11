// CTCDExplosion::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDExplosion@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3165, thiscall, `ret`.
// Identity: slot 8 of CTCDExplosion's vtable 0x01242A54 (defined here: the slot holds this body in 1 vtable(s), all of CTCDExplosion or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDExplosion@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCDExplosion.h (opt-ins as in the landed row CThingCreatureBase_ReactToGenericStrike_00667450.cpp).
#define FABLE_RETAIL_CTCDEXPLOSION_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETODECAPITATION
#define FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETOEXPLOSION
#define FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CHitParameters.h"
#include "engine/CTCDExplosion.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/CCombatAction_GenericResponseToDecapitation.h"
#include "engine/CCombatAction_GenericResponseToDrain.h"
#include "engine/CCombatAction_GenericResponseToExplosion.h"
#include "engine/CCombatAction_GenericStrikeResponse.h"
#include "engine/CCombatAction_GenericStrikeResponseKnockdown.h"
#include "engine/CDefinitionManager.h"
bool CTCDExplosion::HasFrameUpdate() const
{
    return true;
}
