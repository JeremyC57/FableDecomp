// 00446d10 CGamePlayerInterface::IsControlInputSharedBetweenGameActions
// Leaf: ?IsControlInputSharedBetweenGameActions@CGamePlayerInterface@@QBE_NABV?$vector@W4EGameAction@@V?$allocator@W4EGameAction@@@std@@@std@@@Z
// Flags: /Gr
#define FABLE_RETAIL_SHARED_CONTROLS_PASS29
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND_BODY
#include "engine/EGameAction.h"
#include "engine/CInputTypeBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CVectorMap.h"
#include "engine/CGamePlayerInterface.h"

bool CGamePlayerInterface::IsControlInputSharedBetweenGameActions(const std::vector<EGameAction>& game_actions) const
{
    std::vector<CCountedPointer<const CInputTypeBase> > inputs;
    for (std::vector<EGameAction>::const_iterator it=game_actions.begin();it!=game_actions.end();it++) {
        EGameAction game_action=*it;
        CVectorMap<EGameAction,CCountedPointer<const CInputTypeBase> >::const_iterator it_upper=Controls.UpperBound(game_action);
        for (CVectorMap<EGameAction,CCountedPointer<const CInputTypeBase> >::const_iterator it2=Controls.LowerBound(game_action);it2!=it_upper;it2++) {
            const CCountedPointer<const CInputTypeBase>& pinput=it2->second;
            for (std::vector<CCountedPointer<const CInputTypeBase> >::iterator it3=inputs.begin();it3!=inputs.end();it3++) {
                if (**it3==*pinput) return true;
            }
            inputs.push_back(pinput);
        }
    }
    return false;
}