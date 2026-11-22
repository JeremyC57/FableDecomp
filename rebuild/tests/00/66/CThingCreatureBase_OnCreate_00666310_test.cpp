#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CREATURE_ONCREATE_PASS27
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#include "engine/C3DVector.h"
#include "engine/EAnimUpdateMode.h"
#include "engine/ECombatAnimationHandedness.h"
#include "engine/EThingCreatureProperty.h"
static bool bad;
static unsigned setting,appearanceCalls,animCalls,attaches,clearCount,sounds,preload,scaleCount,combatLookups,shotLookups,modeLookups,nameLookups;
const float DELTA_NOTIONAL_ZERO=1.0f/8192.0f;
template<class T> struct CDefPointer {T* value;CDefPointer():value(0){}T* Get()const{return value;}T* operator->()const{return value;}};
struct CThingCreatureBase;
struct CCreatureDef {long OpinionSourceDef;};
struct CCombatAbilityUseProjectileWeaponDef {struct Ability {long ShotAccuracyPercentage;} CombatAbility;};
static CCombatAbilityUseProjectileWeaponDef projectile;
struct CCombatTypeDef {bool GetSubDef(CDefPointer<const CCombatAbilityUseProjectileWeaponDef>* out)const{
 ++shotLookups;if(setting&256){out->value=&projectile;return true;}return false;}};
static CCombatTypeDef combat;
struct CThingCreatureDef {int Graphic;EThingCreatureProperty Property;long CombatTypeDef;};
static CCreatureDef creature;static CThingCreatureDef definition;
struct CThingBaseDef {void GetSubDef(CDefPointer<const CCreatureDef>* out){out->value=(setting&16)?&creature:0;}};
static CThingBaseDef baseDef;
struct CDefinitionManager {
 long GetDefGlobalIndexFromName(const char* name)const{
 const char* names[]={"CREATURE_DEMON_DOOR_FACE_01","CREATURE_NW_DEMON_DOOR_FACE_01","CREATURE_ORACLE_HEAD_01"};
 unsigned i=nameLookups++;if(i>2||strcmp(name,names[i]))bad=true;return long(i)+1;
 }
 bool GetDef(long id,CDefPointer<const CCombatTypeDef>* out)const{
 ++combatLookups;if(id!=13)bad=true;if(setting&128){out->value=&combat;return true;}return false;}
};
static const CThingCreatureBase* current;
struct COpinionReactionManager {void AttachOpinionSource(CThingCreatureBase& obj,long id,void* other){
 if(&obj!=current || id!=12 || other)bad=true;++attaches;}};
struct CWorld {CDefinitionManager defs;COpinionReactionManager opinions;
 const CDefinitionManager& PeekDefinitionManager()const{return defs;}COpinionReactionManager& GetOpinionReactionManager(){return opinions;}};
struct CMainGameComponent {static CMainGameComponent* Get(){static CMainGameComponent app;return &app;}CWorld& GetWorld(){static CWorld world;return world;}};
namespace NGraphicAppearance{struct CTCGraphicAppearance{
 void SetMainGraphic(int g){if(g!=19)bad=true;++appearanceCalls;}void SetScale(float x){if(x!=2)bad=true;++appearanceCalls;}};}
static NGraphicAppearance::CTCGraphicAppearance appearance;
struct CAnimInit{CAnimInit(int type,int flags,long bank,EAnimUpdateMode mode,bool looping,int f,int priority){
 if(type!=6||flags||bank!=37||mode!=ANIM_UPDATE_FROM_SPEED||!looping||f||priority!=100)bad=true;}};
struct CTCAnimationComplex {struct Anim{void Init(const CAnimInit&){++animCalls;}}anim;Anim& GetAnimation(){return anim;}};
static CTCAnimationComplex animation;
struct CThingGameObject {bool OnCreate(){return (setting&4)!=0;}};
struct CThingCreatureBase: CThingGameObject {
 int* PTCModeManager;C3DVector MovementVector; bool DemonDoor,Oracle,CurrentlyFrameUpdating,MeleeAttacker,MeleeAttacked;
 bool HeadPosCached,Blinking,IsShowingDebugInfo;long WFLastAttackedByCreature,IdleCounter,ShotAccuracyPercentage,id;
 float Scale,TurnSpeed;ECombatAnimationHandedness PreviousActionHandedness;
 void GetTC(int** out){if(*out)bad=true;static int mode;*out=&mode;++modeLookups;}
 bool GetTC(CTCAnimationComplex** out){if(setting&2){*out=&animation;return true;}return false;}
 NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return (setting&1)?&appearance:0;}
 const CThingCreatureDef& PeekDefinition()const{return definition;}long GetDefGlobalIndex()const{return id;}
 struct Entry{long BankIndex;};const Entry& GetAnimFromName(const char* name,long n)const{static Entry e={37};if(strcmp(name,"DEFAULT")||n)bad=true;return e;}
 void ClearAttackedByCreature(){++clearCount;}CThingBaseDef& PeekBaseDef(){return baseDef;}
 CWorld& GetWorld(){return CMainGameComponent::Get()->GetWorld();}
 void SetScale(float x){if(x!=DELTA_NOTIONAL_ZERO)bad=true;++scaleCount;}
 void PlayCreatureContinuousSounds(){++sounds;}bool ValidateInitialisation(){return (setting&8)!=0;}
 void PreloadAnims(){++preload;}const CDefinitionManager& PeekDefinitionManager(){return GetWorld().PeekDefinitionManager();}
 bool OnCreate();
};
bool CThingCreatureBase::OnCreate()
{
    PTCModeManager=0;
    GetTC(&PTCModeManager);
    NGraphicAppearance::CTCGraphicAppearance* ptc_appearance=GetTCGraphicAppearance();
    if (ptc_appearance) {
        ptc_appearance->SetMainGraphic(PeekDefinition().Graphic);
        ptc_appearance->SetScale(Scale);
    }
    long demon_door_def_index_1=CMainGameComponent::Get()->GetWorld().PeekDefinitionManager().GetDefGlobalIndexFromName("CREATURE_DEMON_DOOR_FACE_01");
    long demon_door_def_index_2=CMainGameComponent::Get()->GetWorld().PeekDefinitionManager().GetDefGlobalIndexFromName("CREATURE_NW_DEMON_DOOR_FACE_01");
    DemonDoor=GetDefGlobalIndex()==demon_door_def_index_1 || GetDefGlobalIndex()==demon_door_def_index_2;
    long oracle_def_index=CMainGameComponent::Get()->GetWorld().PeekDefinitionManager().GetDefGlobalIndexFromName("CREATURE_ORACLE_HEAD_01");
    Oracle=GetDefGlobalIndex()==oracle_def_index;
    CurrentlyFrameUpdating=false;
    CTCAnimationComplex* ptc_animation;
    if(GetTC(&ptc_animation))
        ptc_animation->GetAnimation().Init(CAnimInit(6,0,GetAnimFromName("DEFAULT",0).BankIndex,ANIM_UPDATE_FROM_SPEED,true,0,100));
    MeleeAttacker=false;
    MeleeAttacked=false;
    if(CThingGameObject::OnCreate()){
        HeadPosCached=false;
        Blinking=false;
        MovementVector.Clear();
        ClearAttackedByCreature();
        WFLastAttackedByCreature=0;
        IsShowingDebugInfo=false;
        TurnSpeed=0.0f;
        IdleCounter=0;
        PreviousActionHandedness=HANDED_NONE;
        CDefPointer<const CCreatureDef> pcreature_def;
        PeekBaseDef().GetSubDef(&pcreature_def);
        if(pcreature_def.Get() && pcreature_def->OpinionSourceDef)
            GetWorld().GetOpinionReactionManager().AttachOpinionSource(*this,pcreature_def->OpinionSourceDef,0);
        if(PeekDefinition().Property==THING_CREATURE_PROPERTY_FIREFLY)SetScale(DELTA_NOTIONAL_ZERO);
        PlayCreatureContinuousSounds();
        if(ValidateInitialisation()){
            PreloadAnims();
            long def_ind=PeekDefinition().CombatTypeDef;
            ShotAccuracyPercentage=50;
            CDefPointer<const CCombatTypeDef> pcombat_def;
            CDefPointer<const CCombatAbilityUseProjectileWeaponDef> pshooty;
            if(def_ind && PeekDefinitionManager().GetDef(def_ind,&pcombat_def) && pcombat_def->GetSubDef(&pshooty))
                ShotAccuracyPercentage=pshooty->CombatAbility.ShotAccuracyPercentage;
            return true;
        }
    }
    return false;
}

int main(){
 for(setting=0;setting<512;++setting)for(int extra=0;extra<4;++extra)for(int id=0;id<4;++id){
 CThingCreatureBase c;current=&c;c.id=id;int sentinel=7;c.PTCModeManager=&sentinel;c.Scale=2;
 c.CurrentlyFrameUpdating=c.MeleeAttacker=c.MeleeAttacked=c.HeadPosCached=c.Blinking=c.IsShowingDebugInfo=true;
 c.MovementVector.X=c.MovementVector.Y=c.MovementVector.Z=2;c.PreviousActionHandedness=HANDED_RIGHT;
 c.WFLastAttackedByCreature=c.IdleCounter=c.ShotAccuracyPercentage=51;c.TurnSpeed=16;
 creature.OpinionSourceDef=(setting&32)?12:0;definition.Graphic=19;definition.CombatTypeDef=(setting&64)?13:0;
 definition.Property=(extra&1)?THING_CREATURE_PROPERTY_FIREFLY:(EThingCreatureProperty)0;
 projectile.CombatAbility.ShotAccuracyPercentage=(extra&2)?90:0;
 bad=false;appearanceCalls=animCalls=attaches=clearCount=sounds=preload=scaleCount=combatLookups=shotLookups=modeLookups=nameLookups=0;
 bool base=(setting&4)!=0,success=base && (setting&8)!=0;
 bool lookup=success && definition.CombatTypeDef!=0,sub=lookup&&(setting&128)!=0,got=sub&&(setting&256)!=0;
 bool result=c.OnCreate();
 if(bad||result!=success||c.DemonDoor!=(id==1||id==2)||c.Oracle!=(id==3)||
 c.CurrentlyFrameUpdating||c.MeleeAttacker||c.MeleeAttacked||!c.PTCModeManager||modeLookups!=1||nameLookups!=3||
 appearanceCalls!=((setting&1)?2u:0u)||animCalls!=unsigned((setting&2)!=0)||
 attaches!=unsigned(base&&(setting&16)&&(setting&32))||scaleCount!=unsigned(base&&(extra&1))||
 clearCount!=unsigned(base)||sounds!=unsigned(base)||preload!=unsigned(success)||combatLookups!=unsigned(lookup)||shotLookups!=unsigned(sub))return 1;
 if(c.HeadPosCached==base||c.Blinking==base||c.IsShowingDebugInfo==base||
 c.MovementVector.X!=(base?0:2)||c.MovementVector.Y!=(base?0:2)||c.MovementVector.Z!=(base?0:2)||
 c.WFLastAttackedByCreature!=(base?0:51)||c.IdleCounter!=(base?0:51)||c.TurnSpeed!=(base?0:16)||
 c.PreviousActionHandedness!=(base?HANDED_NONE:HANDED_RIGHT)||
 c.ShotAccuracyPercentage!=(got?projectile.CombatAbility.ShotAccuracyPercentage:success?50:51))return 2;
 }
 puts("00666310_TEST PASS");return 0;
}