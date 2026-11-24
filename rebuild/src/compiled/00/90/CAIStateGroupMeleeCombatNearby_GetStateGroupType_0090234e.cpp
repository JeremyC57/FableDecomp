// Leaf: ?GetStateGroupType@CAIStateGroup_MeleeCombatNearby@@MBE?AW4EAIStateGroupType@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CAIStateGroup_MeleeCombatNearby.h (class-model switch FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE; was ?GetStateGroupType@CAIStateGroup_MeleeCombatNearby_Methods@@QAEHXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CAIStateGroupMeleeCombatNearby_MoveToEnemyCreatureCombatPosition_0096ec10.cpp.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
#define FABLE_RETAIL_CCOMBATMANAGER_OUTERRING
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCombatManager.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyBase.h"
#include "engine/CAIStateGroup_MeleeCombatNearby.h"
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
EAIStateGroupType CAIStateGroup_MeleeCombatNearby::GetStateGroupType() const { return (EAIStateGroupType)1; }
