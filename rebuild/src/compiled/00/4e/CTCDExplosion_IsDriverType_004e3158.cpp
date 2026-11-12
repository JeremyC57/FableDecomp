// CTCDExplosion::IsDriverType(long) const -- true when the asked driver type is this driver's type (27)
// Leaf: ?IsDriverType@CTCDExplosion@@UBE_NJ@Z
// Identity: slot 28 of CTCDExplosion's vtable (the first virtual CTCDriverBase adds after CTCBase's 28 slots; engine/CTCDriverBase.h
//   FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE, the rough-area scans `push type ; call [edx+70h]`); Ego_r/FableWin name set:
//   ?IsDriverType@CTCDExplosion@@UBE_NJ@Z; the body compares its one argument with a constant (`sete al`, `ret 4`).
// Model: engine/CTCDExplosion.h (opt-in FABLE_RETAIL_CTCDEXPLOSION_DRIVERSLOTS).
// Header: prelude as in the landed row CTCDExplosion_HasFrameUpdate_004e3165.cpp.
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCDEXPLOSION_DRIVERSLOTS
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
bool CTCDExplosion::IsDriverType(long type) const
{
    return type == 27;
}
