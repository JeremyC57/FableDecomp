// 007c99f0 CTCCreatureGeneratorCreator::FrameUpdate
// Flags: none
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CREATOR_UPDATE_PASS29
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING_GETSCRIPTNAME
#define FABLE_RETAIL_CTHING_GETSERIALISATIONSECTIONNAME
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGMARKER_CREATE
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBAL
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GET
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
// Leaf: ?FrameUpdate@CTCCreatureGeneratorCreator@@UAEXXZ
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_LEVELPERSISTENCE
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
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
#include "engine/CTCShapeManager.h"
#include "engine/CTCCreatureGenerator.h"


#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"

#include "engine/CTCCreatureGeneratorCreator.h"
#include "engine/CGlobalDef.h"
#include "engine/CDefString.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CThingMarker.h"
#include "engine/CThingObject.h"
namespace std {
template<> void vector< CDefPointer<const CCreatureGenerationFamilyDef> >::_M_insert_overflow(iterator position,const CDefPointer<const CCreatureGenerationFamilyDef>& value,const __false_type&,size_type fillLength,bool atEnd) {
 const size_type oldSize=size_type(_M_finish-_M_start);
 const size_type length=oldSize+max(oldSize,fillLength);
 iterator start=_M_end_of_storage.allocate(length);
 iterator finish=__uninitialized_copy(_M_start,position,start,__false_type());
 if(fillLength==1) { _Construct(finish,value); ++finish; }
 else finish=__uninitialized_fill_n(finish,fillLength,value,__false_type());
 if(!atEnd) finish=__uninitialized_copy(position,_M_finish,finish,__false_type());
 __destroy_aux(_M_start,_M_finish,__false_type());
 _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);
 _M_start=start; _M_finish=finish; _M_end_of_storage._M_data=start+length;
}
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
// Inline-budget: the 207-byte /Os frame-pointer owner 004e8c1a expands these two typed range loops.
// In speed mode the 245-byte counted-family owner 007ca250 calls the copy at 007c9f70.
template<> FABLE_FORCEINLINE CDefIndex* FABLE_FASTCALL __uninitialized_copy(CDefIndex* first,CDefIndex* last,CDefIndex* result,const __false_type&) {
 CDefIndex* cur=result;for(;first!=last;++first,++cur)_Construct(&*cur,*first);return cur;
}
template<> FABLE_FORCEINLINE CDefIndex* __uninitialized_fill_n(CDefIndex* first,unsigned int n,const CDefIndex& value,const __false_type&) {
 CDefIndex* cur=first;for(;n>0;--n,++cur)_Construct(&*cur,value);return cur;
}
template<> void vector< CDefIndex >::_M_insert_overflow(iterator position,const CDefIndex& value,const __false_type&,size_type fillLength,bool atEnd) {
 const size_type oldSize=size_type(_M_finish-_M_start);
 const size_type length=oldSize+max(oldSize,fillLength);
 iterator start=length?(iterator)malloc(length*sizeof(CDefIndex)):0;
 iterator finish=__uninitialized_copy(_M_start,position,start,__false_type());
 if(fillLength==1) { _Construct(finish,value); ++finish; }
 else finish=__uninitialized_fill_n(finish,fillLength,value,__false_type());
 if(!atEnd) finish=__uninitialized_copy(position,_M_finish,finish,__false_type());
 
 if(_M_start)free(_M_start);
 _M_start=start; _M_finish=finish; _M_end_of_storage._M_data=start+length;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
}
void CTCCreatureGeneratorCreator::FrameUpdate()
{
    if (!Generated) {
        if (CWorld::GetFrame() >= FrameToRegenerate && PeekWorld().IsCreatureGenerationEnabled(GetParentThing())) {
            CTCCreatureGenerator* ptc_generator;
            GetParentThing().GetTC(&ptc_generator);
            const CThingPlayerCreature* phero=PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
            if (IS_THING_ALIVE(phero)) {
                const CArray<CDefIndex>& families=ptc_generator->GetCreatureGeneratorFamilies();
                if (!families.empty()) {
                    const CTCHeroExperience* ptc_experience;
                    phero->PeekTC(&ptc_experience);
                    long lowest_level=-1,best_level=-1;
                    long difficulty_level=ptc_experience->GetRoughExperienceLevel()+1;
                    if (false) {} else for (long i=0;i<families.size();++i) {
                        CDefPointer<const CCreatureGenerationFamilyDef> pfamily_def;
                        GetDefinitionManager().GetDef(families[i],&pfamily_def);
                        if (pfamily_def.Get()) {
                            long level=pfamily_def->DifficultyLevel;
                            if(level<=difficulty_level && level>best_level) best_level=level;
                            if(lowest_level==-1 || level<lowest_level) lowest_level=level;
                        }
                    }
                    if(best_level==-1)best_level=lowest_level;
                    if(best_level!=-1) {
                        CArray<CDefPointer<const CCreatureGenerationFamilyDef> > valid_family_defs;
                        if (false) {} else for(long i=0;i<families.size();++i) {
                            CDefPointer<const CCreatureGenerationFamilyDef> pfamily_def;
                            GetDefinitionManager().GetDef(families[i],&pfamily_def);
                            if(pfamily_def.Get() && pfamily_def->DifficultyLevel==best_level)valid_family_defs.push_back(pfamily_def);
                        }
                        CDefPointer<const CCreatureGenerationFamilyDef> pselected_family_def=valid_family_defs[GFRandom(valid_family_defs.size(),GetWorld().GetWorldSeed())];
                        if(pselected_family_def.Get()) {
                            CTCCreatureGenerator* ptc_child_generator=0;
                            CThing* pgenerator=0;
                            if(pselected_family_def->GeneratorObject>0)
                                pgenerator=CThingObject::Create(pselected_family_def->GeneratorObject,GetParentThing().GetPos(),4,0,0,GetParentThing().GetScriptName());
                            else {
                                long marker_def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_CREATURE_GENERATOR_INVISIBLE");
                                pgenerator=CThingMarker::Create(marker_def_index,GetParentThing().GetPos(),GetParentThing().GetScriptName());
                            }
                            if(IS_THING_ALIVE(pgenerator)) {
                                pgenerator->SetSerialisationSectionName(GetParentThing().GetSerialisationSectionName());
                                pgenerator->GetTC(&ptc_child_generator);
                                ptc_child_generator->CloneFromCreator(*ptc_generator);
                                if(IS_THING_ALIVE(PGenerator))PGenerator->Kill(false);
                                PGenerator=pgenerator;
                                FrameToRegenerate=0;
                                if(ptc_child_generator) {
                                    long best_index=GetDefinitionManager().GetDefGlobalIndexFromName(pselected_family_def->GetInstantiationName());
                                    CArray<CDefIndex> child_family;
                                    child_family.push_back(best_index);
                                    ptc_child_generator->SetCreatureGeneratorFamilies(child_family);
                                    ptc_child_generator->SetAsReadyToInitialise(true);
                                }
                            }
                        }
                    }
                }
            }
            ptc_generator->SetAsActive(false);
        }
        Generated=true;
    }
    if(FrameToRegenerate==0 && !IsGeneratorAlive()) {
        float regen_time=PeekWorld().PeekDefinitionManager().PeekGlobalDef().CreatureGeneratorRegenDelaySeconds;
        FrameToRegenerate=(long)(CWorld::GetFrame()+CMainGameComponent::GetConstantFPS()*regen_time);
    }
}