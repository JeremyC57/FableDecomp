#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
struct State{int frame,limit,checks,stopCheck,things,strings,resources,predicates,startAttempts,startFailures,animationDelay,taskUntil,animations,conversations,people,info,clearInfo,stopFollowing,faces,physicalChecks,abilityChecks,healChecks,expressionChecks,releaseCount,siegeFrame,endFrame;long initialTalks;bool physical,ability,heal,talk,noAssistance,allowFollow;std::string command;std::vector<std::string>lines;std::vector<int>priorities,allies;State():frame(0),limit(8),checks(0),stopCheck(0),things(0),strings(0),resources(0),predicates(0),startAttempts(0),startFailures(0),animationDelay(0),taskUntil(0),animations(0),conversations(0),people(0),info(0),clearInfo(0),stopFollowing(0),faces(0),physicalChecks(0),abilityChecks(0),healChecks(0),expressionChecks(0),releaseCount(0),siegeFrame(100),endFrame(100),initialTalks(0),physical(false),ability(false),heal(false),talk(false),noAssistance(true),allowFollow(false){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString():text(){++state->strings;}CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}bool operator==(const char*s)const{return text==s;}CCharString&operator=(const char*s){text=s;return *this;}};
CCharString operator+(const char*a,const CCharString&b){return CCharString((std::string(a)+b.text).c_str());}CCharString operator+(const CCharString&a,const char*b){return CCharString((a.text+b).c_str());}
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGH=3,SCRIPT_AI_PRIORITY_HIGHEST=4};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};class TRetVoid{public:void Return(){}};
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations=true;}
class CScriptThing{public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}void target(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");}bool MsgIsTalkedToBy(const CCharString&s)const{target(s);return state->talk;}bool MsgIsHitBy(const CCharString&s)const{target(s);++state->physicalChecks;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{target(s);++state->abilityChecks;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{target(s);assert(a==HERO_ABILITY_HEAL_LIFE_SPELL);++state->healChecks;return state->heal;}bool MsgExpressionPerformedTo(CCharString&s)const{assert(id==-1);++state->expressionChecks;s=state->command.c_str();return !state->command.empty();}CCharString GetDataString()const{assert(id==-1);return CCharString("GUARD_1");}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}bool IsValid()const{return valid;}void Release(){valid=false;++state->releaseCount;}void PlayAnimation(const CCharString&s,bool a,bool b,bool c,bool d,bool e,bool f,bool g){assert(valid&&s.text=="SALUTE_STAND_TO_ATTENTION_INTO"&&!a&&!b&&!c&&d&&e==NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations&&!f&&!g);++state->animations;state->taskUntil=state->frame+state->animationDelay;}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:bool*siege,*end;CGameScriptInterfaceBase():hero(-2),siege(0),end(0){}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(siege&&state->frame>=state->siegeFrame)*siege=true;if(end&&state->frame>=state->endFrame)*end=true;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(m.id==-1&&(p==SCRIPT_AI_PRIORITY_HIGH||p==SCRIPT_AI_PRIORITY_HIGHEST));++state->startAttempts;state->priorities.push_back(p);r.valid=state->startAttempts>state->startFailures;return r.valid;}void SetThingHasInformation(const CScriptThing&m,bool a,bool b,bool c){assert(m.id==-1&&!a&&b&&!c);++state->info;}void ClearThingHasInformation(const CScriptThing&m){assert(m.id==-1);++state->clearInfo;}void EntitySetAsAllowedToFollowHero(const CScriptThing&m,bool b){assert(m.id==-1);state->allowFollow=b;}void EntityStopFollowing(const CScriptThing&m){assert(m.id==-1);++state->stopFollowing;}CScriptThing&GetHero(){return hero;}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="ImprisonedBanditLeader");return CScriptThing(1);}void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert((a.id==-1&&b.id==1)||(a.id==1&&b.id==-1));state->allies.push_back(a.id);}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&h,bool b){assert(m.id==-1&&h.id==-2&&!b);++state->faces;}long AddNewConversation(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&!a&&!b);return ++state->conversations;}void AddPersonToConversation(long i,const CScriptThing&h){assert(i==state->conversations&&h.id==-2);++state->people;}void AddLineToConversation(long i,const CCharString&s,bool b,const CScriptThing&m,const CScriptThing&h){assert(i==state->conversations&&!b&&m.id==-1&&h.id==-2);state->lines.push_back(s.text);}};
namespace NScript{class CIsThingConscious{public:CIsThingConscious(const CScriptThing&m){assert(m.id==-1);}};class CQ_SunnyvaleMasterData{public:bool BreakSiegeNoAssistance;CQ_SunnyvaleMasterData():BreakSiegeNoAssistance(true){}};class CQ_BreakSiegeScript{public:bool BanditSiegeStart,EndCutsceneDone;CQ_BreakSiegeScript():BanditSiegeStart(state->siegeFrame==0),EndCutsceneDone(state->endFrame==0){}class CGuardToHelpDefend{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_BreakSiegeScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;long TimesSpokenTo;CGuardToHelpDefend(CGameScriptInterfaceBase&g,CQ_BreakSiegeScript&p,CQ_SunnyvaleMasterData&d):Game(g),Me(-1),ParentClass(p),PMasterData(&d),TimesSpokenTo(state->initialTalks){}void SetPredicate(const CIsThingConscious&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CQ_BreakSiegeScript::CGuardToHelpDefend::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetThingHasInformation(Me,false,true,false);
    Game.EntitySetAsAllowedToFollowHero(Me,true);
    CCharString expression;
    CScriptThing ThePrisoner=Game.GetThingWithScriptName("ImprisonedBanditLeader");
    Game.EntitySetThingAsAllyOfThing(Me,ThePrisoner);
    Game.EntitySetThingAsAllyOfThing(ThePrisoner,Me);
    while (!ParentClass.BanditSiegeStart) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            switch (TimesSpokenTo) {
            case 0: {
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_TALK_ONCE",false,Me,Game.GetHero());
                break;
            }
            case 1: {
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_TALK_TWICE",false,Me,Game.GetHero());
                break;
            }
            case 2: {
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_TALK_THRICE",false,Me,Game.GetHero());
                break;
            }
            default: {
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_TALK_WILL_FOLLOW",false,Me,Game.GetHero());
                break;
            }
            }
            ++TimesSpokenTo;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_ON_HIT",false,Me,Game.GetHero());
        }
        if (Me.MsgExpressionPerformedTo(expression)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (expression=="EXPRESSION_FOLLOW") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                seh_me.PlayAnimation("SALUTE_STAND_TO_ATTENTION_INTO",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_FOLLOW",false,Me,Game.GetHero());
                PMasterData->BreakSiegeNoAssistance=false;
                seh_me.Release();
            } else if (expression=="EXPRESSION_WAIT") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.ClearThingHasInformation(Me);
    while (!ParentClass.EndCutsceneDone) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySetAsAllowedToFollowHero(Me,false);
    Game.EntityStopFollowing(Me);
    seh_me.Release();
    while (!false) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

long run(State&s){state=&s;long talks;{CGameScriptInterfaceBase g;NScript::CQ_BreakSiegeScript p;NScript::CQ_SunnyvaleMasterData d;g.siege=&p.BanditSiegeStart;g.end=&p.EndCutsceneDone;NScript::CQ_BreakSiegeScript::CGuardToHelpDefend t(g,p,d);t.Main();talks=t.TimesSpokenTo;s.noAssistance=d.BreakSiegeNoAssistance;assert(s.things==2&&s.strings==0&&s.resources==0&&s.predicates==1&&s.people==s.conversations&&s.lines.size()==unsigned(s.conversations));}assert(s.things==0);return talks;}
int main(){int cases=0;
 {State s;run(s);assert(s.info==1&&s.allowFollow&&s.clearInfo==0&&s.allies.size()==2&&s.allies[0]==-1&&s.allies[1]==1&&s.lines.empty());++cases;}
 {State s;s.siegeFrame=0;s.endFrame=0;run(s);assert(s.clearInfo==1&&!s.allowFollow&&s.stopFollowing==1&&s.releaseCount==1&&s.expressionChecks==0);++cases;}
 {State s;s.siegeFrame=3;s.endFrame=5;run(s);assert(s.clearInfo==1&&!s.allowFollow&&s.stopFollowing==1&&s.expressionChecks==2);++cases;}
 for(int initial=-1;initial<=4;++initial){State s;s.talk=true;s.limit=7;s.initialTalks=initial;assert(run(s)==initial+5);assert(s.lines.size()==5);for(int i=0;i<5;++i){int n=initial+i;std::string suffix=n==0?"_TALK_ONCE":n==1?"_TALK_TWICE":n==2?"_TALK_THRICE":"_TALK_WILL_FOLLOW";assert(s.lines[i]=="TEXT_QST_018_GUARD_1"+suffix);}++cases;}
 for(int flags=0;flags<8;++flags){State s;s.limit=3;s.physical=(flags&1)!=0;s.ability=(flags&2)!=0;s.heal=(flags&4)!=0;run(s);assert(s.physicalChecks==1&&s.abilityChecks==int(!s.physical)&&s.healChecks==int(!s.physical&&s.ability));bool hit=s.physical||(s.ability&&!s.heal);assert(s.lines.size()==unsigned(hit));if(hit)assert(s.lines[0]=="TEXT_QST_018_GUARD_1_ON_HIT");++cases;}
 for(int cmd=0;cmd<5;++cmd)for(int physics=0;physics<2;++physics){State s;s.limit=3;s.command=cmd==0?"EXPRESSION_FOLLOW":cmd==1?"EXPRESSION_WAIT":cmd==2?"expression_follow":cmd==3?"UNRELATED":"";NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations=physics!=0;run(s);assert(s.animations==int(cmd==0)&&s.noAssistance==(cmd!=0));assert(s.startAttempts==(cmd<2?2:1));if(cmd<2)assert(s.priorities[1]==(cmd==0?4:3));if(cmd==0)assert(s.lines[0]=="TEXT_QST_018_GUARD_1_FOLLOW");++cases;}
 {State s;s.startFailures=3;run(s);assert(s.startAttempts==4&&s.info==1);++cases;}
 {State s;s.startFailures=100;s.limit=5;run(s);assert(s.info==0&&s.allies.empty());++cases;}
 {State s;s.command="EXPRESSION_FOLLOW";s.animationDelay=100;s.limit=5;run(s);assert(s.animations==1&&s.noAssistance&&s.lines.empty());++cases;}
 {State s;s.command="EXPRESSION_FOLLOW";s.animationDelay=2;s.limit=5;run(s);assert(s.animations==1&&!s.noAssistance&&s.lines.size()==1);++cases;}
 for(int mode=0;mode<10;++mode)for(int stop=1;stop<=100;++stop){State s;s.limit=20;s.stopCheck=stop;s.talk=mode==1;s.physical=mode==2;s.ability=mode==3;s.command=mode==4||mode==5?"EXPRESSION_FOLLOW":mode==6?"EXPRESSION_WAIT":"";s.animationDelay=mode==5?5:0;s.startFailures=mode==7?5:0;s.siegeFrame=mode>=8?3:100;s.endFrame=mode==9?5:100;run(s);assert(s.checks<=stop);++cases;}
 assert(cases==1031);std::puts("00d1eb80_TEST PASS");}
