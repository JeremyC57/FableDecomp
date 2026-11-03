// NScript::CQ_SecretPassageScript::SwitchDoorsOnAndOff() -- create temporary barriers, wait for clearing, then remove doors and effects.
// Leaf: ?SwitchDoorsOnAndOff@CQ_SecretPassageScript@NScript@@QAEXXZ
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
// Flags: /Gr
#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SCRIPT_QUEST
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#include "engine/C3DVector.h"
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
inline CScriptThing::CScriptThing() {}

#include "engine/NScript_CQ_SecretPassageScript.h"
namespace std {
template<> void _Construct(CScriptThing*,const CScriptThing&);
template<> vector<CScriptThing>::~vector();
template<> void vector<CScriptThing>::_M_insert_overflow(iterator,const CScriptThing&,const __false_type&,size_type,bool);
}
void NScript::CQ_SecretPassageScript::SwitchDoorsOnAndOff()
{
    DoorsBeingDone=true;
    DoorsBlocked=false;
    while(!DoorsBlocked) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    CScriptThing TempParticle;
    std::vector<CScriptThing> InvisoDoorVec;
    std::vector<CScriptThing> EffectVec;
    Game.GetAllThingsWithScriptName("DoorBlockerEffect",InvisoDoorVec);
    for(long i=0;i<InvisoDoorVec.size();++i) {
        if(IsActiveThreadTerminating()) return;
        CScriptThing TempDoor=Game.CreateObject("OBJECT_ACTIVATION_RECEPTOR_DOOR_TEST",InvisoDoorVec[i].GetPos(),"InvisoDoor");
        Game.EntitySetFacingAngle(TempDoor,InvisoDoorVec[i].GetAngleXY(),true);
        Game.EntitySetAlpha(TempDoor,0.0f,true);
        InvisoDoorVec[i]=TempDoor;
        TempParticle=Game.CreateEffect("HK_MAGIC_BARRIER",InvisoDoorVec[i].GetPos(),"DoorBlock",0.0f,false,false);
        EffectVec.push_back(TempParticle);
    }
    if(IsActiveThreadTerminating()) return;
    while(DoorsBlocked) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    for(long i=0;i<InvisoDoorVec.size();++i) {
        if(IsActiveThreadTerminating()) return;
        Game.RemoveThing(InvisoDoorVec[i],true,true);
        Game.CreateEffect("HK_MAGIC_BARRIER_OFF",EffectVec[i].GetPos(),"DoorBlockOff",0.0f,false,false);
        Game.RemoveThing(EffectVec[i],true,true);
    }
    if(IsActiveThreadTerminating()) return;
    DoorsBeingDone=false;
}
