// NScript::CQ_GuildTrainingScript::CSkillTarget::Main() -- an archery-range dummy: it slides from its position to its static marker, waits (static mode) for a hard hit, then slides between the start and end markers until the skill test is over, scoring each arrow by where it hit (0.25 / 0.5 / 0.75 / bull) with guildmaster or apprentice comments, and reacting to a hit outside the archery ring.
// Leaf: ?Main@CSkillTarget@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D41D00, 4662 bytes; RELOCATION_MATCH, every relocation bound-same, 28 strings equal.
// Name: slot 1 of the entity vtable 0x012CE0CC (0 scalar deleting dtor 0x00D41CE0, 2 Init 0x00D41CA0, 3 GetParentScript
//   0x00D41C90, 5 OnPredicateFail 0x00D41CD0; all landed) that the landed Alloc 0x00D41BF0 stores; Ego_r.pdb and FableWin.pdb
//   ?Main@CSkillTarget@CQ_GuildTrainingScript@NScript@@EAEXXZ.
// Callees: IsActiveThreadTerminating 0x00F35B30, CTimer ctor 0x00CD4450 / dtor 0x00CD4470 (out of line), CCharString 0x0099EBF0,
//   IsDistanceBetweenThingsOver 0x00CBE3EA, rand 0x00BFEB16, __ftol2 0x00BFEA70; pDSTGame 0x0143E8F8, pGlobalScriptDef 0x0143E90C,
//   CScriptThing::NullCoord 0x0143E8E0 (inline GetPos), CScriptThing vtable 0x01238C8C (inline ctor).
// Layout: engine/CQ_GuildTrainingScript.h (FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_ENTITIES, Ego_r.pdb CSkillTarget: ParentClass +0x14,
//   PMasterData +0x18, Speed +0x1c, DummyNumber +0x20, DummyWorth +0x24).
// Shape: each slide step is a named per-site C3DVector built member-wise (step.Z unused, 0.0f), so the frame is 0x19C (four
//   12-byte steps) and the Y product is spilled while X is forwarded; operator* there gives the right frame but drops the
//   (float)i lifetime (fxch / fmul mem). The hit-score ladder is "damage<0.25 / <0.5 / <0.75 / else" (fcomp; test ah,5),
//   the static-mode test "damage>0.5 -> break" (test ah,0x41; je), and i=0 precedes EntitySetTargetable(Me,false).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_ENTITIES
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_distance.h"
extern CGameScriptInterfaceBase* pDSTGame;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
extern "C" int __cdecl rand();
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>&);
inline CScriptThing::CScriptThing() { }
__forceinline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
void NScript::CQ_GuildTrainingScript::CSkillTarget::Main()
{
    float angle=Me.GetAngleXY();
    TRetVoid _Fail_Val;
    CScriptThing EndMarker=Game->GetNearestWithScriptName(Me,"DummyEndMarker");
    CScriptThing StartMarker=Game->GetNearestWithScriptName(Me,"DummyStartMarker");
    CScriptThing StaticMarker;
    if (DummyNumber==1) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Speed=(long)pGlobalScriptDef->GUI_FrontDummySegements;
        StaticMarker=Game->GetNearestWithScriptName(Me,"StaticDummyMarker3");
        DummyWorth=(long)pGlobalScriptDef->GUI_FrontDummyWorth;
    }
    else if (DummyNumber==2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Speed=(long)pGlobalScriptDef->GUI_MiddleDummySegements;
        StaticMarker=Game->GetNearestWithScriptName(Me,"StaticDummyMarker2");
        DummyWorth=(long)pGlobalScriptDef->GUI_MiddleDummyWorth;
    }
    else {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Speed=(long)pGlobalScriptDef->GUI_RearDummySegements;
        StaticMarker=Game->GetNearestWithScriptName(Me,"StaticDummyMarker1");
        DummyWorth=(long)pGlobalScriptDef->GUI_RearDummyWorth;
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    long i=0;
    while (PMasterData->SkillTrainingStarted!=true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        C3DVector MyPos=Me.GetPos();
        C3DVector StartPos=StartMarker.GetPos();
        C3DVector EndPos=EndMarker.GetPos();
        C3DVector StaticPos=StaticMarker.GetPos();
        C3DVector Forward;
        Forward.X=(EndPos.X-StartPos.X)/Speed;
        Forward.Y=(EndPos.Y-StartPos.Y)/Speed;
        C3DVector Back;
        Back.X=(StartPos.X-EndPos.X)/Speed;
        Back.Y=(StartPos.Y-EndPos.Y)/Speed;
        C3DVector ToStatic;
        ToStatic.X=(StaticPos.X-MyPos.X)/Speed;
        ToStatic.Y=(StaticPos.Y-MyPos.Y)/Speed;
        C3DVector ToStart;
        ToStart.X=(StartPos.X-StaticPos.X)/Speed;
        ToStart.Y=(StartPos.Y-StaticPos.Y)/Speed;
        bool backwards=false;
        PMasterData->SkillDummyReset=false;
        do {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            C3DVector step(ToStatic.X*(float)i,ToStatic.Y*(float)i,0.0f);
            Game->EntityTeleportToPosition(Me,C3DVector(MyPos.X+step.X,MyPos.Y+step.Y,MyPos.Z),angle,false,false);
            ++i;
        } while (i!=Speed);
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!PMasterData->MovingDummiesNeeded) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            float damage;
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO",damage)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (damage>0.5) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        break;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        --PMasterData->SkillScore;
                    }
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ++PMasterData->SkillScore;
            i=0;
            Game->EntitySetTargetable(Me,false);
            do {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector step(ToStart.X*(float)i,ToStart.Y*(float)i,0.0f);
                Game->EntityTeleportToPosition(Me,C3DVector(StaticPos.X+step.X,StaticPos.Y+step.Y,StaticPos.Z),angle,false,false);
                ++i;
            } while (i!=Speed);
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (PMasterData->MovingDummiesNeeded!=true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetTargetable(Me,true);
        }
        i=0;
        CTimer CommentTimer;
        CommentTimer=0;
        bool finished=false;
        while (!finished) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (backwards) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector step(Back.X*(float)i,Back.Y*(float)i,0.0f);
                Game->EntityTeleportToPosition(Me,C3DVector(EndPos.X+step.X,EndPos.Y+step.Y,EndPos.Z),angle,false,false);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector step(Forward.X*(float)i,Forward.Y*(float)i,0.0f);
                Game->EntityTeleportToPosition(Me,C3DVector(StartPos.X+step.X,StartPos.Y+step.Y,StartPos.Z),angle,false,false);
            }
            float damage;
            if (Me.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO",damage)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (PMasterData->MovingDummiesNeeded==true) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (IsDistanceBetweenThingsOver(Game->GetHero(),Game->GetThingWithScriptName("ArcheryRing"),6.0f)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->IsQuestActive("Q_GuildTrainingSkill")) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            CScriptThing Guildmaster=Game->GetThingWithScriptName("TheGuildmaster");
                            long Conversation=Game->AddNewConversation(Guildmaster,false,false);
                            Game->AddPersonToConversation(Conversation,Game->GetHero());
                            Game->AddLineToConversation(Conversation,"TEXT_QST_028_MAZE_HIT_OUT",false,Guildmaster,Game->GetHero());
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            CScriptThing Apprentice=Game->GetThingWithScriptName("SkillApprentice");
                            long Conversation=Game->AddNewConversation(Apprentice,false,false);
                            Game->AddPersonToConversation(Conversation,Game->GetHero());
                            Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_SKILL_HIT_OUT",false,Apprentice,Game->GetHero());
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->PlaySoundOnThing(Game->GetHero(),"SND_ARROWIMPACT_02");
                        if (damage<0.25) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            PMasterData->SkillScore+=DummyWorth;
                        }
                        else if (damage<0.5) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            PMasterData->SkillScore+=DummyWorth*2;
                        }
                        else if (damage<0.75) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            PMasterData->SkillScore+=DummyWorth*3;
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            PMasterData->SkillScore+=DummyWorth*4;
                        }
                        if (CommentTimer.GetTime()<=0) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (rand()%4==0) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                CommentTimer=8;
                                if (Game->IsQuestActive("Q_GuildTrainingSkill")) {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    CScriptThing Guildmaster=Game->GetThingWithScriptName("TheRealGuildmaster");
                                    long Conversation=Game->AddNewConversation(Guildmaster,false,false);
                                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                                    if (damage<0.25) {
                                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                        Game->AddLineToConversation(Conversation,"TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT",false,Guildmaster,Game->GetHero());
                                    }
                                    else if (damage<0.5) {
                                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                        Game->AddLineToConversation(Conversation,"TEXT_QST_028_GUILDMASTER_SKILL_MEDIUM_HIT",false,Guildmaster,Game->GetHero());
                                    }
                                    else if (damage<0.75) {
                                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                        Game->AddLineToConversation(Conversation,"TEXT_QST_028_GUILDMASTER_SKILL_GOOD_HIT",false,Guildmaster,Game->GetHero());
                                    }
                                    else {
                                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                        Game->AddLineToConversation(Conversation,"TEXT_QST_028_GUILDMASTER_SKILL_EXCELLENT_HIT",false,Guildmaster,Game->GetHero());
                                    }
                                }
                                else {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    CScriptThing Apprentice=Game->GetThingWithScriptName("SkillApprentice");
                                    long Conversation=Game->AddNewConversation(Apprentice,false,false);
                                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                                    if (damage<0.25) {
                                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                        Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_SKILL_SOFT_HIT",false,Apprentice,Game->GetHero());
                                    }
                                    else if (damage<0.5) {
                                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                        Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_SKILL_MEDIUM_HIT",false,Apprentice,Game->GetHero());
                                    }
                                    else if (damage<0.75) {
                                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                        Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_SKILL_GOOD_HIT",false,Apprentice,Game->GetHero());
                                    }
                                    else {
                                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                        Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_SKILL_EXCELLENT_HIT",false,Apprentice,Game->GetHero());
                                    }
                                }
                            }
                        }
                        Game->EntityPlayObjectAnimation(Me,"GET_HIT",false);
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->PlaySoundOnThing(Game->GetHero(),"SND_ARROWIMPACT_02");
                    Game->EntityPlayObjectAnimation(Me,"GET_HIT",false);
                }
            }
            ++i;
            if (i==Speed) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                backwards=!backwards;
                i=0;
            }
            if (PMasterData->SkillRepeatKnown) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (PMasterData->SkillRepeating==true) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    finished=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                PMasterData->SkillDummyReset=true;
                while (PMasterData->SkillRepeatKnown) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}