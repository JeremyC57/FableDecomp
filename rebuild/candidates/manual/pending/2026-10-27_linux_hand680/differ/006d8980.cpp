// CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType stat) -- raise a trainable stat one level when below its track's maximum: refresh the stat's abilities, signal the level change, and age the hero by the global per-level-up amount
// Grade: DIFFER(182v183) -- retail holds TrainableStatLevels' start in edi (hoisted above the clamp) and the track's Abilities start in ebx (start loaded before the finish), reads the level once and writes a copy+1 (mov ebx,edx ; inc ebx); built re-reads the level through a lea and keeps 2 callee-saved registers; at the tail retail puts the stats pointer in ecx (built eax + mov ecx,eax).
// Next step: the same-TU GetTrainableStatMaxLevel (0x006D6770, landed) inlined puts this-first loads (180v183, probe g); try the
//   whole CTCHeroExperience.cpp neighbourhood (ModifyTrainableStatLevel 0x006D6810, GetCostToUpgradeTrainableStat 0x006D67B0) to
//   find the accessor shape that reads stat first and keeps level+1 in a callee-saved register.
// Leaf: ?UpgradeTrainableStat@CTCHeroExperience@@QAEXW4EHeroTrainableStatType@@@Z
// Retail: Fable.exe @ 0x006D8980, 183 bytes, ret 4. Callees (PDB): UpdateAbilitiesForStat, OnStatLevelChange,
//   CTCBase::GetDefinitionManager, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound, CTCHeroStats::SetAge.
// Model: ChatGPT pass 10 attempt 35 (shared CThing/CTCBase layouts, retail 12-byte vectors), unchanged apart from this header.
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define CThingPhysical CThingPhysicalLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreatureLayout
#include "engine/CThingAICreature.h"
#undef CThingAICreature

namespace std {
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
}
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CBaseClassNonCopyable.h"
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
enum ETCInterfaceType {};
class CTCBase;
namespace std {
template<class T>class allocator {};
template<class K,class V>struct pair{K first;V second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef unsigned int size_type;T*begin(){return this->_M_start;}const T*begin()const{return this->_M_start;}T*end(){return this->_M_finish;}const T*end()const{return this->_M_finish;}size_type size()const{return size_type(this->_M_finish-this->_M_start);}T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {
private:C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);
};
class CThing:public CClass {
public:

    bool HasTC(ETCInterfaceType type)const {
        const unsigned long*bits=reinterpret_cast<const unsigned long*>(reinterpret_cast<const CThingLayout*>(this)->UsingComponents);
        if(bits[(unsigned)type>>5]&(1UL<<((unsigned)type&31)))return true;
        return false;
    }
    template<class T>__forceinline bool GetTC(T**out) {
        const unsigned long*bits=reinterpret_cast<const unsigned long*>(reinterpret_cast<const CThingLayout*>(this)->UsingComponents);
        if(bits[T::StatGetInterfaceType()>>5]&(1UL<<(T::StatGetInterfaceType()&31))) {
            ETCInterfaceType type=T::StatGetInterfaceType();
            CVectorMap<ETCInterfaceType,CTCBase*>&map=*reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>*>(reinterpret_cast<CThingLayout*>(this)->Components);
            std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(type);
            if(it==map.end()||T::StatGetInterfaceType()<it->first)it=map.end();
            *out=(T*)it->second;return true;
        }
        return false;
    }
    bool IsDead()const{return (reinterpret_cast<const unsigned char*>(this)[0x91]&1)!=0;}
    unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingLayout*>(this)->ThingTypeFlags;}
private:unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
};
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCHeroExperience CTCHeroExperienceFields
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience
#define CTCHeroStats CTCHeroStatsFields
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
#define CHeroStatsDef CHeroStatsDefFields
#include "engine/_quarantine/CHeroStatsDef.h"
#undef CHeroStatsDef
#define CAbilityTrackDef CAbilityTrackDefFields
#include "engine/_quarantine/CAbilityTrackDef.h"
#undef CAbilityTrackDef
#include "engine/_quarantine/CAbilityTrackLevelDef.h"
#include "engine/_quarantine/CGlobalDef.h"
// Layout: original CGameDefinitionManager is forward-only in both PDBs; generated header is a contradictory sizeof1 placeholder.
// Native PeekGlobalDef is the pointer atD0, and CGlobalDef::AgeIncreasePerLevelUp is nativeD0/donorD4.
class CGameDefinitionManager{public:const CGlobalDef&PeekGlobalDef()const{return **reinterpret_cast<const CGlobalDef*const*>(reinterpret_cast<const unsigned char*>(this)+0xd0);}};
class CTCBase:public CBaseClassNonCopyable{protected:const CGameDefinitionManager&GetDefinitionManager()const;
 CThing&ParentThing;
private:unsigned char unobservedTCState[sizeof(CTCBaseFields)-sizeof(CBaseClassNonCopyable)-sizeof(CThing*)];};
class CTCHeroStats:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return(ETCInterfaceType)4;}float GetAge()const{return reinterpret_cast<const CTCHeroStatsFields*>(this)->Age;}void SetAge(float);
private:unsigned char unobservedHeroStatsState[sizeof(CTCHeroStatsFields)-sizeof(CTCBase)];};
enum EHeroExperienceStatCategory{};
// Layout: flattened original definition prefix3c; typed observed vector and enum fields from PDB.
class CAbilityTrackDef{unsigned char unobservedTrackState[0x3c];public:EHeroExperienceStatCategory ExperienceStat;std::vector<CAbilityTrackLevelDef>Abilities;long MaxedOutDescription;};
class CHeroStatsDef{unsigned char unobservedDefinitionState[0xfc];public:std::vector<CAbilityTrackDef>StatLevels;private:unsigned char unobservedDefinitionTail[sizeof(CHeroStatsDefFields)-0x108];};
class CDefPointeeBase;template<class T>class CDefPointer{CDefPointeeBase*Object;public:T*operator->()const{return reinterpret_cast<T*>(Object);}};
enum EHeroTrainableStatType{};
class CTCHeroExperience:public CTCBase{public:void UpgradeTrainableStat(EHeroTrainableStatType);
private:void UpdateAbilitiesForStat(EHeroTrainableStatType,bool);void OnStatLevelChange(bool);
 unsigned char unobservedExperiencePrefix[0x24-sizeof(CTCBase)];std::vector<long>TrainableStatLevels;
 unsigned char unobservedExperienceTail[0x5c-0x30];CDefPointer<const CHeroStatsDef>PHeroStatsDef;};
void CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType stat){
 const std::vector<CAbilityTrackLevelDef>&abilities=PHeroStatsDef->StatLevels[stat].Abilities;
 long maxLevel=(long)abilities.size()-1;
 if(maxLevel<0)maxLevel=0;
 if(TrainableStatLevels[stat]<maxLevel){
  TrainableStatLevels[stat]=TrainableStatLevels[stat]+1;UpdateAbilitiesForStat(stat,false);OnStatLevelChange(true);
  const CGlobalDef&global=GetDefinitionManager().PeekGlobalDef();
  CThing*thing=&ParentThing;CTCHeroStats*stats;
  if(thing->GetTC(&stats))stats->SetAge(stats->GetAge()+global.AgeIncreasePerLevelUp);
 }
}
