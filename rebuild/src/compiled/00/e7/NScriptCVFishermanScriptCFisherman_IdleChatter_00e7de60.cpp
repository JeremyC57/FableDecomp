// 00e7de60 NScript::CV_FishermanScript::CFisherman::IdleChatter
// Leaf: ?IdleChatter@CFisherman@CV_FishermanScript@NScript@@AAEXXZ
// Flags:
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#include "engine/CV_FishermanScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
#include "engine/fable_script_distance.h"
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline bool CTimer::operator>(long time) const { return pDSTGame->GetTimer(TimerIndex)>time; }
void NScript::CV_FishermanScript::CFisherman::IdleChatter()
{
    TRetVoid _Fail_Val;
    if (IsDistanceBetweenThingsOver(Me, Game->GetHero(), 8.0f)) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        return;
    }
    if (ChatterTimer > 0) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        return;
    }
    ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
    long Conversation = Game->AddNewConversation(Me,false,false);
    Game->AddPersonToConversation(Conversation,Game->GetHero());
    switch(ParentClass->FishermanTasks) {
    case 4: {
        Game->AddLineToConversation(Conversation,Game->GetValidTextEntryNameWithAttitude(Me,"TEXT_QST_023_IDLE"),false,Me,Game->GetHero());
        break;
    }
    case 2: {
        Game->AddLineToConversation(Conversation,Game->GetValidTextEntryNameWithAttitude(Me,"TEXT_QST_023_TUTORIAL_IDLE"),false,Me,Game->GetHero());
        break;
    }
    }
}