// Deterministic component, subtitle and event fixtures; the complete selected body follows.
#include <stdio.h>
#include <string.h>
#include <list>
#include <string>
typedef std::string CCharString;
struct CWideString {std::string text;};
enum {TCI_OWNED_ENTITY=1,TCI_VILLAGE_MEMBER,TCI_BED,TCI_BERSERK,TCI_BUYABLE_HOUSE,TCI_TAVERN};
template<class T>struct CIntelligentPointer{T* p;explicit CIntelligentPointer(T* t=0):p(t){}T* Get()const{return p;}T*operator->()const{return p;}};
template<class T>struct CDefPointer{T* p;T* operator->()const{return p;}};
struct CTavernDef{long BedCost;};struct BaseDef{CTavernDef tavern;void GetSubDef(CDefPointer<const CTavernDef>* t){t->p=&tavern;}};
struct CThing{void* tc[8];BaseDef base;CThing(){memset(tc,0,sizeof(tc));base.tavern.BedCost=6;}
 template<class T>bool GetTC(int i,T** out){*out=(T*)tc[i];return *out!=0;}bool HasTC(int i)const{return tc[i]!=0;}
 long GetPlayerNumber()const{return 3;}BaseDef& PeekBaseDef(){return base;}};
struct CThingPlayerCreature:CThing{bool melee;bool IsInMelee()const{return melee;}};
struct CThingAICreature:CThing{CIntelligentPointer<CThing> HomeBuilding,WorkBuilding;CThing*GetHomeBuilding(){return HomeBuilding.p;}CThing*GetWorkBuilding(){return WorkBuilding.p;}};
struct CPlayer{CThingPlayerCreature* hero;CThingPlayerCreature* GetPPlayerCharacter(){return hero;}};
struct Manager{CPlayer player;CPlayer& GetMainPlayer(){return player;}};
static Manager& GetPlayerManager(){static Manager m;return m;}
struct CTCOwnedEntity{CIntelligentPointer<CThing> POwner;CThing*GetPOwner(){return POwner.Get();}};
struct CTCVillage{bool LaggyLimbo,IsEnemyBecauseOfCrime;std::list<CIntelligentPointer<CThingAICreature> > VillagePeople;bool GetVillageLimbo()const{return LaggyLimbo;}bool IsHeroEnemyBecauseOfCrime()const{return IsEnemyBecauseOfCrime;}const std::list<CIntelligentPointer<CThingAICreature> >&PeekVillagePeople()const{return VillagePeople;}};
struct CTCVillageMember{CTCVillage* village;CTCVillage* GetPVillage(){return village;}};
struct CTCBed{bool Empty;bool IsEmpty(){return Empty;}};struct CTCBuyableHouse{bool OwnedByPlayer,Rented;bool GetOwnedByPlayer()const{return OwnedByPlayer;}bool GetRented()const{return Rented;}};
struct Font{void* Get(){return 0;}};
struct EventLog{int sent,kind,player,value;} events;
struct CGameEvent{int kind,Player,value;explicit CGameEvent(int n):kind(n),Player(0),value(0){}void SetPlayer(long p){Player=p;}void AddUBYTE(int b){value=b;}};
struct TextBank {CWideString GetTextBySymbol(CCharString s){CWideString w;w.text=s;return w;}};
namespace NGameText { static int price;void ConstructString(CWideString&,long cost){price=cost;} }
struct CMainGameComponent{
 static CMainGameComponent* Get(){static CMainGameComponent c;return &c;}
 Font PCutSceneMainFont;void*PeekCutSceneMainFont(){return PCutSceneMainFont.Get();}TextBank bank;TextBank& GetTextBank(){return bank;}
 void AddGameEvent(const CGameEvent& e){++events.sent;events.kind=e.kind;events.player=e.Player;events.value=e.value;}
};
struct SubtitleFixture{std::string message;int kind;
 void SetText(void*,const std::string& m,const char*,const char*,const char*,int k,float,bool,bool){message=m;kind=k;}
 void SetText(void*,const CWideString& m,const wchar_t*,const wchar_t*,const wchar_t*,int k,float,bool,bool){message=m.text;kind=k;}
};
struct Gui{SubtitleFixture SubtitleRenderer;SubtitleFixture&GetSubtitleRenderer(){return SubtitleRenderer;}} gui;struct CPlayerGui{static Gui&Get(){return gui;}};static Gui* g_pPlayerGui=&gui;
struct World{bool HeroSleepingEnabled;bool IsHeroSleepingEnabled()const{return HeroSleepingEnabled;}};
struct CTCActionUseBase{int uses;void OnActionUse(CThing&){++uses;}};
struct CTCActionUseBed:CTCActionUseBase{World world;World& PeekWorld(){return world;}CThing* ParentThing;CThing&GetParentThing(){return *ParentThing;}
 CIntelligentPointer<CThing> PThingUser,PBuilding;bool WaitingForFadeOut,WaitingForFadeIn,OwnedByHero,UseableByHero,WaitingForConfirmation;
 long PlayerNumberOfThingUsingBed,Cost;bool saved;void AddToGlobalTCs(){saved=true;}void OnActionUse(CThing&);};
void CTCActionUseBed::OnActionUse(CThing& thing_performing_action)
{
    {
        const CIntelligentPointer<CThing> user(&thing_performing_action);
        PThingUser = user;
    }
    bool allowed_to_sleep = true;
    CTCOwnedEntity* tc_owned_entity = 0;
    CTCVillageMember* tc_village_member = 0;
    CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
    if ((GetParentThing().GetTC(TCI_OWNED_ENTITY, &tc_owned_entity) && tc_owned_entity->GetPOwner()
        && tc_owned_entity->GetPOwner()->GetTC(TCI_VILLAGE_MEMBER, &tc_village_member)
        && tc_village_member->GetPVillage() && (tc_village_member->GetPVillage()->GetVillageLimbo()
        || tc_village_member->GetPVillage()->IsHeroEnemyBecauseOfCrime())) || !phero || phero->IsInMelee())
        allowed_to_sleep = false;
    if (!allowed_to_sleep || WaitingForFadeOut || WaitingForFadeIn)
        return;
    CTCActionUseBase::OnActionUse(thing_performing_action);
    PlayerNumberOfThingUsingBed = thing_performing_action.GetPlayerNumber();
    CCharString main("");
    CThing& bed = GetParentThing();
    {
        const CIntelligentPointer<CThing> no_building(0);
        PBuilding = no_building;
    }
    bool building_has_occupants = false;
    if (!PeekWorld().IsHeroSleepingEnabled()) {
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    }
    {
        CTCOwnedEntity* tc_bed_owned_entity;
        if (bed.GetTC(TCI_OWNED_ENTITY, &tc_bed_owned_entity)) {
            {
                const CIntelligentPointer<CThing> building(tc_bed_owned_entity->GetPOwner());
                PBuilding = building;
            }
            CTCVillageMember* tc_bed_village_member;
            if (PBuilding.Get()->GetTC(TCI_VILLAGE_MEMBER, &tc_bed_village_member)) {
                CTCVillage* tc_village = tc_bed_village_member->GetPVillage();
                if (tc_village) {
                    for (std::list<CIntelligentPointer<CThingAICreature> >::const_iterator person = tc_village->PeekVillagePeople().begin();
                         person != tc_village->PeekVillagePeople().end(); ++person) {
                        if (PBuilding.Get() == (*person)->GetHomeBuilding() || PBuilding.Get() == (*person)->GetWorkBuilding()) {
                            building_has_occupants = true;
                            break;
                        }
                    }
                }
                else
                    building_has_occupants = true;
            }
            else
                building_has_occupants = true;
        }
        else
            building_has_occupants = true;
    }
    CTCBuyableHouse* tc_buyable_house = 0;
    CTCBed* tc_bed = 0;
    bed.GetTC(TCI_BED, &tc_bed);
    if (thing_performing_action.HasTC(TCI_BERSERK)) {
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    }
    if (tc_bed && !tc_bed->IsEmpty()) {
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    } else if (OwnedByHero) {
        main = "TEXT_GUI_USEBED_CHECK";
        Cost = 0;
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
    } else if (!UseableByHero) {
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    } else if (!PBuilding.Get()) {
        main = "TEXT_GUI_USEBED_ILLEGAL";
        Cost = -1;
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
    } else if (!building_has_occupants) {
        main = "TEXT_GUI_USEBED_CHECK";
        Cost = 0;
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
    } else {
        if (PBuilding.Get()->HasTC(TCI_BUYABLE_HOUSE) && PBuilding.Get()->GetTC(TCI_BUYABLE_HOUSE, &tc_buyable_house)
            && tc_buyable_house->GetOwnedByPlayer() && !tc_buyable_house->GetRented()) {
            main = "TEXT_GUI_USEBED_CHECK";
            Cost = 0;
            CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                                   "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
        } else if (PBuilding.Get()->HasTC(TCI_TAVERN)) {
            main = "TEXT_GUI_USEBED_COST";
            CDefPointer<const CTavernDef> pdef;
            PBuilding.Get()->PeekBaseDef().GetSubDef(&pdef);
            Cost = pdef->BedCost;
            CWideString display_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(main);
            NGameText::ConstructString(display_text, Cost);
            CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), display_text, L"",
                                                   L"TEXT_GUI_TAVERNGAMES_GENERIC_YES", L"TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
        } else {
            main = "TEXT_GUI_USEBED_ILLEGAL";
            Cost = -1;
            CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                                   "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
        }

    }
    AddToGlobalTCs();
    WaitingForConfirmation = true;
    CGameEvent pause_event(9);
    pause_event.SetPlayer(PlayerNumberOfThingUsingBed);
    pause_event.AddUBYTE(1);
    CMainGameComponent::Get()->AddGameEvent(pause_event);
}
int main(){
 for(int i=0;i<22;++i){
 CTCActionUseBed b;b.WaitingForFadeIn=i==4;b.WaitingForFadeOut=i==5;b.OwnedByHero=i==9;b.UseableByHero=i!=10;
 b.world.HeroSleepingEnabled=i!=6;b.saved=b.WaitingForConfirmation=false;b.Cost=-99;b.uses=0;
 CThing bed,building;CThingPlayerCreature hero;hero.melee=i==3;b.ParentThing=&bed;GetPlayerManager().player.hero=(i==2)?0:&hero;
 CTCOwnedEntity owned;owned.POwner.p=&building;if(i!=11) bed.tc[TCI_OWNED_ENTITY]=&owned;
 CTCVillage village;village.LaggyLimbo=i==0;village.IsEnemyBecauseOfCrime=i==1;CTCVillageMember member={i==12?0:&village};
 if(i!=13)building.tc[TCI_VILLAGE_MEMBER]=&member;CTCBed state={i!=8};if(i!=14)bed.tc[TCI_BED]=&state;
 if(i==7)hero.tc[TCI_BERSERK]=&bed;
 CThingAICreature villager;if(i==15||(i>=17&&i<=20))villager.HomeBuilding.p=&building;if(i==16)villager.WorkBuilding.p=&building;
 village.VillagePeople.push_back(CIntelligentPointer<CThingAICreature>(&villager));
 CTCBuyableHouse house={i!=17,i==18};if(i>=12&&i!=19)building.tc[TCI_BUYABLE_HOUSE]=&house;
 if(i==19||i==20)building.tc[TCI_TAVERN]=&building;
 gui.SubtitleRenderer.message="";events.sent=0;b.OnActionUse(hero);
 if(b.PThingUser.Get()!=&hero)return 1;
 if(i<6){if(b.uses||events.sent||!gui.SubtitleRenderer.message.empty())return 2;continue;}
 if(b.uses!=1)return 3;
 if(i==6||i==7||i==8||i==10){if(gui.SubtitleRenderer.message!="TEXT_GUI_USEBED_FAIL"||events.sent)return 4;continue;}
 if(events.sent!=1||events.kind!=9||events.value!=1||events.player!=3||!b.WaitingForConfirmation||!b.saved)return 5;
 if(i==9 && (b.Cost!=0||gui.SubtitleRenderer.message!="TEXT_GUI_USEBED_CHECK"))return 6;
 if(i==19 && (b.Cost!=6||NGameText::price!=6||gui.SubtitleRenderer.message!="TEXT_GUI_USEBED_COST"))return 9;
 if(i==17||i==18){if(b.Cost!=-1||gui.SubtitleRenderer.message!="TEXT_GUI_USEBED_ILLEGAL")return 10;}
 if(i==12||i==14||i==15||i==16||i==20){if(b.Cost!=0||gui.SubtitleRenderer.message!="TEXT_GUI_USEBED_CHECK")return 11;}
 if(i==11 && (b.Cost!=-1||gui.SubtitleRenderer.message!="TEXT_GUI_USEBED_ILLEGAL"))return 7;
 }
 puts("007ebea0_TEST PASS");return 0;
}