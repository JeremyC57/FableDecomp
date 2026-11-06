// CTCHeroOnlineScoreboard::ChooseAClan() -- build the PC "choose a clan" list: drop the cached logos and hide the connecting message; with the hero's inventory on the scoreboard screen, create the clans list (title, position dummy, one menu entry per clan with its back / select actions, name, description and cached logo), register it with the inventory, wire the scroll arrows and the back / close clickables, then show it
// Leaf: ?ChooseAClan@CTCHeroOnlineScoreboard@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055FEC0, 2132 bytes, plain `ret` (frame-pointer prologue, `and esp,-8`).
// Callees: map<long, CCountedPointer<CTexture> >::_M_erase 0x00567980 (inline clear); CVectorMap LowerBound 0x0040F020 (inline
//   GetTC, TCI_INVENTORY); NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D / CreateMenuEntryComponent
//   0x00428494 / AddObserver (slot 2); CCharString 0x0099EBF0 / ~ 0x0099EAE0; CWideString 0x0099B6B0 / copy 0x0099B720 / ~ 0x0099B510;
//   CCountedPointer<NUISystem::CComponent>(T*) 0x00429C15, CCountedPointer<NUISystem::CAction>(T*) 0x0042AA29, operator new 0x00BFEA1A,
//   CAction 0x0042BE50, malloc 0x00BFEA0E (list<CActionParam*> node), GetCachedImage 0x0055FC90, operator delete 0x00BFE9BC,
//   vector<NUISystem::CComponent*>::_M_insert_overflow 0x00485DBC.
// Layout: Ego_r.pdb CTCHeroOnlineScoreboard with retail STLport sizes (as the landed ShowScoreboard 0x00563130): PictureCache +0x10,
//   MyClans +0x38, ScoreboardState +0x50, PScoreboardScreen +0x58, PConnectingMessage1 +0x64; CClanInfo (0x14) Name / ClanID /
//   Description; CTCInventory CurrentOpenMenu +0x21C (as the landed OpenPCScoreboard 0x00587B93).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CSPRITE_PUBLIC_TEXTURE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CText.h"

class CTexture;

class CTCInventory : public CTCBase { public:                       // TCI_INVENTORY (17)
    virtual void CUnprovenSlot28(); virtual void CUnprovenSlot29(); virtual void CUnprovenSlot30(); virtual void CUnprovenSlot31();
    virtual void CUnprovenSlot32(); virtual void CUnprovenSlot33(); virtual void CUnprovenSlot34(); virtual void CUnprovenSlot35();
    virtual void CUnprovenSlot36(); virtual void CUnprovenSlot37(); virtual void CUnprovenSlot38(); virtual void CUnprovenSlot39();
    virtual void CUnprovenSlot40(); virtual void CUnprovenSlot41(); virtual void CUnprovenSlot42(); virtual void CUnprovenSlot43();
    virtual void CUnprovenSlot44(); virtual void CUnprovenSlot45(); virtual void CUnprovenSlot46(); virtual void CUnprovenSlot47();
    virtual void CUnprovenSlot48(); virtual void CUnprovenSlot49(); virtual void CUnprovenSlot50(); virtual void CUnprovenSlot51();
    virtual void CUnprovenSlot52(); virtual void CUnprovenSlot53(); virtual void CUnprovenSlot54(); virtual void CUnprovenSlot55();
    virtual void CUnprovenSlot56(); virtual void CUnprovenSlot57(); virtual void CUnprovenSlot58(); virtual void CUnprovenSlot59();
    virtual void CUnprovenSlot60(); virtual void CUnprovenSlot61(); virtual void CUnprovenSlot62(); virtual void CUnprovenSlot63();
    virtual void CUnprovenSlot64(); virtual void CUnprovenSlot65(); virtual void CUnprovenSlot66(); virtual void CUnprovenSlot67();
    virtual void CUnprovenSlot68(); virtual void CUnprovenSlot69(); virtual void CUnprovenSlot70(); virtual void CUnprovenSlot71();
    virtual void CUnprovenSlot72(); virtual void CUnprovenSlot73(); virtual void CUnprovenSlot74(); virtual void CUnprovenSlot75();
    virtual void CUnprovenSlot76(); virtual void CUnprovenSlot77(); virtual void CUnprovenSlot78(); virtual void CUnprovenSlot79();
    virtual void CUnprovenSlot80(); virtual void CUnprovenSlot81();
    virtual std::vector<NUISystem::CComponent*>* GetComponents();   // slot 82 (+0x148), as the landed ShowScoreboard 0x00563130
    unsigned char _pad_0x0c[0x21c - 0x0c];
    long CurrentOpenMenu;                                            // +0x21C
};

class CTCHeroOnlineScoreboard : public CTCBase { public:
    class CClanInfo { public:                                              // CTCHeroOnlineScoreboard::CClanInfo (Ego_r.pdb, 0x14)
        CWideString Name;                                           // +0x00
        long        ClanID;                                         // +0x04
        CWideString Description;                                    // +0x08
        CCharString HomePageURL;                                    // +0x0C
        int         NumMembers;                                     // +0x10
    };
    void*                                       PHttp;              // +0x0C scoped_ptr<LHNetworkLib::LHHttp2>
    std::map<long, CCountedPointer<CTexture> >  PictureCache;       // +0x10
    unsigned char                               LocalScoreboard[0xc];   // +0x1C
    unsigned char                               ActivityInfo[0xc];  // +0x28
    CCharString                                 SessionID;          // +0x34
    std::list<CClanInfo>                        MyClans;            // +0x38
    CWideString                                 SelectedClan;       // +0x3C
    unsigned char                               ClanMembers[0x4];   // +0x40
    unsigned char                               DownloadedClanScoreboard[0xc];   // +0x44
    long                                        ScoreboardState;    // +0x50
    long                                        TaskState;          // +0x54
    NUISystem::CComponent*                      PScoreboardScreen;  // +0x58
    NUISystem::CComponent*                      PClanMenuEntry;     // +0x5C
    NUISystem::CComponent*                      PConnectingMessage; // +0x60
    NUISystem::CComponent*                      PConnectingMessage1;    // +0x64
private:
    CCountedPointer<CTexture> GetCachedImage(const CWideString& folder, long id);   // 0x0055FC90
public:
    void ChooseAClan();
};

void CTCHeroOnlineScoreboard::ChooseAClan()
{
    PictureCache.clear();
    PConnectingMessage1->ChangeState(0);
    CTCInventory* inventory;
    if (!ParentThing->GetTC(TCI_INVENTORY, &inventory))
        return;
    if (inventory->CurrentOpenMenu != 8) {
        ScoreboardState = 0;
        return;
    }
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CList* list = (NUISystem::CList*)manager->CreateComponent("PC_SCOREBOARD_CLANS_LIST", false);
    PScoreboardScreen = list;
    NUISystem::CComponent* frame = PScoreboardScreen->FindChild("PC_UI_SCOREBOARD1_FRAME");
    NUISystem::CText* title = (NUISystem::CText*)manager->CreateComponent("PC_TITLE", false);
    {
        CWideString title_text(L"TEXT_GUI_MENU_CHOOSE_CLAN");
        title->SetValue(title_text, true, true);
    }
    CCountedPointer<NUISystem::CComponent> title_ptr(title);
    list->AddNonScrollingChild(title_ptr);
    frame->AddPositionChild(title);
    CCountedPointer<NUISystem::CComponent> dummy(manager->CreateComponent("PC_LIST_POSITION_DUMMY", false));
    list->AddNonScrollingChild(dummy);
    dummy->AddPositionChild(frame);
    list->InitialiseOffsets(MyClans.size());
    for (std::list<CClanInfo>::iterator it = MyClans.begin(); it != MyClans.end(); ++it) {
        CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(it->Name, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        list->AddScrollingChildAndSetOffsets(entry);
        CCountedPointer<NUISystem::CAction> back_action(new NUISystem::CAction);
        back_action->ActionType = NUISystem::UI_ACTION_TYPE_CLOSE_ACTIVE_MENU;
        entry->AddActionOnBack(back_action);
        CCountedPointer<NUISystem::CAction> select_action(new NUISystem::CAction);
        select_action->ActionType = NUISystem::UI_ACTION_TYPE_CHOOSE_CLAN;
        select_action->AddParam(new NUISystem::CActionParamString(it->Name));
        select_action->AddParam(entry.Data);
        entry->AddPreAction(select_action);
        NUISystem::CText* name = (NUISystem::CText*)manager->CreateComponent("PC_SUBTITLE", false);
        name->SetValue(it->Name, true, true);
        CCountedPointer<NUISystem::CComponent> name_ptr(name);
        entry->AddChild(name_ptr);
        frame->AddPositionChild(name);
        NUISystem::CText* description = (NUISystem::CText*)manager->CreateComponent("PC_SCORE_DESCRIPTION", false);
        description->SetValue(it->Description, true, true);
        CCountedPointer<NUISystem::CComponent> description_ptr(description);
        entry->AddChild(description_ptr);
        frame->AddPositionChild(description);
        NUISystem::CSprite* image = (NUISystem::CSprite*)manager->CreateComponent("PC_CLAN_IMAGE", false);
        {
            CWideString cache_name(L"ClanData");
            image->PTexture = GetCachedImage(cache_name, it->ClanID);
        }
        CCountedPointer<NUISystem::CComponent> image_ptr(image);
        entry->AddChild(image_ptr);
        frame->AddPositionChild(image);
    }
    inventory->GetComponents()->push_back(PScoreboardScreen);
    NUISystem::CComponent* child = PScoreboardScreen->FindChild("SCROLL_LIST_UP");
    if (child)
        child->ObserveEvent((NUISystem::EEvent)0x19);
    child = PScoreboardScreen->FindChild("SCROLL_LIST_DOWN");
    if (child)
        child->ObserveEvent((NUISystem::EEvent)0x19);
    child = PScoreboardScreen->FindChild("SCROLL_DESCRIPTION_UP");
    if (child)
        child->ObserveEvent((NUISystem::EEvent)0x19);
    child = PScoreboardScreen->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (child)
        child->ObserveEvent((NUISystem::EEvent)0x19);
    child = PScoreboardScreen->FindChild("PC_BACK_CLICKABLE");
    if (child) {
        NUISystem::CManager::GetInstance()->AddObserver(child);
        child->ObserveEvent((NUISystem::EEvent)0x19);
    }
    child = PScoreboardScreen->FindChild("PC_CLOSE_CLICKABLE");
    if (child) {
        NUISystem::CManager::GetInstance()->AddObserver(child);
        child->ObserveEvent((NUISystem::EEvent)0x19);
    }
    PScoreboardScreen->Initialise();
    list->ChangeState(5);
    ScoreboardState = 0;
}