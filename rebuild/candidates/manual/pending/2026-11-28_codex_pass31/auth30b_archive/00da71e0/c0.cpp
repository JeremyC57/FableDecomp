// Leaf: ?GuardChatter@CQ_MinionClifftopChaseScript@NScript@@QAEXVCScriptThing@@@Z
// Flags: /O2 /Oy /d2inlS64
// Inline-budget: the loop exit expands the Guard destructor, while the nested dialogue exits call it.
#define FABLE_RETAIL_MINION_GUARD_CHATTER_PASS29
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCHARSTRING
#include "engine/TRetVoid.h"
#include "engine/CQ_MinionClifftopChaseScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long value) { pDSTGame->SetTimer(TimerIndex,value); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long value) const { return GetTime()==value; }
inline CScriptThing::~CScriptThing() {}
inline EAIStateGroupType CScriptThing::GetCurrentStateGroupType() const {
    if (PImp.IsNull()) return AI_STATE_NONE;
    return PImp->GetCurrentStateGroupType();
}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
FABLE_FORCEINLINE NScript::CIsThingAlive::~CIsThingAlive() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CQ_MinionClifftopChaseScript::GuardChatter(CScriptThing Guard)
{
    TRetVoid _Fail_Val;
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
    SetPredicate(CIsThingAlive(Guard));
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    {
    CTimer RegulateFollowStateComment;
    RegulateFollowStateComment = 0;
    long PreviousAIState = AI_STATE_NONE;
    long CurrentAIState = Guard.GetCurrentStateGroupType();
    if (false) {} else for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            break;
        if (CurrentAIState != PreviousAIState) {
            if (IsActiveThreadTerminating())
                goto terminated;
            MakeUniqueComment = 25;
            if (RegulateFollowStateComment == 0) {
                if (IsActiveThreadTerminating())
                    goto terminated;
                if (CurrentAIState == AI_STATE_FOLLOWING) {
                    if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                    long HLStyleConversationIndex = Game.AddNewConversation(Guard, false, false);
                    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_SCR_MCC_GUARD_FOLLOWING", false, Guard, Game.GetHero());
                    RegulateFollowStateComment = 10;
                } else if (CurrentAIState == AI_STATE_FIGHTING) {
                    if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                    long HLStyleConversationIndex = Game.AddNewConversation(Guard, false, false);
                    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_SCR_MCC_GUARD_ATTACK", false, Guard, Game.GetHero());
                    RegulateFollowStateComment = 10;
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                goto terminated;
            if (RegulateFollowStateComment == 0 && Game.GetHealth(Guard) < 18.0f) {
                if (IsActiveThreadTerminating())
                    goto terminated;
                long HLStyleConversationIndex = Game.AddNewConversation(Guard, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_045_" + Guard.GetDataString() + "_HURT", false, Guard, Game.GetHero());
                RegulateFollowStateComment = 12;
                MakeUniqueComment = MakeUniqueComment.GetTime() + 2;
            } else if (MakeUniqueComment == 0) {
                if (IsActiveThreadTerminating())
                    goto terminated;
                if (CurrentAIState == AI_STATE_FOLLOWING) {
                    if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                    long HLStyleConversationIndex = Game.AddNewConversation(Guard, false, false);
                    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_045_" + Guard.GetDataString() + "_CHATTER", false, Guard, Game.GetHero());
                } else if (CurrentAIState == AI_STATE_FIGHTING) {
                    if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                    long HLStyleConversationIndex = Game.AddNewConversation(Guard, false, false);
                    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_045_" + Guard.GetDataString() + "_TAUNT", false, Guard, Game.GetHero());
                }
                MakeUniqueComment = 25;
            }
        }
        PreviousAIState = CurrentAIState;
        CurrentAIState = Guard.GetCurrentStateGroupType();
    }
    return _Fail_Val.Return();
    }
terminated:
    
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif

}
