// Complete production Main body below; engine calls use controlled portable fixtures.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
struct TestState {
    int stop, checks, frames, constructors, destructors, baseDestructors;
    int allocations, frees, releases, starts, failures, busy, taskReads;
    int follows, faces, lines, animations, factions, aliveReads, awareReads;
    int predicates, predicateDestructors, firstLine, randomValue, initialTalk;
    int taskDuration;
    bool alive, aware, nearHero, afterLine;
    long timer;
    long* currentTalk;
};
static TestState state;
class CCharString { public: std::string Text; CCharString(const char* text,long=-1):Text(text){} };
class CScriptThing { public: int Identity; CScriptThing(int id=0):Identity(id){} bool IsAlive()const {++state.aliveReads;return state.alive;} };
static CScriptThing Hero(1);
namespace NScript {
class CIsThingAlive { public: CScriptThing Thing; CIsThingAlive(const CScriptThing& thing):Thing(thing){} ~CIsThingAlive(){++state.predicateDestructors;} };
}
class TRetVoid { public: void Return(){} };
using NScript::CIsThingAlive;
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
    CCPPointerInfo(unsigned long count,void(*dispose)(void*),void* data):RefCount(count),DeleteFunc(dispose),Data(data){}
};
template<class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer():Data(0),Info(0){}
    ~CCountedPointer() {
        if(Info) { --Info->RefCount; if(Info->RefCount==0) {Info->DeleteFunc(Info->Data);delete Info;} }
        Data=0;Info=0;
    }
    void ReleaseData() {
        if(Info) { --Info->RefCount; if(Info->RefCount==0) {Info->DeleteFunc(Info->Data);delete Info;} }
        Data=0;Info=0;
    }
};
static void DisposeToken(void* p){assert(p);assert(*(int*)p==7);delete (int*)p;++state.frees;}
class CScriptGameResourceObjectBase {
public: virtual ~CScriptGameResourceObjectBase(){++state.baseDestructors;assert(state.allocations==state.frees);}
};
class CScriptGameResourceObjectScriptedThingBase:public CScriptGameResourceObjectBase {
public:
    CCountedPointer<int> PImp;
    CScriptGameResourceObjectScriptedThingBase(){++state.constructors;}
    virtual ~CScriptGameResourceObjectScriptedThingBase(){++state.destructors;}
    bool IsValid()const{return PImp.Data!=0;}
    void Release(){assert(IsValid());++state.releases;PImp.ReleaseData();}
    void FollowThing(const CScriptThing& hero,float distance,bool run){assert(IsValid()&&hero.Identity==1&&distance==1.0f&&run);++state.follows;}
    bool IsPerformingScriptTask()const{++state.taskReads;if(state.busy){--state.busy;return true;}return false;}
    void PlayAnimation(const CCharString& name,bool a,bool b,bool c,bool d,bool e,bool f,bool h);
};
enum EScriptAIPriority { PRIORITY_NONE=0 };
namespace NGlobalConsole { bool GDefaultToUsingPhysicsInCutsceneAnimations; }
class CGameScriptInterfaceBase {
public:
    void NewScriptFrame(){assert(state.predicates==1&&state.predicateDestructors==1);++state.frames;if(state.timer>0)--state.timer;}
    const CScriptThing& GetHero()const{return Hero;}
    bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing& hero,const CScriptThing& me)const{assert(hero.Identity==1&&me.Identity==2);++state.awareReads;return state.aware;}
    bool StartScriptingEntity(const CScriptThing& me,CScriptGameResourceObjectScriptedThingBase& handle,EScriptAIPriority priority){
        assert(me.Identity==2&&!handle.PImp.Data);assert(priority==3||priority==4);++state.starts;
        if(state.failures){--state.failures;return false;}
        handle.PImp.Data=new int(7);handle.PImp.Info=new CCPPointerInfo(1,DisposeToken,handle.PImp.Data);++state.allocations;return true;
    }
    void EntitySetInFaction(const CScriptThing& me,const CCharString& faction){assert(me.Identity==2&&faction.Text=="FACTION_BANDITS");++state.factions;}
    void EntitySetFacingAngleTowardsThing(const CScriptThing& me,const CScriptThing& hero,bool flag){assert(me.Identity==2&&hero.Identity==1&&!flag);++state.faces;state.busy=state.taskDuration;}
    long AddNewConversation(const CScriptThing& me,bool a,bool b){assert(me.Identity==2&&!a&&!b);return 13;}
    void AddPersonToConversation(long conversation,const CScriptThing& hero){assert(conversation==13&&hero.Identity==1);}
    void AddLineToConversation(long conversation,const CCharString& line,bool flag,const CScriptThing& me,const CScriptThing& hero){
        assert(conversation==13&&!flag&&me.Identity==2&&hero.Identity==1);
        const char* text[]={"TEXT_QST_011_BANDIT_LEADER_GO_AWAY","TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN","TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN_AGAIN","TEXT_QST_011_BANDIT_LEADER_GO_AWAY_SHORT"};
        long index=*state.currentTalk;index=index<3?index:3;assert(line.Text==text[index]);
        if(!state.lines)state.firstLine=(int)index;++state.lines;state.afterLine=true;
    }
    void SetTimer(long index,long seconds)const{
        assert(index==19);if(state.afterLine){long values[]={15,12,10,8};long kind=*state.currentTalk;assert(seconds==values[kind<3?kind:3]);state.afterLine=false;}else assert(seconds==8);
        state.timer=seconds;
    }
    long GetTimer(long index)const{assert(index==19);return state.timer;}
};
static CGameScriptInterfaceBase Interface;
static CGameScriptInterfaceBase* pDSTGame=&Interface;
class CTimer {
public: long TimerIndex;CTimer():TimerIndex(19){}
    CTimer& operator=(long time){pDSTGame->SetTimer(TimerIndex,time);return *this;}
    long GetTime()const{return pDSTGame->GetTimer(TimerIndex);}
    bool operator==(long time)const{return GetTime()==time;}
};
void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const CCharString& name,bool a,bool b,bool c,bool d,bool e,bool f,bool h){
    assert(IsValid()&&name.Text=="ST_OPINION_DISAPPROVAL_BOOING");assert(!a&&!b&&!c&&d&&e==NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations&&!f&&!h);
    assert(*state.currentTalk==0||state.randomValue%5==0);++state.animations;state.busy=state.taskDuration;
}
bool IsDistanceBetweenThingsUnder(const CScriptThing& me,const CScriptThing& hero,float distance){assert(me.Identity==2&&hero.Identity==1&&distance==10.0f);return state.nearHero;}
extern "C" int rand(void){return state.randomValue;}
namespace NScript {
class CV_BanditTollScript {
public:
    bool BanditsAngered;
    class CBanditLeader {
    public:
        CV_BanditTollScript& ParentClass; CGameScriptInterfaceBase* Game; CScriptThing Me; long TalkedToHero;CTimer TalkFrequency;
        CBanditLeader(CV_BanditTollScript& parent):ParentClass(parent),Game(&Interface),Me(2),TalkedToHero(0){}
        void SetPredicate(const CIsThingAlive& predicate){assert(predicate.Thing.Identity==2);++state.predicates;}
        bool IsActiveThreadTerminating()const{assert(state.checks<state.stop);return ++state.checks>=state.stop;}
        void Main();
    };
};
}

void NScript::CV_BanditTollScript::CBanditLeader::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.BanditsAngered && Me.IsAlive() && Game->IsThingAwareOfOtherThingInAnyWay(Game->GetHero(), Me)) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, (EScriptAIPriority)3)) {
                Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game->EntitySetInFaction(Me, "FACTION_BANDITS");
            seh_me.FollowThing(Game->GetHero(), 1.0f, true);
            for (;;) {
                Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        if (TalkFrequency == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            long HLStyleConversationIndex;
            switch (TalkedToHero) {
            case 0: {
                HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY", false, Me, Game->GetHero());
                TalkFrequency = (15);
                break;
            }
            case 1: {
                HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN", false, Me, Game->GetHero());
                TalkFrequency = (12);
                break;
            }
            case 2: {
                HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN_AGAIN", false, Me, Game->GetHero());
                TalkFrequency = (10);
                break;
            }
            default: {
                HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_SHORT", false, Me, Game->GetHero());
                TalkFrequency = (8);
                break;
            }
            }
            if (TalkedToHero == 0 || rand() % 5 == 0) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_BOOING", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                TalkFrequency = (8);
            }
            seh_me.Release();
            TalkedToHero++;
        }
    }
}

int main()
{
    unsigned int cases=0;
    for(int scenario=0;scenario<16;++scenario)for(int talk=0;talk<4;++talk)for(int wait=0;wait<4;++wait)for(int stop=1;stop<=40;++stop){
        memset(&state,0,sizeof(state));state.stop=stop;state.firstLine=-1;state.alive=(scenario&2)!=0;state.aware=(scenario&4)!=0;state.nearHero=(scenario&8)!=0;
        state.failures=(wait&1)?2:0;state.taskDuration=(wait&2)?2:0;state.randomValue=(talk&1)?6:5;
        NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations=(wait&1)!=0;
        NScript::CV_BanditTollScript parent;parent.BanditsAngered=(scenario&1)!=0;
        NScript::CV_BanditTollScript::CBanditLeader leader(parent);leader.TalkedToHero=talk;state.initialTalk=talk;state.currentTalk=&leader.TalkedToHero;
        leader.Main();
        assert(state.checks==stop&&state.frames>0&&state.frames<=stop+1);
        assert(state.predicates==1&&state.predicateDestructors==1);
        assert(state.constructors==(stop>1?1:0)&&state.destructors==state.constructors&&state.baseDestructors==state.constructors);
        assert(state.allocations==state.frees);
        assert(leader.TalkedToHero-talk==state.releases);
        if(!parent.BanditsAngered)assert(state.aliveReads==0&&state.awareReads==0);
        if(!state.alive)assert(state.awareReads==0);
        const bool attacks=parent.BanditsAngered&&state.alive&&state.aware;
        if(attacks){assert(state.lines==0&&state.faces==0&&state.animations==0&&state.releases==0);assert(state.follows==state.factions);}
        else {assert(state.follows==0&&state.factions==0);if(!state.nearHero)assert(state.lines==0&&state.faces==0&&state.starts==0);}
        if(state.lines)assert(state.firstLine==talk);
        if(stop==40){if(attacks)assert(state.follows==1);else if(state.nearHero)assert(state.lines>=1&&leader.TalkedToHero>talk);}
        ++cases;
    }
    assert(cases==10240U);puts("00e4aa00_TEST PASS");return 0;
}