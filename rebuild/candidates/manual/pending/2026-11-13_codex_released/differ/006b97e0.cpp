// CFactionManager::CFactionManager(CWorld const &) -- builds faction relationships from definitions
// Leaf: ??0CFactionManager@@QAE@ABVCWorld@@@Z
// Grade: DIFFER(1450v1438) -- per-site inlining: retail calls the first-loop GetDef and tree lower_bound and expands the CVectorMap Insert, VC7.1 RTM does the opposite, frame 0x48 vs 0x3C; 10 shapes (GetDef/Compare without inline, CCharString name, named counted pointer, CRC local, named iterator, early-return GetDef, hoisted def pointer, unsigned counters) do not move the inline decisions. Graded in Codex's header context
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#define FABLE_RETAIL_FACTIONS
#define FABLE_RETAIL_QUEST_COMPLETION
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CFactionManager.h"
#include "engine/CFactionDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CDefString.h"
template<> inline long __fastcall CBasicString<char>::Compare(const char* str1,const char* str2){
    while(*str1 || *str2){
        if(*str1<*str2)return -1;
        if(*str1>*str2)return 1;
        ++str1;++str2;
    }
    return 0;
}
template<> bool CBasicString<char>::operator<(const CBasicString<char>& str) const {
    return Compare(PData,str.PData)<0;
}
template<class T> inline bool CDefinitionManager::GetDef(long global_index,CDefPointer<const T>* def)const{
    if(global_index>0){
        CDefPointer<const CDefClassBase> pbase_def=GetPDefFromGlobalIndex(global_index);
        if(!pbase_def.IsNull()){
            def->SetDynamicCast(pbase_def);
            return true;
        }
    }
    return false;
}
CFactionManager::CFactionManager(const CWorld& world)
{
    const CGameDefinitionManager& definition_manager=world.PeekDefinitionManager();
    long number_of_factions=definition_manager.GetNoInstantiatedDefsOfType("FACTION");
    for(long c1=1;c1<number_of_factions;++c1){
        long global_index=definition_manager.GetGlobalIndexFromClassIndex("FACTION",c1);
        CDefPointer<const CFactionDef> pfaction_def;
        definition_manager.GetDef(global_index,&pfaction_def);
        CDefString name=pfaction_def->GetInstantiationName();
        Factions.Insert(name.GetCRC(),CCountedPointer<CFaction>(new CFaction(name)));
    }
    Factions.Update();
    for(long c1=1;c1<number_of_factions;++c1){
        long global_index=definition_manager.GetGlobalIndexFromClassIndex("FACTION",c1);
        CDefPointer<const CFactionDef> pfaction_def;
        definition_manager.GetDef(global_index,&pfaction_def);
        CCharString name=pfaction_def->GetInstantiationName();
        CFaction& faction=*Factions.Find(name.GetCRC())->second;
        for(long c2=0;c2<pfaction_def->AlliedFactions.size();++c2){
            CVectorMap<unsigned long,CCountedPointer<CFaction> >::iterator allied_faction=Factions.Find(pfaction_def->AlliedFactions[c2].GetCRC());
            if(allied_faction!=Factions.end())faction.SetAsAlliedWithFaction(*allied_faction->second);
        }
        for(long c2=0;c2<pfaction_def->EnemyFactions.size();++c2){
            CVectorMap<unsigned long,CCountedPointer<CFaction> >::iterator enemy_faction=Factions.Find(pfaction_def->EnemyFactions[c2].GetCRC());
            if(enemy_faction!=Factions.end())faction.SetAsEnemyOfFaction(*enemy_faction->second);
        }
    }
}
