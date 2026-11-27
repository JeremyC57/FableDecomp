// 0074b2b0 CTCCreatureGenerator::CreateCreatures
// Flags: none
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PRECEEDING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCREATUREDEF_SIZE_BODY
// Leaf: ?CreateCreatures@CTCCreatureGenerator@@IAEXPAVCThingCreatureBase@@@Z
#define FABLE_RETAIL_GENERATOR_CREATE_PASS29
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_LEVELPERSISTENCE
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CCREATUREACTION_AMBUSH_VIRTUAL_MODEL
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE_RTTI_DECLS
#define FABLE_RETAIL_PLAYER
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
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
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
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGAICREATUREINIT_DTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_VECTOR_AT_INLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCCREATUREGENERATOR_UNDEAD
#define FABLE_RETAIL_CTCCREATUREGENERATOR_HASHITCREATIONLIMIT_DEFINITION
#include "rebuild_abi.h"
#include "engine/CSoundPair.h"
class CAnimationEntry;
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
#include "engine/CCreatureDef.h"
#include "engine/CCreatureActionBase.h"
// Use the shared 81-slot combat-action declaration for this class and its table.
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatActionBase.h"
#undef FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CTCShapeManager.h"
#include "engine/CTCGeneratedCreature.h"
#include "engine/CTCCreatureGenerator.h"


#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCCreatureHive.h"
#include "engine/CCreatureAction_Ambush.h"
#include "rebuild_abi.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_OnGenerate.h"
void CTCCreatureGenerator::CreateCreatures(CThingCreatureBase* pTriggerer)
{
    if (GetParentThing().HasTC(TCI_CREATURE_GENERATOR_CREATOR))
        return;
    CTCCreatureHive* ptc_hive;
    if (GetParentThing().GetTC(&ptc_hive))
        ptc_hive->Open();
    if (CreatureFamilies.empty() || HasHitCreationLimit())
        return;
    long chooser = GFRandom(CreatureFamilies.size(),PeekWorld().GetWorldSeed());
    CDefPointer<const CCreatureGenerationFamilyDef> def;
    GetDefinitionManager().GetDef(CreatureFamilies.at(chooser), &def);
    ECreatureGeneratorGenerateType gtype = def->GenerationTypes[GFRandom(def->GenerationTypes.size(),GetWorld().GetWorldSeed())];
    std::vector<C3DVector>* ppos_list_to_use = 0;
    bool jump_out = false;
    bool drop_in = false;
    switch (gtype) {
    case GENERATE_NORMAL: ppos_list_to_use=&OpenPositions;break;
    case GENERATE_AMBUSH_DROP_IN: ppos_list_to_use=&OpenPositions;drop_in=true;break;
    case GENERATE_AMBUSH_JUMP_OUT: ppos_list_to_use=&HiddenPositions;jump_out=true;break;
    case GENERATE_GENERATOR_ANIMATION: return;
    case GENERATE_UNDEAD: CreateCreaturesUndead(pTriggerer);return;
    default: return;
    }
    if (ppos_list_to_use->empty()) return;
    const CTCCreatureNavigation* pTCNavigate=0;
    C3DVector navPosStart=GetParentThing().GetPos();
    if (DoNavCheckOnGeneration && !jump_out) {
        CThingPlayerCreature* phero=GetWorld().PeekMyPlayer()->GetPPlayerCharacter();
        if (IS_THING_ALIVE(phero)) { phero->PeekTC(&pTCNavigate); navPosStart=phero->GetPos(); }
        if (!pTCNavigate) return;
    }
    for (std::vector<long>::const_iterator it=def->Creatures.begin();it!=def->Creatures.end();++it) {
        CDefPointer<const CThingCreatureDef> pdef;
        if (GetParentThing().PeekDefinitionManager().GetDef(*it,&pdef)) {
            CDefPointer<const CCreatureDef> psub_def;
            if (pdef->GetSubDef(&psub_def) && PeekWorld().IsCreatureGenerationEnabledForGroup(psub_def->CreatureGroup)) {
                const C3DVector& creature_pos=(*ppos_list_to_use)[GFRandom(ppos_list_to_use->size(),GetWorld().GetWorldSeed())];
                if (pTCNavigate && !pTCNavigate->IsAbleToNavigateFromPosToPos(navPosStart,creature_pos,0.0f)) continue;
                CThingAICreatureInit init(0,0,false,GenParams.ScriptNameOfAllGeneratedCreatures);
                CThingAICreature* pcreatedcreature=CThingAICreature::Create(*it,creature_pos,GetParentThing().GetPlayerNumber(),init);
                pcreatedcreature->SetAsGameStatePersisted(false);
                pcreatedcreature->SetAsLevelPersistent(false);
                if (jump_out) {
                    if (pTriggerer)
                        pcreatedcreature->SetCurrentAction(CCreatureAction_Ambush(*pcreatedcreature,*pTriggerer,GetParentThing().GetPos()));
                } else if (drop_in) {
                    pcreatedcreature->SetCurrentAction(CCreatureAction_OnGenerate(*pcreatedcreature));
                }
                CreatedCreatures.push_back(pcreatedcreature);
                ++g_GeneratedCreatureCount;
                ++CreationCount;
                if (HasHitCreationLimit()) break;
            }
        }
    }
}