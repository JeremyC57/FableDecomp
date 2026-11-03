#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
struct State {
    int frame,limit,checks,stopCheck,things,strings,resources,movies,pauses,predicates;
    int initialFailures,reclaimFailures,initialAttempts,reclaimAttempts,otherClaims;
    int siegeAt,secondAt,endAt,removed,removedFrame,screened,fade,objectCreated,attached;
    int cutscenes,bodyguards,questNames,objectives,heroHits,spellHits,healChecks,otherHits,releases;
    bool demands,startScreen,attacked,physical,ability,heal,other,success,heroAllowed;
    bool failOtherClaims,fixed;
    std::vector<std::string> destinations;
    std::vector<int> enabled;
    State():frame(0),limit(12),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauses(0),predicates(0),initialFailures(0),reclaimFailures(0),initialAttempts(0),reclaimAttempts(0),otherClaims(0),siegeAt(1),secondAt(1),endAt(1),removed(0),removedFrame(-1),screened(0),fade(0),objectCreated(0),attached(0),cutscenes(0),bodyguards(0),questNames(0),objectives(0),heroHits(0),spellHits(0),healChecks(0),otherHits(0),releases(0),demands(false),startScreen(false),attacked(false),physical(false),ability(false),heal(false),other(false),success(false),heroAllowed(false),failOtherClaims(false),fixed(false) {}
    bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck)||(removed&&frame>removedFrame);}
};
State* state;
class CCharString {
public:
    std::string text;
    CCharString(const char* p=""):text(p){++state->strings;}
    CCharString(const CCharString& p):text(p.text){++state->strings;}
    ~CCharString(){--state->strings;}
    bool operator<(const CCharString& p)const{return text<p.text;}
    bool operator==(const char* p)const{return text==p;}
};
class CScriptThing {
public:
    int id;
    CScriptThing(int n=0):id(n){++state->things;}
    CScriptThing(const CScriptThing& p):id(p.id){++state->things;}
    ~CScriptThing(){--state->things;}
    CCharString GetDataString()const {assert(id>=100&&id<100+(int)state->destinations.size());return CCharString(state->destinations[id-100].c_str());}
    bool MsgIsHitBy(const CCharString& p)const{assert(id==0);if(p.text=="SCRIPT_NAME_HERO"){++state->heroHits;return state->physical;}assert(p.text.empty());++state->otherHits;return state->other;}
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& p)const{assert(id==0&&p.text=="SCRIPT_NAME_HERO");++state->spellHits;return state->ability;}
    bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString& p)const{assert(id==0&&a==14&&p.text=="SCRIPT_NAME_HERO");++state->healChecks;return state->heal;}
};
class TRetVoid {public:void Return(){}};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL=14 };
class CScriptGameResourceObjectScriptedThingBase {
public:
    int id; bool valid;
    CScriptGameResourceObjectScriptedThingBase():id(-1),valid(false){++state->resources;}
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& p):id(p.id),valid(p.valid){++state->resources;}
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& p){id=p.id;valid=p.valid;return *this;}
    ~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}
    bool IsValid()const{return valid;}
    void Release(){assert(valid);valid=false;++state->releases;}
};
class CScriptGameResourceObjectMovieBase {
public:
    CScriptGameResourceObjectMovieBase(){++state->movies;assert(state->movies==1);}
    ~CScriptGameResourceObjectMovieBase(){assert(state->pauses==0);--state->movies;}
};
class CGameScriptInterfaceBase {
    CScriptThing hero;
public:
    CGameScriptInterfaceBase():hero(1){}
    void NewScriptFrame(){assert(state->frame<100);++state->frame;}
    CScriptThing& GetHero(){return hero;}
    bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& r,EScriptAIPriority p){assert(p==SCRIPT_AI_PRIORITY_HIGHEST);r.id=t.id;bool ok=true;if(t.id==0){if(!state->releases)ok=++state->initialAttempts>state->initialFailures;else ok=++state->reclaimAttempts>state->reclaimFailures;}else{assert(t.id>=1&&t.id<=3);++state->otherClaims;ok=!state->failOtherClaims;}r.valid=ok;return ok;}
    CScriptThing GetThingWithScriptName(const CCharString& n){if(n.text=="KG_ChiefBreakSiege")return CScriptThing(2);if(n.text=="KG_Sheriff")return CScriptThing(3);assert(n.text=="M_Blockage");return CScriptThing(4);}
    void FadeScreenOutUntilNextCallToFadeScreenIn(float a,float b){assert(a==0.5f&&b==0.0f&&state->movies==0);++state->fade;}
    void StartMovieSequence(const CCharString& n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty()&&state->movies==1&&state->cutscenes==0);}
    void PauseAllNonScriptedEntities(bool b){if(b){assert(state->movies==1&&state->pauses==0);++state->pauses;}else{assert(state->movies==1&&state->pauses==1);--state->pauses;}}
    void FixMovieSequenceCamera(bool b){assert(state->pauses==1);if(b)assert(state->cutscenes==0);else assert(state->cutscenes==2);state->fixed=b;}
    CScriptThing CreateObjectOnEntity(const CCharString& n,const CScriptThing& t,const CCharString& label){assert(n.text=="OBJECT_BANDIT_PALISADES_01"&&t.id==4&&label.text=="Blockage"&&state->bodyguards==3&&!state->fixed);++state->objectCreated;return CScriptThing(5);}
    CCharString GetActiveQuestName(){++state->questNames;return CCharString("Q_BreakSiege");}
    void EntityAttachToScript(const CScriptThing& t,const CCharString& n){assert(t.id==5&&n.text=="Q_BreakSiege"&&state->objectCreated==1);++state->attached;}
    long GetAllThingsWithScriptName(const CCharString& n,std::vector<CScriptThing>& out){assert(n.text=="TeleportPoint"&&state->attached==1);for(unsigned i=0;i<state->destinations.size();++i)out.push_back(CScriptThing(100+i));return (long)out.size();}
    void SetTeleporterAsActive(const CScriptThing& t,bool b){assert(t.id>=100&&b&&state->pauses==1);state->enabled.push_back(t.id-100);}
    void KickOffQuestStartScreen(const CCharString& n,bool a,bool b){assert(n.text=="Q_BreakSiege"&&a&&!b);++state->screened;}
    void SetQuestCardObjective(const CCharString& n,const CCharString& text,const CCharString& a,const CCharString& b){assert(n.text=="Q_BreakSiege"&&text.text=="TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_02"&&a.text=="KnotholeGlade"&&b.text=="KnotholeGlade"&&state->pauses==1);++state->objectives;}
    void RemoveThing(const CScriptThing& t,bool a,bool b){assert(t.id==0&&!a&&b&&state->movies==0&&state->pauses==0&&state->reclaimAttempts>state->reclaimFailures);++state->removed;state->removedFrame=state->frame;}
};
class CWideScreenMagicPauseEntities {
    CGameScriptInterfaceBase* Game;
public:
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& g):Game(&g){Game->PauseAllNonScriptedEntities(true);}
    ~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}
};
void RunCutsceneMacro(const CCharString& n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& m,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool a,bool b){assert(!a&&b&&state->movies==1&&state->pauses==1&&state->fixed);assert(m.find(CCharString("HERO"))->second.id==1&&m.find(CCharString("LEADER"))->second.id==0);if(!state->cutscenes){assert(n.text=="CS_BREAKSIEGE_GIVEN"&&m.size()==2&&state->bodyguards==1);}else{assert(state->cutscenes==1&&n.text=="CS_BREAKSIEGE_INTRO"&&m.size()==4&&state->bodyguards==2);assert(m.find(CCharString("CHIEF"))->second.id==2&&m.find(CCharString("GUARD"))->second.id==3);}++state->cutscenes;}
namespace NScript {
class CIsThingAlive {
    CScriptThing Thing;
public:
    CIsThingAlive(const CScriptThing& t):Thing(t){assert(t.id==0);++state->predicates;}
    ~CIsThingAlive(){--state->predicates;}
};
class CQ_BreakSiegeScript {
public:
    bool BanditDemandsKnown,QuestStartScreened,HeroLetInTown,BanditSiegeStart,StartSecondWave,AttackedBanditLeader,MissionSucceeded,EndCutsceneDone;
    CQ_BreakSiegeScript():BanditDemandsKnown(state->demands),QuestStartScreened(state->startScreen),HeroLetInTown(state->heroAllowed),BanditSiegeStart(false),StartSecondWave(false),AttackedBanditLeader(state->attacked),MissionSucceeded(state->success),EndCutsceneDone(false){}
    void SendBodyguardsToTheRightPlace(CCharString a,CCharString b){const char* first[]={"BodyguardTeleportMarker1_BSGIVEN","BodyguardTeleportMarker1_BSINTRO1","BodyguardTeleportMarker1_BSINTRO2"};const char* second[]={"BodyguardTeleportMarker2_BSGIVEN","BodyguardTeleportMarker2_BSINTRO1","BodyguardTeleportMarker2_BSINTRO2"};assert(state->bodyguards<3&&a.text==first[state->bodyguards]&&b.text==second[state->bodyguards]);assert(state->cutscenes==state->bodyguards);++state->bodyguards;}
    class CBanditSiegeLeader {
    public:
        CGameScriptInterfaceBase& Game; CScriptThing Me; CQ_BreakSiegeScript& ParentClass;
        CBanditSiegeLeader(CGameScriptInterfaceBase& g,CQ_BreakSiegeScript& p):Game(g),Me(0),ParentClass(p){}
        void SetPredicate(const CIsThingAlive&){assert(state->predicates==1);}
        bool IsActiveThreadTerminating()const{ParentClass.BanditSiegeStart=state->frame>=state->siegeAt;ParentClass.StartSecondWave=state->frame>=state->secondAt;ParentClass.EndCutsceneDone=state->frame>=state->endAt;return state->term();}
        void Main();
    };
};
}

void NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!ParentClass.BanditDemandsKnown) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.FadeScreenOutUntilNextCallToFadeScreenIn(0.5f,0.0f);
        CScriptThing Chief=Game.GetThingWithScriptName("KG_ChiefBreakSiege");
        CScriptThing Guard=Game.GetThingWithScriptName("KG_Sheriff");
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        CScriptGameResourceObjectScriptedThingBase seh_chief;
        CScriptGameResourceObjectScriptedThingBase seh_guard;
        Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
        Game.StartScriptingEntity(Chief,seh_chief,SCRIPT_AI_PRIORITY_HIGHEST);
        Game.StartScriptingEntity(Guard,seh_guard,SCRIPT_AI_PRIORITY_HIGHEST);
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSGIVEN","BodyguardTeleportMarker2_BSGIVEN");
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_hero;
        mymap["LEADER"]=seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        Game.FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_BREAKSIEGE_GIVEN",mymap,0,0,false,true);
        mymap["HERO"]=seh_hero;
        mymap["LEADER"]=seh_me;
        mymap["CHIEF"]=seh_chief;
        mymap["GUARD"]=seh_guard;
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSINTRO1","BodyguardTeleportMarker2_BSINTRO1");
        RunCutsceneMacro("CS_BREAKSIEGE_INTRO",mymap,0,0,false,true);
        Game.FixMovieSequenceCamera(false);
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSINTRO2","BodyguardTeleportMarker2_BSINTRO2");
        CScriptThing NoWayToKHG=Game.CreateObjectOnEntity("OBJECT_BANDIT_PALISADES_01",Game.GetThingWithScriptName("M_Blockage"),"Blockage");
        Game.EntityAttachToScript(NoWayToKHG,Game.GetActiveQuestName());
        std::vector<CScriptThing> Teleporters;
        Game.GetAllThingsWithScriptName("TeleportPoint",Teleporters);
        for (long i=0;i<Teleporters.size();++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Teleporters[i].GetDataString()=="KnotholeGlade") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.SetTeleporterAsActive(Teleporters[i],true);
                break;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.QuestStartScreened) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.KickOffQuestStartScreen(Game.GetActiveQuestName(),true,false);
            ParentClass.QuestStartScreened=true;
        }
        ParentClass.HeroLetInTown=true;
        Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_02","KnotholeGlade","KnotholeGlade");
    }
    if (!ParentClass.QuestStartScreened) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.KickOffQuestStartScreen(Game.GetActiveQuestName(),true,false);
        ParentClass.QuestStartScreened=true;
    }
    ParentClass.BanditDemandsKnown=true;
    seh_me.Release();
    while (!ParentClass.BanditSiegeStart) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.StartSecondWave) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.AttackedBanditLeader && (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Me.MsgIsHitBy(""))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.AttackedBanditLeader=true;
        }
        if (ParentClass.MissionSucceeded) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!ParentClass.EndCutsceneDone) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.RemoveThing(Me,false,true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
void run(State& s){state=&s;{CGameScriptInterfaceBase g;NScript::CQ_BreakSiegeScript p;NScript::CQ_BreakSiegeScript::CBanditSiegeLeader t(g,p);t.Main();s.demands=p.BanditDemandsKnown;s.startScreen=p.QuestStartScreened;s.heroAllowed=p.HeroLetInTown;s.attacked=p.AttackedBanditLeader;assert(s.things==2&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauses==0&&s.predicates==0);assert(s.removed<=1&&s.enabled.size()<=1);}assert(s.things==0);}
int main(){int cases=0;
    for(int flags=0;flags<256;++flags){State s;s.demands=(flags&1)!=0;s.startScreen=(flags&2)!=0;s.attacked=(flags&4)!=0;s.physical=(flags&8)!=0;s.ability=(flags&16)!=0;s.heal=(flags&32)!=0;s.other=(flags&64)!=0;s.success=(flags&128)!=0;bool intro=!s.demands,screen=!s.startScreen,hit=s.physical||(s.ability&&!s.heal)||s.other,was=s.attacked;s.destinations.push_back("");s.destinations.push_back("KnotholeGlade");s.destinations.push_back("KnotholeGlade");run(s);assert(s.demands&&s.startScreen&&s.attacked==(was||hit)&&s.heroAllowed==intro);assert(s.fade==int(intro)&&s.cutscenes==2*int(intro)&&s.bodyguards==3*int(intro)&&s.otherClaims==3*int(intro)&&s.objectCreated==int(intro)&&s.attached==int(intro)&&s.objectives==int(intro));assert(s.screened==int(screen)&&s.removed==int(s.success)&&s.releases==1&&s.initialAttempts==1&&s.reclaimAttempts==int(s.success));assert(s.enabled.size()==unsigned(intro));if(intro)assert(s.enabled[0]==1);if(was)assert(s.heroHits==0&&s.spellHits==0&&s.healChecks==0&&s.otherHits==0);if(s.physical)assert(s.spellHits==0&&s.healChecks==0&&s.otherHits==0);++cases;}
    for(int length=0;length<7;++length)for(int match=-1;match<length;++match){State s;for(int i=0;i<length;++i)s.destinations.push_back(i==match?"KnotholeGlade":"Oakvale");run(s);assert(s.enabled.size()==unsigned(match>=0));if(match>=0)assert(s.enabled[0]==match);++cases;}
    for(int first=0;first<4;++first)for(int second=0;second<4;++second){State s;s.demands=true;s.success=true;s.initialFailures=first;s.reclaimFailures=second;s.limit=30;run(s);assert(s.initialAttempts==first+1&&s.reclaimAttempts==second+1&&s.removed==1&&s.frame==first+second+2);++cases;}
    for(int mode=0;mode<3;++mode){State s;s.demands=true;s.success=true;s.siegeAt=mode==0?50:1;s.secondAt=mode==1?50:1;s.endAt=mode==2?50:1;s.limit=5;run(s);assert(s.frame==5&&!s.removed&&s.releases==1&&s.reclaimAttempts==int(mode==2));++cases;}
    {State s;s.demands=true;s.success=true;s.siegeAt=3;s.secondAt=5;s.endAt=7;s.limit=30;run(s);assert(s.removed==1&&s.removedFrame==7&&s.frame==8);++cases;}
    {State s;s.failOtherClaims=true;run(s);assert(s.otherClaims==3&&s.cutscenes==2&&s.demands&&s.objectives==1);++cases;}
    {State s;s.initialFailures=50;s.limit=5;run(s);assert(s.initialAttempts==4&&s.releases==0&&s.fade==0&&!s.demands);++cases;}
    {State s;s.demands=true;s.success=true;s.reclaimFailures=50;s.limit=5;run(s);assert(s.reclaimAttempts==4&&s.releases==1&&!s.removed);++cases;}
    {State s;s.limit=0;run(s);assert(s.frame==1&&s.initialAttempts==0&&s.fade==0);++cases;}
    for(int mode=0;mode<12;++mode)for(int stop=1;stop<=160;++stop){State s;s.stopCheck=stop;s.limit=20;s.demands=mode&1;s.startScreen=mode&2;s.attacked=mode&4;s.physical=mode==2;s.ability=mode==3||mode==4;s.heal=mode==4;s.other=mode==5;s.success=mode>=6;s.initialFailures=mode==6?50:mode%3;s.reclaimFailures=mode==7?50:mode%2;s.siegeAt=mode==8?50:1;s.secondAt=mode==9?50:3;s.endAt=mode==10?50:4;s.failOtherClaims=mode==11;s.destinations.push_back("");s.destinations.push_back("KnotholeGlade");run(s);assert(s.checks<=stop);++cases;}
    assert(cases==2228);std::puts("00d21b40_TEST PASS");
}
