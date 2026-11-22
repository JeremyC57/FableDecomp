// 0088bf30 CAIBrain::InitStateGroups
// Leaf: ?InitStateGroups@CAIBrain@@AAEXV?$CDefPointer@$$CBVCBrainDef@@@@@Z
// Flags: /d2inlT35
// Inline-budget: at 0088BFF5 and 0088C05F the original string equality expands but Compare is still called.
#define FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26
#define FABLE_RETAIL_CAIBRAIN_STATEGROUPINFO
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
// Inline-budget: native vector overflow at 0088CC20 expands both range copies and the fill loop; its only outside calls are allocation and release.
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#include "engine/CDefPointer.h"
#include "engine/CBrainDef.h"
#include "engine/CAIStateGroupInfo.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
void CAIBrain::InitStateGroups(CDefPointer<const CBrainDef> pbrain_def)
{
    CAIStateGroupBase* state_group;
    if (pbrain_def.Get()) {
        long max_groups = pbrain_def->Behaviours.size();
        for (long c1 = 0; c1 < max_groups; ++c1) {
            CCharString behaviour_name = pbrain_def->Behaviours[c1].Name.GetString();
            if (!behaviour_name.IsEmpty()) {
                bool disabled = false;
                for (long c2 = 0; c2 < pbrain_def->DisabledBehaviours.size(); ++c2) {
                    if (behaviour_name == pbrain_def->DisabledBehaviours[c2]) {
                        disabled = true;
                        break;
                    }
                }
                if (!disabled) {
                    for (long c3 = 0; StateGroupInfo[c3].AllocFunc; ++c3) {
                        if (StateGroupInfo[c3].StateGroupName == behaviour_name) {
                            state_group = StateGroupInfo[c3].AllocFunc();
                            state_group->Initialise(*this, Creature, pbrain_def->Behaviours[c1].Priority,
                                (EScriptAIPriority)pbrain_def->Behaviours[c1].ScriptAIPriority, StateGroupInfo[c3].StateGroupName);
                            StateGroupList.Insert(state_group->GetPriority(), state_group);
                            break;
                        }
                    }
                }
            }
        }
    }
    StateGroupList.Update();
}