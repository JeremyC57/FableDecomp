#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include <ctype.h>
static void check(bool ok){if(!ok)abort();}
struct CCharString: public std::string {
    CCharString() {}
    CCharString(const char* x):std::string(x){}
    CCharString(const std::string& x):std::string(x){}
    CCharString GetAsUpperCase()const {
        CCharString text(*this);
        for (size_t i=0;i<text.size();++i) text[i]=(char)toupper((unsigned char)text[i]);
        return text;
    }
};
static CCharString operator+(const CCharString& a,const char* b) {return std::string(a).append(b);}
static CCharString operator+(const char* a,const CCharString& b) {return std::string(a).append(b);}
static CCharString operator+(const CCharString& a,const CCharString& b) {return std::string(a).append(b);}
struct CRegionScriptDef {
    long RandomVillagerMax, RandomGuardMax, RandomBanditMax;
    mutable int references, releases, lastReleases;
    void Retain() const { ++references; }
    void Release() const {
        check(references > 0);
        ++releases;
        if (--references == 0) ++lastReleases;
    }
};
enum ENullDef {NULL_DEF};
template<class T> struct CDefPointer {
    T* item;
    explicit CDefPointer(ENullDef):item(0){}
    ~CDefPointer() {
        if (item) {
            item->Release();
            item = 0;
        }
    }
    T* operator->() const { check(item != 0); return item; }
private:
    CDefPointer(const CDefPointer&);
    CDefPointer& operator=(const CDefPointer&);
};
struct TRetVoid{void Return(){}};
struct Population {
    long RandomEntityVillagerMax,RandomEntityGuardMax,RandomEntityBanditMax;
    long RandomEntityVillagerCount,RandomEntityGuardCount,RandomEntityBanditCount;
    float RandomEntityVillagerPercentage;
    Population():RandomEntityVillagerMax(21),RandomEntityGuardMax(22),RandomEntityBanditMax(23),
        RandomEntityVillagerCount(31),RandomEntityGuardCount(32),RandomEntityBanditCount(33),RandomEntityVillagerPercentage(0.5f){}
};
struct Scenario {
    bool levels,unloaded,loaded,definition;
    int count,personalState,regionState,stop;
};
static Scenario cfg;
static int queries,step,frame;
static std::vector<CCharString> activated;
struct CScriptDefinitionManager {
    CRegionScriptDef definition;
    CScriptDefinitionManager(){definition.RandomVillagerMax=7;definition.RandomGuardMax=11;definition.RandomBanditMax=13;}
    static CScriptDefinitionManager& Get(){static CScriptDefinitionManager manager;return manager;}
    bool GetDef(const CCharString& name,CDefPointer<const CRegionScriptDef>* result){
        ++queries;check(name=="REGION_OAKVALE" && result->item==0);
        if (cfg.definition) {
            definition.Retain();
            result->item = &definition;
        }
        return cfg.definition;
    }
};
struct CGameScriptInterfaceBase {
    bool MsgOnLevelLoaded(std::list<CCharString>& v)const{
        if(!cfg.levels || frame) return false;
        if(cfg.count>0)v.push_back("Bower");
        if(cfg.count>1)v.push_back("Lake");
        return true;
    }
    bool MsgOnRegionUnloaded(CCharString& name)const {name="Old";return cfg.unloaded && !frame;}
    bool MsgOnRegionLoaded(CCharString& name)const {name="Oakvale";return cfg.loaded && !frame;}
    int state(const CCharString& n)const{
        if(n=="RegionScript_Oakvale")return cfg.regionState;
        if(n=="PersonalScript_Bower")return cfg.personalState;
        check(n=="PersonalScript_Lake");return (cfg.personalState+1)%3;
    }
    bool IsQuestRegistered(const CCharString& n)const{return state(n)!=0;}
    bool IsQuestActive(const CCharString& n)const{return state(n)==2;}
    void ActivateQuest(const CCharString& n){check(state(n)==1);activated.push_back(n);}
    void NewScriptFrame(){++frame;}
};
namespace NScript {
struct CPersonalScriptMainScript {
    CGameScriptInterfaceBase Game;
    Population population;
    Population* GetMasterData(){return &population;}
    bool IsActiveThreadTerminating()const{return ++step==cfg.stop || frame!=0;}
    void Main();
};
}

namespace NScript {
void CPersonalScriptMainScript::Main()
{
    TRetVoid _Fail_Val;
    if (false) {
    } else for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        {
            std::list<CCharString> loaded_level_names_list;
            if (Game.MsgOnLevelLoaded(loaded_level_names_list)) {
                if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
// Inline-budget: retail calls the list destructor at the first termination exit.
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                if (false) {} else for (std::list<CCharString>::iterator it = loaded_level_names_list.begin(); it != loaded_level_names_list.end(); ++it) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CCharString level_name = *it;
                    CCharString script_to_activate = "PersonalScript_" + level_name;
                    if (Game.IsQuestRegistered(script_to_activate) && !Game.IsQuestActive(script_to_activate)) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Game.ActivateQuest(script_to_activate);
                        CCharString msg = CCharString("") + "Loading personal script " + script_to_activate;
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (!Game.IsQuestRegistered(script_to_activate) || Game.IsQuestActive(script_to_activate)) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    }
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            CCharString UnloadedRegionName;
            if (Game.MsgOnRegionUnloaded(UnloadedRegionName)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                GetMasterData()->RandomEntityVillagerMax = 0;
                GetMasterData()->RandomEntityGuardMax = 0;
                GetMasterData()->RandomEntityBanditMax = 0;
                GetMasterData()->RandomEntityVillagerCount = 0;
                GetMasterData()->RandomEntityGuardCount = 0;
                GetMasterData()->RandomEntityBanditCount = 0;
            }
            CCharString region_name;
            if (Game.MsgOnRegionLoaded(region_name)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                // Inferred: This scope keeps the handle in its native stack slot and omits the normal-exit clear at 00CDE171.
                {
                    CDefPointer<const CRegionScriptDef> pdef(NULL_DEF);
                    CCharString RegionDefName = CCharString("REGION_") + CCharString(region_name).GetAsUpperCase();
                    if (CScriptDefinitionManager::Get().GetDef(RegionDefName, &pdef)) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        GetMasterData()->RandomEntityVillagerMax = pdef->RandomVillagerMax;
                        GetMasterData()->RandomEntityGuardMax = pdef->RandomGuardMax;
                        GetMasterData()->RandomEntityBanditMax = pdef->RandomBanditMax;
                        GetMasterData()->RandomEntityVillagerCount = 0;
                        GetMasterData()->RandomEntityGuardCount = 0;
                        GetMasterData()->RandomEntityBanditCount = 0;
                        GetMasterData()->RandomEntityVillagerPercentage = 1.0f;
                    }
                    CCharString script_to_activate = "RegionScript_" + region_name;
                    if (Game.IsQuestRegistered(script_to_activate) && !Game.IsQuestActive(script_to_activate)) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Game.ActivateQuest(script_to_activate);
                        CCharString msg = CCharString("") + "Loading region script " + script_to_activate;
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (!Game.IsQuestRegistered(script_to_activate)) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            CCharString msg = CCharString("") + "Cannot load script " + script_to_activate + ": not registered";
                        } else if (Game.IsQuestActive(script_to_activate)) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            CCharString msg = CCharString("") + "Script " + script_to_activate + " already active";
                        }
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}

}

struct Reference {
    int ticks, definitions; bool advanced,zero,assigned;
    std::vector<CCharString> activations;
    Reference():ticks(0),definitions(0),advanced(false),zero(false),assigned(false){}
    bool stopped(){return ++ticks==cfg.stop;}
    void run(){
        if(stopped())return;
        if(cfg.levels){
            if(stopped())return;
            for(int i=0;i<cfg.count;++i){
                if(stopped())return;
                int state=(cfg.personalState+i)%3;
                if(stopped())return;
                if(state==1)activations.push_back(i?"PersonalScript_Lake":"PersonalScript_Bower");
                else if(stopped())return;
            }
            if(stopped())return;
        }
        if(cfg.unloaded){if(stopped())return;zero=true;}
        if(cfg.loaded){
            if(stopped())return;
            ++definitions;
            if(cfg.definition){if(stopped())return;assigned=true;zero=true;}
            if(stopped())return;
            if(cfg.regionState==1)activations.push_back("RegionScript_Oakvale");
            else if(stopped())return;
        }
        advanced=true;
        ++ticks;
    }
};
int main()
{
    for(int otherOwners=0;otherOwners<=1;++otherOwners)
    for(int flags=0;flags<16;++flags)for(int count=0;count<3;++count)
        for(int p=0;p<3;++p)for(int r=0;r<3;++r)for(int stop=0;stop<19;++stop){
            cfg.levels=(flags&1)!=0;cfg.unloaded=(flags&2)!=0;cfg.loaded=(flags&4)!=0;cfg.definition=(flags&8)!=0;
            cfg.count=count;cfg.personalState=p;cfg.regionState=r;cfg.stop=stop;
            CRegionScriptDef& resource = CScriptDefinitionManager::Get().definition;
            resource.references = otherOwners;
            resource.releases = 0;
            resource.lastReleases = 0;
            activated.clear();step=frame=queries=0;
            NScript::CPersonalScriptMainScript manager;Reference ref;ref.run();manager.Main();
            const int acquired = cfg.definition ? ref.definitions : 0;
            check(resource.releases == acquired);
            check(resource.lastReleases == (otherOwners == 0 ? acquired : 0));
            check(resource.references == otherOwners);
            const Population& actual=manager.population;
            check(frame==int(ref.advanced) && ref.ticks==step && queries==ref.definitions && activated==ref.activations);
            check(actual.RandomEntityVillagerMax==(ref.assigned?7:ref.zero?0:21));
            check(actual.RandomEntityGuardMax==(ref.assigned?11:ref.zero?0:22));
            check(actual.RandomEntityBanditMax==(ref.assigned?13:ref.zero?0:23));
            check(actual.RandomEntityVillagerCount==(ref.zero?0:31));
            check(actual.RandomEntityGuardCount==(ref.zero?0:32));
            check(actual.RandomEntityBanditCount==(ref.zero?0:33));
            check(actual.RandomEntityVillagerPercentage==(ref.assigned?1.0f:0.5f));
        }
    puts("00cddcb0_TEST PASS");
    return 0;
}