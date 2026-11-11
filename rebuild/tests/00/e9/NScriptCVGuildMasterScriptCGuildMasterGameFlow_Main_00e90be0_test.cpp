// Deterministic dependencies around the unchanged selected Main body.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cassert>
static int Strings=0,Handles=0,Movies=0;
class CCharString { public:
 std::string Text;CCharString():Text(){++Strings;}CCharString(const char* p):Text(p){++Strings;}
 CCharString(const CCharString& p):Text(p.Text){++Strings;}~CCharString(){--Strings;}
 CCharString& operator=(const CCharString& p){Text=p.Text;return *this;}
 bool operator<(const CCharString& p)const{return Text<p.Text;}
};
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
struct GameStub;static GameStub* Current=0;
class CScriptThing{ public:
 int Id;C3DVector Position;CScriptThing(int n=0):Id(n),Position(){}
 const C3DVector& GetPos()const{return Position;}
 bool MsgIsTalkedToBy(const char*)const;bool MsgIsHitBy(const char*)const;
 bool MsgIsHitByAnySpecialAbilityFrom(const char*)const;bool MsgIsHitBySpecialAbilityFrom(int,const char*)const;
};
class CScriptGameResourceObjectScriptedThingBase{ public:
 int Id;CScriptGameResourceObjectScriptedThingBase():Id(0){++Handles;}
 CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& h):Id(h.Id){++Handles;}
 ~CScriptGameResourceObjectScriptedThingBase(){--Handles;}
 CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& h){Id=h.Id;return *this;}
 bool IsValid()const{return Id!=0;}void Release(){Id=0;}CScriptThing GetScriptThing()const{return CScriptThing(Id);}
 void Speak(const CScriptThing&,const CCharString&,int,bool,bool,bool);bool IsPerformingScriptTask()const;
};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,HERO_ABILITY_HEAL_LIFE_SPELL=14,GROUP_SELECT_NONE=0,EGP_WAITING_FOR_WIZARD_BATTLE=99};
struct GameStub{
 int Frame,StopAt,Checks,StopCheck,StartDelays,HeroStartDelays,QuestFrom,QuestTo,TalkFrame,HitFrame,SpecialFrame,HealFrame,ResidueFrame,Answer,AnswerDelays,TaskDelays,TaskLeft,PauseDepth,Started,Teleports,Effects,Markers,FocalCards,WizardCards,Journals,Questions,Intro,CameraChanges,QuestObjectives;
 bool TalkConsumed,HitConsumed,SpecialConsumed,Alive,TeleportDisabled;
 CScriptThing Hero;std::vector<std::string> Speeches;
 GameStub():Frame(0),StopAt(8),Checks(0),StopCheck(0),StartDelays(0),HeroStartDelays(0),QuestFrom(99),QuestTo(100),TalkFrame(-1),HitFrame(-1),SpecialFrame(-1),HealFrame(-1),ResidueFrame(-1),Answer(1),AnswerDelays(0),TaskDelays(0),TaskLeft(0),PauseDepth(0),Started(0),Teleports(0),Effects(0),Markers(0),FocalCards(0),WizardCards(0),Journals(0),Questions(0),Intro(0),CameraChanges(0),QuestObjectives(0),TalkConsumed(false),HitConsumed(false),SpecialConsumed(false),Alive(true),TeleportDisabled(false),Hero(2){Current=this;}
 bool T(){++Checks;return Frame>=StopAt||(StopCheck&&Checks>=StopCheck);}
 void NewScriptFrame(){++Frame;assert(Frame<300);}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int n){assert(n==4);int& d=t.Id==1?StartDelays:HeroStartDelays;if(d){--d;return false;}h.Id=t.Id;return true;}
 const CScriptThing& GetHero(){return Hero;}
 bool IsQuestActive(const CCharString& s){assert(s.Text=="Q_EndGameFocalSites");return Frame>=QuestFrom&&Frame<QuestTo;}
 void StartMovieSequence(const CCharString& s,CScriptGameResourceObjectMovieBase&){assert(s.Text.empty());++Started;}
 void FixMovieSequenceCamera(bool b){assert(b==(CameraChanges%2==0));++CameraChanges;}
 void SetTeleportingAsActive(bool b){assert(!b);TeleportDisabled=true;}
 void GiveHeroQuestCardDirectly(const CCharString& card,const CCharString& q,bool b){assert(!b);if(q.Text=="Q_EndGameFocalSites"){assert(card.Text=="OBJECT_QUEST_CARD_ACTIVATE_FOCAL_SITES");++FocalCards;}else{assert(q.Text=="Q_WizardBattle"&&card.Text=="OBJECT_QUEST_CARD_WIZARD_BATTLE");++WizardCards;}}
 void SetQuestCardObjective(const CCharString& q,const CCharString& t,const CCharString& a,const CCharString& b){assert(q.Text=="Q_EndGameFocalSites"&&t.Text=="TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_01"&&a.Text.empty()&&b.Text.empty());++QuestObjectives;}
 CScriptThing GetThingWithScriptName(const CCharString& s){if(s.Text=="TeleporterResidue"){CScriptThing t(3);t.Position=C3DVector(4,8,16);return t;}assert(s.Text=="FocalSitesHSP");return CScriptThing(4);}
 CScriptThing CreateEffect(const CCharString& n,const C3DVector& p,const CCharString& s,float f,bool a,bool b){assert(n.Text=="NEWTELEPORTER2"&&p.X==4&&p.Y==8&&p.Z==16&&s.Text.empty()&&f==0&&!a&&!b);++Effects;return CScriptThing(5);}
 void MiniMapAddMarker(const CScriptThing& t,const CCharString& s){assert(t.Id==3&&s.Text=="HUD_ORB_QUEST_CORE");++Markers;}
 void EntityTeleportToThing(const CScriptThing& a,const CScriptThing& b,bool f){assert(a.Id==2&&b.Id==4&&!f);++Teleports;}
 float GetHealth(const CScriptThing& t){assert(t.Id==1);return Alive?1.0f:0.0f;}
 void GiveHeroYesNoQuestion(const CCharString& q,const CCharString& y,const CCharString& n,const CCharString& s,bool b){assert(q.Text=="TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_QUESTION_TAKE_CARD"&&y.Text=="TEXT_OBJECT_HERO_ANSWER_YES"&&n.Text=="TEXT_OBJECT_HERO_ANSWER_NO"&&s.Text.empty()&&b);++Questions;}
 long MsgIsQuestionAnsweredYesOrNo(){if(AnswerDelays){--AnswerDelays;return -1;}return Answer;}
 void TakeObjectFromHero(const CCharString& s){assert(s.Text=="OBJECT_MAZE_JOURNAL");++Journals;}
};
bool CScriptThing::MsgIsTalkedToBy(const char* s)const{assert(Id==1&&!std::strcmp(s,"SCRIPT_NAME_HERO"));if(Current->Frame==Current->TalkFrame&&!Current->TalkConsumed){Current->TalkConsumed=true;return true;}return false;}
bool CScriptThing::MsgIsHitBy(const char* s)const{assert(Id==1&&!std::strcmp(s,"SCRIPT_NAME_HERO"));if(Current->Frame==Current->HitFrame&&!Current->HitConsumed){Current->HitConsumed=true;return true;}return false;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const char*)const{return Current->Frame==Current->SpecialFrame&&!Current->SpecialConsumed;}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int n,const char*)const{assert(n==14);Current->SpecialConsumed=true;return Current->Frame==Current->HealFrame;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& t,const CCharString& s,int n,bool a,bool b,bool c){assert(Id==1&&t.Id==2&&n==0&&!a&&b&&!c);Current->Speeches.push_back(s.Text);Current->TaskLeft=Current->TaskDelays;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{if(Current->TaskLeft){--Current->TaskLeft;return true;}return false;}
struct CWideScreenMagicPauseEntities{GameStub* G;CWideScreenMagicPauseEntities(GameStub& g):G(&g){++G->PauseDepth;}~CWideScreenMagicPauseEntities(){--G->PauseDepth;}};
static bool IsDistanceFromThingToPositionUnder(const CScriptThing& t,const C3DVector& p,float f){assert(t.Id==2&&p.X==4&&p.Y==8&&p.Z==16&&f==3);return Current->Frame==Current->ResidueFrame;}
static void RunCutsceneMacro(const CCharString& n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& m,int a,int b,bool c,bool d){assert(n.Text=="CS_FOCALSITEINTRO"&&m.size()==2&&m["HERO"].Id==2&&m["GUILDDUDE"].Id==1&&a==0&&b==0&&!c&&d);++Current->Intro;}
struct CIsThingAlive{CScriptThing Thing;CIsThingAlive(const CScriptThing& t):Thing(t){}};
struct GuildMaster{
 GameStub* Game;CScriptThing Me;struct Master{int PostSavePosition;Master():PostSavePosition(0){}}Data;
 struct Parent{CCharString GetGuildMasterSpeech(){return "CURRENT_GUILDMASTER_SPEECH";}}ParentClass;
 GuildMaster(GameStub& g):Game(&g),Me(1){}Master* GetMasterData(){return &Data;}
 void SetPredicate(const CIsThingAlive& p){assert(p.Thing.Id==1);}bool IsActiveThreadTerminating(){return Game->T();}void Main();
};

void GuildMaster::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
        if (Game.IsQuestActive("Q_EndGameFocalSites")) {
            if (IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                if (seh_hero.IsValid()) seh_hero.Release();
                while (!Game.StartScriptingEntity(Game.GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"] = seh_hero;
                mymap["GUILDDUDE"] = seh_me;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_FOCALSITEINTRO", mymap, 0, 0, false, true);
                Game.FixMovieSequenceCamera(false);
            }
            Game.SetTeleportingAsActive(false);
            Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ACTIVATE_FOCAL_SITES", "Q_EndGameFocalSites", false);
            Game.SetQuestCardObjective("Q_EndGameFocalSites", "TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_01", "", "");
            CScriptThing Effect = Game.CreateEffect("NEWTELEPORTER2", Game.GetThingWithScriptName("TeleporterResidue").GetPos(), "", 0.0f, false, false);
            Game.MiniMapAddMarker(Game.GetThingWithScriptName("TeleporterResidue"), "HUD_ORB_QUEST_CORE");
            while (Game.IsQuestActive("Q_EndGameFocalSites")) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (IsDistanceFromThingToPositionUnder(Game.GetHero(), Game.GetThingWithScriptName("TeleporterResidue").GetPos(), 3.0f)) {
                    if (IsActiveThreadTerminating()) return;
                    Game.EntityTeleportToThing(Game.GetHero(), Game.GetThingWithScriptName("FocalSitesHSP"), false);
                }
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), ParentClass.GetGuildMasterSpeech(), GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_081_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                }
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_081_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (GetMasterData()->PostSavePosition == EGP_WAITING_FOR_WIZARD_BATTLE) {
                if (IsActiveThreadTerminating()) return;
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game.GiveHeroYesNoQuestion("TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_QUESTION_TAKE_CARD", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long YNQAnswer;
                while ((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) < 0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                if (YNQAnswer == 1) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_YES", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_WIZARD_BATTLE", "Q_WizardBattle", false);
                    Game.TakeObjectFromHero("OBJECT_MAZE_JOURNAL");
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_NO", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), ParentClass.GetGuildMasterSpeech(), GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
}
}

static bool Balanced(const GameStub& g){return Strings==0&&Handles==0&&Movies==0&&g.PauseDepth==0;}
static bool Has(const GameStub& g,const char* s){for(unsigned i=0;i<g.Speeches.size();++i)if(g.Speeches[i]==s)return true;return false;}
int main(){bool ok=true;
 {GameStub g;GuildMaster w(g);w.Main();ok=ok&&Balanced(g)&&g.Speeches.empty()&&g.Started==0;}
 {GameStub g;GuildMaster w(g);g.QuestFrom=1;g.QuestTo=6;g.TalkFrame=3;g.HitFrame=4;g.ResidueFrame=2;w.Main();ok=ok&&Balanced(g)&&g.Intro==1&&g.CameraChanges==2&&g.FocalCards==1&&g.QuestObjectives==1&&g.Effects==1&&g.Markers==1&&g.TeleportDisabled&&g.Teleports==1&&g.Started==3&&g.Speeches.size()==2&&Has(g,"CURRENT_GUILDMASTER_SPEECH")&&Has(g,"TEXT_QST_081_ATTACKED");}
 {GameStub g;GuildMaster w(g);g.TalkFrame=2;w.Main();ok=ok&&Balanced(g)&&Has(g,"CURRENT_GUILDMASTER_SPEECH")&&g.Questions==0;}
 for(int answer=0;answer<=1;++answer){GameStub g;GuildMaster w(g);w.Data.PostSavePosition=EGP_WAITING_FOR_WIZARD_BATTLE;g.TalkFrame=2;g.Answer=answer;g.AnswerDelays=1;g.TaskDelays=1;g.StopAt=12;w.Main();ok=ok&&Balanced(g)&&g.Questions==1&&g.WizardCards==answer&&g.Journals==answer&&Has(g,"TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE")&&Has(g,answer?"TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_YES":"TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_NO");}
 for(int inside=0;inside<2;++inside)for(int mode=0;mode<3;++mode){GameStub g;GuildMaster w(g);if(inside){g.QuestFrom=1;g.QuestTo=6;}if(mode==0)g.HitFrame=3;else{g.SpecialFrame=3;if(mode==2)g.HealFrame=3;}w.Main();ok=ok&&Balanced(g)&&Has(g,"TEXT_QST_081_ATTACKED")==bool(mode!=2);}
 {GameStub g;GuildMaster w(g);g.Alive=false;g.HitFrame=2;g.TalkFrame=3;w.Main();ok=ok&&Balanced(g)&&g.Speeches.empty()&&g.Started==2;}
 {GameStub g;GuildMaster w(g);g.StartDelays=2;g.HeroStartDelays=1;g.QuestFrom=3;g.QuestTo=6;w.Main();ok=ok&&Balanced(g)&&g.Intro==1&&g.StartDelays==0&&g.HeroStartDelays==0;}
 {GameStub g;GuildMaster w(g);g.StartDelays=100;w.Main();ok=ok&&Balanced(g)&&g.Started==0;}
 {GameStub g;GuildMaster w(g);g.QuestFrom=1;g.QuestTo=20;g.TalkFrame=2;g.TaskDelays=100;w.Main();ok=ok&&Balanced(g)&&g.Speeches.size()==1;}
 // Exit at every observed termination check across focal-site and question paths.
 for(int mode=0;mode<3;++mode)for(int stop=1;stop<=75;++stop){GameStub g;GuildMaster w(g);g.StopCheck=stop;if(mode==0){g.QuestFrom=1;g.QuestTo=6;g.TalkFrame=2;g.HitFrame=4;g.TaskDelays=1;}else{w.Data.PostSavePosition=EGP_WAITING_FOR_WIZARD_BATTLE;g.TalkFrame=2;g.Answer=mode-1;g.AnswerDelays=1;g.TaskDelays=1;}w.Main();ok=ok&&Balanced(g);}
 std::printf(ok?"00e90be0_TEST PASS\n":"00e90be0_TEST FAIL\n");return ok?0:1;
}