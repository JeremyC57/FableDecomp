#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <map>
struct C3DVector { float X,Y,Z; C3DVector():X(0),Y(0),Z(0){} };
class CCharString { public: const char* text; CCharString(const char* x=""):text(x){} bool operator<(const CCharString& x)const{return strcmp(text,x.text)<0;} };
class CRGBColour { public: unsigned char B,G,R,A; CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):B(b),G(g),R(r),A(a){} };
class CScriptThing { public: int id; C3DVector pos; CScriptThing(int x=0):id(x){} const C3DVector& GetPos()const{return pos;} };
class CScriptGameResourceObjectScriptedThingBase {};
class CScriptGameResourceObjectMovieBase {};
struct CScriptDef { float HH_UpperBarLimit,HH_BarDecrementAmount,HH_BarDecrementInterval;long HH_ClearedHHRenown; };
CScriptDef def={10,3,2,47}; CScriptDef* pGlobalScriptDef=&def;
static int checks,terminate_at,registered,deregistered,cutscenes,items,created,completed,deactivated,shown,removed,renown,enabled,pauses,updates,frames,timer_reads,sets;
static bool was_inside; static float* bar; static bool* in_house;
class CGameScriptInterfaceBase { public:
 void NewScriptFrame() { ++frames; if(!*in_house){*in_house=true;was_inside=true;}else{float values[3]={1,5,12};*bar=values[(frames-(was_inside?2:1))%3];} }
 void DisplayQuestInfo(bool x){assert(x);++shown;}
 long AddQuestInfoBar(float top,float start,const CRGBColour& a,const CRGBColour& b,const CCharString& icon,const CCharString& text,float scale){assert(top==10&&start==0&&a.R==0&&a.G==255&&a.B==0&&a.A==255&&b.R==255&&b.G==0&&b.B==0&&b.A==255&&!strcmp(icon.text,"HUD_QUEST_ICON_UNDEAD")&&!strcmp(text.text,"")&&scale==1);return 19;}
 void UpdateQuestInfoBar(long counter,float value,float a,float b){const float expected[]={0,2,12};assert(counter==19&&updates<3&&value==expected[updates]&&a==-1&&b==-1);++updates;}
 void RemoveQuestInfoElement(long n){assert(n==19);++removed;}
 void StartMovieSequence(const CCharString& name,CScriptGameResourceObjectMovieBase&){assert(!strcmp(name.text,""));}
 void PauseAllNonScriptedEntities(bool value){++pauses;assert(value==(pauses%2==1));}
 CScriptThing GetThingWithScriptName(const CCharString& name){assert(!strcmp(name.text,"HH_DiggingSpot"));return CScriptThing(7);}
 void AddItemToContainer(const CScriptThing& spot,const CCharString& name){assert(spot.id==7&&!strcmp(name.text,"OBJECT_TATTOO_CARD_BACK_02"));++items;}
 void GiveHeroRenownPoints(long n){assert(n==47);++renown;}
 void SetCreatureGeneratorsEnabled(const CCharString& name,bool value){assert(!strcmp(name.text,"HauntedHouse")&&value);++enabled;}
 long GetAllThingsWithScriptName(const CCharString& name,std::vector<CScriptThing>& things)const;
 CScriptThing CreateCreature(const CCharString& name,const C3DVector&,const CCharString& script,bool flag){assert(!strcmp(name.text,"CREATURE_LAKE_NYMPH_01")&&!strcmp(script.text,"")&&!flag);++created;return CScriptThing(8);}
 void Pause(float n){assert(n==1);}
 CCharString GetActiveQuestName(){return CCharString("HauntedQuest");}
 void SetQuestAsCompleted(const CCharString& name,bool a,bool b,bool c){assert(!strcmp(name.text,"HauntedQuest")&&!a&&b&&!c);++completed;}
 void DeactivateQuestLater(const CCharString& name,unsigned long n){assert(!strcmp(name.text,"HauntedQuest")&&n==0);++deactivated;}
};
static int nymph_count;
long CGameScriptInterfaceBase::GetAllThingsWithScriptName(const CCharString& name,std::vector<CScriptThing>& things)const{assert(!strcmp(name.text,"M_NymphHere"));for(int i=0;i<nymph_count;++i)things.push_back(CScriptThing(i));return nymph_count;}
class CTimer { public: CTimer(){++registered;}~CTimer(){++deregistered;}CTimer& operator=(long n){assert(n==1||n==2);++sets;return *this;}bool operator==(long n)const{assert(n==0);int i=timer_reads++;return i%3!=2;} };
void RunCutsceneMacro(const CCharString& name,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool a,bool b){assert(!strcmp(name.text,"CS_HAUNTED_CELLAROPENS")&&!a&&b);++cutscenes;}
class TRetVoid { public: void Return() {} };
class CWideScreenMagicPauseEntities {
    CGameScriptInterfaceBase* Game;
public:
    explicit CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& g):Game(&g){Game->PauseAllNonScriptedEntities(true);}
    ~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}
};
namespace NScript {
class CV_HauntedHouseScript { public:float UndeadBar;long GuiCounter;bool CellarDoorOpen,InHouse;
 class CM_QuestControl { public: CV_HauntedHouseScript& ParentClass;CGameScriptInterfaceBase& Game;CM_QuestControl(CV_HauntedHouseScript& p,CGameScriptInterfaceBase& g):ParentClass(p),Game(g){}bool IsActiveThreadTerminating()const{++checks;return terminate_at==checks;}void Main();};};
void CV_HauntedHouseScript::CM_QuestControl::Main()
{
    TRetVoid _Fail_Val;
    while (!ParentClass.InHouse) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    bool PreviouslyKilledEnoughUndead = ParentClass.UndeadBar >= pGlobalScriptDef->HH_UpperBarLimit;
    CTimer DecrementTimer;
    DecrementTimer = 1;
    Game.DisplayQuestInfo(true);
    ParentClass.GuiCounter = Game.AddQuestInfoBar(pGlobalScriptDef->HH_UpperBarLimit, 0.0f, CRGBColour(0, 255, 0, 255),
        CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_UNDEAD", "", 1.0f);
    while (ParentClass.UndeadBar < pGlobalScriptDef->HH_UpperBarLimit) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (DecrementTimer == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (ParentClass.UndeadBar < pGlobalScriptDef->HH_BarDecrementAmount) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.UndeadBar = 0.0f;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.UndeadBar -= pGlobalScriptDef->HH_BarDecrementAmount;
            }
            DecrementTimer = (long)pGlobalScriptDef->HH_BarDecrementInterval;
        }
        Game.UpdateQuestInfoBar(ParentClass.GuiCounter, ParentClass.UndeadBar, -1.0f, -1.0f);
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    Game.RemoveQuestInfoElement(ParentClass.GuiCounter);
    if (!ParentClass.CellarDoorOpen) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        ParentClass.CellarDoorOpen = true;
        RunCutsceneMacro("CS_HAUNTED_CELLAROPENS", mymap, 0, 0, false, true);
        CScriptThing DiggingPlace = Game.GetThingWithScriptName("HH_DiggingSpot");
        Game.AddItemToContainer(DiggingPlace, "OBJECT_TATTOO_CARD_BACK_02");
    }
    Game.GiveHeroRenownPoints(pGlobalScriptDef->HH_ClearedHHRenown);
    Game.SetCreatureGeneratorsEnabled("HauntedHouse", true);
    std::vector<CScriptThing> NymphsHere;
    Game.GetAllThingsWithScriptName("M_NymphHere", NymphsHere);
    for (long i = 0; i < NymphsHere.size(); i++) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        Game.CreateCreature("CREATURE_LAKE_NYMPH_01", NymphsHere[i].GetPos(), "", false);
        Game.Pause(1.0f);
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(), false, true, false);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    return;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
}
}

static int run(int stop,bool inside,bool cellar,int nymphs,float initial){
 checks=registered=deregistered=cutscenes=items=created=completed=deactivated=shown=removed=renown=enabled=pauses=updates=frames=timer_reads=sets=0;was_inside=false;terminate_at=stop;nymph_count=nymphs;
 NScript::CV_HauntedHouseScript parent;parent.UndeadBar=initial;parent.GuiCounter=0;parent.CellarDoorOpen=cellar;parent.InHouse=inside;bar=&parent.UndeadBar;in_house=&parent.InHouse;
 CGameScriptInterfaceBase game;NScript::CV_HauntedHouseScript::CM_QuestControl control(parent,game);control.Main();assert(registered==deregistered);
 if(!stop){assert(completed==1&&deactivated==1&&shown==1&&removed==1&&renown==1&&enabled==1&&created==nymphs);assert(cutscenes==(cellar?0:1)&&items==cutscenes&&pauses==2*cutscenes);assert(updates==(initial<10?3:0)&&sets==(initial<10?3:1));}
 else{assert(checks==stop&&completed==0&&deactivated==0);}
 return checks;
}
int main() {
    const float startingBars[]={0,10,12};
    for(int start=0;start<3;++start) for(int inside=0;inside<2;++inside)
    for(int cellar=0;cellar<2;++cellar) for(int nymphs=0;nymphs<=2;++nymphs){
        int total=run(0,inside!=0,cellar!=0,nymphs,startingBars[start]);
        for(int stop=1;stop<=total;++stop)run(stop,inside!=0,cellar!=0,nymphs,startingBars[start]);
    }
    puts("00e96110_TEST PASS");return 0;
}