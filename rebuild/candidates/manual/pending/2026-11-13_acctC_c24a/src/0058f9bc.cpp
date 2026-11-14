// CTCInventory::OpenPCPhotojournal() -- open the PC photojournal screen: drop the old menu components, mark the photojournal open, build the review screen, list the profile's "*.jpg" screenshots newest first as menu entries (each with a photo sub-list holding the sprite, thumbnail and description, and the load / scroll actions), hook up the clickables and keep the review screen as a menu component
// Leaf: ?OpenPCPhotojournal@CTCInventory@@QAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058F9BC, 2012 bytes, thiscall `leave ; ret`; /GS frame-pointer size TU (the biased
//   `lea ebp,[esp-24Ch]` frame, cookie 0x0139C8A8 checked by 0x00BFE9F9). Identity: Ego_r.pdb / FableWin.pdb ?OpenPCPhotojournal@CTCInventory@@QAEXXZ.
// Callees: CTCInventory::DeleteMenuComponents 0x00587A41; NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D /
//   CreateMenuEntryComponent 0x00428494 / AddObserver (slot 2); CComponent FindChild (slot 3), FindChildNonRecursive (8), Initialise (43),
//   ChangeState (48), AddChild (59), AddActionOnSelected (67), AddActionOnBack (68), GetStates (82) with map<unsigned long,CUIState>
//   operator[] 0x0042D5B1, AddPositionChild (124); CObserver::ObserveEvent (+4 base, slot 3); CSprite SetWidth / SetHeight (143 / 144);
//   CText SetValue (144) / ForceWidth (149); CList AddScrollingChild (144), AddScrollingChildAndSetOffsets (152), SetActionOnScrollUp /
//   Down (154 / 155); CUserProfileManager::Get 0x0040D2A0, GetValidProfileName 0x00406350, NResourceDirectoryNames::GetPhotojournalDir
//   0x0041A600, CWideString operator+ 0x0099BE70 / 0x0099BF30; CAFile StartFileScan 0x00999760 / ScanNextFile 0x00999850 /
//   GetFileModifiedTime 0x009992A0 / SplitPath 0x00997620; map<CDateAndTime,CWideString> map() 0x00594803 / operator[] 0x00594A08 /
//   ~map 0x005940D5; _Rb_global<bool>::_M_decrement 0x004294E2 (the reverse iterator); CAction() 0x0042BE50, CCountedPointer<CAction>
//   (T*) 0x0042AA29 / dtor 0x00429160, CCountedPointer<CComponent>(T*) 0x00429C15 / dtor 0x004291DE; list<CActionParam*>::push_back
//   0x0042B646; CActionParamString(CWideString&) 0x0054DAE0; vector<CComponent*>::push_back 0x00486024.
// Shape: the OpenPCMapMenu / OpenPCOptionsMenu (landed) spellings. What closed it: implicit const char* -> CCharString and
//   unsigned short* -> CWideString conversions for the reference arguments (the temporaries built first, the address re-taken with
//   lea), one cast pointer local each for the thumbnail (CSprite), the description (CText) and the photo sub-list (CList) (retail
//   keeps each in edi; with repeated casts the -1 length constant took the register for the whole loop), 510.0f (0x43FF0000).
// Header: FABLE_RETAIL_CDATEANDTIME_LESS (hdr/ overlay: CDateAndTime::operator< for the visible map operator[] body).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
#define FABLE_RETAIL_CTCINVENTORY_OPENPC
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_REVERSE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CAFILE_MODIFIEDTIME
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CDATEANDTIME_LESS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME_MEMBER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CDateAndTime.h"
#include "engine/CAFile.h"
#include "engine/CUserProfileManager.h"
#include "engine/CTCInventory.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamString.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/CCountedPointer.h"

using NUISystem::CComponent;
using NUISystem::CAction;

CWideString __fastcall GetValidProfileName(const CWideString& name);   // 0x00406350 ?GetValidProfileName@@YI?AVCWideString@@ABV1@@Z
namespace NResourceDirectoryNames {
CWideString __fastcall GetPhotojournalDir();                            // 0x0041A600 ?GetPhotojournalDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
}

void CTCInventory::OpenPCPhotojournal()
{
    DeleteMenuComponents();
    CurrentOpenMenu = (CURRENT_OPEN_MENU)5;

    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CComponent* review = manager->CreateComponent("PC_PHOTOJOURNAL_REVIEW", false);
    CComponent* frame = review->FindChild("PC_UI_PHOTOJOURNAL_FRAME");
    CComponent* dummy = review->FindChild("PC_LIST_POSITION_DUMMY");
    if (dummy && frame)
        dummy->AddPositionChild(frame);
    CComponent* list = review->FindChild("PC_PHOTOJOURNAL_LIST");

    // the profile's screenshots, keyed (and so sorted) by their modification time
    CUserProfileManager& profiles = CUserProfileManager::Get();
    CWideString pattern = NResourceDirectoryNames::GetPhotojournalDir() + GetValidProfileName(profiles.ProfileName) + L"\\" + L"*.jpg";
    std::map<CDateAndTime, CWideString> photos;
    CAFile::CFileScanInfo scanInfo;
    unsigned short fileName[0x20C / 2];
    bool found = CAFile::StartFileScan(pattern, scanInfo, fileName);
    while (found) {
        CDateAndTime time = CAFile::GetFileModifiedTime(fileName);
        photos[time] = fileName;
        found = CAFile::ScanNextFile(pattern, scanInfo, fileName);
    }

    // newest first
    for (std::map<CDateAndTime, CWideString>::const_reverse_iterator photo = photos.rbegin(); photo != photos.rend(); ++photo) {
        CWideString name = (*photo).second;
        CWideString title;
        CAFile::SplitPath(name, 0, 0, &title, 0);
        // Retail reads the modification time of the scan buffer's last name again and never uses it (0x0058FB8F..0x0058FBA9:
        // CWideString(fileName), GetFileModifiedTime into the loop's CDateAndTime slot [ebp-80h], the temporary destroyed).
        CDateAndTime time = CAFile::GetFileModifiedTime(fileName);
        CCountedPointer<CComponent> entry = manager->CreateMenuEntryComponent(title, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_PHOTOJOURNAL", 0.0f, "", 0.0f, "PC_PHOTOJOURNAL_MENU_ENTRY_BUTTON");
        CComponent* text = entry->FindChild("MENU_ENTRY_TEXT");
        if (text)
            static_cast<NUISystem::CText*>(text)->ForceWidth(410.0f);
        CComponent* button = entry->FindChildNonRecursive("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE");
        if (button) {
            CComponent* shape = button->FindChild("PC_MENU_ENTRY_SHAPE_BOTTOM_RIGHT");
            if (shape)
                shape->GetStates()[0].Position.X = 510.0f;
        }
        CCountedPointer<CAction> close(new CAction);
        close->ActionType = NUISystem::UI_ACTION_TYPE_CLOSE_ACTIVE_MENU;
        entry->AddActionOnBack(close);

        CCountedPointer<CComponent> subList = manager->CreateComponent("PC_INDIVIDUAL_PHOTO_SUB_LIST", false);
        CCountedPointer<CComponent> photoEntry = manager->CreateComponent("PC_INDIVIDUAL_PHOTO_MENU_ENTRY", false);
        CComponent* sprite = photoEntry->FindChild("PC_INDIVIDUAL_PHOTO_SPRITE");
        if (sprite) {
            static_cast<NUISystem::CSprite*>(sprite)->SetWidth(563);
            static_cast<NUISystem::CSprite*>(sprite)->SetHeight(420);
            if (frame)
                frame->AddPositionChild(sprite);
            CCountedPointer<CComponent> thumbnail = manager->CreateComponent("PC_PHOTOJOURNAL_THUMBNAIL", false);
            NUISystem::CSprite* thumbnailSprite = static_cast<NUISystem::CSprite*>(thumbnail.Get());
            if (frame)
                frame->AddPositionChild(thumbnailSprite);
            thumbnailSprite->SetWidth(160);
            thumbnailSprite->SetHeight(120);
            review->AddPositionChild(thumbnailSprite);
            entry->AddChild(thumbnail);
            CCountedPointer<CAction> load(new CAction);
            load->ActionType = NUISystem::UI_ACTION_TYPE_LOAD_PHOTO;
            load->UIActionParams.push_back(sprite);
            load->UIActionParams.push_back(thumbnail.Get());
            load->UIActionParams.push_back(new NUISystem::CActionParamString(name));
            entry->AddActionOnSelected(load);
            CCountedPointer<CComponent> description = manager->CreateComponent("PC_PHOTOJOURNAL_DESCRIPTION", false);
            NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
            descriptionText->SetValue(title, true, true);
            descriptionText->ForceWidth(834.0f);
            entry->AddChild(description);
            if (frame)
                frame->AddPositionChild(description.Get());
        }
        NUISystem::CList* photoList = static_cast<NUISystem::CList*>(subList.Get());
        photoList->AddScrollingChild(photoEntry);
        entry->AddChild(subList);
        CCountedPointer<CAction> scrollDown(new CAction);
        scrollDown->ActionType = NUISystem::UI_ACTION_TYPE_SCROLL_PHOTO_DOWN;
        scrollDown->UIActionParams.push_back(list);
        photoList->SetActionOnScrollDown(scrollDown);
        CCountedPointer<CAction> scrollUp(new CAction);
        scrollUp->ActionType = NUISystem::UI_ACTION_TYPE_SCROLL_PHOTO_UP;
        scrollUp->UIActionParams.push_back(list);
        photoList->SetActionOnScrollUp(scrollUp);

        CComponent* scrollListUp = review->FindChild("PC_PHOTOJOURNAL_SCROLL_LIST_UP");
        CComponent* scrollListDown = review->FindChild("PC_PHOTOJOURNAL_SCROLL_LIST_DOWN");
        if (scrollListUp) {
            NUISystem::CManager::GetInstance()->AddObserver(scrollListUp);
            scrollListUp->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
        if (scrollListDown) {
            NUISystem::CManager::GetInstance()->AddObserver(scrollListDown);
            scrollListDown->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
        static_cast<NUISystem::CList*>(list)->AddScrollingChildAndSetOffsets(entry);
    }

    if (frame) {
        CComponent* pageTitle = review->FindChild("PC_PHOTOJOURNAL_TITLE");
        if (pageTitle)
            frame->AddPositionChild(pageTitle);
    }
    review->Initialise();
    CComponent* backClickable = list->FindChild("PC_BACK_CLICKABLE_PHOTOJOURNAL");
    if (backClickable) {
        NUISystem::CManager::GetInstance()->AddObserver(backClickable);
        backClickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    CComponent* closeClickable = list->FindChild("PC_CLOSE_CLICKABLE_PHOTOJOURNAL");
    if (closeClickable) {
        NUISystem::CManager::GetInstance()->AddObserver(closeClickable);
        closeClickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    list->ChangeState(5);
    Components.push_back(review);
}
