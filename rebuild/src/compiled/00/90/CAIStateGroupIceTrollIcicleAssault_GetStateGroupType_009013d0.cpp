// Leaf: ?GetStateGroupType@CAIStateGroup_IceTrollIcicleAssault@@MBE?AW4EAIStateGroupType@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CAIStateGroup_IceTrollIcicleAssault.h (class-model switch FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT; was ?GetStateGroupType@CAIStateGroup_IceTrollIcicleAssault_Methods@@QAEHXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CAIStateGroupIceTrollIcicleAssault_Update_009627c0.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT
#define FABLE_RETAIL_CCREATUREACTION_ICETROLLICICLEASSAULTINTO
#include "rebuild_abi.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCStealth.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_IceTrollIcicleAssaultInto.h"
#include "engine/CAIStateGroup_IceTrollIcicleAssault.h"
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
EAIStateGroupType CAIStateGroup_IceTrollIcicleAssault::GetStateGroupType() const { return (EAIStateGroupType)1; }
