// 007fe040 CTCAISummon::SummonCreatures
// Flags: none
#define FABLE_RETAIL_GENERATOR_CREATE_PASS29
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PRECEEDING
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_CSUMMONDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_FINDMAXZINCELL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DCOORDI
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
// Leaf: ?SummonCreatures@CTCAISummon@@QAEJPBVC3DVector@@@Z
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE_RTTI_DECLS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ISCREATUREGENERATIONENABLEDFORGROUP
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/ThingAlive.h"
#include "engine/CArray.h"
#include "engine/CDefIndex.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCreatureGenerationFamilyDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CCreatureActionBase.h"
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatActionBase.h"
#undef FABLE_RETAIL_KRAKEN_ROAR


#include "engine/CTCCreatureNavigation.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_OnGenerate.h"
#include "engine/CTCAISummon.h"
#include "engine/CWorldMap.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
long CTCAISummon::SummonCreatures(const C3DVector* summonCentre)
{
    long numberToSummon=GetNumberCanSummon();
    long numberSummoned=0;
    if(numberToSummon>0){
        long summoneeEffect=GetSummonDef().SummoneeEffect;
        C3DVector offset(GetSummonDef().SummonRadius,0,0);
        float rotAngle=1.f/numberToSummon;
        C3DVector centre=summonCentre?*summonCentre:GetParentThing().GetPos();
        const CTCCreatureNavigation* pTCNavigate=0;
        GetParentThing().PeekTC(&pTCNavigate);
        if(false){}else for(long i=0;i<numberToSummon;++i){
            C3DVector pos=centre+offset;
            C2DCoordI coord=pos.ToC2DCoordI();
            if(GetWorld().GetWorldMap().IsPosInMap(coord)&&GetWorld().GetWorldMap().IsPosPassable(coord)&&
                (!pTCNavigate||pTCNavigate->IsAbleToNavigateFromPosToPos(centre,pos,0.f))){
                CThingAICreatureInit init;
                init.ScriptName="SUMMONED_CREATURE";
                long summonDefIndex=GetSummonDef().CreatureToSummon;
                if(GetSummonDef().CreatureFamilyToSummon>0){
                    CDefPointer<const CCreatureGenerationFamilyDef> pcf_def;
                    CGameDefinitionManager::Get().GetDef(GetSummonDef().CreatureFamilyToSummon,&pcf_def);
                    long num_cret_fam=pcf_def->Creatures.size();
                    if(num_cret_fam)summonDefIndex=pcf_def->Creatures[GFRandom(num_cret_fam,GetWorld().GetWorldSeed())];
                }
                CDefPointer<const CThingCreatureDef> def;
                CGameDefinitionManager::Get().GetDef(summonDefIndex,&def);
                CThingAICreature* pCreature=CThingAICreature::Create(summonDefIndex,pos,def->DefaultOwner,init);
                if(IS_THING_ALIVE(pCreature)){
                    C3DVector facing=offset;
                    if(summonCentre)facing*=-1.f;
                    facing.Normalise();
                    pCreature->GetPhysicsTC()->SetFacingAnglesFromRHSet(CRightHandedSet(C3DVector(0,0,1),facing));
                    pCreature->SetCurrentAction(CCreatureAction_OnGenerate(*pCreature));
                    MySummonedCreatures.push_back(pCreature);
                    AllSummonedCreatures.push_back(pCreature);
                    ++numberSummoned;
                    if(summoneeEffect>0)CTCDParticleEmitter::Create(summoneeEffect,pos,false);
                }
            }
            offset.RotateXY(rotAngle);
        }
        LastSummonTick=CWorld::GetFrame();
    }
    return numberSummoned;
}