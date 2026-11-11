// Selected Main body executed with deterministic interface stubs.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings=0,Movies=0,Handles=0;
class CCharString { public:
 std::string Text;
 CCharString():Text(){++Strings;} CCharString(const char* p):Text(p){++Strings;}
 CCharString(const CCharString& p):Text(p.Text){++Strings;}~CCharString(){--Strings;}
 CCharString& operator=(const char* p){Text=p;return *this;}
 CCharString& operator=(const CCharString& p){Text=p.Text;return *this;}
 bool operator==(const CCharString& p)const{return Text==p.Text;}
};
struct GameStub;static GameStub* Current=0;
class CScriptThing { public:
 int Id;CScriptThing(int n=0):Id(n){}
 bool MsgIsPresentedWithItem(CCharString&)const;bool MsgIsTalkedToBy(const char*)const;
};
class CScriptGameResourceObjectScriptedThingBase { public:
 bool Valid;CScriptGameResourceObjectScriptedThingBase():Valid(false){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}
 bool IsValid()const{return Valid;}void Release(){Valid=false;}
 CScriptThing GetScriptThing()const{return CScriptThing(1);}
 void Speak(const CScriptThing&,const char*,int,bool,bool,bool);
 bool IsPerformingScriptTask()const;
 void ClearCommands();void MoveToThing(const CScriptThing&,float,int,bool,bool,bool,bool);
};
class CScriptGameResourceObjectMovieBase { public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct GameStub {
 int Frame,StopAt,Checks,StopCheck,StartDelays,SpeechFrames,TaskUntil,PauseDepth,Question,AnswerPolls,AnswerDelay,Answer1,Answer2;
 int Taken,TruthGifts,LieGifts,Morality,Walks,Fades,Cleared,Information,Predicates,MoviesStarted,Coin;
 bool Healthy,HasLetter,Talk,TalkConsumed,PresentConsumed,Cancelled,Far;
 const char* Presented;std::vector<std::string> Lines,Cameras,Questions;
 GameStub():Frame(0),StopAt(20),Checks(0),StopCheck(0),StartDelays(0),SpeechFrames(0),TaskUntil(0),PauseDepth(0),Question(0),AnswerPolls(0),AnswerDelay(0),Answer1(1),Answer2(0),Taken(0),TruthGifts(0),LieGifts(0),Morality(0),Walks(0),Fades(0),Cleared(0),Information(0),Predicates(0),MoviesStarted(0),Coin(0),Healthy(true),HasLetter(true),Talk(true),TalkConsumed(false),PresentConsumed(false),Cancelled(false),Far(false),Presented(0){Current=this;}
 bool T(){++Checks;return Cancelled||Frame>=StopAt||(StopCheck&&Checks>=StopCheck);}
 void NewScriptFrame(){++Frame;assert(Frame<1000);}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(t.Id==1&&p==4);if(StartDelays){--StartDelays;return false;}h.Valid=true;return true;}
 CScriptThing GetHero(){return CScriptThing(2);}
 void SetThingHasInformation(const CScriptThing& t,bool a,bool b,bool c){assert(t.Id==1&&a&&b&&!c);++Information;}
 void ClearThingHasInformation(const CScriptThing& t){assert(t.Id==1);++Cleared;}
 void StartMovieSequence(const CCharString& text,CScriptGameResourceObjectMovieBase&){assert(text.Text.empty());++MoviesStarted;}
 void FixMovieSequenceCamera(bool){}
 void EntitySetFacingAngleTowardsThing(const CScriptThing& a,const CScriptThing& b,bool){assert(a.Id==1&&b.Id==2);}
 void Pause(float n){assert(n==1.0f);}
 void CameraUseCameraPoint(const CCharString& p,const CScriptThing& t,float a,int b,int c){assert(t.Id==1&&a==-1.0f&&b==0&&c==-1);Cameras.push_back(p.Text);}
 bool IsObjectInThingsPossession(const CCharString& p,const CScriptThing& t){assert(p.Text=="LOVE_LETTER"&&t.Id==2);return HasLetter;}
 float GetHealth(const CScriptThing& t){assert(t.Id==1);return Healthy?1.0f:0.0f;}
 void GiveHeroYesNoQuestion(const CCharString& q,const CCharString& yes,const CCharString& no,const CCharString& empty,bool a){assert(yes.Text=="TEXT_OBJECT_HERO_ANSWER_YES"&&no.Text=="TEXT_OBJECT_HERO_ANSWER_NO"&&empty.Text.empty()&&a);Questions.push_back(q.Text);Question=q.Text.find("LETTER_FROM")!=std::string::npos?2:1;AnswerPolls=0;}
 long MsgIsQuestionAnsweredYesOrNo(){if(AnswerPolls++<AnswerDelay)return -1;return Question==1?Answer1:Answer2;}
 void TakeObjectFromHero(const CCharString& s){assert(s.Text=="LOVE_LETTER"&&HasLetter);HasLetter=false;++Taken;}
 void GiveHeroObject(const CCharString& s,int n,bool b){assert(n==-1&&!b);if(s.Text=="REPLY_LETTER")++TruthGifts;else {assert(s.Text=="OBJECT_SICK_CHILD_WOMANS_HERO_LETTER");++LieGifts;}}
 void GiveHeroMorality(long n){Morality+=(int)n;}
 CScriptThing GetThingWithScriptName(const CCharString& s){assert(s.Text=="MansLoverLeavesHere");return CScriptThing(3);}
};
bool CScriptThing::MsgIsPresentedWithItem(CCharString& s)const{assert(Id==1);if(Current->Presented&&!Current->PresentConsumed){Current->PresentConsumed=true;s=Current->Presented;return true;}return false;}
bool CScriptThing::MsgIsTalkedToBy(const char* s)const{assert(Id==1&&!std::strcmp(s,"SCRIPT_NAME_HERO"));if(Current->Talk&&!Current->TalkConsumed){Current->TalkConsumed=true;return true;}return false;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& h,const char* t,int group,bool a,bool b,bool c){assert(Valid&&h.Id==2&&group==0&&!a&&b&&!c);Current->Lines.push_back(t);Current->TaskUntil=Current->Frame+Current->SpeechFrames;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
void CScriptGameResourceObjectScriptedThingBase::ClearCommands(){Current->TaskUntil=Current->Frame;}
void CScriptGameResourceObjectScriptedThingBase::MoveToThing(const CScriptThing& t,float n,int kind,bool a,bool b,bool c,bool d){assert(t.Id==3&&n==1.0f&&kind==0&&!a&&b&&!c&&d);++Current->Walks;}
struct CWideScreenMagicPauseEntities {GameStub* G;CWideScreenMagicPauseEntities(GameStub& g):G(&g){++G->PauseDepth;}~CWideScreenMagicPauseEntities(){--G->PauseDepth;}};
struct CIsThingAlive {CIsThingAlive(const CScriptThing& t){assert(t.Id==1);}};
struct TRetVoid{void Return(){}};
struct ScriptDef {const char* ObjectToGiveToMansLover;const char* ObjectFromMansLover;long TellingTheTruth,LyingToWomanMorality;};
static ScriptDef Def={"LOVE_LETTER","REPLY_LETTER",5,-7};static ScriptDef* pGlobalScriptDef=&Def;
enum {SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,ENTITY_MOVE_WALK=0,ML_NOT_RECEIVED_LETTER=0,ML_IN_LOVE_WITH_YOU=1,ML_NOT_IN_LOVE_WITH_YOU=2};
static bool IsDistanceBetweenThingsOver(const CScriptThing& a,const CScriptThing& b,float n){assert(a.Id==1&&b.Id==3&&n==2.0f);return Current->Far;}
static void FadeOutAndKillEntity(const CScriptThing& a,float n,bool b,bool c){assert(a.Id==1&&n==1.5f&&b&&c);++Current->Fades;Current->Cancelled=true;}
static int FixtureRand(){return Current->Coin;}
struct Lover {
 GameStub* Game;CScriptThing Me;bool LeavingRegion;
 struct Parent {int MansLoverState;Parent():MansLoverState(0){}}ParentClass;
 Lover(GameStub& g):Game(&g),Me(1),LeavingRegion(false){}
 void SetPredicate(const CIsThingAlive&){++Game->Predicates;}bool IsActiveThreadTerminating(){return Game->T();}
 void Main();
};
#define rand FixtureRand
void Lover::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
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
    if (ParentClass.MansLoverState==ML_NOT_RECEIVED_LETTER) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me,true,true,false);
    }
    CCharString PresentedItem;
    CCharString ItemName;
    bool TriggerSpeechFromPresentingItem=false;
    ItemName=pGlobalScriptDef->ObjectToGiveToMansLover;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsPresentedWithItem(PresentedItem) && ItemName==PresentedItem) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TriggerSpeechFromPresentingItem=true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || TriggerSpeechFromPresentingItem) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (TriggerSpeechFromPresentingItem) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.FixMovieSequenceCamera(true);
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                Game.Pause(1.0f);
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),true);
                Game.NewScriptFrame();
                Game.CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.FixMovieSequenceCamera(true);
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                Game.Pause(1.0f);
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),true);
                Game.NewScriptFrame();
                Game.CameraUseCameraPoint("CAM_REL_WOMAN",Me,-1.0f,0,-1);
            }
            if (Game.IsObjectInThingsPossession(pGlobalScriptDef->ObjectToGiveToMansLover,Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                bool AnsweredYes=false;
                if (!TriggerSpeechFromPresentingItem) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_INTRO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    while (YNQAnswer<0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (YNQAnswer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        AnsweredYes=true;
                    }

                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    AnsweredYes=true;
                }
                if (AnsweredYes || TriggerSpeechFromPresentingItem) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (TriggerSpeechFromPresentingItem) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        TriggerSpeechFromPresentingItem=false;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
                    }
                    Game.TakeObjectFromHero(pGlobalScriptDef->ObjectToGiveToMansLover);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_LETTER_FROM_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    while (YNQAnswer<0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.CameraUseCameraPoint("CAM_REL_WOMAN",Me,-1.0f,0,-1);
                    if (YNQAnswer==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_20",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject(pGlobalScriptDef->ObjectFromMansLover,-1,false);
                        Game.GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                        ParentClass.MansLoverState=ML_NOT_IN_LOVE_WITH_YOU;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_20",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_30",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER",-1,false);
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_LIKES_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroMorality(pGlobalScriptDef->LyingToWomanMorality);
                        ParentClass.MansLoverState=ML_IN_LOVE_WITH_YOU;
                    }
                    Game.ClearThingHasInformation(Me);
                    LeavingRegion=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_NOT_SEEN_10",GROUP_SELECT_NONE,false,true,false);
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
                if (LeavingRegion && rand()%2==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_LEAVING_REGION_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    switch (ParentClass.MansLoverState) {
                    case ML_NOT_RECEIVED_LETTER:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_INTRO_PRELETTER",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case ML_IN_LOVE_WITH_YOU:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case ML_NOT_IN_LOVE_WITH_YOU:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_MAN",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    default:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    }
                }
            }
            Game.FixMovieSequenceCamera(false);
        }
        if (LeavingRegion && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing ExitStageLeft=Game.GetThingWithScriptName("MansLoverLeavesHere");
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

#undef rand
static bool Balanced(const GameStub& g){return Strings==0&&Movies==0&&Handles==0&&g.PauseDepth==0;}
static bool HasLine(const GameStub& g,const char* suffix){std::string s=std::string("TEXT_QST_B10_MANS_LOVER_")+suffix;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==s)return true;return false;}
int main(){bool ok=true;
 {GameStub g;Lover l(g);l.Main();ok=ok&&Balanced(g)&&g.Taken==1&&g.TruthGifts==1&&g.LieGifts==0&&g.Morality==5&&l.ParentClass.MansLoverState==2&&l.LeavingRegion&&g.Fades==1&&g.Questions.size()==2&&HasLine(g,"INTRO")&&HasLine(g,"MAN_LIKES_ME_20")&&!HasLine(g,"MAN_NOT_SEEN_10");}
 {GameStub g;Lover l(g);g.Answer1=0;l.Main();ok=ok&&Balanced(g)&&g.Taken==0&&g.HasLetter&&!l.LeavingRegion&&HasLine(g,"MAN_NOT_SEEN_10")&&!HasLine(g,"FROM_YOU_10")&&g.Questions.size()==1;}
 {GameStub g;Lover l(g);g.Answer1=2;l.Main();ok=ok&&Balanced(g)&&g.Taken==0&&HasLine(g,"MAN_NOT_SEEN_10");}
 {GameStub g;Lover l(g);g.Answer2=1;l.Main();ok=ok&&Balanced(g)&&g.LieGifts==1&&g.Morality==-7&&l.ParentClass.MansLoverState==1&&HasLine(g,"FROM_YOU_20")&&HasLine(g,"FROM_YOU_30")&&HasLine(g,"LIKES_YOU_10");}
 {GameStub g;Lover l(g);g.Presented="LOVE_LETTER";g.Talk=false;g.Answer1=0;g.Answer2=1;l.Main();ok=ok&&Balanced(g)&&g.Taken==1&&g.LieGifts==1&&g.Questions.size()==1&&!HasLine(g,"INTRO")&&!HasLine(g,"MAN_NOT_SEEN_10");}
 {GameStub g;Lover l(g);g.Presented="OTHER_ITEM";g.Talk=false;l.Main();ok=ok&&Balanced(g)&&g.Taken==0&&g.Lines.empty()&&g.MoviesStarted==0;}
 const int states[]={0,1,2,3,9};const char* lines[]={"INTRO_PRELETTER","IN_LOVE_WITH_YOU_10","IN_LOVE_WITH_MAN","IN_LOVE_WITH_YOU_10","IN_LOVE_WITH_YOU_10"};
 for(int i=0;i<5;++i){GameStub g;Lover l(g);g.HasLetter=false;l.ParentClass.MansLoverState=states[i];l.Main();ok=ok&&Balanced(g)&&HasLine(g,lines[i])&&g.Lines.size()==1&&g.Questions.empty()&&g.Information==(states[i]==0?1:0);}
 {GameStub g;Lover l(g);g.HasLetter=false;l.LeavingRegion=true;g.Far=true;l.Main();ok=ok&&Balanced(g)&&HasLine(g,"LEAVING_REGION_10")&&g.Walks>0&&g.Fades==0;}
 {GameStub g;Lover l(g);g.HasLetter=false;l.LeavingRegion=true;g.Far=true;g.Coin=1;l.ParentClass.MansLoverState=2;l.Main();ok=ok&&Balanced(g)&&HasLine(g,"IN_LOVE_WITH_MAN")&&!HasLine(g,"LEAVING_REGION_10");}
 {GameStub g;Lover l(g);g.Healthy=false;l.Main();ok=ok&&Balanced(g)&&g.Lines.empty()&&g.Taken==1&&g.TruthGifts==1;}
 {GameStub g;Lover l(g);g.StopAt=1;l.Main();ok=ok&&Balanced(g)&&g.Information==0&&g.MoviesStarted==0;}
 {GameStub g;Lover l(g);g.StartDelays=2;g.AnswerDelay=2;g.SpeechFrames=1;g.StopAt=40;l.Main();ok=ok&&Balanced(g)&&g.TruthGifts==1&&g.Frame>=10;}
 {GameStub g;Lover l(g);g.AnswerDelay=100;g.StopAt=5;l.Main();ok=ok&&Balanced(g)&&g.Taken==0&&g.MoviesStarted==1;}
 {GameStub g;Lover l(g);g.SpeechFrames=100;g.StopAt=5;l.Main();ok=ok&&Balanced(g)&&g.Questions.empty()&&g.Taken==0;}
 for(int stop=1;stop<=90;++stop){GameStub g;Lover l(g);g.StopCheck=stop;g.AnswerDelay=1;g.SpeechFrames=1;l.Main();ok=ok&&Balanced(g);}
 std::printf(ok?"00ec9d30_TEST PASS\n":"00ec9d30_TEST FAIL\n");return ok?0:1;
}