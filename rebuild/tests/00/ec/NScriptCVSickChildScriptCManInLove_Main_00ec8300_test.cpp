// Complete Main body with recording game and deterministic resource callees.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Things,Handles,Movies;
class CCharString{ public:std::string text;CCharString():text(){++Strings;}CCharString(const char* p):text(p){++Strings;}CCharString(const CCharString& p):text(p.text){++Strings;}~CCharString(){--Strings;}CCharString& operator=(const CCharString& p){text=p.text;return *this;}};
class CDefString{ public:const char* name;CDefString(const char* n):name(n){}operator CCharString()const{return CCharString(name);}};
struct GlobalDef{CDefString ObjectFromMansLover,ObjectToGiveToMansLover,IngredientObject;float TellingTheTruth,LyingToManAboutWomanLikingHer,LyingToManAboutWomanNotLikingHer;GlobalDef():ObjectFromMansLover("LOVERS_GIFT"),ObjectToGiveToMansLover("MANS_LETTER"),IngredientObject("MUSHROOM"),TellingTheTruth(5),LyingToManAboutWomanLikingHer(-7),LyingToManAboutWomanNotLikingHer(-9){}};
static GlobalDef Def;static GlobalDef* pGlobalScriptDef=&Def;
struct GameStub;static GameStub* Current;
class C3DVector{ public:int id;C3DVector(int i=0):id(i){}};
class CScriptThing{ public:int id;CScriptThing(int i=0):id(i){++Things;}CScriptThing(const CScriptThing& p):id(p.id){++Things;}~CScriptThing(){--Things;}C3DVector GetPos()const{return C3DVector(id);}bool MsgIsTalkedToBy(const CCharString&)const;bool MsgIsHitBy(const CCharString&)const;bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&)const;bool MsgIsHitBySpecialAbilityFrom(int,const CCharString&)const;};
class CScriptGameResourceObjectScriptedThingBase{ public:int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release(){id=0;}CScriptThing GetScriptThing()const{return CScriptThing(id);}bool IsPerformingScriptTask()const;void Speak(const CScriptThing&,const char*,int,bool,bool,bool);void ClearCommands();void MoveToThing(const CScriptThing&,float,int,bool,bool,bool,bool);};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct Parent{long MansLoverState;Parent():MansLoverState(1){}};
enum{ML_NOT_RECEIVED_LETTER=0,ML_IN_LOVE_WITH_YOU=1,ML_NOT_IN_LOVE_WITH_YOU=2,ML_KILLED=3,SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,ENTITY_MOVE_WALK=0,ESSG_WANDER_NEAR=4,HERO_ABILITY_HEAL_LIFE_SPELL=14};typedef int EScriptingStateGroups;
struct GameStub{
 int Frame,TerminateAt,StartAt,TaskUntil,TaskLength,QuestionDelay,QuestionReady,QuestionCount,AnswerIndex,HitAt,SpecialAt,HealAt,PauseDepth,ClearCount,LetterGifts,IngredientGifts,Takes,Removes,Walks,Fades,ExitDistance,SetupCalls;
 bool HoldingGift,HoldingLetter,HasInfo,Stopped,Live,PredicateOkay;float Health,Morality;std::vector<int>TalkFrames,Answers;std::vector<std::string>Lines,Questions;
 GameStub():Frame(0),TerminateAt(25),StartAt(0),TaskUntil(0),TaskLength(1),QuestionDelay(1),QuestionReady(0),QuestionCount(0),AnswerIndex(0),HitAt(-1),SpecialAt(-1),HealAt(-1),PauseDepth(0),ClearCount(0),LetterGifts(0),IngredientGifts(0),Takes(0),Removes(0),Walks(0),Fades(0),ExitDistance(4),SetupCalls(0),HoldingGift(false),HoldingLetter(false),HasInfo(false),Stopped(false),Live(true),PredicateOkay(true),Health(10),Morality(0){Current=this;TalkFrames.push_back(1);Answers.push_back(1);Answers.push_back(1);}
 void NewScriptFrame(){++Frame;assert(Frame<500);if(!PredicateOkay)Stopped=true;}
 CScriptThing GetHero(){return CScriptThing(2);}bool IsObjectInThingsPossession(const CCharString& n,const CScriptThing& h){assert(h.id==2);if(n.text=="LOVERS_GIFT")return HoldingGift;assert(n.text=="OBJECT_SICK_CHILD_WOMANS_HERO_LETTER");return HoldingLetter;}
 void SetThingHasInformation(const CScriptThing& t,bool a,bool b,bool c){assert(t.id==1&&a&&b&&!c);HasInfo=true;}void ClearThingHasInformation(const CScriptThing& t){assert(t.id==1);HasInfo=false;++ClearCount;}
 void SetIsPushableByHero(const CScriptThing& t,bool b){assert(t.id==1&&!b);++SetupCalls;}void SetCreatureBrain(const CScriptThing& t,const CCharString& n){assert(t.id==1&&n.text=="BRAIN_PASSIVE_OVERRIDE");++SetupCalls;}void SetWanderCentrePoint(const CScriptThing& t,const C3DVector& p){assert(t.id==1&&p.id==1);++SetupCalls;}void SetWanderMinDistance(const CScriptThing& t,float f){assert(t.id==1&&f==0);++SetupCalls;}void SetWanderMaxDistance(const CScriptThing& t,float f){assert(t.id==1&&f==10);++SetupCalls;}void SetScriptingStateGroup(const CScriptThing& t,int v){assert(t.id==1&&v==4);++SetupCalls;}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(t.id==1&&p==4);if(Frame<StartAt)return false;h.id=t.id;return true;}
 void StartMovieSequence(const CCharString& n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}float GetHealth(const CScriptThing& t){assert(t.id==1);return Health;}
 void GiveHeroObject(const CCharString& n,long count,bool b){assert(count==-1&&!b);if(n.text=="MANS_LETTER")++LetterGifts;else{assert(n.text=="MUSHROOM");++IngredientGifts;}}
 void RemoveItemFromContainer(const CScriptThing& t,const CCharString& n){assert(t.id==1&&n.text=="MUSHROOM");++Removes;}void GiveHeroMorality(float f){Morality+=f;}
 void TakeObjectFromHero(const CCharString& n){++Takes;if(n.text=="LOVERS_GIFT")HoldingGift=false;else{assert(n.text=="OBJECT_SICK_CHILD_WOMANS_HERO_LETTER");HoldingLetter=false;}}
 void GiveHeroYesNoQuestion(const CCharString& q,const CCharString& y,const CCharString& n,const CCharString& e,bool b){assert(y.text=="TEXT_OBJECT_HERO_ANSWER_YES"&&n.text=="TEXT_OBJECT_HERO_ANSWER_NO"&&e.text.empty()&&b);Questions.push_back(q.text);++QuestionCount;QuestionReady=Frame+QuestionDelay;AnswerIndex=QuestionCount-1;}
 long MsgIsQuestionAnsweredYesOrNo(){if(Frame<QuestionReady)return -1;assert(AnswerIndex<(int)Answers.size());return Answers[AnswerIndex];}
 CScriptThing GetNearestWithDefName(const CScriptThing& t,const CCharString& n){assert(t.id==1&&n.text=="REGION_EXIT_POINT");return CScriptThing(3);}
};
bool CScriptThing::MsgIsTalkedToBy(const CCharString& n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");for(unsigned i=0;i<Current->TalkFrames.size();++i)if(Current->TalkFrames[i]==Current->Frame)return true;return false;}
bool CScriptThing::MsgIsHitBy(const CCharString& n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->HitAt;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->SpecialAt;}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int ability,const CCharString& n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO"&&ability==14);return Current->Frame==Current->HealAt;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& h,const char* n,int v,bool a,bool b,bool c){assert(id==1&&h.id==2&&v==0&&!a&&b&&!c);Current->Lines.push_back(n);Current->TaskUntil=Current->Frame+Current->TaskLength;}
void CScriptGameResourceObjectScriptedThingBase::ClearCommands(){Current->TaskUntil=Current->Frame;}
void CScriptGameResourceObjectScriptedThingBase::MoveToThing(const CScriptThing& t,float f,int mt,bool a,bool b,bool c,bool d){assert(id==1&&t.id==3&&f==1&&mt==0&&!a&&b&&!c&&d);++Current->Walks;--Current->ExitDistance;}
struct CWideScreenMagicPauseEntities{GameStub* g;CWideScreenMagicPauseEntities(GameStub& p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
struct CIsThingAlive{bool okay;CIsThingAlive(const CScriptThing& t):okay(Current->Live){assert(t.id==1);}};struct TRetVoid{void Return()const{}};
static bool IsDistanceBetweenThingsOver(const CScriptThing& a,const CScriptThing& b,float d){assert(a.id==1&&b.id==3&&d==2);return Current->ExitDistance>2;}
static void FadeOutAndKillEntity(const CScriptThing& t,float f,bool a,bool b){assert(t.id==1&&f==1.5f&&a&&b);++Current->Fades;Current->Stopped=true;}
struct Man{GameStub* Game;CScriptThing Me;Parent ParentClass;bool GivenLetter,GotReward,HelpedGuyOut;Man(GameStub& g):Game(&g),Me(1),GivenLetter(false),GotReward(false),HelpedGuyOut(false){}void SetPredicate(const CIsThingAlive& p){Game->PredicateOkay=p.okay;}bool IsActiveThreadTerminating()const{return Game->Stopped||Game->Frame>=Game->TerminateAt;}void Main();};
void Man::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    const CDefString& ObjectFromMansLover=pGlobalScriptDef->ObjectFromMansLover;
    if (Game.IsObjectInThingsPossession(ObjectFromMansLover,Game.GetHero())
        || Game.IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER",Game.GetHero())
        || !GivenLetter || ParentClass.MansLoverState==ML_KILLED) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me,true,true,false);
    }
    Game.SetIsPushableByHero(Me,false);
    Game.SetCreatureBrain(Me,"BRAIN_PASSIVE_OVERRIDE");
    Game.SetWanderCentrePoint(Me,Me.GetPos());
    Game.SetWanderMinDistance(Me,0.0f);
    Game.SetWanderMaxDistance(Me,10.0f);
    Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)                                  // inline control: retail calls ~seh_me at this exit (0x00EC85E2 `jne 0x00EC9BEA`)
#endif
                return;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!GivenLetter && ParentClass.MansLoverState!=ML_KILLED) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_INTRO_50",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.GiveHeroObject(pGlobalScriptDef->ObjectToGiveToMansLover,-1,false);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_INTRO_60",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_INTRO_70",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                GivenLetter=true;
                Game.ClearThingHasInformation(Me);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if ((Game.IsObjectInThingsPossession(pGlobalScriptDef->ObjectFromMansLover,Game.GetHero())
                    || Game.IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER",Game.GetHero())) && !GotReward) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_RETURNING_OBJECT_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    while (YNQAnswer<0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (YNQAnswer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.TakeObjectFromHero(pGlobalScriptDef->ObjectFromMansLover);
                        Game.TakeObjectFromHero("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER");
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_RETURNED_OBJECT_TO_ME",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                        Game.RemoveItemFromContainer(Me,pGlobalScriptDef->IngredientObject);
                        if (ParentClass.MansLoverState==ML_NOT_IN_LOVE_WITH_YOU) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game.GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game.GiveHeroMorality(pGlobalScriptDef->LyingToManAboutWomanLikingHer);
                        }
                        GotReward=true;
                        HelpedGuyOut=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_DIDNT_RETURN_OBJECT_TO_ME",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_WOMANS_OPINION_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                        long YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                        while (YNQAnswer<0) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (YNQAnswer==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_HAPPY",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game.GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                            Game.RemoveItemFromContainer(Me,pGlobalScriptDef->IngredientObject);
                            GotReward=true;
                            HelpedGuyOut=true;
                            if (ParentClass.MansLoverState==ML_NOT_IN_LOVE_WITH_YOU) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game.GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                            }
                            else {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game.GiveHeroMorality(pGlobalScriptDef->LyingToManAboutWomanLikingHer);
                            }
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_UNHAPPY",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game.GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                            Game.RemoveItemFromContainer(Me,pGlobalScriptDef->IngredientObject);
                            GotReward=true;
                            if (ParentClass.MansLoverState==ML_NOT_IN_LOVE_WITH_YOU) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game.GiveHeroMorality(pGlobalScriptDef->LyingToManAboutWomanNotLikingHer);
                            }
                            else {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game.GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                            }
                        }
                    }
                    Game.ClearThingHasInformation(Me);
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!GotReward) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (ParentClass.MansLoverState==ML_KILLED) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game.GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                            Game.RemoveItemFromContainer(Me,pGlobalScriptDef->IngredientObject);
                            GotReward=true;
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.ClearCommands();
                        if (HelpedGuyOut) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_HAPPY_10",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                        else if (ParentClass.MansLoverState==ML_KILLED) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_UNHAPPY_10",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    }
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (GotReward && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing ExitStageLeft=Game.GetNearestWithDefName(Me,"REGION_EXIT_POINT");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (IsDistanceBetweenThingsOver(Me,ExitStageLeft,2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToThing(ExitStageLeft,1.0f,ENTITY_MOVE_WALK,false,true,false,true);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me,1.5f,true,true);
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}
// Attempts: see STATUS.tsv / ATTEMPTS.log (best = probe/mil_a3.cpp)

static bool Balanced(const GameStub& g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&g.PauseDepth==0;}
static bool HasLine(const GameStub& g,const char* n){for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)return true;return false;}
int main(){bool ok=true;
 {GameStub g;Man m(g);g.TalkFrames.push_back(8);m.Main();ok=ok&&Balanced(g)&&g.SetupCalls==6&&m.GivenLetter&&!m.GotReward&&g.LetterGifts==1&&g.IngredientGifts==0&&g.ClearCount==1&&HasLine(g,"TEXT_QST_B10_MAN_IN_LOVE_INTRO_70")&&HasLine(g,"TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER");}
 for(int state=1;state<=2;++state){GameStub g;Man m(g);m.GivenLetter=true;m.ParentClass.MansLoverState=state;g.HoldingGift=state==2;g.HoldingLetter=state==1;m.Main();ok=ok&&Balanced(g)&&m.GotReward&&m.HelpedGuyOut&&!g.HoldingGift&&!g.HoldingLetter&&g.Takes==2&&g.IngredientGifts==1&&g.Removes==1&&g.ClearCount==1&&g.Morality==(state==2?5:-7)&&g.Walks==2&&g.Fades==1&&HasLine(g,"TEXT_QST_B10_MAN_IN_LOVE_RETURNED_OBJECT_TO_ME");}
 for(int answer=0;answer<=1;++answer)for(int state=1;state<=2;++state){GameStub g;Man m(g);m.GivenLetter=true;m.ParentClass.MansLoverState=state;g.HoldingGift=true;g.Answers[0]=0;g.Answers[1]=answer;m.Main();float morality=answer?(state==2?5:-7):(state==2?-9:5);ok=ok&&Balanced(g)&&m.GotReward&&m.HelpedGuyOut==(answer!=0)&&g.HoldingGift&&g.Takes==0&&g.QuestionCount==2&&g.IngredientGifts==1&&g.Removes==1&&g.ClearCount==1&&g.Morality==morality&&HasLine(g,answer?"TEXT_QST_B10_MAN_IN_LOVE_HAPPY":"TEXT_QST_B10_MAN_IN_LOVE_UNHAPPY");}
 {GameStub g;Man m(g);m.ParentClass.MansLoverState=ML_KILLED;m.Main();ok=ok&&Balanced(g)&&!m.GivenLetter&&m.GotReward&&!m.HelpedGuyOut&&g.IngredientGifts==1&&g.Removes==1&&g.ClearCount==0&&g.HasInfo&&HasLine(g,"TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED");}
 {GameStub g;Man m(g);m.GivenLetter=true;g.TerminateAt=6;m.Main();ok=ok&&Balanced(g)&&!m.GotReward&&g.IngredientGifts==0&&HasLine(g,"TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER");}
 for(int branch=0;branch<3;++branch){GameStub g;Man m(g);m.GivenLetter=true;m.GotReward=true;m.HelpedGuyOut=branch==0;if(branch==1)m.ParentClass.MansLoverState=ML_KILLED;m.Main();const char* line=branch==0?"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_HAPPY_10":branch==1?"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10":"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_UNHAPPY_10";ok=ok&&Balanced(g)&&HasLine(g,line)&&g.IngredientGifts==0&&g.Fades==1;}
 for(int kind=0;kind<4;++kind){GameStub g;Man m(g);m.GivenLetter=true;g.TalkFrames.clear();g.TerminateAt=5;if(kind==0||kind==3)g.HitAt=1;if(kind==1||kind==2)g.SpecialAt=1;if(kind>=2)g.HealAt=1;m.Main();ok=ok&&Balanced(g)&&HasLine(g,"TEXT_QST_B10_MAN_IN_LOVE_ON_HIT")== (kind!=2);}
 {GameStub g;Man m(g);g.TerminateAt=3;m.Main();ok=ok&&Balanced(g)&&!m.GivenLetter&&g.Frame==3&&g.Lines.size()==2&&g.LetterGifts==0;}
 {GameStub g;Man m(g);m.GivenLetter=true;g.HoldingGift=true;g.QuestionDelay=100;g.TerminateAt=3;m.Main();ok=ok&&Balanced(g)&&!m.GotReward&&g.QuestionCount==1&&g.Takes==0&&g.Frame==3;}
 {GameStub g;Man m(g);g.StartAt=10;g.TerminateAt=3;m.Main();ok=ok&&Balanced(g)&&g.Lines.empty()&&!m.GivenLetter&&g.Frame==3;}
 {GameStub g;Man m(g);g.Health=0;m.Main();ok=ok&&Balanced(g)&&m.GivenLetter&&g.LetterGifts==1&&g.Lines.empty()&&g.ClearCount==1;}
 {GameStub g;Man m(g);g.Live=false;m.Main();ok=ok&&Balanced(g)&&g.Frame==1&&g.Stopped&&g.SetupCalls==0&&g.Lines.empty();}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0;std::printf(ok?"00ec8300_TEST PASS\n":"00ec8300_TEST FAIL\n");return ok?0:1;
}