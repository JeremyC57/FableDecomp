// NScript::CPersonalScript_EmptyScript::CChatter1::Main() -- join the second chatter to a conversation, then repeat the four-step walking route.
// Leaf: ?Main@CChatter1@CPersonalScript_EmptyScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONAL_SCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CPersonalScript_EmptyScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"
#include "engine/TRetVoid.h"
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
template<class P1,class P2> inline NScript::CPredicate_And<P1,P2>::~CPredicate_And() {}
inline bool CScriptThing::IsAlive()const {return PImp.Get()!=0 && PImp->IsAlive();}
void NScript::CPersonalScript_EmptyScript::CChatter1::Main()
{
    TRetVoid _Fail_Val;
    CScriptThing marker1=Game.GetThingWithScriptName("ChatPath11");
    CScriptThing marker2=Game.GetThingWithScriptName("ChatPath12");
    CScriptThing marker3=Game.GetThingWithScriptName("ChatPath13");
    SetPredicate(CIsThingAlive(Me)&&CIsThingAlive(marker1)&&CIsThingAlive(marker2)&&CIsThingAlive(marker3));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    CScriptThing chatter2=Game.GetThingWithScriptName("Chatter2");
    if(chatter2.IsAlive()){
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        long conversation_index=Game.AddNewConversation(Me,false,false);
        Game.AddPersonToConversation(conversation_index,chatter2);
    }
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid())seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,(EScriptAIPriority)4)){
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
    }
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    while(!IsActiveThreadTerminating()){
        seh_me.MoveToThing(marker1,0.0f,(EScriptEntityMoveType)0,true,false,false,true);
        seh_me.MoveToThing(marker2,0.0f,(EScriptEntityMoveType)0,true,false,false,true);
        seh_me.MoveToThing(marker3,0.0f,(EScriptEntityMoveType)0,true,false,false,true);
        seh_me.MoveToThing(marker2,0.0f,(EScriptEntityMoveType)0,true,false,false,true);
        Game.NewScriptFrame();
    }
}
