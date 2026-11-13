// CTCHeroOnlineScoreboard::DisplayScoreboardGUI() -- build the PC clan scoreboard: drop the cached pictures and go idle; with the hero's inventory still on the scoreboard screen, reset the mouse pointer and hide the connecting message; an empty scoreboard shows TEXT_GUI_MENU_CLAN_EMPTY, otherwise one menu entry per downloaded activity (best score, its holder, description, then a sub list of every record with the holder's picture) is added to the clan menu entry
// Leaf: ?DisplayScoreboardGUI@CTCHeroOnlineScoreboard@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00563920, 3143 bytes, plain `ret` (two epilogues: the empty-scoreboard return 0x00563A5B and the
//   final 0x00564566); frame `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,120h`, no EH frame. Name: Ego_r.pdb / FableWin.pdb
//   ?DisplayScoreboardGUI@CTCHeroOnlineScoreboard@@AAEXXZ (private); its only caller is GlobalUpdate 0x00564570 (call 0x00564837).
// Callees (reloc_bindings / Ego_r / FableWin spellings): map<long,CCountedPointer<CTexture> >::_M_erase 0x00567980 (inline clear);
//   NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent(const CCharString&, bool) 0x0041DB1D / CreateMenuEntryComponent
//   0x00428494; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (forced GetTC); IsScoreboardEmpty 0x0055E7F0;
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0; DisplayErrorMessage 0x0055EF00; ActivityEnumToCharString 0x0055E7C0 (private,
//   FableWin ?ActivityEnumToCharString@CTCHeroOnlineScoreboard@@AAE?AVCCharString@@ABW4EOnlineScoreboardActivity@1@@Z);
//   CActivity::GetScoreAsString 0x0055E6B0 (Ego_r ?GetScoreAsString@CActivity@CTCHeroOnlineScoreboard@@QAE?AVCWideString@@M@Z);
//   GetCachedImage 0x0055FC90; the DownloadedClanScoreboard tree's find 0x00566FE0 and operator[] 0x0056A680; the list<CClanMember>
//   find 0x00567A40 and __find 0x00567240 (/Gr __fastcall); pair<float,CWideString> copy constructor 0x00564B20; CCountedPointer
//   <CComponent> (T*) 0x00429C15 / ~ 0x00429C53, CCountedPointer<CAction> (T*) 0x0042AA29 / ~ 0x00429B72, ~CCountedPointer<CTexture>
//   0x0041BC90 and its operator= 0x00429A62; operator new 0x00BFEA1A + CAction 0x0042BE50; list<CActionParam*>::push_back 0x0042B646;
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / ToWideString 0x0099E480; CWideString() 0x0099AED0 /
//   (const unsigned short*) 0x0099B6B0 / ~ 0x0099B510 / operator= 0x0099B7D0 / operator+=(const CWideString&) 0x0099B8D0 /
//   operator+=(const unsigned short*) 0x0099B940. Virtual slots: CComponent FindChild 3, ChangeColour 38, ChangeState(unsigned long)
//   48, AddChild 59, AddChildDynamically 60, AddActionOnInactive 70, AddPositionChild 124, CObserver ObserveEvent / ProcessEvent;
//   CText SetValue(const CWideString&, bool, bool) 144 / SetValue(const CCharString&, bool, bool) 145 / ForceWidth 149 (engine/CText.h);
//   CList AddNonScrollingChild 145, AddScrollingChildAndSetOffsets 152, AddScrollingChildDynamicallyAndSetOffsets 153,
//   InitialiseOffsets 156.
// Layout: Ego_r.pdb CTCHeroOnlineScoreboard with retail STLport sizes, as the landed ChooseAClan 0x0055FEC0 (same TU): PictureCache
//   +0x10, ActivityInfo +0x28 (CActivity, 0x10: Name / Description / UnitOfMeasurement), SelectedClan +0x3C, ClanMembers +0x40
//   (CClanMember, 0x0C: Name / MemberID / Description), DownloadedClanScoreboard +0x44, ScoreboardState +0x50, TaskState +0x54,
//   PScoreboardScreen +0x58, PClanMenuEntry +0x5C, PConnectingMessage1 +0x64. CTCInventory: shared header (PMouse_Pointer +0x214
//   through the FableWin inline GetMousePointer, CurrentOpenMenu +0x21C).
// Inline split (c2 inline trace, bin/inl): retail CALLS every ~CCountedPointer (15 + CAction + CTexture), the pair copy constructor,
//   the outer list find and both tree members, and EXPANDS the inner-loop find (so __find is called there) and the pair assignment.
//   With the default regime (expand iff c1*(1/f+6)/7 < 74) that is exactly one more halving of the static frequency ahead of the
//   activity loop than the three early returns and the empty test give (dtors c1 47: inner loop f 0.195 -> 74, after the loop
//   0.156 -> 83; outer find c1 27 f 0.039 -> 121, pair copy c1 21 -> 94; inner find f 0.195 -> 42). The `if (frame)` around the
//   loop is that branch: the frame pointer was already dereferenced (frame->AddPositionChild(title)), so VC7.1 folds the null test
//   and emits nothing, but its 0.5 split is in the estimate (FITTED: any test c2 folds there gives the same bytes, e.g. a repeated
//   PScoreboardScreen test or `|| inventory == 0` after the menu test; a test that emits code does not).
// Shapes: `size() > 0` on the score vector is retail's `sar ecx,3 ; test ecx,ecx ; jbe`; the outer list find assigned to a named
//   iterator compares the result slot with a reloaded end() (`mov eax,[edi+40h] ; cmp [esp+0C4h],eax`); the best record is one
//   pair<float, CWideString> copy-constructed from element 0 and re-assigned in the inner loop (fld / fstp + CWideString::operator=).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_INPUT
#define FABLE_RETAIL_CSPRITE_PUBLIC_TEXTURE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
#define FABLE_RETAIL_CTCINVENTORY_MOUSEPOINTER
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "engine/CRGBColour.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CAction.h"
#include "engine/CText.h"
#include "engine/CTCInventory.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"

class CTexture;

class CTCHeroOnlineScoreboard : public CTCBase { public:
    enum EOnlineScoreboardActivity { };
    class CActivity { public:                                       // CTCHeroOnlineScoreboard::CActivity (Ego_r.pdb, 0x10)
        CCharString   Name;                                         // +0x00
        CCharString   Description;                                  // +0x04
        CCharString   UnitOfMeasurement;                            // +0x08
        unsigned char NumDecPlacesToShow;                           // +0x0C
        bool          LowerIsBetter;                                // +0x0D
        CWideString GetScoreAsString(float score);                  // 0x0055E6B0
    };
    class CClanInfo { public:                                       // CTCHeroOnlineScoreboard::CClanInfo (Ego_r.pdb, 0x14)
        CWideString Name;
        long        ClanID;
        CWideString Description;
        CCharString HomePageURL;
        int         NumMembers;
    };
    class CClanMember { public:                                     // CTCHeroOnlineScoreboard::CClanMember (Ego_r.pdb, 0x0C)
        CWideString Name;                                           // +0x00
        long        MemberID;                                       // +0x04
        CWideString Description;                                    // +0x08
        bool operator==(const CWideString& name) { return Name == name; }
    };
    void*                                       PHttp;              // +0x0C scoped_ptr<LHNetworkLib::LHHttp2>
    std::map<long, CCountedPointer<CTexture> >  PictureCache;       // +0x10
    std::map<EOnlineScoreboardActivity, float>  LocalScoreboard;    // +0x1C
    std::vector<CActivity>                      ActivityInfo;       // +0x28
    CCharString                                 SessionID;          // +0x34
    std::list<CClanInfo>                        MyClans;            // +0x38
    CWideString                                 SelectedClan;       // +0x3C
    std::list<CClanMember>                      ClanMembers;        // +0x40
    std::map<EOnlineScoreboardActivity, std::vector<std::pair<float, CWideString> > > DownloadedClanScoreboard;   // +0x44
    long                                        ScoreboardState;    // +0x50
    long                                        TaskState;          // +0x54
    NUISystem::CComponent*                      PScoreboardScreen;  // +0x58
    NUISystem::CComponent*                      PClanMenuEntry;     // +0x5C
    NUISystem::CComponent*                      PConnectingMessage; // +0x60
    NUISystem::CComponent*                      PConnectingMessage1;    // +0x64
    bool IsScoreboardEmpty();                                       // 0x0055E7F0
    void DisplayErrorMessage(const CWideString& text);              // 0x0055EF00
private:
    CCharString ActivityEnumToCharString(const EOnlineScoreboardActivity& activity);   // 0x0055E7C0
    CCountedPointer<CTexture> GetCachedImage(const CWideString& folder, long id);     // 0x0055FC90
    void DisplayScoreboardGUI();
};

void CTCHeroOnlineScoreboard::DisplayScoreboardGUI()
{
    PictureCache.clear();
    ScoreboardState = 0;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CTCInventory* inventory;
    if (!ParentThing->GetTC(TCI_INVENTORY, &inventory))
        return;
    if (inventory->CurrentOpenMenu != 8 || PScoreboardScreen == 0)
        return;
    inventory->GetMousePointer()->ChangeColour(CRGBColour(255, 255, 255, 255), 1.0f, false);
    inventory->GetMousePointer()->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    PConnectingMessage1->ChangeState(NUISystem::UI_STATE_INACTIVE);
    if (IsScoreboardEmpty()) {
        CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_CLAN_EMPTY");
        DisplayErrorMessage(text);
        return;
    }
    NUISystem::CList* list = (NUISystem::CList*)manager->CreateComponent("PC_SUB_LIST", false);
    NUISystem::CComponent* frame = PScoreboardScreen->FindChild("PC_UI_SCOREBOARD1_FRAME");
    NUISystem::CText* title = (NUISystem::CText*)manager->CreateComponent("PC_TITLE", false);
    title->SetValue(SelectedClan, true, true);
    CCountedPointer<NUISystem::CComponent> title_ptr(title);
    list->AddNonScrollingChild(title_ptr);
    frame->AddPositionChild(title);
    CCountedPointer<NUISystem::CAction> delete_action(new NUISystem::CAction);
    delete_action->ActionType = NUISystem::UI_ACTION_TYPE_DELETE;
    delete_action->UIActionParams.push_back(list);
    list->AddActionOnInactive(delete_action);
    CCountedPointer<NUISystem::CComponent> list_ptr(list);
    PClanMenuEntry->AddChildDynamically(list_ptr);
    list->InitialiseOffsets(39);
    if (frame) {
        for (long i = 0; i < 39; ++i) {
            EOnlineScoreboardActivity activity = (EOnlineScoreboardActivity)i;
            if (DownloadedClanScoreboard.find(activity) != DownloadedClanScoreboard.end() && DownloadedClanScoreboard[activity].size() > 0) {
                CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(ActivityEnumToCharString(activity).ToWideString(),
                    0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                NUISystem::CComponent* entry_text = entry->FindChild("MENU_ENTRY_TEXT");
                if (entry_text)
                    ((NUISystem::CText*)entry_text)->ForceWidth(230.0f);
                std::pair<float, CWideString> record = DownloadedClanScoreboard[activity][0];
                CWideString score = ActivityInfo[i].GetScoreAsString(record.first);
                CCountedPointer<NUISystem::CComponent> score_text(manager->CreateComponent("PC_SCORE_TEXT", false));
                ((NUISystem::CText*)score_text.Data)->SetValue(score, true, true);
                CCountedPointer<NUISystem::CComponent> holder_text(manager->CreateComponent("PC_SCORE_HOLDER_TEXT", false));
                NUISystem::CText* holder = (NUISystem::CText*)holder_text.Data;
                score = record.second;
                holder->SetValue(score, true, true);
                holder->ForceWidth(100.0f);
                CCountedPointer<NUISystem::CComponent> button(manager->CreateComponent("PC_SCORE_BUTTON", false));
                button->AddChild(score_text);
                button->AddChild(holder_text);
                entry->AddChild(button);
                NUISystem::CText* subtitle = (NUISystem::CText*)manager->CreateComponent("PC_SUBTITLE", false);
                subtitle->SetValue(ActivityEnumToCharString(activity).ToWideString(), true, true);
                CCountedPointer<NUISystem::CComponent> subtitle_ptr(subtitle);
                entry->AddChild(subtitle_ptr);
                frame->AddPositionChild(subtitle);
                NUISystem::CText* description = (NUISystem::CText*)manager->CreateComponent("PC_SCORE_DESCRIPTION", false);
                std::list<CClanMember>::iterator holder_member = std::find(ClanMembers.begin(), ClanMembers.end(), record.second);
                if (holder_member != ClanMembers.end())
                    description->SetValue(ActivityInfo[i].Description, true, true);
                CCountedPointer<NUISystem::CComponent> description_ptr(description);
                entry->AddChild(description_ptr);
                frame->AddPositionChild(description);
                CCountedPointer<NUISystem::CComponent> records_ptr(manager->CreateComponent("PC_SUB_LIST", false));
                NUISystem::CList* records = (NUISystem::CList*)records_ptr.Data;
                NUISystem::CText* records_title = (NUISystem::CText*)manager->CreateComponent("PC_TITLE", false);
                CWideString records_text;
                records_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(ActivityEnumToCharString(activity));
                records_text += L" ";
                records_text += CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_SCOREBOARD_RECORDS");
                records_title->SetValue(records_text, true, true);
                CCountedPointer<NUISystem::CComponent> records_title_ptr(records_title);
                records->AddNonScrollingChild(records_title_ptr);
                frame->AddPositionChild(records_title);
                records->InitialiseOffsets(DownloadedClanScoreboard[activity].size());
                for (unsigned long j = 0; j < DownloadedClanScoreboard[activity].size(); ++j) {
                    record = DownloadedClanScoreboard[activity][j];
                    CWideString record_score = ActivityInfo[i].GetScoreAsString(record.first);
                    CCountedPointer<NUISystem::CComponent> record_entry(manager->CreateMenuEntryComponent(record_score,
                        0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                    CCountedPointer<NUISystem::CComponent> name_text(manager->CreateComponent("PC_SCORE_HOLDER_TEXT", false));
                    NUISystem::CText* name = (NUISystem::CText*)name_text.Data;
                    name->SetValue(record.second, true, true);
                    record_entry->AddChild(name_text);
                    name->ForceWidth(100.0f);
                    NUISystem::CText* record_subtitle = (NUISystem::CText*)manager->CreateComponent("PC_SUBTITLE", false);
                    record_subtitle->SetValue(record.second, true, true);
                    CCountedPointer<NUISystem::CComponent> record_subtitle_ptr(record_subtitle);
                    record_entry->AddChild(record_subtitle_ptr);
                    frame->AddPositionChild(record_subtitle);
                    NUISystem::CText* record_description = (NUISystem::CText*)manager->CreateComponent("PC_SCORE_DESCRIPTION", false);
                    std::list<CClanMember>::iterator member = std::find(ClanMembers.begin(), ClanMembers.end(), record.second);
                    if (member != ClanMembers.end())
                        record_description->SetValue((*member).Description, true, true);
                    CCountedPointer<NUISystem::CComponent> record_description_ptr(record_description);
                    record_entry->AddChild(record_description_ptr);
                    frame->AddPositionChild(record_description);
                    NUISystem::CSprite* image = (NUISystem::CSprite*)manager->CreateComponent("PC_CLAN_IMAGE", false);
                    {
                        CWideString cache_name(L"MemberData");
                        image->PTexture = GetCachedImage(cache_name, (*member).MemberID);
                    }
                    CCountedPointer<NUISystem::CComponent> image_ptr(image);
                    record_entry->AddChild(image_ptr);
                    frame->AddPositionChild(image);
                    records->AddScrollingChildAndSetOffsets(record_entry);
                }
                entry->AddChild(records_ptr);
                list->AddScrollingChildDynamicallyAndSetOffsets(entry);
            }
        }
    }
    PClanMenuEntry->ProcessEvent(NUISystem::UI_EVENT_SELECT);
    PClanMenuEntry = 0;
    ScoreboardState = 0;
    TaskState = 100;
}
