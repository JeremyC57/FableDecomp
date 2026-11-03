// Selected source SHA256 846c9b2191b787f94abaacf2c15229a0716e0360d195360191a0ca59cd5a83aa
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct State {int frame,limit,checks,stopCheck,starts,startFailures,resources,movies,pauses,predicates,predicateObjects,things,strings,releases,taskFrames,taskUntil,persistent,damageable,combo,killable,opened,regionOff,regionOn,healthChecks,keys,taken,questions,questionStart,cameras,fixedCalls,drawableCalls,healthMask;int answer[2],delay[2];bool initialValid,talk,repeatTalk,drawable,fixed;float health;std::vector<std::string>text;std::vector<int>takeFrames;State():frame(0),limit(100),checks(0),stopCheck(0),starts(0),startFailures(0),resources(0),movies(0),pauses(0),predicates(0),predicateObjects(0),things(0),strings(0),releases(0),taskFrames(0),taskUntil(0),persistent(0),damageable(0),combo(0),killable(0),opened(0),regionOff(0),regionOn(0),healthChecks(0),keys(0),taken(0),questions(0),questionStart(0),cameras(0),fixedCalls(0),drawableCalls(0),healthMask(-1),initialValid(false),talk(true),repeatTalk(false),drawable(true),fixed(false),health(0.5f){answer[0]=answer[1]=1;delay[0]=delay[1]=0;}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&s):text(s.text){++state->strings;}~CCharString(){--state->strings;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};class TRetVoid{public:void Return(){}};
class CScriptThing{public:int kind;CScriptThing(int k=0):kind(k){++state->things;}CScriptThing(const CScriptThing&x):kind(x.kind){++state->things;}~CScriptThing(){--state->things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(kind==0&&s.text=="SCRIPT_NAME_HERO");return state->talk&&(state->repeatTalk||state->questions==0);}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(state->initialValid){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->movies==0&&state->pauses==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(3);}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.kind==1&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauses==1&&state->movies==1&&valid&&!state->drawable&&state->fixed);state->text.push_back(s);state->taskUntil=state->frame+state->taskFrames;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;assert(state->movies==1);}~CScriptGameResourceObjectMovieBase(){assert(state->pauses==0);--state->movies;}};
class CGameScriptInterfaceBase{public:CScriptThing hero;CGameScriptInterfaceBase():hero(1){}void NewScriptFrame(){assert(state->frame<255);++state->frame;}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(t.kind==0&&p==SCRIPT_AI_PRIORITY_HIGHEST);++state->starts;if(state->starts<=state->startFailures)return false;r.valid=true;return true;}void SetThingPersistent(const CScriptThing&t,bool b){assert(t.kind==0&&b);++state->persistent;}void EntitySetAsDamageable(const CScriptThing&t,bool b){assert(t.kind==0&&!b);++state->damageable;}void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&t,bool b){assert(t.kind==0&&!b);++state->combo;}void EntitySetAsKillable(const CScriptThing&t,bool a,bool b){assert(t.kind==0&&!a&&!b);++state->killable;}CScriptThing GetNearestWithDefName(const CScriptThing&t,const CCharString&s){assert(t.kind==0&&s.text=="REGION_EXIT_POINT");return CScriptThing(2);}void SetRegionExitAsActive(const CScriptThing&t,bool b){assert(t.kind==2);if(b){assert(state->opened==1&&state->drawable);++state->regionOn;}else{assert(!state->opened);++state->regionOff;}}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){state->pauses+=b?1:-1;assert(state->pauses>=0&&state->pauses<=1);}float GetHealth(const CScriptThing&t){assert(t.kind==3&&state->pauses==1);int n=state->healthChecks++;return state->healthMask<0?state->health:((state->healthMask&(1<<n))?0.5f:0.0f);}CScriptThing&GetHero(){return hero;}void FixMovieSequenceCamera(bool b){assert(state->pauses==1);state->fixed=b;++state->fixedCalls;}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="CAM_NDD_FACE"&&state->fixed);return CScriptThing(4);}void CameraUseCameraPoint(const CScriptThing&t,const void*a,const void*b,float f,long c,long d){assert(t.kind==4&&!a&&!b&&f==-1.0f&&c==0&&d==-1&&state->fixed);++state->cameras;}void EntitySetAsDrawable(const CScriptThing&t,bool b){assert(t.kind==1&&state->pauses==1);state->drawable=b;++state->drawableCalls;}void GiveHeroYesNoQuestion(const CCharString&q,const CCharString&yes,const CCharString&no,const CCharString&other,bool flag){assert(state->pauses==1&&!state->drawable&&state->fixed&&yes.text=="TEXT_OBJECT_HERO_ANSWER_YES"&&no.text=="TEXT_OBJECT_HERO_ANSWER_NO"&&other.text.empty()&&flag);bool second=q.text=="TEXT_QST_B19_SILVER_DOOR_SECOND_QUESTION";assert(second||q.text=="TEXT_QST_B19_SILVER_DOOR_QUESTION");assert(second||state->questions==0||state->repeatTalk);state->questionStart=state->frame;++state->questions;}long MsgIsQuestionAnsweredYesOrNo(){assert(state->questions>0);int n=state->questions>1&&!state->repeatTalk?1:0;return state->frame<state->questionStart+state->delay[n]?-1:state->answer[n];}bool IsObjectInThingsPossession(const CCharString&s,const CScriptThing&t){assert(s.text=="OBJECT_SILVER_KEY"&&t.kind==1);return state->keys>0;}void TakeObjectFromHero(const CCharString&s){assert(s.text=="OBJECT_SILVER_KEY"&&state->keys>0&&state->pauses==1);if(!state->takeFrames.empty())assert(state->frame>state->takeFrames.back());state->takeFrames.push_back(state->frame);--state->keys;++state->taken;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{CScriptThing Thing;public:CIsThingAlive(const CScriptThing&t):Thing(t){assert(t.kind==0&&state->resources==1);++state->predicateObjects;}~CIsThingAlive(){--state->predicateObjects;}};class CV_DemonDoorsScript{public:void FadeOutFaceAndOpenDoor(const CScriptThing&t,const CCharString&s){assert(t.kind==0&&s.text=="DD_KeyHoarder"&&state->pauses==1&&state->regionOff==1&&state->regionOn==0&&state->keys==0);++state->opened;}class CDF_KeyHoarder{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CV_DemonDoorsScript&ParentClass;CDF_KeyHoarder(CGameScriptInterfaceBase&g,CV_DemonDoorsScript&p):Game(g),ParentClass(p){}void SetPredicate(const CIsThingAlive&){assert(state->predicateObjects==1);++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CV_DemonDoorsScript::CDF_KeyHoarder::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetThingPersistent(Me,true);
    Game.EntitySetAsDamageable(Me,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game.EntitySetAsKillable(Me,false,false);
    CScriptThing region_exit=Game.GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game.SetRegionExitAsActive(region_exit,false);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            Game.CameraUseCameraPoint(Game.GetThingWithScriptName("CAM_NDD_FACE"),0,0,-1.0f,0,-1);
            Game.EntitySetAsDrawable(Game.GetHero(),false);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B19_SILVER_DOOR_INTRO",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            long answer;
            Game.GiveHeroYesNoQuestion("TEXT_QST_B19_SILVER_DOOR_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
            while ((answer=Game.MsgIsQuestionAnsweredYesOrNo())<0) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (answer==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B19_SILVER_DOOR_REFUSAL",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.EntitySetAsDrawable(Game.GetHero(),true);
                Game.FixMovieSequenceCamera(false);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!Game.IsObjectInThingsPossession("OBJECT_SILVER_KEY",Game.GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B19_SILVER_DOOR_ACCEPT_NONE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_KeyHoarder");
                    Game.EntitySetAsDrawable(Game.GetHero(),true);
                    Game.SetRegionExitAsActive(region_exit,true);
                    Game.FixMovieSequenceCamera(false);
                    return;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B19_SILVER_DOOR_FIRST_OK",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B19_SILVER_DOOR_SECOND_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    while ((answer=Game.MsgIsQuestionAnsweredYesOrNo())<0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B19_SILVER_DOOR_REFUSAL",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.EntitySetAsDrawable(Game.GetHero(),true);
                        Game.FixMovieSequenceCamera(false);
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B19_SILVER_DOOR_ACCEPT_SOME",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        while (Game.IsObjectInThingsPossession("OBJECT_SILVER_KEY",Game.GetHero())) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game.TakeObjectFromHero("OBJECT_SILVER_KEY");
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_KeyHoarder");
                        Game.EntitySetAsDrawable(Game.GetHero(),true);
                        Game.SetRegionExitAsActive(region_exit,true);
                        Game.FixMovieSequenceCamera(false);
                        return;
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

void run(State&s){state=&s;{CGameScriptInterfaceBase g;NScript::CV_DemonDoorsScript p;NScript::CV_DemonDoorsScript::CDF_KeyHoarder t(g,p);t.Main();assert(s.things==2&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauses==0&&s.predicateObjects==0&&s.predicates==1);assert(s.persistent==s.damageable&&s.damageable==s.combo&&s.combo==s.killable&&s.killable==s.regionOff);assert(s.regionOn==s.opened&&s.opened<=1);}assert(s.things==0);}
int main(){int cases=0;const char*intro="TEXT_QST_B19_SILVER_DOOR_INTRO";const char*refusal="TEXT_QST_B19_SILVER_DOOR_REFUSAL";const char*none="TEXT_QST_B19_SILVER_DOOR_ACCEPT_NONE";const char*first="TEXT_QST_B19_SILVER_DOOR_FIRST_OK";const char*some="TEXT_QST_B19_SILVER_DOOR_ACCEPT_SOME";float health[]={0.5f,0.0f,-1.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int a=0;a<3;++a)for(int b=0;b<3;++b)for(int keys=0;keys<6;++keys)for(int h=0;h<4;++h){State s;s.answer[0]=a;s.answer[1]=b;s.keys=keys;s.health=health[h];bool second=a!=0&&keys>0,success=a!=0&&(!keys||b!=0);run(s);assert(s.opened==int(success)&&s.questions==1+int(second)&&s.taken==(success?keys:0)&&s.keys==(success?0:keys)&&s.drawable&&!s.fixed&&s.cameras==1&&s.fixedCalls==2&&s.drawableCalls==2);assert(s.healthChecks==2+int(second));if(h==0){assert(s.text.size()==unsigned(2+int(second))&&s.text[0]==intro);if(!a)assert(s.text[1]==refusal);else if(!keys)assert(s.text[1]==none);else assert(s.text[1]==first&&s.text[2]==(b?some:refusal));}else assert(s.text.empty());assert(s.frame==(success?1+keys:100));++cases;}
 for(int mask=0;mask<8;++mask){State s;s.keys=2;s.healthMask=mask;run(s);const char*expected[]={intro,first,some};unsigned j=0;for(int n=0;n<3;++n)if(mask&(1<<n))assert(s.text[j++]==expected[n]);assert(s.text.size()==j&&s.opened&&s.taken==2);++cases;}
 for(int a=0;a<5;++a)for(int b=0;b<5;++b)for(int task=0;task<4;++task){State s;s.keys=3;s.delay[0]=a;s.delay[1]=b;s.taskFrames=task;run(s);assert(s.opened&&s.taken==3&&s.frame==1+a+b+3*task+3&&s.text.size()==3);++cases;}
 for(int q=0;q<2;++q){State s;s.keys=2;s.answer[q]=-3;s.limit=8;run(s);assert(!s.opened&&!s.taken&&s.questions==q+1&&s.frame==8&&!s.drawable&&s.fixed);++cases;}
 for(int stop=0;stop<2;++stop){State s;s.limit=stop;run(s);assert(s.frame==1&&s.starts==0&&s.regionOff==0&&s.text.empty());++cases;}
 for(int valid=0;valid<2;++valid)for(int fail=0;fail<6;++fail){State s;s.initialValid=valid!=0;s.startFailures=fail;run(s);assert(s.starts==fail+1&&s.releases==valid&&s.opened&&s.frame==1+fail);++cases;}
 {State s;s.startFailures=20;s.limit=4;run(s);assert(s.starts==3&&s.regionOff==0&&!s.opened);++cases;}
 {State s;s.talk=false;s.limit=5;run(s);assert(s.questions==0&&s.cameras==0&&s.drawable&&!s.fixed&&s.regionOff==1&&s.frame==5);++cases;}
 {State s;s.answer[0]=0;s.repeatTalk=true;s.limit=5;run(s);assert(s.questions==4&&s.text.size()==8&&!s.opened&&s.drawable&&!s.fixed);++cases;}
 for(int limit=2;limit<7;++limit){State s;s.keys=8;s.limit=limit;run(s);assert(s.taken==limit-2&&s.keys==10-limit&&!s.opened&&!s.drawable&&s.fixed);++cases;}
 for(int mode=0;mode<16;++mode)for(int stop=1;stop<=160;++stop){State s;s.limit=30;s.stopCheck=stop;s.keys=mode%5;s.answer[0]=mode==0?0:1;s.answer[1]=mode==1?0:1;s.delay[0]=mode==2?50:mode%3;s.delay[1]=mode==3?50:mode%2;s.taskFrames=mode==4?50:mode%3;s.startFailures=mode==5?50:mode%2;s.initialValid=mode==6;s.talk=mode!=7;s.health=mode==8?0.0f:mode==9?std::numeric_limits<float>::quiet_NaN():0.5f;s.answer[0]=mode==10?-1:s.answer[0];s.answer[1]=mode==11?-1:s.answer[1];s.healthMask=mode>=12?mode-12:-1;run(s);assert(s.checks<=stop&&s.keys+s.taken==mode%5);++cases;}
 assert(cases==2908);std::puts("00e740c0_TEST PASS");}
