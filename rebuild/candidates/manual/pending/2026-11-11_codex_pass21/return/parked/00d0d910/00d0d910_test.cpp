// Complete selected Main body with deterministic portable dependency doubles.
#include <stdio.h>
#include <string>
#include <vector>
#include <assert.h>
static int Strings,Things,Handles,Movies,Timers,Threads;
struct CCharString {std::string text;CCharString(const char*s="",long n=-1):text(n<0?std::string(s):std::string(s,n)){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}};
static CCharString operator+(const CCharString&a,const CCharString&b){return CCharString((a.text+b.text).c_str());}
struct GameStub;static GameStub*Current;
struct CScriptThing {int id;CScriptThing(int n=0):id(n){++Things;}CScriptThing(const CScriptThing&t):id(t.id){++Things;}~CScriptThing(){--Things;}bool MsgIsHitBy(const CCharString&)const;bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&)const;bool MsgIsHitBySpecialAbilityFrom(int,const CCharString&)const;bool MsgIsTalkedToBy(const CCharString&)const;void SetFriendsWithEverythingFlag(bool);};
struct CTimer;static std::vector<CTimer*>TimerList;
struct CTimer {long time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}CTimer&operator=(long t){time=t;return *this;}long GetTime()const{return time;}};
struct CScriptGameResourceObjectMovieBase{CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct CScriptGameResourceObjectScriptedThingBase {int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release();CScriptThing GetScriptThing()const{assert(id==1);return CScriptThing(id);}void Speak(const CScriptThing&,const char*,int,bool,bool,bool);bool IsPerformingScriptTask()const;};
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,HERO_ABILITY_HEAL_LIFE_SPELL=14};
struct CSpawnedFuncBase {CSpawnedFuncBase(){++Threads;}virtual ~CSpawnedFuncBase(){--Threads;}};
struct CQ_BanditCampBossBattleScript {bool Gate2Open,SpokenToSecondGuard;std::vector<CSpawnedFuncBase*>Owned;CQ_BanditCampBossBattleScript():Gate2Open(false),SpokenToSecondGuard(false){}~CQ_BanditCampBossBattleScript(){for(unsigned i=0;i<Owned.size();++i)delete Owned[i];}void OpenGate(float,CCharString){}void AddSpawnedFunction(CSpawnedFuncBase*p,const CCharString&n){assert(Gate2Open&&n.text.empty());Owned.push_back(p);}};
template<class C,class A,class B>struct CSpawnedFunc2:public CSpawnedFuncBase {CSpawnedFunc2(const CCharString&,long,C*,void(C::*)(A,B),A,B);};
struct GlobalDef{long EXPERIENCE_GRANT_MEDIUM;GlobalDef():EXPERIENCE_GRANT_MEDIUM(100){}};static GlobalDef Def;static GlobalDef*pGlobalScriptDef=&Def;
struct GameStub {CQ_BanditCampBossBattleScript*P;int Frame,StopAt,StartAt,TaskUntil,TaskLength,HitKind,HitAt,Starts,Releases,EnemyTargets,ClearInfo,Experience,PauseDepth,Friends,Enemies,QueryCount,StopQuery,SpawnCalls;bool Alive,PredicateOkay,Pass,Near,Killable;float Health;std::vector<int>TalkFrames,HitFrames;std::vector<std::string>Lines;std::string LongLine;
GameStub():P(0),Frame(0),StopAt(30),StartAt(0),TaskUntil(0),TaskLength(1),HitKind(0),HitAt(-1),Starts(0),Releases(0),EnemyTargets(0),ClearInfo(0),Experience(0),PauseDepth(0),Friends(0),Enemies(0),QueryCount(0),StopQuery(-1),SpawnCalls(0),Alive(true),PredicateOkay(true),Pass(false),Near(true),Killable(true),Health(10){Current=this;}
void NewScriptFrame(){++Frame;assert(Frame<300);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;}
bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&h,int p){assert(t.id==1&&p==4);++Starts;if(Frame<StartAt)return false;h.id=1;return true;}
CScriptThing GetHero(){return CScriptThing(2);}float GetHealth(const CScriptThing&t){assert(t.id==1);return Health;}
void EntitySetAsKillable(const CScriptThing&t,bool b,bool c){assert(t.id==1&&c);Killable=b;}
void ModifyThingHealth(const CScriptThing&t,float f,bool b){assert(t.id==1&&f==10000.0f&&!b);Health+=f;}
long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);return 7;}
void AddPersonToConversation(long n,const CScriptThing&h){assert(n==7&&h.id==2);}
void AddLineToConversation(long n,const CCharString&t,bool b,const CScriptThing&m,const CScriptThing&h){assert(n==7&&!b&&m.id==1&&h.id==2);Lines.push_back(t.text);}
void StartMovieSequence(const CCharString&n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());}
void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}
bool IsObjectInThingsPossession(const CCharString&n,const CScriptThing&h){assert(n.text=="OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS"&&h.id==2);return Pass;}
void GiveHeroExperience(long n){assert(n==100&&!P->Gate2Open);Experience+=(int)n;}
void ClearThingHasInformation(const CScriptThing&t){assert(t.id==1);++ClearInfo;}
void GiveThingBestEnemyTarget(const CScriptThing&t,const CScriptThing&h){assert(t.id==1&&h.id==2);++EnemyTargets;}
};
template<class C,class A,class B>CSpawnedFunc2<C,A,B>::CSpawnedFunc2(const CCharString&n,long q,C*p,void(C::*f)(A,B),A a,B b){assert(n.text=="ParentClass.OpenGate"&&q==0&&p==Current->P&&f==&C::OpenGate&&a==2.0f&&b.text=="Gate2Outer");++Current->SpawnCalls;}
static bool HitNow(){if(Current->Frame==Current->HitAt)return true;for(unsigned i=0;i<Current->HitFrames.size();++i)if(Current->Frame==Current->HitFrames[i])return true;return false;}
bool CScriptThing::MsgIsHitBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return HitNow()&&Current->HitKind==1;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return HitNow()&&Current->HitKind>=2;}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int a,const CCharString&n)const{assert(id==1&&a==14&&n.text=="SCRIPT_NAME_HERO");return HitNow()&&Current->HitKind==3;}
bool CScriptThing::MsgIsTalkedToBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");for(unsigned i=0;i<Current->TalkFrames.size();++i)if(Current->Frame==Current->TalkFrames[i])return true;return false;}
void CScriptThing::SetFriendsWithEverythingFlag(bool b){if(b)++Current->Friends;else ++Current->Enemies;}
void CScriptGameResourceObjectScriptedThingBase::Release(){assert(id==1);id=0;++Current->Releases;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&h,const char*n,int group,bool a,bool b,bool c){assert(id==1&&h.id==2&&group==0&&!a&&b&&!c);Current->Lines.push_back(n);Current->TaskUntil=Current->Frame+(Current->LongLine==n?100:Current->TaskLength);}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&h,const CScriptThing&m,float f){assert(h.id==2&&m.id==1&&f==8.0f);return Current->Near;}
struct CIsThingAlive{bool b;CIsThingAlive(const CScriptThing&t):b(Current->Alive){assert(t.id==1);}};
struct TRetVoid{void Return(){}};
struct CWideScreenMagicPauseEntities{GameStub*g;CWideScreenMagicPauseEntities(GameStub&p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
struct Gate2 {GameStub*Game;CScriptThing Me;CQ_BanditCampBossBattleScript ParentClass;Gate2(GameStub&g):Game(&g),Me(1){g.P=&ParentClass;}void SetPredicate(const CIsThingAlive&p){Game->PredicateOkay=p.b;}bool IsActiveThreadTerminating()const{++Game->QueryCount;return !Game->PredicateOkay||Game->Frame>=Game->StopAt||(Game->StopQuery>=0&&Game->QueryCount>=Game->StopQuery);}void Main();};
void Gate2::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.EntitySetAsKillable(Me, false, true);
    CTimer MoanTimer;
    MoanTimer = 0;
    bool TalkedTo = false;
    long HitNumber = 0;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.Gate2Open) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                switch (HitNumber) {
                case 0:
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_HIT_FIRST", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    ++HitNumber;
                    break;
                case 1:
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_HIT_SECOND", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    break;
                case 2:
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_HIT_THIRD", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    break;
                case 3:
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_HIT_FOURTH", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Me.SetFriendsWithEverythingFlag(false);
                    Game.ModifyThingHealth(Me, 10000.0f, false);
                    Game.EntitySetAsKillable(Me, true, true);
                    Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                    Game.ClearThingHasInformation(Me);
                    break;
                }
            }
            if (MoanTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game.GetHero(), Me, 8.0f)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                if (!TalkedTo) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    TalkedTo = true;
                    Game.AddLineToConversation(Conversation, "TEXT_QST_009_BANDIT2_COMMENT_FIRST", false, Me, Game.GetHero());
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.AddLineToConversation(Conversation, "TEXT_QST_009_BANDIT2_COMMENT_SECOND", false, Me, Game.GetHero());
                }
                MoanTimer = 10;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.SpokenToSecondGuard = true;
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (!Game.IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", Game.GetHero())) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_GATE_BLOCKED", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_MEDIUM);
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_GIVEN_CAMP_PASS", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    }
                    ParentClass.Gate2Open = true;
                    ParentClass.AddSpawnedFunction(new CSpawnedFunc2<CQ_BanditCampBossBattleScript, float, CCharString>(
                        CCharString("ParentClass.", -1) + CCharString("OpenGate", -1), 0, &ParentClass,
                        &CQ_BanditCampBossBattleScript::OpenGate, 2.0f, "Gate2Outer"), CCharString(""));
                    long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_009_BANDIT2_PASS", false, Me, Game.GetHero());
                    Game.ClearThingHasInformation(Me);
                }
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
        } else {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_ASIDE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_ATTACKED_NEW", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                }
                Me.SetFriendsWithEverythingFlag(false);
                Game.EntitySetAsKillable(Me, true, true);
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                Game.ClearThingHasInformation(Me);
                if (seh_me.IsValid())
                    seh_me.Release();
                while (true) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}
static int Count(const GameStub&g,const char*n){int c=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)++c;return c;}
static bool Balanced(const GameStub&g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty()&&g.PauseDepth==0&&Threads==(int)g.P->Owned.size();}
static bool Check(bool b,int n){if(!b)printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
{GameStub g;Gate2 o(g);o.Main();ok=Check(Balanced(g)&&Count(g,"TEXT_QST_009_BANDIT2_COMMENT_FIRST")==1&&Count(g,"TEXT_QST_009_BANDIT2_COMMENT_SECOND")==2&&!g.Killable&&!o.ParentClass.Gate2Open,++n)&&ok;}
{GameStub g;Gate2 o(g);g.Near=false;o.Main();ok=Check(Balanced(g)&&g.Lines.empty(),++n)&&ok;}
{GameStub g;Gate2 o(g);g.TalkFrames.push_back(2);o.Main();ok=Check(Balanced(g)&&o.ParentClass.SpokenToSecondGuard&&!o.ParentClass.Gate2Open&&Count(g,"TEXT_QST_009_BANDIT2_GATE_BLOCKED")==1&&g.Experience==0&&g.SpawnCalls==0,++n)&&ok;}
{GameStub g;Gate2 o(g);g.Pass=true;g.TalkFrames.push_back(2);g.TalkFrames.push_back(10);o.Main();ok=Check(Balanced(g)&&o.ParentClass.Gate2Open&&o.ParentClass.SpokenToSecondGuard&&g.Experience==100&&g.SpawnCalls==1&&g.ClearInfo==1&&Count(g,"TEXT_QST_009_BANDIT2_GIVEN_CAMP_PASS")==1&&Count(g,"TEXT_QST_009_BANDIT2_PASS")==1&&Count(g,"TEXT_QST_009_BANDIT2_ASIDE")==1,++n)&&ok;}
for(int open=0;open<2;++open)for(int hit=1;hit<4;++hit){GameStub g;Gate2 o(g);o.ParentClass.Gate2Open=open!=0;g.HitKind=hit;g.HitAt=2;o.Main();ok=Check(Balanced(g)&&g.EnemyTargets==int(open&&hit!=3)&&g.ClearInfo==int(open&&hit!=3)&&g.Friends==int(!open&&hit!=3)&&Count(g,open?"TEXT_QST_009_BANDIT2_ATTACKED_NEW":"TEXT_QST_009_BANDIT2_HIT_FIRST")==int(hit!=3),++n)&&ok;}
{GameStub g;Gate2 o(g);g.HitKind=1;g.HitFrames.push_back(2);g.HitFrames.push_back(6);g.HitFrames.push_back(10);o.Main();ok=Check(Balanced(g)&&g.Friends==3&&Count(g,"TEXT_QST_009_BANDIT2_HIT_FIRST")==1&&Count(g,"TEXT_QST_009_BANDIT2_HIT_SECOND")==2&&Count(g,"TEXT_QST_009_BANDIT2_HIT_THIRD")==0,++n)&&ok;}
for(int open=0;open<2;++open){GameStub g;Gate2 o(g);o.ParentClass.Gate2Open=open!=0;g.HitKind=1;g.HitAt=2;g.Health=0;o.Main();ok=Check(Balanced(g)&&Count(g,open?"TEXT_QST_009_BANDIT2_ATTACKED_NEW":"TEXT_QST_009_BANDIT2_HIT_FIRST")==0&&g.EnemyTargets==open,++n)&&ok;}
{GameStub g;Gate2 o(g);g.Health=0;g.Pass=true;g.TalkFrames.push_back(2);o.Main();ok=Check(Balanced(g)&&o.ParentClass.Gate2Open&&g.Experience==100&&g.SpawnCalls==1&&Count(g,"TEXT_QST_009_BANDIT2_GIVEN_CAMP_PASS")==0,++n)&&ok;}
{GameStub g;Gate2 o(g);g.Alive=false;o.Main();ok=Check(Balanced(g)&&g.Frame==1&&g.Starts==0&&g.Lines.empty(),++n)&&ok;}
{GameStub g;Gate2 o(g);g.StartAt=100;g.StopAt=4;o.Main();ok=Check(Balanced(g)&&g.Frame==4&&g.Lines.empty(),++n)&&ok;}
for(int open=0;open<2;++open){GameStub g;Gate2 o(g);o.ParentClass.Gate2Open=open!=0;g.HitKind=1;g.HitAt=2;g.TaskLength=100;g.StopAt=4;o.Main();ok=Check(Balanced(g)&&g.Frame==4&&g.Friends==0&&g.EnemyTargets==0,++n)&&ok;}
for(int pass=0;pass<2;++pass){GameStub g;Gate2 o(g);g.Pass=pass!=0;g.TalkFrames.push_back(2);g.TaskLength=100;g.StopAt=4;o.Main();ok=Check(Balanced(g)&&!o.ParentClass.Gate2Open&&g.Experience==100*pass&&g.SpawnCalls==0,++n)&&ok;}
for(int stop=5;stop<=25;stop+=5){GameStub g;Gate2 o(g);g.Pass=true;g.TalkFrames.push_back(2);g.HitKind=1;g.HitAt=10;g.StopQuery=stop;o.Main();ok=Check(Balanced(g)&&g.QueryCount==stop,++n)&&ok;}
ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Timers==0&&Threads==0&&TimerList.empty();printf(ok?"00d0d910_TEST PASS\n":"00d0d910_TEST FAIL\n");return ok?0:1;}
