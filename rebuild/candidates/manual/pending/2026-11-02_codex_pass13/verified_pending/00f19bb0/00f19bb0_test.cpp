// Selected source SHA256 42187b4a95595d58272d6bf6546d23ed2f7a3dc89829c98e0edc794c3e8110a3
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct State {
 int arenaState,introUntil,cutStart,cutUntil,bertStart,bertUntil,frame,limit,checks,stopCheck;
 int things,strings,resources,movies,pauseDepth,paused,unpaused,startAttempts,startFailures,taskDelay,taskUntil,cleared,released,modified,allies,healthChecks,physicalChecks,abilityChecks,healChecks,talkChecks,clearedEnemy,facing,villageAssigned,validOnDestruct,moveDelay,moveUntil;
 bool early,cham,talk,physical,ability,heal,inHit,initialValid,busyOnClaim,meAlive,markerAlive;
 float health,distance;
 std::vector<std::string> speech,setup;std::vector<int> moves;
 State():arenaState(3),introUntil(0),cutStart(0),cutUntil(0),bertStart(0),bertUntil(0),frame(0),limit(1),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),paused(0),unpaused(0),startAttempts(0),startFailures(0),taskDelay(0),taskUntil(0),cleared(0),released(0),modified(0),allies(0),healthChecks(0),physicalChecks(0),abilityChecks(0),healChecks(0),talkChecks(0),clearedEnemy(0),facing(0),villageAssigned(0),validOnDestruct(0),moveDelay(1),moveUntil(0),early(false),cham(false),talk(false),physical(false),ability(false),heal(false),inHit(false),initialValid(false),busyOnClaim(false),meAlive(true),markerAlive(true),health(100.0f),distance(0.0f){}
 int stage()const{return frame<introUntil?2:(frame>=cutStart&&frame<cutUntil?4:arenaState);}bool bert()const{return frame>=bertStart&&frame<bertUntil;}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}
};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}};
class C3DVector{public:float X,Y,Z;C3DVector():X(1.5f),Y(-2.0f),Z(3.25f){}};
class CScriptThing{public:int id;bool fromHandle;CScriptThing(int n=-1,bool h=false):id(n),fromHandle(h){++state->things;}CScriptThing(const CScriptThing&o):id(o.id),fromHandle(o.fromHandle){++state->things;}CScriptThing&operator=(const CScriptThing&o){assert(id==0&&o.id==10);id=o.id;fromHandle=o.fromHandle;++state->villageAssigned;return *this;}~CScriptThing(){--state->things;}const C3DVector&GetPos()const{assert(id==1);static C3DVector p;return p;}float GetAngleXY()const{assert(id==1);return 1.25f;}void target(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");}bool MsgIsTalkedToBy(const CCharString&s)const{target(s);++state->talkChecks;return state->talk;}bool MsgIsHitBy(const CCharString&s)const{target(s);++state->physicalChecks;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{target(s);++state->abilityChecks;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString&s)const{target(s);assert(a==14);++state->healChecks;return state->heal;}};
bool IsDistanceBetweenThingsOver(const CScriptThing&a,const CScriptThing&b,float d){assert(a.id==-1&&b.id==1&&d==1.0f);return state->meAlive&&state->markerAlive&&state->distance*state->distance>d*d;}
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1,ENTITY_MOVE_SNEAK=2 };
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(state->initialValid){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->movies==0&&state->pauseDepth==0);state->validOnDestruct=valid;--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->released;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-1,true);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.id==-2&&valid&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauseDepth==1);state->speech.push_back(s);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}void ClearCommands(){assert(valid);++state->cleared;state->taskUntil=state->frame;}void MoveToPosition(const C3DVector&p,float f,EScriptEntityMoveType m,bool a,bool b){assert(valid&&p.X==1.5f&&p.Y==-2.0f&&p.Z==3.25f&&f==0.0f&&!a&&b);assert(m==ENTITY_MOVE_WALK||m==ENTITY_MOVE_RUN);state->moves.push_back(m);state->taskUntil=state->moveUntil=state->frame+state->moveDelay;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;assert(state->movies==1);}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m){assert(m.id==-1&&!m.fromHandle);}CScriptThing GetNearestWithDefName(const CScriptThing&m,const CCharString&s){self(m);assert(s.text=="VILLAGE_ARENA_CELLS");state->setup.push_back("village");return CScriptThing(10);}void EntitySetOpinionReactionsEnabled(const CScriptThing&m,bool b){self(m);assert(!b);state->setup.push_back("reactions");}void EntitySetInFaction(const CScriptThing&m,const CCharString&s){self(m);assert(s.text=="FACTION_HERO");state->setup.push_back("faction");}void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b);state->setup.push_back("killable");}void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&m,bool b){self(m);assert(!b);state->setup.push_back("combo");}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="GuardingDoorMarkerLeft");return CScriptThing(1);}void ClearHeroEnemyOfGuards(const CScriptThing&m){assert(m.id==10);++state->clearedEnemy;}void EntitySetFacingAngle(const CScriptThing&m,float a,bool b){self(m);assert(a==1.25f&&!b);++state->facing;}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(state->moveUntil&&state->frame>=state->moveUntil)state->distance=0.0f;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){self(m);assert(p==SCRIPT_AI_PRIORITY_HIGHEST);r.valid=++state->startAttempts>state->startFailures;if(r.valid&&state->busyOnClaim)state->taskUntil=state->frame+20;return r.valid;}CScriptThing&GetHero(){return hero;}float GetHealth(const CScriptThing&m){assert(m.id==-1&&m.fromHandle&&state->pauseDepth==1);++state->healthChecks;return state->health;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->paused;}else{assert(state->pauseDepth==1);--state->pauseDepth;++state->unpaused;}}void ModifyThingHealth(const CScriptThing&m,float f,bool b){self(m);assert(f==10000.0f&&!b&&state->pauseDepth==1);++state->modified;}void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert(state->pauseDepth==1);if(state->allies%2==0)assert(a.id==-1&&b.id==-2);else assert(a.id==-2&&b.id==-1);++state->allies;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CQ_ArenaScript{public:enum {MAS_CELLS_PRE_ARENA_INTRO=2,MAS_CELLS_PRE_ARENA_NORMAL=3,MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE=4,MAS_CELLS_PRE_ARENA_NORMAL_GO=5};long ArenaState;bool InHitCutsceneAlready,NeedBertForSpeech;CScriptThing CellsVillage;CQ_ArenaScript():ArenaState(state->stage()),InHitCutsceneAlready(state->inHit),NeedBertForSpeech(state->bert()),CellsVillage(0){}class CArenaCellDoorGuard2{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_ArenaScript&ParentClass;bool EarlyTalk,ChamTalk;CArenaCellDoorGuard2(CGameScriptInterfaceBase&g,CQ_ArenaScript&p):Game(g),Me(-1),ParentClass(p),EarlyTalk(state->early),ChamTalk(state->cham){}bool IsActiveThreadTerminating()const{ParentClass.ArenaState=state->stage();ParentClass.NeedBertForSpeech=state->bert();return state->term();}void Main();};};}

void NScript::CQ_ArenaScript::CArenaCellDoorGuard2::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    ParentClass.CellsVillage=Game.GetNearestWithDefName(Me,"VILLAGE_ARENA_CELLS");
    Game.EntitySetOpinionReactionsEnabled(Me,false);
    Game.EntitySetInFaction(Me,"FACTION_HERO");
    Game.EntitySetAsKillable(Me,false,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    while (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_INTRO) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    bool Turned=false;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.ClearHeroEnemyOfGuards(ParentClass.CellsVillage);
            seh_me.ClearCommands();
            seh_me.Release();
            while (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.ClearHeroEnemyOfGuards(ParentClass.CellsVillage);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            seh_me.MoveToPosition(Game.GetThingWithScriptName("GuardingDoorMarkerLeft").GetPos(),0.0f,ENTITY_MOVE_RUN,false,true);
            Turned=false;
        }
        if (ParentClass.NeedBertForSpeech) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.ClearHeroEnemyOfGuards(ParentClass.CellsVillage);
            seh_me.ClearCommands();
            seh_me.Release();
            while (ParentClass.NeedBertForSpeech) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_NORMAL && !EarlyTalk) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_EARLY_FIRST",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                EarlyTalk=true;
            } else if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_NORMAL_GO && !ChamTalk) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_LATE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ChamTalk=true;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_VERY_LATE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        } else if (IsDistanceBetweenThingsOver(Me,Game.GetThingWithScriptName("GuardingDoorMarkerLeft"),1.0f) && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToPosition(Game.GetThingWithScriptName("GuardingDoorMarkerLeft").GetPos(),0.0f,ENTITY_MOVE_WALK,false,true);
            Turned=false;
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
            }
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.InHitCutsceneAlready=true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_ATTACKED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.ModifyThingHealth(Me,10000.0f,false);
                Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
                Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
                ParentClass.InHitCutsceneAlready=false;
            }
        } else if (!Turned) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Turned=true;
                Game.EntitySetFacingAngle(Me,Game.GetThingWithScriptName("GuardingDoorMarkerLeft").GetAngleXY(),false);
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

void run(State&s){state=&s;{CGameScriptInterfaceBase g;NScript::CQ_ArenaScript p;NScript::CQ_ArenaScript::CArenaCellDoorGuard2 t(g,p);t.Main();s.inHit=p.InHitCutsceneAlready;s.early=t.EarlyTalk;s.cham=t.ChamTalk;assert(s.things==3&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.paused==s.unpaused&&s.villageAssigned==1);const char*names[]={"village","reactions","faction","killable","combo"};assert(s.setup.size()==5);for(unsigned i=0;i<5;++i)assert(s.setup[i]==names[i]);}assert(s.things==0);}
int main(){int cases=0;const char*lines[]={"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_EARLY_FIRST","TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_LATE","TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_VERY_LATE","TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_ATTACKED"};float health[]={0.5f,0.0f,-1.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int stage=-2;stage<=10;++stage)if(stage!=2&&stage!=4)for(int flags=0;flags<4;++flags)for(int h=0;h<4;++h){State s;s.talk=true;s.arenaState=stage;s.early=(flags&1)!=0;s.cham=(flags&2)!=0;s.health=health[h];bool early=s.early,cham=s.cham;int n=2;if(stage==3&&!early){early=true;n=0;}else if(stage==5&&!cham){cham=true;n=1;}run(s);assert(s.early==early&&s.cham==cham&&s.paused==1&&s.released==0&&s.validOnDestruct==1&&s.physicalChecks==0&&s.moves.empty()&&s.facing==0);assert(s.healthChecks==1);if(h==0)assert(s.speech.size()==1&&s.speech[0]==lines[n]);else assert(s.speech.empty());++cases;}
 for(int flags=0;flags<16;++flags){State s;s.physical=(flags&1)!=0;s.ability=(flags&2)!=0;s.heal=(flags&4)!=0;s.inHit=(flags&8)!=0;bool was=s.inHit,hit=s.physical||(s.ability&&!s.heal);run(s);assert(s.physicalChecks==1&&s.abilityChecks==int(!s.physical)&&s.healChecks==int(!s.physical&&s.ability));assert(s.startAttempts==1&&s.released==0&&s.paused==int(hit&&!was)&&s.modified==int(hit&&!was)&&s.allies==2*s.modified&&s.inHit==was&&s.facing==int(!hit));assert(s.speech.size()==unsigned(hit&&!was));if(!s.speech.empty())assert(s.speech[0]==lines[3]);++cases;}
 for(int flags=0;flags<8;++flags){State s;s.talk=true;s.physical=(flags&1)!=0;s.ability=(flags&2)!=0;s.heal=(flags&4)!=0;s.distance=4.0f;run(s);assert(s.physicalChecks==0&&s.abilityChecks==0&&s.healChecks==0&&s.speech.size()==1&&s.speech[0]==lines[0]&&s.modified==0&&s.moves.empty());++cases;}
 for(int h=0;h<4;++h){State s;s.physical=true;s.health=health[h];run(s);assert(s.healthChecks==1&&s.speech.size()==unsigned(h==0)&&s.modified==1&&s.allies==2&&!s.inHit);++cases;}
 float distances[]={0.0f,0.5f,1.0f,1.5f,-1.5f,std::numeric_limits<float>::quiet_NaN()};
 for(int d=0;d<6;++d)for(int alive=0;alive<4;++alive)for(int busy=0;busy<2;++busy){State s;s.distance=distances[d];s.meAlive=(alive&1)!=0;s.markerAlive=(alive&2)!=0;s.busyOnClaim=busy!=0;s.physical=true;bool moving=(d==3||d==4)&&alive==3&&!busy;run(s);assert(s.moves.size()==unsigned(moving)&&s.modified==int(!moving)&&s.physicalChecks==int(!moving));if(moving)assert(s.moves[0]==0);else assert(s.cleared==busy);++cases;}
 for(int delay=0;delay<5;++delay){State s;s.introUntil=delay;s.talk=true;s.limit=delay+1;run(s);assert(s.speech.size()==1&&s.early&&s.frame==delay+1);++cases;}
 for(int delay=1;delay<5;++delay){State s;s.cutUntil=delay;s.limit=delay+1;run(s);assert(s.clearedEnemy==2&&s.cleared==2&&s.released==1&&s.startAttempts==2&&s.moves.size()==1&&s.moves[0]==1&&s.facing==0);++cases;}
 for(int delay=1;delay<5;++delay){State s;s.bertUntil=delay;s.limit=delay+1;run(s);assert(s.clearedEnemy==1&&s.cleared==1&&s.released==1&&s.startAttempts==2&&s.moves.empty()&&s.facing==1);++cases;}
 for(int mode=0;mode<3;++mode){State s;s.limit=3;s.introUntil=mode==0?50:0;s.cutUntil=mode==1?50:0;s.bertUntil=mode==2?50:0;run(s);assert(s.speech.empty()&&s.moves.empty()&&s.facing==0&&s.frame==3);assert(s.startAttempts==int(mode!=0)&&s.released==int(mode!=0));++cases;}
 {State s;s.limit=0;run(s);assert(s.startAttempts==0&&s.talkChecks==0);++cases;}
 {State s;s.talk=true;s.initialValid=true;s.busyOnClaim=true;run(s);assert(s.cleared==1&&s.released==1&&s.speech.size()==1);++cases;}
 {State s;s.talk=true;s.startFailures=3;s.limit=4;run(s);assert(s.startAttempts==4&&s.frame==4&&s.speech.size()==1);++cases;}
 {State s;s.startFailures=50;s.limit=3;run(s);assert(s.startAttempts==3&&s.speech.empty()&&s.released==0&&!s.validOnDestruct);++cases;}
 {State s;s.talk=true;s.taskDelay=50;s.limit=3;run(s);assert(s.speech.size()==1&&s.frame==3&&s.released==0&&s.paused==1&&!s.early);++cases;}
 {State s;s.physical=true;s.taskDelay=50;s.limit=3;run(s);assert(s.speech.size()==1&&s.modified==0&&s.inHit&&s.released==0&&s.paused==1);++cases;}
 {State s;s.talk=true;s.limit=5;run(s);assert(s.speech.size()==5&&s.released==0&&s.early);for(int i=0;i<5;++i)assert(s.speech[i]==lines[i?2:0]);++cases;}
 {State s;s.limit=5;run(s);assert(s.facing==1&&s.moves.empty()&&s.physicalChecks==5);++cases;}
 {State s;s.limit=5;s.distance=4.0f;s.moveDelay=2;run(s);assert(s.moves.size()==1&&s.moves[0]==0&&s.facing==1);++cases;}
 {State s;s.limit=6;s.cutStart=2;s.cutUntil=3;run(s);assert(s.facing==2&&s.moves.size()==1&&s.moves[0]==1&&s.clearedEnemy==2);++cases;}
 {State s;s.limit=6;s.bertStart=2;s.bertUntil=3;run(s);assert(s.facing==1&&s.moves.empty()&&s.clearedEnemy==1);++cases;}
 for(int mode=0;mode<20;++mode)for(int stop=1;stop<=150;++stop){State s;s.limit=25;s.stopCheck=stop;s.talk=mode<10;s.physical=mode>=10&&mode<15;s.arenaState=mode%3==0?5:3;s.early=mode%4==0;s.cham=mode%5==0;s.introUntil=mode==8?3:0;s.cutUntil=mode==9?4:0;s.bertUntil=mode==10?5:0;s.taskDelay=mode==6||mode==12?3:0;s.startFailures=mode==7||mode==11?5:0;s.inHit=mode==13;s.initialValid=mode==14;s.busyOnClaim=mode==15;s.distance=mode>=16?4.0f:0.0f;s.moveDelay=mode>=16?5:1;s.cutStart=mode==18?2:0;s.cutUntil=mode==18?3:s.cutUntil;s.bertStart=mode==19?2:0;s.bertUntil=mode==19?3:s.bertUntil;s.health=mode==5?0.0f:100.0f;run(s);assert(s.checks<=stop&&s.allies==2*s.modified);++cases;}
 assert(cases==3279);std::puts("00f19bb0_TEST PASS");}
