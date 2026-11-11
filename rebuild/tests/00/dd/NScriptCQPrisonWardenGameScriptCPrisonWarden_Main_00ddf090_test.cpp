// Execute the selected Warden Main body; game calls are deterministic stubs.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
static int Strings=0,Handles=0,Movies=0;
class CCharString { public:
 std::string Text;CCharString():Text(){++Strings;}CCharString(const char* p):Text(p){++Strings;}
 CCharString(const CCharString& p):Text(p.Text){++Strings;}~CCharString(){--Strings;}
 CCharString& operator=(const CCharString& p){Text=p.Text;return *this;}CCharString& operator=(const char* p){Text=p;return *this;}
 bool operator<(const CCharString& p)const{return Text<p.Text;}
 void Sprintf(const char* fmt,long race,long line){assert(!std::strcmp(fmt,"TEXT_QST_055_WARDEN_POEM_%d_%.2d"));char b[128];std::sprintf(b,"TEXT_QST_055_WARDEN_POEM_%ld_%.2ld",race,line);Text=b;}
};
class C3DVector { public:
 float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector operator-(const C3DVector& p)const{return C3DVector(X-p.X,Y-p.Y,Z-p.Z);}
 float GetMagnitude()const{return (float)std::sqrt(X*X+Y*Y+Z*Z);}
};
struct GameStub;static GameStub* Current=0;
class CScriptThing { public:
 int Id;C3DVector Position;float Angle;CScriptThing(int n=0):Id(n),Position(),Angle(0.5f){}
 const C3DVector& GetPos()const{return Position;}float GetAngleXY()const{return Angle;}
 bool MsgIsHitBy(const char*)const;bool MsgIsHitByAnySpecialAbilityFrom(const char*)const;bool MsgIsHitBySpecialAbilityFrom(int,const char*)const;
};
class CScriptGameResourceObjectScriptedThingBase { public:
 int Id;CScriptGameResourceObjectScriptedThingBase():Id(0){++Handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& p):Id(p.Id){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}
 CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& p){Id=p.Id;return *this;}
 bool IsValid()const{return Id!=0;}void Release(){Id=0;}
};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
class CRGBColour { public:int R,G,B,A;CRGBColour(int r,int g,int b,int a):R(r),G(g),B(b),A(a){}};
struct ScriptDef {long PrisonPoetryMaxLines;float PrisonWardenGameSneakModifier;};static ScriptDef Def={3,0.5f};static ScriptDef* pGlobalScriptDef=&Def;
struct MasterData{bool PrisonGameDoneBefore,PrisonKeyStolenByHero;long PrisonRaceNumber;MasterData():PrisonGameDoneBefore(false),PrisonKeyStolenByHero(false),PrisonRaceNumber(0){}};
struct GameStub {
 int Frame,StopAt,Checks,StopCheck,StartDelays,PauseDepth,Conversations,LastConversation,RemovedConversations,Noisy,Finished,BookStart,BookKey,BookEmpty,Intro,IntroShort,Deactivated,FadeIns,Displayed,Sheathed,Created,Removed,Teleported,KeysTaken,KeysGiven,HandsCleared,MoviesStarted;
 bool Heard,Seen,NormalHit,SpecialHit,HealHit,HitsConsumed,ActiveConversation;
 float Step;CScriptThing Hero;std::vector<std::string> Scenes,Poems,BookNames,Markers;std::vector<float> NoiseBars,PoemBars;
 GameStub():Frame(0),StopAt(6),Checks(0),StopCheck(0),StartDelays(0),PauseDepth(0),Conversations(0),LastConversation(-1),RemovedConversations(0),Noisy(0),Finished(0),BookStart(0),BookKey(0),BookEmpty(0),Intro(0),IntroShort(0),Deactivated(0),FadeIns(0),Displayed(0),Sheathed(0),Created(0),Removed(0),Teleported(0),KeysTaken(0),KeysGiven(0),HandsCleared(0),MoviesStarted(0),Heard(false),Seen(false),NormalHit(false),SpecialHit(false),HealHit(false),HitsConsumed(false),ActiveConversation(false),Step(0),Hero(2){Current=this;Def.PrisonPoetryMaxLines=3;Def.PrisonWardenGameSneakModifier=0.5f;}
 bool T(){++Checks;return Frame>=StopAt||(StopCheck&&Checks>=StopCheck);}
 void NewScriptFrame(){++Frame;Hero.Position.X+=Step;assert(Frame<1000);}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(p==4);if(t.Id==1&&StartDelays){--StartDelays;return false;}h.Id=t.Id;return true;}
 const CScriptThing& GetHero(){return Hero;}
 void EntitySheatheWeapons(const CScriptThing& t,bool b){assert(t.Id==1&&!b);++Sheathed;}
 void StartMovieSequence(const CCharString& s,CScriptGameResourceObjectMovieBase&){assert(s.Text.empty());++MoviesStarted;}
 void FixMovieSequenceCamera(bool){}
 void Pause(float t){assert(t==0.5f);}
 void DisplayQuestInfo(bool b){assert(b);++Displayed;}
 long AddQuestInfoBar(float max,float start,CRGBColour a,CRGBColour b,const CCharString& icon,const CCharString& text,float factor){assert(start==0&&a.R==255&&a.G==0&&a.B==0&&a.A==255&&b.R==0&&b.G==255&&b.B==0&&b.A==255&&text.Text.empty()&&factor==1);if(icon.Text=="HUD_CLOCK_ICON"){assert(max==Def.PrisonPoetryMaxLines);return 10;}assert(icon.Text=="HUD_QUEST_ICON_GUARD"&&max==1);return 11;}
 void FadeScreenIn(){++FadeIns;}
 bool CanThingBe_Heard_ByOtherThing(const CScriptThing& a,const CScriptThing& b){assert(a.Id==2&&b.Id==1);return Heard;}
 bool CanThingBe_Seen_ByOtherThing(const CScriptThing& a,const CScriptThing& b){assert(a.Id==2&&b.Id==1);return Seen;}
 void UpdateQuestInfoBar(long id,float n,float a,float b){assert(a==-1&&b==-1);if(id==10)PoemBars.push_back(n);else{assert(id==11);NoiseBars.push_back(n);}}
 void RemoveConversation(long,bool b){assert(!b);++RemovedConversations;}
 bool IsConversationActive(long id){assert(id==LastConversation);return ActiveConversation;}
 long AddNewConversation(const CScriptThing& t,bool a,bool b){assert(t.Id==1&&!a&&!b);LastConversation=++Conversations;return LastConversation;}
 void AddLineToConversation(long c,const CCharString& p,bool b,const CScriptThing& t,const CScriptThing& other){assert(c==LastConversation&&!b&&t.Id==1&&other.Id==0);Poems.push_back(p.Text);}
 void DeactivateQuestLater(const CCharString& q,int n){assert(q.Text=="Q_PrisonWardenGame"&&n==1);++Deactivated;}
 CScriptThing GetThingWithScriptName(const CCharString& s){Markers.push_back(s.Text);CScriptThing t(3);if(s.Text=="HeroReadBookRed")t.Angle=0.125f;else if(s.Text=="HeroReadBookGreen")t.Angle=0.25f;else{assert(s.Text=="HeroReadBookBlue");t.Angle=0.5f;}return t;}
 void EntityTeleportToThing(const CScriptThing& t,const CScriptThing& d,bool b){assert(t.Id==2&&(d.Id==3||d.Id==0)&&!b);++Teleported;}
 void EntitySetFacingAngle(const CScriptThing& t,float a,bool b){assert(b);if(t.Id==4)assert(a==0.75f);else assert(t.Id==2);}
 CScriptThing CreateObject(const CCharString& n,const C3DVector& p,const CCharString& s){assert(p.X==4&&p.Y==8&&p.Z==16&&s.Text.empty());BookNames.push_back(n.Text);++Created;return CScriptThing(4);}
 void RemoveThing(const CScriptThing& t,bool a,bool b){assert(t.Id==5&&!a&&b);++Removed;}
 void MakeHeroCarryItemInHand(const CCharString& s){assert(s.Text.empty());++HandsCleared;}
 void TakeObjectFromHero(const CCharString& s){assert(s.Text=="OBJECT_PRISON_CELL_KEY");++KeysTaken;}
 void GiveHeroObject(const CCharString& s,int n,bool b){assert(s.Text=="OBJECT_PRISON_CELL_KEY"&&n==-1&&!b);++KeysGiven;}
};
bool CScriptThing::MsgIsHitBy(const char* s)const{assert(Id==1&&!std::strcmp(s,"SCRIPT_NAME_HERO"));if(Current->NormalHit&&!Current->HitsConsumed){Current->HitsConsumed=true;return true;}return false;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const char*)const{return Current->SpecialHit&&!Current->HitsConsumed;}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int a,const char*)const{assert(a==14);Current->HitsConsumed=true;return Current->HealHit;}
struct CWideScreenMagicPauseEntities{GameStub* G;CWideScreenMagicPauseEntities(GameStub& g):G(&g){++G->PauseDepth;}~CWideScreenMagicPauseEntities(){--G->PauseDepth;}};
struct TRetVoid{void Return(){}};
template<class T,class L,class U>inline void GFLimit(T& x,const L& lo,const U& hi){if(x<lo)x=lo;else if(x>hi)x=hi;}
static void RunCutsceneMacro(const CCharString& n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& cast,int a,int b,bool c,bool d){assert(cast["HERO"].Id==2&&cast["GUARD"].Id==1&&a==0&&b==0&&!c);Current->Scenes.push_back(n.Text);if(n.Text=="CS_PRISON_GAME_INTRO")++Current->Intro;else if(n.Text=="CS_PRISON_GAME_INTRO_SHORT")++Current->IntroShort;else if(n.Text=="CS_PRISON_GAME_NOISY")++Current->Noisy;else if(n.Text=="CS_PRISON_GAME_FINISHED")++Current->Finished;else if(n.Text=="CS_PRISON_GAME_BOOK_START"){assert(!d);++Current->BookStart;}else if(n.Text=="CS_PRISON_GAME_BOOK_KEY")++Current->BookKey;else{assert(n.Text=="CS_PRISON_GAME_BOOK_EMPTY");++Current->BookEmpty;}}
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,HERO_ABILITY_HEAL_LIFE_SPELL=14};
struct Warden {
 GameStub* Game;CScriptThing Me;MasterData Data;
 struct Parent {bool SecretDiaryOpened;long BookOpened;CScriptThing SThingBookOpened;Parent():SecretDiaryOpened(false),BookOpened(0),SThingBookOpened(5){SThingBookOpened.Position=C3DVector(4,8,16);}}ParentClass;
 Warden(GameStub& g):Game(&g),Me(1){}MasterData* GetMasterData(){return &Data;}
 bool IsActiveThreadTerminating(){return Game->T();}void Main();
};
void Warden::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySheatheWeapons(Me,false);
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_hero;
        mymap["GUARD"]=seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        Game.FixMovieSequenceCamera(true);
        if (GetMasterData()->PrisonGameDoneBefore) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            RunCutsceneMacro("CS_PRISON_GAME_INTRO_SHORT",mymap,0,0,false,true);
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            GetMasterData()->PrisonGameDoneBefore=true;
            RunCutsceneMacro("CS_PRISON_GAME_INTRO",mymap,0,0,false,true);
        }
        Game.FixMovieSequenceCamera(false);
    }
    Game.Pause(0.5f);
    Game.DisplayQuestInfo(true);
    long PoemTimeBarIndex=Game.AddQuestInfoBar((float)pGlobalScriptDef->PrisonPoetryMaxLines,0.0f,CRGBColour(255,0,0,255),CRGBColour(0,255,0,255),"HUD_CLOCK_ICON","",1.0f);
    long SneakNoiseIndex=Game.AddQuestInfoBar(1.0f,0.0f,CRGBColour(255,0,0,255),CRGBColour(0,255,0,255),"HUD_QUEST_ICON_GUARD","",1.0f);
    Game.FadeScreenIn();
    float HeroNoiseValue=0.0f;
    long CurrentLineOfPoetry=0;
    long ConversationPoetryIndex=-1;
    C3DVector OldHeroPos=Game.GetHero().GetPos();
    float DistanceTravelled=0.0f;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DistanceTravelled=(Game.GetHero().GetPos()-OldHeroPos).GetMagnitude();
        if (Game.CanThingBe_Heard_ByOtherThing(Game.GetHero(),Me)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            HeroNoiseValue+=DistanceTravelled*pGlobalScriptDef->PrisonWardenGameSneakModifier;
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (DistanceTravelled>0.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                HeroNoiseValue+=(pGlobalScriptDef->PrisonWardenGameSneakModifier*0.4f)*DistanceTravelled;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                HeroNoiseValue-=0.01f;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me) || HeroNoiseValue>=1.0f) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.UpdateQuestInfoBar(SneakNoiseIndex,1.0f,-1.0f,-1.0f);
            Game.RemoveConversation(ConversationPoetryIndex,false);
            CurrentLineOfPoetry=CurrentLineOfPoetry-1;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
// Inline-budget: retail calls both cast assignments at this cold site; entry assignments expand.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            mymap["HERO"]=seh_hero;
            mymap["GUARD"]=seh_me;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_PRISON_GAME_NOISY",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
            HeroNoiseValue=0.0f;
        }
        GFLimit(HeroNoiseValue,0,1);
        Game.UpdateQuestInfoBar(SneakNoiseIndex,HeroNoiseValue,-1.0f,-1.0f);
        if (ParentClass.SecretDiaryOpened) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.RemoveConversation(ConversationPoetryIndex,false);
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
// Inline-budget: retail calls both cast assignments at this cold site; entry assignments expand.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                mymap["HERO"]=seh_hero;
                mymap["GUARD"]=seh_me;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_PRISON_GAME_BOOK_START",mymap,0,0,false,false);
                CCharString ObjectName("OBJECT_BOOK_OPEN_01");
                CScriptThing BookMarker;
                if (ParentClass.BookOpened==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    BookMarker=Game.GetThingWithScriptName("HeroReadBookRed");
                    ObjectName="OBJECT_BOOK_OPEN_01";
                }
                else if (ParentClass.BookOpened==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    BookMarker=Game.GetThingWithScriptName("HeroReadBookGreen");
                    ObjectName="OBJECT_BOOK_OPEN_04";
                }
                else if (ParentClass.BookOpened==2) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    BookMarker=Game.GetThingWithScriptName("HeroReadBookBlue");
                    ObjectName="OBJECT_BOOK_OPEN_02";
                }
                Game.EntityTeleportToThing(Game.GetHero(),BookMarker,false);
                Game.EntitySetFacingAngle(Game.GetHero(),BookMarker.GetAngleXY(),true);
                CScriptThing NewBook=Game.CreateObject(ObjectName,ParentClass.SThingBookOpened.GetPos(),"");
                Game.EntitySetFacingAngle(NewBook,ParentClass.SThingBookOpened.GetAngleXY()+0.25f,true);
                Game.RemoveThing(ParentClass.SThingBookOpened,false,true);
                if (GetMasterData()->PrisonRaceNumber>=1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    RunCutsceneMacro("CS_PRISON_GAME_BOOK_KEY",mymap,0,0,false,true);
                    Game.MakeHeroCarryItemInHand("");
                    Game.TakeObjectFromHero("OBJECT_PRISON_CELL_KEY");
                    Game.GiveHeroObject("OBJECT_PRISON_CELL_KEY",-1,false);
                    GetMasterData()->PrisonKeyStolenByHero=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    RunCutsceneMacro("CS_PRISON_GAME_BOOK_EMPTY",mymap,0,0,false,true);
                }
                Game.FixMovieSequenceCamera(false);
            }
            Game.DeactivateQuestLater("Q_PrisonWardenGame",1);
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }

        if (ConversationPoetryIndex==-1 || !Game.IsConversationActive(ConversationPoetryIndex)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CurrentLineOfPoetry=CurrentLineOfPoetry+1;
            if (CurrentLineOfPoetry>=pGlobalScriptDef->PrisonPoetryMaxLines) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
// Inline-budget: retail calls both cast assignments at this cold site; entry assignments expand.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    mymap["HERO"]=seh_hero;
                    mymap["GUARD"]=seh_me;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_PRISON_GAME_FINISHED",mymap,0,0,false,true);
                    Game.FixMovieSequenceCamera(false);
                }
                Game.DeactivateQuestLater("Q_PrisonWardenGame",1);
                while (true) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                CCharString poem_line;
                poem_line.Sprintf("TEXT_QST_055_WARDEN_POEM_%d_%.2d",GetMasterData()->PrisonRaceNumber%3+1,CurrentLineOfPoetry);
                Game.AddLineToConversation(HLStyleConversationIndex,poem_line,false,Me,CScriptThing());
                ConversationPoetryIndex=HLStyleConversationIndex;
                Game.UpdateQuestInfoBar(PoemTimeBarIndex,(float)CurrentLineOfPoetry,-1.0f,-1.0f);
            }
        }
        OldHeroPos=Game.GetHero().GetPos();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}

static bool Balanced(const GameStub& g){return Strings==0&&Handles==0&&Movies==0&&g.PauseDepth==0;}
int main(){bool ok=true;
 {GameStub g;Warden w(g);w.Main();ok=ok&&Balanced(g)&&g.Intro==1&&w.Data.PrisonGameDoneBefore&&g.Finished==1&&g.Deactivated==1&&g.Poems.size()==2&&g.Poems[0]=="TEXT_QST_055_WARDEN_POEM_1_01"&&g.Poems[1]=="TEXT_QST_055_WARDEN_POEM_1_02"&&g.PoemBars.size()==2&&g.PoemBars[1]==2;}
 {GameStub g;Warden w(g);w.Data.PrisonGameDoneBefore=true;w.Data.PrisonRaceNumber=2;g.ActiveConversation=true;w.Main();ok=ok&&Balanced(g)&&g.IntroShort==1&&g.Intro==0&&g.Finished==0&&g.Poems.size()==1&&g.Poems[0]=="TEXT_QST_055_WARDEN_POEM_3_01";}
 {GameStub g;Warden w(g);Def.PrisonPoetryMaxLines=100;g.Heard=true;g.Step=1;g.StopAt=2;w.Main();ok=ok&&Balanced(g)&&g.Noisy==0&&g.NoiseBars.size()==2&&g.NoiseBars[0]==0&&g.NoiseBars[1]==0.5f;}
 {GameStub g;Warden w(g);Def.PrisonPoetryMaxLines=100;Def.PrisonWardenGameSneakModifier=0.625f;g.Step=1;g.StopAt=2;w.Main();ok=ok&&Balanced(g)&&g.Noisy==0&&g.NoiseBars.size()==2&&g.NoiseBars[1]==0.25f;}
 {GameStub g;Warden w(g);Def.PrisonPoetryMaxLines=100;g.Heard=true;g.Step=2;g.StopAt=2;w.Main();ok=ok&&Balanced(g)&&g.Noisy==1&&g.NoiseBars.size()==3&&g.NoiseBars[1]==1&&g.NoiseBars[2]==0;}
 {GameStub g;Warden w(g);g.NormalHit=true;g.StopAt=1;w.Main();ok=ok&&Balanced(g)&&g.Noisy==1&&g.Poems.size()==1&&g.Poems[0]=="TEXT_QST_055_WARDEN_POEM_1_00";}
 {GameStub g;Warden w(g);g.SpecialHit=true;g.StopAt=1;w.Main();ok=ok&&Balanced(g)&&g.Noisy==1;}
 {GameStub g;Warden w(g);g.SpecialHit=true;g.HealHit=true;g.StopAt=1;w.Main();ok=ok&&Balanced(g)&&g.Noisy==0;}
 {GameStub g;Warden w(g);g.Seen=true;g.StopAt=1;w.Main();ok=ok&&Balanced(g)&&g.Noisy==1;}
 const char* books[]={"OBJECT_BOOK_OPEN_01","OBJECT_BOOK_OPEN_04","OBJECT_BOOK_OPEN_02"};
 for(int i=0;i<3;++i){GameStub g;Warden w(g);w.ParentClass.SecretDiaryOpened=true;w.ParentClass.BookOpened=i;w.Main();ok=ok&&Balanced(g)&&g.BookStart==1&&g.BookEmpty==1&&g.BookKey==0&&g.Created==1&&g.Removed==1&&g.Teleported==1&&g.BookNames[0]==books[i]&&g.Deactivated==1&&g.Poems.empty()&&!w.Data.PrisonKeyStolenByHero;}
 {GameStub g;Warden w(g);w.ParentClass.SecretDiaryOpened=true;w.Data.PrisonRaceNumber=1;w.Main();ok=ok&&Balanced(g)&&g.BookKey==1&&g.BookEmpty==0&&g.KeysTaken==1&&g.KeysGiven==1&&g.HandsCleared==1&&w.Data.PrisonKeyStolenByHero;}
 {GameStub g;Warden w(g);g.StartDelays=2;w.Main();ok=ok&&Balanced(g)&&g.StartDelays==0&&g.Intro==1;}
 {GameStub g;Warden w(g);g.StartDelays=100;g.StopAt=1;w.Main();ok=ok&&Balanced(g)&&g.Intro==0&&g.Displayed==0;}
 {GameStub g;Warden w(g);g.StopCheck=1;w.Main();ok=ok&&Balanced(g)&&g.Intro==0;}
 for(int n=1;n<=70;++n){GameStub g;Warden w(g);w.ParentClass.SecretDiaryOpened=true;w.Data.PrisonRaceNumber=1;g.StopCheck=n;w.Main();ok=ok&&Balanced(g);}
 for(int n=1;n<=65;++n){GameStub g;Warden w(g);g.StopCheck=n;g.NormalHit=true;w.Main();ok=ok&&Balanced(g);}
 std::printf(ok?"00ddf090_TEST PASS\n":"00ddf090_TEST FAIL\n");return ok?0:1;
}