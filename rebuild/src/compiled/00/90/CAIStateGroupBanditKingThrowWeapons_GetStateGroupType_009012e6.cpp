// Leaf: ?GetStateGroupType@CAIStateGroup_BanditKingThrowWeapons@@MBE?AW4EAIStateGroupType@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CAIStateGroup_BanditKingThrowWeapons.h (class-model switch FABLE_RETAIL_CAISTATEGROUP_BANDITKINGTHROWWEAPONS; was ?GetStateGroupType@CAIStateGroup_BanditKingThrowWeapons_Methods@@QAEHXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CAIStateGroupBanditKingThrowWeapons_Update_00961640.cpp.
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_TARGETFACE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITKINGTHROWWEAPONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETHEALTH
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGTHROWWEAPONS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroup_BanditKingThrowWeapons.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCBoss.h"
#include "engine/CCreatureAction_BanditKingThrowWeapons.h"
#include "engine/CCreatureAction_BanditKingChangeWeapons.h"
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
EAIStateGroupType CAIStateGroup_BanditKingThrowWeapons::GetStateGroupType() const { return (EAIStateGroupType)1; }
