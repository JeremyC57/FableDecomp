// Full selected Main with observable dependency doubles; no game data.
#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
static int Strings,Things,Handles,Movies,Threads;
struct CCharString {std::string text;CCharString(const char*s=""):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}bool operator<(const CCharString&s)const{return text<s.text;}};
static CCharString operator+(const CCharString&a,const CCharString&b){return CCharString((a.text+b.text).c_str());}
struct GameStub;static GameStub*Current;
struct CScriptThing {int id;CScriptThing(int n=0):id(n){++Things;}CScriptThing(const CScriptThing&t):id(t.id){++Things;}~CScriptThing(){--Things;}bool MsgIsUsedBy(const char*)const;};
struct CScriptGameResourceObjectScriptedThingBase {int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&x):id(x.id){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}};
struct CScriptGameResourceObjectMovieBase {CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct CSpawnedFuncBase {CSpawnedFuncBase(){++Threads;}virtual ~CSpawnedFuncBase(){--Threads;}};
struct CQ_OpeningGraveyardSecretPassageScript {bool finished_quest,DoorChange,skeleton_visited,SkeletonStuff[4];CCharString hero_object[4];std::vector<CSpawnedFuncBase*>Owned;CQ_OpeningGraveyardSecretPassageScript():finished_quest(false),DoorChange(false),skeleton_visited(false){const char*n[]={"sword","shield","armour","helmet"};for(int i=0;i<4;++i){SkeletonStuff[i]=false;hero_object[i]=n[i];}}~CQ_OpeningGraveyardSecretPassageScript(){for(unsigned i=0;i<Owned.size();++i)delete Owned[i];}void FadeObjectUp(CScriptThing){}void AddSpawnedFunction(CSpawnedFuncBase*p,const CCharString&name){assert(name.text.empty());Owned.push_back(p);}};
template<class C,class A>struct CSpawnedFunc1:public CSpawnedFuncBase {A Par1;CSpawnedFunc1(const CCharString&,long,C*,void(C::*)(A),A);};
struct GlobalDef{long EXPERIENCE_GRANT_LARGE;GlobalDef():EXPERIENCE_GRANT_LARGE(100){}};static GlobalDef Def;static GlobalDef*pGlobalScriptDef=&Def;
enum{SCRIPT_AI_PRIORITY_HIGHEST=4};
struct GameStub {CQ_OpeningGraveyardSecretPassageScript*P;int Frame,StopAt,StopQuery,Queries,NearAt,UseAt,PauseDepth,CameraDepth,Experience,OpenCount,Spawns,Usable,Created;unsigned Possession,Taken,CreatedMask;std::vector<std::string>Scenes,Objectives;bool SlowUse;
GameStub():P(0),Frame(0),StopAt(5),StopQuery(-1),Queries(0),NearAt(0),UseAt(1),PauseDepth(0),CameraDepth(0),Experience(0),OpenCount(0),Spawns(0),Usable(0),Created(0),Possession(0),Taken(0),CreatedMask(0),SlowUse(false){Current=this;}
void NewScriptFrame(){++Frame;assert(Frame<100);}
CScriptThing GetHero(){return CScriptThing(2);}
CScriptThing GetThingWithScriptName(const char*n){if(std::string(n)=="CryptCutsceneMarker")return CScriptThing(3);if(std::string(n)=="GraveyardDemon")return CScriptThing(4);assert(std::string(n)=="DemonDoorGate");return CScriptThing(5);}
void SetThingAsUsable(const CScriptThing&t,bool b){assert(b&&(t.id==1||t.id==5));++Usable;}
bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&h,int p){assert(p==4&&(t.id==2||t.id==4));h.id=t.id;return true;}
void StartMovieSequence(const CCharString&n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());}
void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}
void FixMovieSequenceCamera(bool b){CameraDepth=b?1:0;}
int Index(const CCharString&n){for(int i=0;i<4;++i)if(P->hero_object[i].text==n.text)return i;assert(false);return -1;}
bool IsObjectInThingsPossession(const CCharString&n,const CScriptThing&h){assert(h.id==2);return (Possession&(1u<<Index(n)))!=0;}
CScriptThing CreateObjectOnEntity(const CCharString&n,const CScriptThing&m,const CCharString&z){assert(m.id==1&&z.text.empty());int i=Index(n);assert(!(CreatedMask&(1u<<i)));CreatedMask|=1u<<i;++Created;return CScriptThing(10+i);}
void SetThingPersistent(const CScriptThing&t,bool b){assert(b&&(t.id==5||(t.id>=10&&t.id<14)));}
void EntitySetAlpha(const CScriptThing&t,float a,bool b){assert(t.id>=10&&t.id<14&&a==0.0f&&b);}
void TakeObjectFromHero(const CCharString&n){unsigned bit=1u<<Index(n);assert(Possession&bit);Possession&=~bit;Taken|=bit;}
void GiveHeroExperience(long n){assert(n==100);Experience+=(int)n;}
void OpenDoor(const CScriptThing&t){assert(t.id==5&&P->DoorChange);++OpenCount;}
CCharString GetActiveQuestName(){return CCharString("quest");}
void SetQuestCardObjective(const CCharString&q,const CCharString&o,const CCharString&a,const CCharString&b){assert(q.text=="quest"&&b.text=="Graveyard");assert(a.text.empty()||a.text=="GraveyardCircle");Objectives.push_back(o.text);}
};
bool CScriptThing::MsgIsUsedBy(const char*n)const{assert(id==1&&std::string(n)=="SCRIPT_NAME_HERO");return Current->Frame>=Current->UseAt&&(!Current->SlowUse||Current->Frame%2==0);}
template<class C,class A>CSpawnedFunc1<C,A>::CSpawnedFunc1(const CCharString&n,long i,C*p,void(C::*f)(A),A a):Par1(a){assert(n.text=="ParentClass.FadeObjectUp"&&i==0&&p==Current->P&&f==&C::FadeObjectUp&&a.id>=10&&a.id<14);++Current->Spawns;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&h,const CScriptThing&m,float f){assert(h.id==2&&m.id==3&&f==2.0f);return Current->Frame>=Current->NearAt;}
static void RunCutsceneMacro(const char*n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,int a,int b,bool c,bool d){assert(a==0&&b==0&&!c&&d&&Current->PauseDepth==1&&Current->CameraDepth==1);assert(m.find(CCharString("HERO"))!=m.end()&&m[CCharString("HERO")].id==2);std::string name(n);if(name=="CS_OPENGRAVE_SKELETON_COMPLETE")assert(m[CCharString("DOOR")].id==4&&Current->P->DoorChange);Current->Scenes.push_back(name);}
struct CWideScreenMagicPauseEntities {GameStub*g;CWideScreenMagicPauseEntities(GameStub&p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
struct TRetVoid{void Return(){}};
struct Skeleton {enum{SWORD,SHIELD,ARMOUR,HELMET,LAST};GameStub*Game;CScriptThing Me;CQ_OpeningGraveyardSecretPassageScript ParentClass;bool collected_all,returned[4];CCharString skeleton_object[4];Skeleton(GameStub&g):Game(&g),Me(1),collected_all(false){g.P=&ParentClass;for(int i=0;i<4;++i){returned[i]=false;skeleton_object[i]=ParentClass.hero_object[i];}}bool IsActiveThreadTerminating(){++Game->Queries;return Game->Frame>=Game->StopAt||(Game->StopQuery>=0&&Game->Queries>=Game->StopQuery);}void Main();};
void Skeleton::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    bool first_time=true;
    bool Triggered=false;
    Game.SetThingAsUsable(Me,true);
    while (!collected_all) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!Triggered) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!IsDistanceBetweenThingsUnder(Game.GetHero(),Game.GetThingWithScriptName("CryptCutsceneMarker"),2.0f)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Triggered=true;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_OPENGRAVE_CRYPT_ENTER",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03","","Graveyard");
        }
        ParentClass.skeleton_visited=true;
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        for (int i=0; i<LAST; ++i) {
            if (Game.IsObjectInThingsPossession(ParentClass.hero_object[i],Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                returned[i]=true;
                CScriptThing new_item=Game.CreateObjectOnEntity(skeleton_object[i],Me,"");
                Game.SetThingPersistent(new_item,true);
                Game.EntitySetAlpha(new_item,0.0f,true);
                Game.TakeObjectFromHero(ParentClass.hero_object[i]);
                ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_OpeningGraveyardSecretPassageScript,CScriptThing>(CCharString("ParentClass.")+CCharString("FadeObjectUp"),0,&ParentClass,&CQ_OpeningGraveyardSecretPassageScript::FadeObjectUp,new_item),CCharString(""));
                if (!first_time) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        collected_all=true;
        for (int j=0; j<LAST; ++j) {
            ParentClass.SkeletonStuff[j]=!returned[j];
            if (!returned[j]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                collected_all=false;
            }
        }
        if (collected_all) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_LARGE);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            ParentClass.DoorChange=true;
            {
                CScriptGameResourceObjectScriptedThingBase seh_Demon;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetThingWithScriptName("GraveyardDemon"),seh_Demon,SCRIPT_AI_PRIORITY_HIGHEST);
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                mymap["HERO"]=seh_hero;
                mymap["DOOR"]=seh_Demon;
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_COMPLETE",mymap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                CScriptThing door=Game.GetThingWithScriptName("DemonDoorGate");
                Game.SetThingAsUsable(Game.GetThingWithScriptName("DemonDoorGate"),true);
                Game.OpenDoor(door);
                Game.SetThingPersistent(door,true);
            }
            ParentClass.finished_quest=true;
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_04","GraveyardCircle","Graveyard");
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            if (!Game.IsObjectInThingsPossession(ParentClass.hero_object[SWORD],Game.GetHero()) && !returned[SWORD]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_SWORD",mymap,0,0,false,true);
            }
            else if (!Game.IsObjectInThingsPossession(ParentClass.hero_object[SHIELD],Game.GetHero()) && !returned[SHIELD]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_SHIELD",mymap,0,0,false,true);
            }
            else if (!Game.IsObjectInThingsPossession(ParentClass.hero_object[ARMOUR],Game.GetHero()) && !returned[ARMOUR]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_ARMOUR",mymap,0,0,false,true);
            }
            else if (!Game.IsObjectInThingsPossession(ParentClass.hero_object[HELMET],Game.GetHero()) && !returned[HELMET]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_HELMET",mymap,0,0,false,true);
            }
            Game.FixMovieSequenceCamera(false);
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03","","Graveyard");
        }
        first_time=false;
    }
    while (!IsActiveThreadTerminating()) Game.NewScriptFrame();
}
}
static void Balanced(){assert(Strings==0&&Things==0&&Handles==0&&Movies==0&&Threads==0);}
static void Normal(unsigned mask,bool held){GameStub g;g.Possession=held?mask:0;{Skeleton s(g);if(!held)for(int i=0;i<4;++i)s.returned[i]=(mask&(1u<<i))!=0;s.Main();assert(g.PauseDepth==0&&g.CameraDepth==0);assert(g.Spawns==(held?g.Created:0));assert(g.CreatedMask==(held?mask:0));assert(g.Taken==(held?mask:0));assert(g.Possession==0);assert(s.ParentClass.skeleton_visited);for(int i=0;i<4;++i){assert(s.returned[i]==((mask&(1u<<i))!=0));assert(s.ParentClass.SkeletonStuff[i]==!s.returned[i]);}assert(s.collected_all==(mask==15));assert(s.ParentClass.finished_quest==(mask==15));assert(s.ParentClass.DoorChange==(mask==15));assert(g.Experience==(mask==15?100:0));assert(g.OpenCount==(mask==15?1:0));assert(!g.Scenes.empty()&&g.Scenes[0]=="CS_OPENGRAVE_CRYPT_ENTER");if(mask==15){assert(g.Scenes.size()==2&&g.Scenes[1]=="CS_OPENGRAVE_SKELETON_COMPLETE");assert(g.Objectives.back()=="TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_04");}else{const char*rem[]={"CS_OPENGRAVE_SKELETON_REMIND_SWORD","CS_OPENGRAVE_SKELETON_REMIND_SHIELD","CS_OPENGRAVE_SKELETON_REMIND_ARMOUR","CS_OPENGRAVE_SKELETON_REMIND_HELMET"};int i=0;while(mask&(1u<<i))++i;assert(g.Scenes.size()>1);for(unsigned k=1;k<g.Scenes.size();++k)assert(g.Scenes[k]==rem[i]);}}Balanced();}
int main(){for(unsigned mask=0;mask<16;++mask){Normal(mask,false);Normal(mask,true);}for(int mode=0;mode<4;++mode){GameStub g;if(mode==0)g.NearAt=100;if(mode==1)g.UseAt=100;if(mode==2){g.NearAt=3;g.SlowUse=true;}if(mode==3)g.Possession=15;{Skeleton s(g);if(mode==3)s.collected_all=true;s.Main();assert(g.PauseDepth==0);if(mode<2){assert(g.Created==0&&g.Experience==0);assert(g.Scenes.size()==(mode==0?0u:1u));}if(mode==3){assert(g.Scenes.empty()&&g.Created==0&&g.Possession==15&&!s.ParentClass.finished_quest);}}Balanced();}
// Sweep cancellation at each reachable query along all four reminder arms
// and the all-items completion path; every normal scope must release its objects.
for(int route=0;route<5;++route){unsigned mask=route==4?15u:((1u<<route)-1u);int maxQuery;{GameStub base;base.Possession=mask;{Skeleton s(base);s.Main();}maxQuery=base.Queries;}Balanced();for(int stop=1;stop<=maxQuery;++stop){GameStub g;g.StopQuery=stop;g.Possession=mask;{Skeleton s(g);s.Main();assert(g.PauseDepth==0);assert(g.Experience==0||g.Experience==100);assert(g.Spawns==g.Created);assert((g.Taken&~mask)==0);assert(g.OpenCount<=1);assert(g.Queries==stop);}Balanced();}}
puts("00dc93f0_TEST PASS");return 0;}
