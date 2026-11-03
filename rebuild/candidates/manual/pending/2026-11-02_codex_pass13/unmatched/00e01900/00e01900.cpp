// NScript::CQ_TraderEscortScript::MakeTraderComment(const CCharString&, const CScriptThing&, ETraderComment) -- build trader dialogue and regulate repeat comments.
// Leaf: ?MakeTraderComment@CQ_TraderEscortScript@NScript@@QAE_NABVCCharString@@ABVCScriptThing@@W4ETraderComment@12@@Z
#define FABLE_RETAIL_TRADER_COMMENT_TYPES
#define FABLE_RETAIL_CSCRIPTBASE_TYPED
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_TEXT_COMPARE
#define FABLE_RETAIL_CCHARSTRING_TEXT_COMPARE_EXPANDED
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGN
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CQ_TraderEscortScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRet.h"
CCharString FABLE_FASTCALL operator+(const CCharString&,const CCharString&);
extern CGameScriptInterfaceBase* pDSTGame;
__forceinline CTimer& CTimer::operator=(long value) { pDSTGame->SetTimer(TimerIndex,value); return *this; }
inline bool CTimer::operator>(long value) const { return pDSTGame->GetTimer(TimerIndex)>value; }
inline bool CScriptThing::IsDead() const { return !IsAlive(); }
__forceinline bool CScriptThing::operator!= (const CScriptThing& other) const { return !(*this==other); }
__forceinline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
CCharString CScriptThing::GetDataString() const { if(PImp.IsNull())return NullString;return PImp->GetDataString(); }
bool NScript::CQ_TraderEscortScript::MakeTraderComment(const CCharString& comment_to_make,const CScriptThing& speaker,ETraderComment comment_type)
{
    TRet<bool> _Fail_Val(false);
    CScriptThing Trader=speaker;
    if(Trader.IsDead()) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        Trader=Game.GetNearestWithScriptName(Game.GetHero(),"DarkwoodTrader");
    }
    if(Trader.IsDead()) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        return false;
    }
    if(comment_type==TRADER_COMMENT_ATTACKED_SPEECH) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing TraderA=Game.GetNearestWithScriptName(Trader,"DarkwoodTrader");
        CScriptThing TraderB=Game.GetFurthestWithScriptName(Trader,"DarkwoodTrader");
        if(TraderA==TraderB) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            TraderB=CScriptThing();
        }
        if(TraderA.GetDataString()!="SCARED") {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing temp=TraderA;
            TraderA=TraderB;
            TraderB=temp;
        }
        if(comment_to_make=="ROCK_TROLL_CLOSE") {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.PlayCriteriaSoundOnThing(Game.GetThingWithScriptName("RockTrollTrigger"),"EARTH_TROLL_OFFSCREEN_ROAR");
            Game.Pause(1.0f);
        }
        long HLStyleConversationIndex=Game.AddNewConversation(speaker,false,false);
        Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
        if(TraderA.IsAlive()) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.AddPersonToConversation(HLStyleConversationIndex,TraderA);
            if(TraderB.IsAlive()) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.AddPersonToConversation(HLStyleConversationIndex,TraderB);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_"+comment_to_make,false,Trader,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+TraderA.GetDataString()+"_INTRO_RESPONSE_NO",false,TraderA,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+TraderB.GetDataString()+"_INTRO_RESPONSE_OATH",false,TraderB,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+TraderA.GetDataString()+"_INTRO_RESPONSE_BAD_NEWS",false,TraderA,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_INTRO_RESPONSE_REWARD",false,Trader,Game.GetHero());
            } else {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_"+comment_to_make,false,Trader,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+TraderA.GetDataString()+"_INTRO_RESPONSE_NO",false,TraderA,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_INTRO_RESPONSE_REWARD",false,Trader,Game.GetHero());
            }
        }
        CommentTimer=pGlobalScriptDef->TE_TraderCommentDelay;
        return true;
    }
    if(CommentTimer>0) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        return false;
    }
    long HLStyleConversationIndex=Game.AddNewConversation(Trader,false,false);
    Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+Trader.GetDataString()+"_"+comment_to_make,false,Trader,Game.GetHero());
    if(comment_type==TRADER_COMMENT_MULTIPLE) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing near_trader=Game.GetNearestWithScriptName(Trader,"DarkwoodTrader");
        if(near_trader.IsAlive() && near_trader!=Trader && Game.TextEntryExists("TEXT_QST_067_"+near_trader.GetDataString()+"_"+comment_to_make+"_RESPONSE")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.AddPersonToConversation(HLStyleConversationIndex,near_trader);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_067_"+near_trader.GetDataString()+"_"+comment_to_make+"_RESPONSE",false,near_trader,Game.GetHero());
        }
    }
    CommentTimer=pGlobalScriptDef->TE_TraderCommentDelay;
    return true;
}
