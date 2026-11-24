// Leaf: ?GetStateGroupType@CAIStateGroup_BriarRose@@UBE?AW4EAIStateGroupType@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CAIStateGroup_BriarRose.h (class-model switch FABLE_RETAIL_BRIAR_ROSE; was ?GetStateGroupType@CAIStateGroup_BriarRose_Methods@@QAEHXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CAIStateGroupBriarRose_Update_00956e70.cpp.
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#include "engine/EAIAction.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAIStateGroup_BriarRose.h"
#include "engine/CAIBrain.h"
#include "engine/CBriarRoseDef.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CCreatureAction_BriarRoseIntoInvulnerable.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
EAIStateGroupType CAIStateGroup_BriarRose::GetStateGroupType() const { return (EAIStateGroupType)1; }
