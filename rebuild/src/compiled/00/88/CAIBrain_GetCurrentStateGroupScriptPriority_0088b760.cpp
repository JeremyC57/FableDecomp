// 0088b760 CAIBrain::GetCurrentStateGroupScriptPriority
// Leaf: ?GetCurrentStateGroupScriptPriority@CAIBrain@@QBE?AW4EScriptAIPriority@@XZ
// Identity: Ego_r.pdb 0x0082F530 + FableWin.pdb 0x02804180 (public const, returns EScriptAIPriority); its only landed caller,
// CThingAICreature::GetCurrentStateGroupScriptPriority 0x006D5AD0, tail-jumps here and binds this spelling (NF118).
// Body: the active state group's ScriptAIPriority (+0x08) unless there is none or it is Terminated (+0x10); else the lowest
// priority (0). Layout: engine/CAIBrain.h PActiveStateGroup +0x38, engine/CAIStateGroupBase.h retail members.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAIBRAIN_GETCURRENTSTATEGROUPSCRIPTPRIORITY
#include "fable_compiler.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };

EScriptAIPriority CAIBrain::GetCurrentStateGroupScriptPriority() const
{
    return (PActiveStateGroup && !PActiveStateGroup->Terminated) ? PActiveStateGroup->ScriptAIPriority : SCRIPT_AI_PRIORITY_LOWEST;
}
