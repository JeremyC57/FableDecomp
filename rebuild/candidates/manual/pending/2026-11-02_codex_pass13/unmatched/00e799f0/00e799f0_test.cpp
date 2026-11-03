#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <limits>
struct State{int frame,limit,checks,stop,things,strings,resources,movies,depth,paused,unpaused,timers,registered,deregistered,starts,retries,predicates,releases,delay,waits,stage,finalStage,faces,nearReads[2],speech[3],intro,objective,conversations,people,lines,hitReads,specialReads,healReads,healthReads,talkReads;long values[2];int reads[2],sets[2];bool talk,explained,finalExplained,married,physical,ability,heal,near10,near15,valid;float health;std::vector<int> destruction;State():frame(0),limit(3),checks(0),stop(0),things(0),strings(0),resources(0),movies(0),depth(0),paused(0),unpaused(0),timers(0),registered(0),deregistered(0),starts(0),retries(0),predicates(0),releases(0),delay(0),waits(0),stage(0),finalStage(0),faces(0),intro(0),objective(0),conversations(0),people(0),lines(0),hitReads(0),specialReads(0),healReads(0),healthReads(0),talkReads(0),talk(false),explained(false),finalExplained(false),married(false),physical(false),ability(false),heal(false),near10(false),near15(false),valid(false),health(100){for(int i=0;i<2;++i){nearReads[i]=reads[i]=sets[i]=0;values[i]=0;}for(int i=0;i<3;++i)speech[i]=0;}bool term(){assert(checks<1000);++checks;return frame>=limit||(stop&&checks>=stop);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&s):text(s.text){++state->strings;}~CCharString(){--state->strings;}operator const char*()const{return text.c_str();}};CCharString operator+(const CCharString&a,const char*b){return CCharString((a.text+b).c_str());}
class CScriptThing{public:int id;CScriptThing(int x=-1):id(x){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->talkReads;return state->talk;}bool MsgIsHitBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->hitReads;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->specialReads;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString&s)const{assert(id==-1&&a==14&&s.text=="SCRIPT_NAME_HERO");++state->healReads;return state->heal;}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&m,const CScriptThing&h,float n){assert(m.id==-1&&h.id==-2);if(n==10){++state->nearReads[0];return state->near10;}assert(n==15);++state->nearReads[1];return state->near15;}
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum ETextGroupSelectionMethod{GROUP_SELECT_RANDOM_NO_REPEAT=2};class TRetVoid{public:void Return(){}};
class CScriptGameResourceObjectScriptedThingBase{public:int left;bool valid;CScriptGameResourceObjectScriptedThingBase():left(0),valid(state->valid){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;}CScriptThing GetScriptThing()const{return CScriptThing(10);}void Speak(const CScriptThing&h,const char*t,int a,bool b,bool c,bool d){assert(h.id==-2&&a==2&&!b&&c&&!d&&state->depth==1&&valid);std::string name(t);if(name=="TEXT_QST_B18_RHODRI_LETTER")++state->speech[0];else{std::string tail=state->married?"_MARRIED":"";if(name=="TEXT_QST_B18_RHODRI_CHATTER"+tail)++state->speech[1];else{assert(name=="TEXT_QST_B18_RHODRI_ATTACKED"+tail);++state->speech[2];}}left=state->delay;}bool IsPerformingScriptTask(){++state->waits;return left-->0;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(!state->movies);++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(!state->depth);--state->movies;}};
class CTimer{int id;public:CTimer():id(state->registered++){assert(id<2);++state->timers;}~CTimer(){assert(id==state->timers-1);--state->timers;++state->deregistered;state->destruction.push_back(id);}bool operator==(long n)const{assert(n==0);++state->reads[id];return state->values[id]==0;}CTimer&operator=(long n){assert(n==(id==0?7:4));state->values[id]=n;++state->sets[id];return *this;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<100);++state->frame;}CScriptThing&GetHero(){return hero;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(m.id==-1&&p==4);++state->starts;r.valid=state->starts>state->retries;return r.valid;}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&h,bool b){assert(m.id==-1&&h.id==-2&&!b);++state->faces;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1&&!state->depth);}void PauseAllNonScriptedEntities(bool b){if(b){assert(!state->depth);++state->depth;++state->paused;}else{assert(state->depth==1);--state->depth;++state->unpaused;}}CCharString GetActiveQuestName(){return "Q_ExposeMayor";}void SetQuestCardObjective(const CCharString&q,const CCharString&a,const CCharString&b,const CCharString&c){assert(q.text=="Q_ExposeMayor"&&a.text=="TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_02"&&b.text=="HauntedHouse"&&c.text.empty());++state->objective;}float GetHealth(const CScriptThing&m){assert(m.id==10);++state->healthReads;return state->health;}long AddNewConversation(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&!a&&!b&&!state->depth);++state->conversations;return 91;}void AddPersonToConversation(long n,const CScriptThing&h){assert(n==91&&h.id==-2);++state->people;}void AddLineToConversation(long n,const CCharString&t,bool b,const CScriptThing&m,const CScriptThing&h){assert(n==91&&t.text=="TEXT_QST_B18_RHODRI_ATTENTION"&&!b&&m.id==-1&&h.id==-2);++state->lines;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase&Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(g){Game.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game.PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingConscious{CScriptThing Thing;public:CIsThingConscious(const CScriptThing&t):Thing(t){assert(t.id==-1);}};class CQ_SunnyvaleMasterData{public:bool HeroMarriedLadyGrey;CQ_SunnyvaleMasterData():HeroMarriedLadyGrey(state->married){}};class CV_ExposeMayorScript{public:enum STAGE_IN_PATH{STAGE_NONE=0,STAGE_TWO=1,STAGE_THREE=2,STAGE_FOUR=3,STAGE_FIVE=4,STAGE_PATH_COMPLETE=5,STAGE_MAX=6};STAGE_IN_PATH StageNumber;CScriptGameResourceObjectScriptedThingBase seh_Rhodri;CV_ExposeMayorScript():StageNumber((STAGE_IN_PATH)state->stage){}void PlayCutscene(CCharString a,bool b,CCharString c){assert(a.text=="CS_MAYOREXPOSE_RHODRI"&&b&&c.text=="NULL"&&!state->depth&&StageNumber==STAGE_TWO);++state->intro;}class CEM_Rhodri{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CV_ExposeMayorScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;bool ExplainedPlight;CEM_Rhodri(CGameScriptInterfaceBase&g,CV_ExposeMayorScript&p,CQ_SunnyvaleMasterData&d):Game(g),ParentClass(p),PMasterData(&d),ExplainedPlight(state->explained){}void SetPredicate(const CIsThingConscious&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CV_ExposeMayorScript::CEM_Rhodri::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase &seh_Rhodri=ParentClass.seh_Rhodri;
        if (seh_Rhodri.IsValid()) seh_Rhodri.Release();
    }
    while (!Game.StartScriptingEntity(Me,ParentClass.seh_Rhodri,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer TalkTimer;
    CTimer FaceHeroTimer;
    while (ParentClass.StageNumber!=STAGE_PATH_COMPLETE) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (FaceHeroTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),10.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            FaceHeroTimer=4;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.StageNumber==STAGE_TWO) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ExplainedPlight=true;
                ParentClass.PlayCutscene("CS_MAYOREXPOSE_RHODRI",true,"NULL");
                ParentClass.StageNumber=STAGE_THREE;
                Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_02","HauntedHouse","");
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (ParentClass.StageNumber==STAGE_FIVE) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Rhodri.GetScriptThing())>0.0f) {
                        ParentClass.seh_Rhodri.Speak(Game.GetHero(),"TEXT_QST_B18_RHODRI_LETTER",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Rhodri.GetScriptThing())>0.0f) {
                        ParentClass.seh_Rhodri.Speak(Game.GetHero(),CCharString("TEXT_QST_B18_RHODRI_CHATTER")+(PMasterData->HeroMarriedLadyGrey?"_MARRIED":""),GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(ParentClass.seh_Rhodri.GetScriptThing())>0.0f) {
                ParentClass.seh_Rhodri.Speak(Game.GetHero(),CCharString("TEXT_QST_B18_RHODRI_ATTACKED")+(PMasterData->HeroMarriedLadyGrey?"_MARRIED":""),GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (!ExplainedPlight && TalkTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TalkTimer=7;
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B18_RHODRI_ATTENTION",false,Me,Game.GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static int cases=0;void run(State&s,bool complete){state=&s;++cases;bool entered=s.stage!=5;bool intro=entered&&s.talk&&s.stage==1;bool talk=entered&&s.talk&&s.stage!=1;bool hit=entered&&(s.physical||(s.ability&&!s.heal));bool explained=s.explained||intro;bool face=entered&&s.values[1]==0&&s.near10;bool attention=entered&&!explained&&s.values[0]==0&&s.near15;bool good=s.health>0;long oldTalk=s.values[0],oldFace=s.values[1];if(complete)s.limit=3+s.retries+s.delay*((talk&&good?1:0)+(hit&&good?1:0));
 {CGameScriptInterfaceBase game;NScript::CV_ExposeMayorScript parent;NScript::CQ_SunnyvaleMasterData data;NScript::CV_ExposeMayorScript::CEM_Rhodri me(game,parent,data);me.Main();s.finalStage=parent.StageNumber;s.finalExplained=me.ExplainedPlight;if(complete){assert(s.predicates==1&&s.starts==s.retries+1&&s.releases==(s.valid?1:0));assert(s.registered==2&&s.deregistered==2);}}
 assert(!s.things&&!s.strings&&!s.resources&&!s.movies&&!s.depth&&!s.timers);assert(s.paused==s.unpaused&&s.registered==s.deregistered);if(complete){assert(s.finalStage==(intro?2:s.stage)&&s.finalExplained==explained);assert(s.intro==(intro?1:0)&&s.objective==s.intro);assert(s.faces==(face?1:0)&&s.sets[1]==s.faces);assert(s.reads[1]==(entered?1:0));assert(s.nearReads[0]==(entered&&oldFace==0?1:0));assert(s.reads[0]==(entered&&!explained?1:0));assert(s.nearReads[1]==(entered&&!explained&&oldTalk==0?1:0));assert(s.sets[0]==(attention?1:0));assert(s.conversations==s.sets[0]&&s.people==s.conversations&&s.lines==s.conversations);assert(s.paused==(talk?1:0)+(hit?1:0));assert(s.healthReads==(talk?1:0)+(hit?1:0));assert(s.speech[0]==(talk&&s.stage==4&&good?1:0));assert(s.speech[1]==(talk&&s.stage!=4&&good?1:0));assert(s.speech[2]==(hit&&good?1:0));assert(s.talkReads==(entered?1:0)&&s.hitReads==(entered?1:0));assert(s.specialReads==(entered&&!s.physical?1:0));assert(s.healReads==(entered&&!s.physical&&s.ability?1:0));assert(s.destruction.size()==2&&s.destruction[0]==1&&s.destruction[1]==0);}}
int main(){for(int stage=0;stage<7;++stage)for(int mask=0;mask<64;++mask){State s;s.stage=stage;s.talk=(mask&1)!=0;s.explained=(mask&2)!=0;s.married=(mask&4)!=0;s.physical=(mask&8)!=0;s.ability=(mask&16)!=0;s.heal=(mask&32)!=0;run(s,true);}float h[4]={100,0,-10,std::numeric_limits<float>::quiet_NaN()};for(int i=0;i<4;++i)for(int st=0;st<7;++st)for(int m=0;m<4;++m){State s;s.stage=st;s.health=h[i];s.talk=(m&1)!=0;s.physical=(m&2)!=0;run(s,true);}for(int a=-2;a<=4;++a)for(int b=-2;b<=4;++b){State s;s.values[0]=a;s.values[1]=b;s.near10=s.near15=true;run(s,true);}for(int d=0;d<4;++d)for(int r=0;r<4;++r)for(int mode=0;mode<16;++mode){State s;s.delay=d;s.retries=r;s.near10=(mode&1)!=0;s.near15=(mode&2)!=0;s.stage=(mode&4)?4:2;s.married=(mode&8)!=0;s.talk=s.physical=true;run(s,true);}for(int valid=0;valid<2;++valid){State s;s.valid=valid!=0;run(s,true);}for(int stop=1;stop<=100;++stop)for(int mode=0;mode<12;++mode){State s;s.stop=stop;s.limit=12;s.talk=(mode&1)!=0;s.physical=(mode&2)!=0;s.married=(mode&4)!=0;s.explained=(mode&8)!=0;s.stage=mode%7;s.retries=mode%4;s.delay=mode%4;s.near10=s.near15=true;run(s,false);}assert(cases==2067);std::puts("00e799f0_TEST PASS");return 0;}
