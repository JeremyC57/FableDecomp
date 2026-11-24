// 00666310 CThingCreatureBase::OnCreate
// Leaf: ?OnCreate@CThingCreatureBase@@MAE_NXZ
// Flags:
#define FABLE_RETAIL_CREATURE_ONCREATE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONCREATE
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY
#define FABLE_RETAIL_CANIMATETHING_INIT
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_STATGETTYPENAME
#define FABLE_RETAIL_CCOMBATABILITYPROJECTILEATTACKDATA
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#include "engine/EThingCreatureProperty.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CCreatureDef.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CCombatAbilityUseProjectileWeaponDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

// Inline-budget: the GetSubDef<CCreatureDef> instantiation at 00485815 is from a
// size-optimized translation unit with a frame. The other two lookups use speed optimization.
#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma optimize("y", off)
#endif
template<> inline bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const CSubDefInfo* info=GetPSubDefInfo(CCreatureDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
extern const float DELTA_NOTIONAL_ZERO;
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