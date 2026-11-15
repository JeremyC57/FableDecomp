// Full production member bodies with explicit dependency doubles.
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <map>
struct CCharString { std::string value; CCharString(const char* s=""):value(s){} bool operator<(const CCharString& x)const{return value<x.value;} };
struct CWideString { static int conversions; static CWideString CreateFromCharString(const char* s){if(*s)std::exit(99);++conversions;return CWideString();} }; int CWideString::conversions=0;
template<class T> struct CCountedPointer { T* Data; CCountedPointer(T* p=0):Data(p){} T* Get()const{return Data;} T* operator->()const{return Data;} };
struct C2DVector{float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
enum {HERO_STAT_STRENGTH=0,HERO_STAT_SKILL=1,HERO_STAT_WILL=2,UI_EVENT_MOUSE_MOVEMENT=17,UI_ACTION_TYPE_SEND_EVENT=22};
namespace NHeroInformationScreens { enum EScreenType{OLD_SCREEN=3,INVENTORY_EXPERIENCE_SCREEN=8}; struct CScreen{EScreenType type;CScreen():type(OLD_SCREEN){} EScreenType GetScreenType(){return type;}}; struct CManager{CScreen current;int selects;CManager():selects(0){} CScreen*GetCurrentScreen(){return &current;}void SelectScreenOfType(EScreenType t){++selects;current.type=t;}}; }
struct CPlayerGui { NHeroInformationScreens::CManager manager;static CPlayerGui&Get(){static CPlayerGui g;return g;}NHeroInformationScreens::CManager*GetPHeroInfoScreensManager(){return &manager;} };
namespace NUISystem {
struct CActionParamSLONG{long value;CActionParamSLONG(long n):value(n){}};
struct CAction{int ActionType;std::vector<CActionParamSLONG*>UIActionParams;CAction():ActionType(0){}};
struct CComponent { std::string name;std::map<std::string,CComponent*>children;std::vector<CComponent*>positionChildren;std::vector<CAction*>actions;int movements,initialises,state;C2DVector position; CComponent(const std::string& n=""):name(n),movements(0),initialises(0),state(0){}virtual~CComponent(){} CComponent*FindChild(const char*n){std::map<std::string,CComponent*>::iterator i=children.find(n);return i==children.end()?0:i->second;}void ObserveEvent(int e){if(e!=UI_EVENT_MOUSE_MOVEMENT)std::exit(98);++movements;}void AddPositionChild(CComponent*p){positionChildren.push_back(p);}void Initialise(){++initialises;}void ChangeState(int s){state=s;}void AddActionOnLeftUnclicked(CCountedPointer<CAction>a){actions.push_back(a.Get());}void ChangePosition(const C2DVector&p,float z,bool b){if(z!=0||b)std::exit(97);position=p;} };
struct CViewport:CComponent {CViewport(const char*n):CComponent(n){}};
struct CYesNo:CComponent {CYesNo():CComponent("PC_DIALOG_YESNO_GAME"){} };
struct CList:CComponent {std::vector<CComponent*>nonScrolling;int offsets;CList():CComponent("PC_EXPERIENCE_SPENDING_MENU"),offsets(0){}void AddNonScrollingChild(CCountedPointer<CComponent>c){nonScrolling.push_back(c.Get());}void InitialiseOffsets(int n){offsets=n;} };
struct CManager { static CManager*active;CList screen;CComponent frame,title,up,down,back,close,listup,listdown,sliders,mouse,yes,no,slider[4];CViewport viewport,textViewport;CYesNo dialog;std::vector<std::string>creates;std::vector<CComponent*>observers;int sounds;CCharString soundName;int soundPriority;void*heroInventory;void*abilitiesInventory;void*experienceInventory;C2DVector scale;
 CManager(bool children):frame("PC_UI_FRAME"),title("PC_TITLE_EXPERIENCE_SPENDING"),up("SCROLL_DESCRIPTION_UP"),down("SCROLL_DESCRIPTION_DOWN"),back("PC_BACK_CLICKABLE"),close("PC_CLOSE_CLICKABLE"),listup("SCROLL_LIST_UP"),listdown("SCROLL_LIST_DOWN"),sliders("PC_EXP_SPENDING_SLIDER_CONTAINER"),mouse("UI_MOUSE_POINTER"),yes("YES"),no("NO"),viewport("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT"),textViewport("PC_DESCRIPTION_VIEWPORT"),sounds(0),soundPriority(0),heroInventory(0),abilitiesInventory(0),experienceInventory(0),scale(1.25f,.75f){active=this;screen.children[frame.name]=&frame;if(children){screen.children[title.name]=&title;screen.children[up.name]=&up;screen.children[down.name]=&down;screen.children[back.name]=&back;screen.children[close.name]=&close;screen.children[listup.name]=&listup;screen.children[listdown.name]=&listdown;screen.children[viewport.name]=&viewport;screen.children[textViewport.name]=&textViewport;dialog.children["PC_YESNO_GAME_YESBUTTON"]=&yes;dialog.children["PC_YESNO_GAME_NOBUTTON"]=&no;}}
 static CManager*GetInstance(){return active;}C2DVector GetUIScale(){return scale;}CComponent*CreateComponent(const char*n,bool b){if(b)std::exit(96);creates.push_back(n);if(std::string(n)==screen.name)return &screen;if(std::string(n)==sliders.name)return &sliders;if(std::string(n)==mouse.name)return &mouse;if(std::string(n)==dialog.name)return &dialog;std::exit(95);return 0;}CComponent*CreateComponent(const CCharString&n,bool b){return CreateComponent(n.value.c_str(),b);}void AddObserver(CComponent*c){observers.push_back(c);}unsigned long PlaySample(const CCharString&n,int p){++sounds;soundName=n;soundPriority=p;return 77;}void SetTCHeroExperienceInventory(void*p){heroInventory=p;}void SetAbilitiesInventory(void*p){abilitiesInventory=p;}void SetExperienceInventory(void*p){experienceInventory=p;}
}; CManager*CManager::active=0;
struct CUIScreenUnitConverter{static float GetBackBufferWidth(){return 1280;}static float GetBackBufferHeight(){return 720;}};
}
using namespace NUISystem;
struct CTCHeroExperience{long available,stat[3];CTCHeroExperience(){available=2;stat[0]=5;stat[1]=6;stat[2]=7;}long GetExperiencePointsAvailableToSpend(){return available;}long GetStatExperience(int n){return stat[n];}};
struct CTCInventoryStats{int calls;CTCInventoryStats():calls(0){}std::vector<CComponent*>CreateExperienceSliders(CComponent*c,long n,bool b){if(c!=&CManager::active->sliders||n!=2||b)std::exit(94);++calls;std::vector<CComponent*>v;for(int i=0;i<4;++i)v.push_back(&CManager::active->slider[i]);return v;}};
struct CTCInventoryAbilities{};
struct CThing{CTCInventoryStats stats;CTCHeroExperience hero;CTCInventoryAbilities abilities;bool hasStats,hasHero;CThing():hasStats(true),hasHero(true){}bool GetTC(CTCInventoryStats**p){if(!hasStats)return false;*p=&stats;return true;}bool GetTC(CTCHeroExperience**p){if(!hasHero)return false;*p=&hero;return true;}};
namespace NInventory{struct CTCInventoryBase{int opens;CTCInventoryBase():opens(0){}void OpenInventory(){++opens;}};}
struct CSliderAnimator{CComponent*pSlider;float StartValue,EndValue,MaxValue,a,Overall;bool Play,Finished;void SetPlay(bool play){Play=play;}CSliderAnimator(CComponent*p,float s,float e,float m,float o):pSlider(p),StartValue(s),EndValue(e),MaxValue(m),a(0),Overall(o),Play(false),Finished(false){}};
struct CTCInventoryExperience:NInventory::CTCInventoryBase {CThing*ParentThing;bool inventory,FirstRenderFrame,OpenedExperienceSpending,CreateBecauseOfUndo;NHeroInformationScreens::EScreenType PreviousScreenType;std::map<int,int>UndoableStatBuys,UndoableAbilityBuys;std::map<CCharString,unsigned long>TutorialSoundHandles;std::vector<CComponent*>ExpSliders,Components;std::list<CSliderAnimator>Animators;std::list<CSliderAnimator>::iterator AnimIt;CViewport*pViewport,*pTextViewport;CComponent*PMousePointer;CYesNo*pYesNo;long OrigGeneral,OrigStrength,OrigSkill,OrigWill,PointsGeneral,PointsStrength,PointsSkill,PointsWill;int categoryCalls;
 CTCInventoryExperience(CThing*p):ParentThing(p),inventory(false),FirstRenderFrame(true),OpenedExperienceSpending(false),CreateBecauseOfUndo(false),PreviousScreenType(NHeroInformationScreens::OLD_SCREEN),pViewport(0),pTextViewport(0),PMousePointer(0),pYesNo(0),OrigGeneral(100),OrigStrength(200),OrigSkill(300),OrigWill(400),PointsGeneral(10),PointsStrength(20),PointsSkill(30),PointsWill(40),categoryCalls(0){}CThing& GetParentThing(){return*ParentThing;}bool IsInventoryScreen(){return inventory;}CTCHeroExperience*GetTCHeroExperience(){return &ParentThing->hero;}CTCInventoryAbilities*GetTCInventoryAbilities(){return &ParentThing->abilities;}void ConstructCategoryStrengthPC(CList*p){if(p!=&CManager::active->screen)std::exit(93);++categoryCalls;}long GetOriginalExperience(long pool);void OpenInventoryInternalPC(bool reset);
};
long CTCInventoryExperience::GetOriginalExperience(long pool)
{
    if (pool == 0)
        return OrigGeneral;
    else if (pool == 1)
        return OrigStrength;
    else if (pool == 2)
        return OrigSkill;
    else if (pool == 3)
        return OrigWill;
    return 0;
}
void CTCInventoryExperience::OpenInventoryInternalPC(bool reset)
{
    if (!IsInventoryScreen()) {
        PreviousScreenType = CPlayerGui::Get().GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        CPlayerGui::Get().GetPHeroInfoScreensManager()->SelectScreenOfType(NHeroInformationScreens::INVENTORY_EXPERIENCE_SCREEN);
    }
    NInventory::CTCInventoryBase::OpenInventory();
    if (!reset)
        FirstRenderFrame = false;
    CManager* manager = CManager::GetInstance();
    if (reset) {
        UndoableStatBuys.clear();
        UndoableAbilityBuys.clear();
        if (!OpenedExperienceSpending) {
            CCharString sound("SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_TOPLEVEL");
            TutorialSoundHandles[sound] = manager->PlaySample(sound, 4);
        }
    }
    manager->SetTCHeroExperienceInventory(GetTCHeroExperience());
    manager->SetAbilitiesInventory(GetTCInventoryAbilities());
    manager->SetExperienceInventory(this);
    CWideString title = CWideString::CreateFromCharString("");
    CComponent* plist_component = manager->CreateComponent("PC_EXPERIENCE_SPENDING_MENU", false);
    CComponent* frame = plist_component->FindChild("PC_UI_FRAME");
    CList* screen = static_cast<CList*>(plist_component);
    CComponent* clickable = screen->FindChild("SCROLL_DESCRIPTION_UP");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("PC_BACK_CLICKABLE");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("PC_CLOSE_CLICKABLE");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("SCROLL_LIST_UP");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("SCROLL_LIST_DOWN");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    CComponent* titleText = screen->FindChild("PC_TITLE_EXPERIENCE_SPENDING");
    if (titleText)
        frame->AddPositionChild(titleText);
    pViewport = static_cast<CViewport*>(screen->FindChild("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT"));
    pTextViewport = static_cast<CViewport*>(screen->FindChild("PC_DESCRIPTION_VIEWPORT"));
    if (pViewport)
        frame->AddPositionChild(pViewport);
    if (pTextViewport)
        frame->AddPositionChild(pTextViewport);
    CTCInventoryStats* stats = 0;
    if (GetParentThing().GetTC(&stats)) {
        CCountedPointer<CComponent> sliders(manager->CreateComponent("PC_EXP_SPENDING_SLIDER_CONTAINER", false));
        frame->AddPositionChild(sliders.Get());
        ExpSliders = stats->CreateExperienceSliders(sliders.Get(), 2, false);
        screen->AddNonScrollingChild(sliders);
        AnimIt = Animators.end();
        if (!reset) {
            CTCHeroExperience* heroExperience = 0;
            if (GetParentThing().GetTC(&heroExperience)) {
            float remaining = (float)(PointsGeneral + PointsStrength + PointsSkill + PointsWill);
            remaining -= (float)(heroExperience->GetExperiencePointsAvailableToSpend() + heroExperience->GetStatExperience(HERO_STAT_STRENGTH)
                                 + heroExperience->GetStatExperience(HERO_STAT_SKILL) + heroExperience->GetStatExperience(HERO_STAT_WILL));
            CSliderAnimator general(ExpSliders[0], (float)PointsGeneral, (float)heroExperience->GetExperiencePointsAvailableToSpend(),
                                    (float)GetOriginalExperience(0), remaining);
            CSliderAnimator strength(ExpSliders[1], (float)PointsStrength, (float)heroExperience->GetStatExperience(HERO_STAT_STRENGTH),
                                     (float)GetOriginalExperience(1), remaining);
            CSliderAnimator skill(ExpSliders[2], (float)PointsSkill, (float)heroExperience->GetStatExperience(HERO_STAT_SKILL),
                                  (float)GetOriginalExperience(2), remaining);
            CSliderAnimator will(ExpSliders[3], (float)PointsWill, (float)heroExperience->GetStatExperience(HERO_STAT_WILL),
                                 (float)GetOriginalExperience(3), remaining);
            Animators.clear();
            if (CreateBecauseOfUndo) {
                Animators.push_back(general);
                Animators.push_back(strength);
                Animators.push_back(skill);
                Animators.push_back(will);
            } else {
                Animators.push_front(general);
                Animators.push_front(strength);
                Animators.push_front(skill);
                Animators.push_front(will);
            }
            CreateBecauseOfUndo = false;
            AnimIt = Animators.begin();
            AnimIt->SetPlay(true);
            }
        }
    }
    screen->InitialiseOffsets(3);
    ConstructCategoryStrengthPC(screen);
    screen->Initialise();
    if (reset) {
        screen->ChangeState(5);
        {
            CCharString pointerName("UI_MOUSE_POINTER");
            CComponent* pointer = CManager::GetInstance()->CreateComponent(pointerName, false);
            Components.push_back(pointer);
        }
        Components[0]->Initialise();
        PMousePointer = Components[0];
    }
    Components.push_back(screen);
    CComponent* dialog = manager->CreateComponent("PC_DIALOG_YESNO_GAME", false);
    CComponent* yes = dialog->FindChild("PC_YESNO_GAME_YESBUTTON");
    if (yes) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        action->UIActionParams.push_back(new CActionParamSLONG(4));
        yes->AddActionOnLeftUnclicked(action);
    }
    CComponent* no = dialog->FindChild("PC_YESNO_GAME_NOBUTTON");
    if (no) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        action->UIActionParams.push_back(new CActionParamSLONG(5));
        no->AddActionOnLeftUnclicked(action);
    }
    dialog->Initialise();
    C2DVector position((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - (CManager::GetInstance()->GetUIScale().X * 104.0f),
                       CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    dialog->ChangePosition(position, 0.0f, false);
    Components.push_back(dialog);
    pYesNo = static_cast<CYesNo*>(dialog);
}

static void ck(bool x,int line){if(!x){std::printf("FAIL line %d\n",line);std::exit(1);}}
#define CHECK(x) ck((x),__LINE__)
static void scenario(bool reset,bool opened,bool undo,bool children,bool stats,bool hero,bool inventory,long seed){CManager manager(children);CThing thing;thing.hasStats=stats;thing.hasHero=hero;CTCInventoryExperience x(&thing);x.PointsGeneral+=seed*3;x.PointsStrength-=seed*2;x.PointsSkill+=seed*5;x.PointsWill-=seed;thing.hero.available+=seed;thing.hero.stat[0]+=seed*2;thing.hero.stat[1]-=seed;thing.hero.stat[2]+=seed*3;long starts[4]={x.PointsGeneral,x.PointsStrength,x.PointsSkill,x.PointsWill};long maxima[4]={x.OrigGeneral,x.OrigStrength,x.OrigSkill,x.OrigWill};float overall=float(starts[0]+starts[1]+starts[2]+starts[3])-float(thing.hero.available+thing.hero.stat[0]+thing.hero.stat[1]+thing.hero.stat[2]);x.inventory=inventory;x.OpenedExperienceSpending=opened;x.CreateBecauseOfUndo=undo;x.UndoableStatBuys[1]=1;x.UndoableAbilityBuys[2]=2;CPlayerGui::Get().manager=NHeroInformationScreens::CManager();int conversions=CWideString::conversions;x.OpenInventoryInternalPC(reset);CHECK(x.opens==1);CHECK(x.FirstRenderFrame==reset);CHECK(CPlayerGui::Get().manager.selects==(inventory?0:1));CHECK(manager.heroInventory==&thing.hero&&manager.abilitiesInventory==&thing.abilities&&manager.experienceInventory==&x);CHECK(CWideString::conversions==conversions+1);CHECK(x.UndoableStatBuys.empty()==reset&&x.UndoableAbilityBuys.empty()==reset);CHECK(manager.sounds==(reset&&!opened?1:0));if(manager.sounds){CHECK(manager.soundName.value=="SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_TOPLEVEL"&&manager.soundPriority==4);CHECK(x.TutorialSoundHandles[manager.soundName]==77);}CHECK(manager.observers.size()==(children?6:0));CHECK(manager.frame.positionChildren.size()==(children?(stats?4:3):(stats?1:0)));CHECK(x.pViewport==(children?&manager.viewport:0)&&x.pTextViewport==(children?&manager.textViewport:0));CHECK(thing.stats.calls==(stats?1:0));CHECK(manager.screen.nonScrolling.size()==(stats?1:0));if(stats)CHECK(x.ExpSliders.size()==4);CHECK(manager.screen.offsets==3&&manager.screen.initialises==1&&x.categoryCalls==1);CHECK(manager.screen.state==(reset?5:0));CHECK(x.Components.size()==(reset?3:2));CHECK(x.Components[x.Components.size()-2]==&manager.screen&&x.Components.back()==&manager.dialog);CHECK(x.PMousePointer==(reset?&manager.mouse:0));CHECK(manager.mouse.initialises==(reset?1:0));CHECK(manager.dialog.initialises==1&&x.pYesNo==&manager.dialog);CHECK(manager.dialog.position.X==510.0f&&manager.dialog.position.Y==321.0f);if(children){CHECK(manager.yes.actions.size()==1&&manager.no.actions.size()==1);CHECK(manager.yes.actions[0]->ActionType==UI_ACTION_TYPE_SEND_EVENT&&manager.no.actions[0]->ActionType==UI_ACTION_TYPE_SEND_EVENT);CHECK(manager.yes.actions[0]->UIActionParams[0]->value==4&&manager.no.actions[0]->UIActionParams[0]->value==5);}if(!reset&&stats&&hero){CHECK(x.Animators.size()==4&&!x.CreateBecauseOfUndo);int i=0;for(std::list<CSliderAnimator>::iterator it=x.Animators.begin();it!=x.Animators.end();++it,++i){int p=undo?i:3-i;CHECK(it->pSlider==&manager.slider[p]);CHECK(it->StartValue==float(starts[p])&&it->MaxValue==float(maxima[p])&&it->Overall==overall);CHECK(it->EndValue==float(p?thing.hero.stat[p-1]:thing.hero.available));CHECK(it->Play==(i==0)&&!it->Finished&&it->a==0);}CHECK(x.AnimIt==x.Animators.begin());}else{CHECK(x.Animators.empty()&&x.CreateBecauseOfUndo==undo);}}
int main(){for(long seed=-8;seed<8;++seed)for(unsigned m=0;m<128;++m) scenario((m&1)!=0,(m&2)!=0,(m&4)!=0,(m&8)!=0,(m&16)!=0,(m&32)!=0,(m&64)!=0,seed);std::puts("00577ac0_TEST PASS");return 0;}