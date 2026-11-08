// CTCInventory::OpenPCOptionsMenu() -- open the PC options screen: close the open menu components, mark the options menu open and reuse the PC_OPTIONS_LIST component if it is still open, else build it (frame pictures / descriptions / subtitles, the in-game menu's PC save and load entries, the accept / back / restore-defaults actions of the gameplay, audio, video and redefine-keys pages, the left / right arrow actions of every slider, the resolution and anti-aliasing text-slider entries), then observe its scroll buttons, grey out the arrows of the detail sliders and text sliders that cannot move, append the "Hz" text to every refresh rate and put the list in state 5
// Inline-budget: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (the shared fable_string.h __forceinline opt-in) -- retail expands CCharString::operator==(const char*) in place at 0x0058AF70..0x0058AF9D (null arm: `repe cmpsb` of the literal with ""; else `call 0x004115A0`); plain C++ calls it out of line (13019v13048), /d2inlS10 / S12 / S15 already over-inline the rest (13153 / 13514 / 13520v13048), /d2inlT200 changes nothing
// Leaf: ?OpenPCOptionsMenu@CTCInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058AF10, 13048 bytes = 12980 bytes of code (`add ebp,78h ; leave ; ret` ends at 0x0058E1C3)
//   + the 17-entry anti-aliasing switch table 0x0058E1C4..0x0058E207. Size-optimised TU with the biased frame pointer
//   (`push ebp ; lea ebp,[esp-78h] ; sub esp,0E4h`): `#pragma optimize("s", on)`.
// Callees: CTCInventory::DeleteMenuComponents 0x00587A41; NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D;
//   CBasicString<char>::operator==(const char*) 0x004115A0; CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / copy 0x0099EC30;
//   CThing::GetTC (bitset test 0x00410DE0 + CVectorMap LowerBound 0x0040F020); CTCInGameMenu::OpenInGameMenuPC 0x004804DE;
//   operator new 0x00BFEA1A; CAction() 0x0042BE50; CCountedPointer<CAction>(CAction*) 0x0042AA29 / ~ 0x00429160;
//   CAction::AddParam 0x0053FDE0; list<CActionParam*>::push_back 0x0042B646; CRedefinerList::SetAcceptButton 0x00556570;
//   list<CComponent*> ctor 0x0042ABAF / insert 0x0042AB8A / dtor 0x0042ABCA; pair<CCharString,CCharString>(pair<const char*,
//   const char*>) 0x00592495; list<pair<CCharString,CCharString> > ctor 0x0059326D / insert 0x00593ADE / dtor 0x00593B03;
//   list<pair<CComponent*,CComponent*> > ctor 0x00593288 / insert 0x00593B1A / dtor 0x005932A3; CUserProfileManager::Get
//   0x0040D2A0 / GetDisplayModes 0x00406880; CCountedPointer<CComponent>(CComponent*) 0x00429C15 / ~ 0x004291DE;
//   GFIntToWideString 0x00405F00; CWideString() 0x0099AED0 / ~ 0x0099B510 / operator=(const CWideString&) 0x0099B7D0 /
//   operator=(const wchar_t*) 0x0099B800; operator+(const CWideString&, const wchar_t*) 0x0099BF30 / (const CWideString&,
//   const CWideString&) 0x0099BE70; GFGetSystemManager 0x009A4EC0; CDisplayManager::EnumerateAntiAliasingModes 0x009C05C0;
//   free 0x00BFEA14 (the CArray destructor); vector<CComponent*>::push_back 0x00486024; vector<CCharString>::push_back 0x0044BFF0
//   / erase(first, last) 0x0043336A / dtor 0x00414EA0; _Rb_global<bool>::_M_increment 0x004292C0 (state-map iteration);
//   CMainGameComponent::Get / GetTextBank (inline), NGameText::CDataBank::GetTextBySymbol 0x009C95E0; and the CComponent virtuals
//   FindChild (+0x0C), FindAllChildren (+0x14), FindAllChildrenOfType (+0x18), SetPAcceptButton (+0x1C), SetColour (+0x34),
//   GetColour (+0x38), Initialise (+0xAC), ChangeState (+0xC0), GetParent (+0xD0), AddChild (+0xEC), AddActionOnSelected (+0x10C),
//   AddActionOnInactive (+0x118), AddActionOnHovered (+0x138), AddActionOnLeftUnclicked (+0x140), GetStates (+0x148), GetName
//   (+0x150), MakeUnavailableDynamically (+0x174), FindFirstChildOfTypeNonRecursive (+0x184), AddPositionChild (+0x1F0),
//   CViewport::AddFakeChild (+0x238), CText::GetValue (+0x238) / SetValue (+0x240), CObserver::ObserveEvent / SetPreventObserving
//   (through the +4 CObserver base) and CObservable::AddObserver (+8 of the manager).
// Shape: `list` is a plain local: the inline STLport vector::push_back body, which VC7.1 declines here (the call to 0x00486024
//   stays), binds its `const T&` to a temporary that shares list's home [ebp+10h], so c2 keeps list in edi / ebx between
//   the spills (an out-of-line push_back declaration address-exposes list and loses both the register and the 0xE4 frame).
//   The anti-aliasing switch has its own `case SURFACE_MULTISAMPLE_NONE` (same text as default) so the table starts at 0;
//   the text-slider entries test the counted pointer before naming the CText; the widget loop walks the slider iterator
//   declared first and increments it first; the colour, the redefiner list and each quit picture are named locals (their
//   construction / load precedes the call's vtable fetch); the gameplay / audio / video pages build an OBSERVE action for the
//   accept button that is never attached (as retail).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_OPENPC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAKE_PAIR_ARRAYS
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CACTIONPARAMEBOOL
#define FABLE_RETAIL_CREDEFINERLIST_SETACCEPTBUTTON
#define FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_ANTIALIASING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "stlport/fable_containers.h"
#include "engine/CDisplayManager.h"
#include "engine/CUserProfileManager.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInGameMenu.h"
#include "engine/CSystemManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CSlider.h"
#include "engine/NUISystem_CTextSlider.h"
#include "engine/NUISystem_CRedefinerList.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/NUISystem_CActionParamEBOOL.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"

using NUISystem::CComponent;
using NUISystem::CViewport;
using NUISystem::CText;
using NUISystem::CSlider;
using NUISystem::CTextSlider;
using NUISystem::CRedefinerList;
using NUISystem::CUIState;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;
using NUISystem::CActionParamEBOOL;

CWideString __fastcall GFIntToWideString(long value);               // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

typedef std::map<unsigned long, CUIState, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CUIState> > > CUIStateMap;

void CTCInventory::OpenPCOptionsMenu()
{
    DeleteMenuComponents();
    CurrentOpenMenu = (CURRENT_OPEN_MENU)4;

    // reuse the options list if it is already one of the open menu components
    CComponent* list = 0;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    for (unsigned long i = 0; i < Components.size(); ++i) {
        if (list)
            break;
        if (Components[i]->GetName() == "PC_OPTIONS_LIST")
            list = Components[i];
    }

    if (!list) {
        list = manager->CreateComponent("PC_OPTIONS_LIST", false);
        CComponent* frame = list->FindChild("PC_UI_FADING_FRAME");
        CComponent* description = list->FindChild("PC_DESCRIPTION_VIEWPORT");
        CViewport* viewport = 0;
        if (description)
            viewport = static_cast<CViewport*>(description);

        // the fading frame owns the screen's pictures, descriptions and subtitles
        if (frame) {
            CComponent* clickable = frame->FindChild("PC_BACK_CLICKABLE");
            manager->AddObserver(clickable);
            clickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
            clickable = frame->FindChild("PC_CLOSE_CLICKABLE");
            manager->AddObserver(clickable);
            clickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);

            CComponent* child = list->FindChild("PC_TITLE_OPTIONS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_SAVE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_SAVE_DESC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_SAVE_SUB");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_LOAD");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_LOAD_DESC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_LOAD_SUB");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_GAMEPLAY_PREFS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_GAMEPLAY_PREFS_DESC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_GAMEPLAY_PREFS_SUB");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_AUDIO_PREFS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_AUDIO_PREFS_DESC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_AUDIO_PREFS_SUB");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_VIDEO_PREFS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_VIDEO_PREFS_DESC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_VIDEO_PREFS_SUB");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_REDEFINE_KEYS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_REDEFINE_KEYS_DESC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_REDEFINE_KEYS_SUB");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_AUDIO_PREFS_TITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_GAMEPLAY_PREFS_TITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_VIDEO_PREFS_TITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_REDEFINE_KEYS_TITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_SCOREBOARD_PREFS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_SCOREBOARD_PREFS_SUB");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_SCOREBOARD_PREFS_DESC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_SCOREBOARD_PREFS_TITLE");
            if (child)
                frame->AddPositionChild(child);
            if (description)
                frame->AddPositionChild(description);

            // the quit entry's ring pictures, description and subtitle
            CComponent* quit = list->FindChild("PC_MENU_ENTRY_QUIT");
            std::list<CComponent*> pictures;
            quit->FindAllChildren("PC_RING_PIC_QUIT", pictures);
            for (std::list<CComponent*>::iterator it = pictures.begin(), last = pictures.end(); it != last; ++it) {
                CComponent* picture = *it;
                frame->AddPositionChild(picture);
            }
            child = quit->FindChild("PC_QUIT_DESC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = quit->FindChild("PC_QUIT_SUB");
            if (child)
                frame->AddPositionChild(child);
        }

        // the in-game menu fills the save / load entries
        CTCInGameMenu* inGameMenu = 0;
        ParentThing->GetTC(TCI_IN_GAME_MENU, &inGameMenu);
        inGameMenu->OpenInGameMenuPC(PMouse_Pointer, list);

        std::list<CComponent*> sliderPages;
        {
            CComponent* gameplayPage = list->FindChild("PC_GAMEPLAY_PREFS_LIST");
            CComponent* audioPage = list->FindChild("PC_AUDIO_PREFS_LIST");
            CComponent* videoPage = list->FindChild("PC_VIDEO_PREFS_LIST");
            CComponent* redefinePage = list->FindChild("PC_REDEFINE_KEYS_LIST");

            // gameplay page: the accept / back / restore buttons are observed while the page is selected
            {
                CComponent* acceptContainer = gameplayPage->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER");
                CComponent* backContainer = gameplayPage->FindChild("PC_OPTIONS_BACK_CONTAINER");
                CComponent* restoreContainer = gameplayPage->FindChild("PC_OPTIONS_RESTORE_DEFAULTS_CONTAINER");
                CComponent* accept = acceptContainer->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
                CComponent* back = backContainer->FindChild("PC_OPTIONS_BACK");
                CComponent* restore = restoreContainer->FindChild("PC_OPTIONS_RESTORE_DEFAULTS");
                CCountedPointer<CAction> observeAccept(new CAction);
                observeAccept->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeAccept->AddParam(accept, observeAccept);
                observeAccept->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                CCountedPointer<CAction> observeBack(new CAction);
                observeBack->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeBack->AddParam(back, observeBack);
                observeBack->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                gameplayPage->AddActionOnSelected(observeBack);
                CCountedPointer<CAction> observeRestore(new CAction);
                observeRestore->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeRestore->AddParam(restore, observeRestore);
                observeRestore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                gameplayPage->AddActionOnSelected(observeRestore);
                CCountedPointer<CAction> ignoreAccept(new CAction);
                ignoreAccept->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreAccept->AddParam(accept, ignoreAccept);
                ignoreAccept->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                gameplayPage->AddActionOnInactive(ignoreAccept);
                CCountedPointer<CAction> ignoreBack(new CAction);
                ignoreBack->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreBack->AddParam(back, ignoreBack);
                ignoreBack->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                gameplayPage->AddActionOnInactive(ignoreBack);
                CCountedPointer<CAction> ignoreRestore(new CAction);
                ignoreRestore->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreRestore->AddParam(restore, ignoreRestore);
                ignoreRestore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                gameplayPage->AddActionOnInactive(ignoreRestore);
                CCountedPointer<CAction> sendSelect(new CAction);
                sendSelect->ActionType = NUISystem::UI_ACTION_TYPE_SEND_EVENT;
                sendSelect->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_SELECT));
                accept->AddActionOnLeftUnclicked(sendSelect);
                CCountedPointer<CAction> sendBack(new CAction);
                sendBack->ActionType = NUISystem::UI_ACTION_TYPE_SEND_EVENT;
                sendBack->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_BACK));
                back->AddActionOnLeftUnclicked(sendBack);
                CCountedPointer<CAction> restoreDefaults(new CAction);
                restoreDefaults->ActionType = NUISystem::UI_ACTION_TYPE_RESTORE_DEFAULTS_GAMEPLAY;
                restore->AddActionOnLeftUnclicked(restoreDefaults);
            }
            // audio page
            {
                CComponent* acceptContainer = audioPage->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER");
                CComponent* backContainer = audioPage->FindChild("PC_OPTIONS_BACK_CONTAINER");
                CComponent* restoreContainer = audioPage->FindChild("PC_OPTIONS_RESTORE_DEFAULTS_CONTAINER");
                CComponent* accept = acceptContainer->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
                CComponent* back = backContainer->FindChild("PC_OPTIONS_BACK");
                CComponent* restore = restoreContainer->FindChild("PC_OPTIONS_RESTORE_DEFAULTS");
                CCountedPointer<CAction> observeAccept(new CAction);
                observeAccept->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeAccept->AddParam(accept, observeAccept);
                observeAccept->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                CCountedPointer<CAction> observeBack(new CAction);
                observeBack->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeBack->AddParam(back, observeBack);
                observeBack->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                audioPage->AddActionOnSelected(observeBack);
                CCountedPointer<CAction> observeRestore(new CAction);
                observeRestore->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeRestore->AddParam(restore, observeRestore);
                observeRestore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                audioPage->AddActionOnSelected(observeRestore);
                CCountedPointer<CAction> ignoreAccept(new CAction);
                ignoreAccept->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreAccept->AddParam(accept, ignoreAccept);
                ignoreAccept->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                audioPage->AddActionOnInactive(ignoreAccept);
                CCountedPointer<CAction> ignoreBack(new CAction);
                ignoreBack->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreBack->AddParam(back, ignoreBack);
                ignoreBack->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                audioPage->AddActionOnInactive(ignoreBack);
                CCountedPointer<CAction> ignoreRestore(new CAction);
                ignoreRestore->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreRestore->AddParam(restore, ignoreRestore);
                ignoreRestore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                audioPage->AddActionOnInactive(ignoreRestore);
                CCountedPointer<CAction> sendSelect(new CAction);
                sendSelect->ActionType = NUISystem::UI_ACTION_TYPE_SEND_EVENT;
                sendSelect->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_SELECT));
                accept->AddActionOnLeftUnclicked(sendSelect);
                CCountedPointer<CAction> sendBack(new CAction);
                sendBack->ActionType = NUISystem::UI_ACTION_TYPE_SEND_EVENT;
                sendBack->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_BACK));
                back->AddActionOnLeftUnclicked(sendBack);
                CCountedPointer<CAction> restoreDefaults(new CAction);
                restoreDefaults->ActionType = NUISystem::UI_ACTION_TYPE_RESTORE_DEFAULTS_AUDIO;
                restore->AddActionOnLeftUnclicked(restoreDefaults);
            }
            // video page
            {
                CComponent* acceptContainer = videoPage->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER");
                CComponent* backContainer = videoPage->FindChild("PC_OPTIONS_BACK_CONTAINER");
                CComponent* restoreContainer = videoPage->FindChild("PC_OPTIONS_RESTORE_DEFAULTS_CONTAINER");
                CComponent* accept = acceptContainer->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
                CComponent* back = backContainer->FindChild("PC_OPTIONS_BACK");
                CComponent* restore = restoreContainer->FindChild("PC_OPTIONS_RESTORE_DEFAULTS");
                CCountedPointer<CAction> observeAccept(new CAction);
                observeAccept->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeAccept->AddParam(accept, observeAccept);
                observeAccept->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                CCountedPointer<CAction> observeBack(new CAction);
                observeBack->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeBack->AddParam(back, observeBack);
                observeBack->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                videoPage->AddActionOnSelected(observeBack);
                CCountedPointer<CAction> observeRestore(new CAction);
                observeRestore->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeRestore->AddParam(restore, observeRestore);
                observeRestore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                videoPage->AddActionOnSelected(observeRestore);
                CCountedPointer<CAction> ignoreAccept(new CAction);
                ignoreAccept->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreAccept->AddParam(accept, ignoreAccept);
                ignoreAccept->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                videoPage->AddActionOnInactive(ignoreAccept);
                CCountedPointer<CAction> ignoreBack(new CAction);
                ignoreBack->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreBack->AddParam(back, ignoreBack);
                ignoreBack->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                videoPage->AddActionOnInactive(ignoreBack);
                CCountedPointer<CAction> ignoreRestore(new CAction);
                ignoreRestore->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreRestore->AddParam(restore, ignoreRestore);
                ignoreRestore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                videoPage->AddActionOnInactive(ignoreRestore);
                CCountedPointer<CAction> sendSelect(new CAction);
                sendSelect->ActionType = NUISystem::UI_ACTION_TYPE_SEND_EVENT;
                sendSelect->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_SELECT));
                accept->AddActionOnLeftUnclicked(sendSelect);
                CCountedPointer<CAction> sendBack(new CAction);
                sendBack->ActionType = NUISystem::UI_ACTION_TYPE_SEND_EVENT;
                sendBack->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_BACK));
                back->AddActionOnLeftUnclicked(sendBack);
                CCountedPointer<CAction> restoreDefaults(new CAction);
                restoreDefaults->ActionType = NUISystem::UI_ACTION_TYPE_RESTORE_DEFAULTS_VIDEO;
                restore->AddActionOnLeftUnclicked(restoreDefaults);
            }
            // redefine-keys page: no restore button (hidden), the edit boxes are set on select and the redefiner list accepts
            {
                CComponent* acceptContainer = redefinePage->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER");
                CComponent* backContainer = redefinePage->FindChild("PC_OPTIONS_BACK_CONTAINER");
                CComponent* restoreContainer = redefinePage->FindChild("PC_OPTIONS_RESTORE_DEFAULTS_CONTAINER");
                CComponent* accept = acceptContainer->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
                CComponent* back = backContainer->FindChild("PC_OPTIONS_BACK");
                CComponent* restore = restoreContainer->FindChild("PC_OPTIONS_RESTORE_DEFAULTS");
                CRGBColour hidden(0, 0, 0, 0);
                restore->SetColour(hidden);
                for (CUIStateMap::iterator state = restore->GetStates().begin(); state != restore->GetStates().end(); ++state)
                    state->second.Colour.A = 0;
                CCountedPointer<CAction> observeAccept(new CAction);
                observeAccept->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeAccept->AddParam(accept, observeAccept);
                observeAccept->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                CCountedPointer<CAction> setEditBoxes(new CAction);
                setEditBoxes->ActionType = NUISystem::UI_ACTION_TYPE_SET_EDITBOX_VALUES;
                redefinePage->AddActionOnSelected(setEditBoxes);
                CCountedPointer<CAction> observeBack(new CAction);
                observeBack->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeBack->AddParam(back, observeBack);
                observeBack->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                redefinePage->AddActionOnSelected(observeBack);
                CCountedPointer<CAction> ignoreAccept(new CAction);
                ignoreAccept->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreAccept->AddParam(accept, ignoreAccept);
                ignoreAccept->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                redefinePage->AddActionOnInactive(ignoreAccept);
                CCountedPointer<CAction> ignoreBack(new CAction);
                ignoreBack->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreBack->AddParam(back, ignoreBack);
                ignoreBack->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                redefinePage->AddActionOnInactive(ignoreBack);
                CCountedPointer<CAction> acceptDeactivate(new CAction);
                acceptDeactivate->ActionType = NUISystem::UI_ACTION_TYPE_DEACTIVATE_EDITBOX;
                acceptDeactivate->UIActionParams.push_back(new CActionParamEBOOL(true));
                accept->AddActionOnLeftUnclicked(acceptDeactivate);
                CCountedPointer<CAction> sendSelect(new CAction);
                sendSelect->ActionType = NUISystem::UI_ACTION_TYPE_SEND_EVENT;
                sendSelect->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_SELECT));
                accept->AddActionOnLeftUnclicked(sendSelect);
                CCountedPointer<CAction> backDeactivate(new CAction);
                backDeactivate->ActionType = NUISystem::UI_ACTION_TYPE_DEACTIVATE_EDITBOX;
                back->AddActionOnLeftUnclicked(backDeactivate);
                CCountedPointer<CAction> sendBack(new CAction);
                sendBack->ActionType = NUISystem::UI_ACTION_TYPE_SEND_EVENT;
                sendBack->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_BACK));
                back->AddActionOnLeftUnclicked(sendBack);
                CRedefinerList* redefiner = static_cast<CRedefinerList*>(redefinePage->FindChild("PC_UI_REDEFINER_LIST"));
                redefiner->SetAcceptButton(accept);
            }

            // the page each slider below sits on, in slider order
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(gameplayPage);
            sliderPages.push_back(audioPage);
            sliderPages.push_back(audioPage);
            sliderPages.push_back(audioPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
            sliderPages.push_back(videoPage);
        }

        // each slider's whole entry and the slider inside it
        std::list<std::pair<CCharString, CCharString> > sliderNames;
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_CAMERA_UP_DOWN", "PC_OPTIONS_CAMERA_UP_DOWN_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_SUBTITLES", "PC_OPTIONS_SUBTITLES_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_HUD_TRANSPARENCY", "PC_SLIDER_HUD_TRANSPARENCY"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_TUTORIALS", "PC_OPTIONS_TUTORIALS_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_GUILD_MASTER", "PC_OPTIONS_GUILD_MASTER_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_HUD_TOOLTIPS", "PC_OPTIONS_HUD_TOOLTIPS_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_CAMERA_RESETTING", "PC_OPTIONS_CAMERA_RESETTING_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_BOW_CAMERA", "PC_OPTIONS_BOW_CAMERA_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_CAMERA_SENSITIVITY", "PC_SLIDER_CAMERA_SENSITIVITY"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_TARGETING_STATUS", "PC_OPTIONS_TARGETING_STATUS_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_MUSIC", "PC_SLIDER_MUSIC"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_SOUND", "PC_SLIDER_SOUND"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_DIALOGUE", "PC_SLIDER_DIALOGUE"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_RESOLUTION", "PC_OPTIONS_RESOLUTION_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_REFRESH_RATE", "PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_ANTIALIASING", "PC_OPTIONS_ANTIALIASING_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_TEXTURE_DETAIL", "PC_SLIDER_TEXTURE_DETAIL"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_MESH_DETAIL", "PC_SLIDER_MESH_DETAIL"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_SHADOW_DETAIL", "PC_SLIDER_SHADOW_DETAIL"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_BRIGHTNESS", "PC_SLIDER_BRIGHTNESS"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_VERTICAL_SYNC", "PC_OPTIONS_VERTICAL_SYNC_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_TEXT_SLIDER_WHOLE_SCREEN_ASPECT_RATIO", "PC_OPTIONS_SCREEN_ASPECT_RATIO_TEXT_SLIDER"));
        sliderNames.push_back(std::make_pair("PC_OPTIONS_SLIDER_WHOLE_EFFECTS_DETAIL", "PC_SLIDER_EFFECTS_DETAIL"));

        std::list<std::pair<CComponent*, CComponent*> > sliders;
        for (std::list<std::pair<CCharString, CCharString> >::iterator name = sliderNames.begin(); name != sliderNames.end(); ++name) {
            CComponent* whole = list->FindChild(name->first);
            if (whole) {
                CComponent* slider = whole->FindChild(name->second);
                if (slider)
                    sliders.push_back(std::make_pair(whole, slider));
            }
        }

        // the arrows either side of each slider scroll the page to it, slide it, and are observed with the page
        std::list<std::pair<CComponent*, CComponent*> >::iterator entry = sliders.begin();
        for (std::list<CComponent*>::iterator page = sliderPages.begin(); page != sliderPages.end() && entry != sliders.end(); ++entry, ++page) {
            CComponent* sliderPage = *page;
            if (sliderPage) {
                CComponent* whole = entry->first;
                if (whole) {
                    CComponent* slider = entry->second;
                    if (slider) {
                        CComponent* accept = sliderPage->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
                        if (accept)
                            slider->SetPAcceptButton(accept);
                        CComponent* arrow = whole->FindChild("PC_OPTIONS_LEFT_ARROW");
                        if (arrow) {
                            CCountedPointer<CAction> scrollTo(new CAction);
                            scrollTo->ActionType = NUISystem::UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UNTIL_CHILD_CONTAINING;
                            scrollTo->AddParam(whole, scrollTo);
                            arrow->AddActionOnHovered(scrollTo);
                            CCountedPointer<CAction> slide(new CAction);
                            slide->ActionType = NUISystem::UI_ACTION_TYPE_SEND_OWNED_EVENT;
                            slide->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_SLIDER_LEFT));
                            slide->AddParam(slider, slide);
                            arrow->AddActionOnLeftUnclicked(slide);
                            CCountedPointer<CAction> observeArrow(new CAction);
                            observeArrow->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                            observeArrow->AddParam(arrow, observeArrow);
                            observeArrow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                            sliderPage->AddActionOnSelected(observeArrow);
                            CCountedPointer<CAction> ignoreArrow(new CAction);
                            ignoreArrow->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                            ignoreArrow->AddParam(arrow, ignoreArrow);
                            ignoreArrow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                            sliderPage->AddActionOnInactive(ignoreArrow);
                        }
                        arrow = whole->FindChild("PC_OPTIONS_RIGHT_ARROW");
                        if (arrow) {
                            CCountedPointer<CAction> scrollTo(new CAction);
                            scrollTo->ActionType = NUISystem::UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UNTIL_CHILD_CONTAINING;
                            scrollTo->AddParam(whole, scrollTo);
                            arrow->AddActionOnHovered(scrollTo);
                            CCountedPointer<CAction> slide(new CAction);
                            slide->ActionType = NUISystem::UI_ACTION_TYPE_SEND_OWNED_EVENT;
                            slide->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_SLIDER_RIGHT));
                            slide->AddParam(slider, slide);
                            arrow->AddActionOnLeftUnclicked(slide);
                            CCountedPointer<CAction> observeArrow(new CAction);
                            observeArrow->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                            observeArrow->AddParam(arrow, observeArrow);
                            observeArrow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                            sliderPage->AddActionOnSelected(observeArrow);
                            CCountedPointer<CAction> ignoreArrow(new CAction);
                            ignoreArrow->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
                            ignoreArrow->AddParam(arrow, ignoreArrow);
                            ignoreArrow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                            sliderPage->AddActionOnInactive(ignoreArrow);
                        }
                    }
                }
            }
        }

        // the resolution text slider: one "<width>X<height>X<bits>" entry per display mode
        CComponent* resolutionSlider = list->FindChild("PC_OPTIONS_RESOLUTION_TEXT_SLIDER");
        const std::vector<CDisplayModeDesc>& displayModes = CUserProfileManager::Get().GetDisplayModes();
        for (std::vector<CDisplayModeDesc>::const_iterator mode = displayModes.begin(); mode != displayModes.end(); ++mode) {
            CCountedPointer<CComponent> entryPointer(NUISystem::CManager::GetInstance()->CreateComponent("PC_OPTIONS_TEXT_IN_TEXT_SLIDER_TEMPLATE", false));
            if (!entryPointer.IsNull()) {
                CText* entryText = static_cast<CText*>(entryPointer.Get());
                {
                    CWideString modeText = GFIntToWideString(mode->Dimensions.X) + L"X" + GFIntToWideString(mode->Dimensions.Y) + L"X" + GFIntToWideString(mode->BitsPerPixel);
                    entryText->SetValue(modeText, true, true);
                }
                resolutionSlider->AddChild(entryPointer);
            }
        }

        // the anti-aliasing text slider: "<n>X" per supported multisample level, "off" for none
        CComponent* antialiasingSlider = list->FindChild("PC_OPTIONS_ANTIALIASING_TEXT_SLIDER");
        CArray<ESurfaceMultisampleType> antialiasingModes;
        GFGetSystemManager()->GetDisplayManager()->EnumerateAntiAliasingModes(antialiasingModes);
        for (CArray<ESurfaceMultisampleType>::iterator level = antialiasingModes.begin(); level != antialiasingModes.end(); ++level) {
            CCountedPointer<CComponent> entryPointer(NUISystem::CManager::GetInstance()->CreateComponent("PC_OPTIONS_TEXT_IN_TEXT_SLIDER_TEMPLATE", false));
            if (!entryPointer.IsNull()) {
                CText* entryText = static_cast<CText*>(entryPointer.Get());
                {
                    CWideString levelText;
                    switch (*level) {
                    case SURFACE_MULTISAMPLE_NONE: levelText = L"TEXT_GUI_MENU_OFF"; break;
                    case SURFACE_MULTISAMPLE_2:  levelText = GFIntToWideString(2) + L"X"; break;
                    case SURFACE_MULTISAMPLE_3:  levelText = GFIntToWideString(3) + L"X"; break;
                    case SURFACE_MULTISAMPLE_4:  levelText = GFIntToWideString(4) + L"X"; break;
                    case SURFACE_MULTISAMPLE_5:  levelText = GFIntToWideString(5) + L"X"; break;
                    case SURFACE_MULTISAMPLE_6:  levelText = GFIntToWideString(6) + L"X"; break;
                    case SURFACE_MULTISAMPLE_7:  levelText = GFIntToWideString(7) + L"X"; break;
                    case SURFACE_MULTISAMPLE_8:  levelText = GFIntToWideString(8) + L"X"; break;
                    case SURFACE_MULTISAMPLE_9:  levelText = GFIntToWideString(9) + L"X"; break;
                    case SURFACE_MULTISAMPLE_10: levelText = GFIntToWideString(10) + L"X"; break;
                    case SURFACE_MULTISAMPLE_11: levelText = GFIntToWideString(11) + L"X"; break;
                    case SURFACE_MULTISAMPLE_12: levelText = GFIntToWideString(12) + L"X"; break;
                    case SURFACE_MULTISAMPLE_13: levelText = GFIntToWideString(13) + L"X"; break;
                    case SURFACE_MULTISAMPLE_14: levelText = GFIntToWideString(14) + L"X"; break;
                    case SURFACE_MULTISAMPLE_15: levelText = GFIntToWideString(15) + L"X"; break;
                    case SURFACE_MULTISAMPLE_16: levelText = GFIntToWideString(16) + L"X"; break;
                    default: levelText = L"TEXT_GUI_MENU_OFF"; break;
                    }
                    entryText->SetValue(levelText, true, true);
                }
                antialiasingSlider->AddChild(entryPointer);
            }
        }

        list->Initialise();
        Components.push_back(list);
    }

    // the scroll buttons are observed for mouse movement
    CComponent* scroll = list->FindChild("SCROLL_LIST_UP");
    if (scroll) {
        manager->AddObserver(scroll);
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = list->FindChild("SCROLL_DESCRIPTION_UP");
    if (scroll) {
        manager->AddObserver(scroll);
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = list->FindChild("SCROLL_LIST_DOWN");
    if (scroll) {
        manager->AddObserver(scroll);
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = list->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (scroll) {
        manager->AddObserver(scroll);
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }

    // a detail slider with no range: its arrows stop listening and fade to half alpha, its text becomes unavailable
    std::vector<CCharString> names;
    names.push_back("PC_SLIDER_TEXTURE_DETAIL");
    names.push_back("PC_SLIDER_MESH_DETAIL");
    names.push_back("PC_SLIDER_EFFECTS_DETAIL");
    names.push_back("PC_SLIDER_SHADOW_DETAIL");
    for (std::vector<CCharString>::iterator name = names.begin(); name != names.end(); ++name) {
        CSlider* slider = static_cast<CSlider*>(list->FindChild(*name));
        if (slider && slider->Min.X == slider->Max.X && slider->GetParent() && slider->GetParent()->GetParent()) {
            CComponent* whole = slider->GetParent()->GetParent();
            CComponent* arrow = whole->FindChild("PC_OPTIONS_LEFT_ARROW");
            if (arrow) {
                arrow->SetPreventObserving(true);
                for (CUIStateMap::iterator state = arrow->GetStates().begin(); state != arrow->GetStates().end(); ++state)
                    state->second.Colour.A /= 2;
                CRGBColour colour = arrow->GetColour();
                colour.A /= 2;
                arrow->SetColour(colour);
            }
            arrow = whole->FindChild("PC_OPTIONS_RIGHT_ARROW");
            if (arrow) {
                arrow->SetPreventObserving(true);
                for (CUIStateMap::iterator state = arrow->GetStates().begin(); state != arrow->GetStates().end(); ++state)
                    state->second.Colour.A /= 2;
                CRGBColour colour = arrow->GetColour();
                colour.A /= 2;
                arrow->SetColour(colour);
            }
            CComponent* text = whole->FindFirstChildOfTypeNonRecursive(NUISystem::UI_TYPE_TEXT);
            if (text)
                text->MakeUnavailableDynamically();
        }
    }

    // the same for a text slider with a single entry
    names.clear();
    names.push_back("PC_OPTIONS_RESOLUTION_TEXT_SLIDER");
    names.push_back("PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER");
    names.push_back("PC_OPTIONS_ANTIALIASING_TEXT_SLIDER");
    for (std::vector<CCharString>::iterator name = names.begin(); name != names.end(); ++name) {
        CTextSlider* slider = static_cast<CTextSlider*>(list->FindChild(*name));
        if (slider && slider->MaxChild == 0 && slider->GetParent()) {
            CComponent* whole = slider->GetParent();
            CComponent* arrow = whole->FindChild("PC_OPTIONS_LEFT_ARROW");
            if (arrow) {
                arrow->SetPreventObserving(true);
                for (CUIStateMap::iterator state = arrow->GetStates().begin(); state != arrow->GetStates().end(); ++state)
                    state->second.Colour.A /= 2;
                CRGBColour colour = arrow->GetColour();
                colour.A /= 2;
                arrow->SetColour(colour);
            }
            arrow = whole->FindChild("PC_OPTIONS_RIGHT_ARROW");
            if (arrow) {
                arrow->SetPreventObserving(true);
                for (CUIStateMap::iterator state = arrow->GetStates().begin(); state != arrow->GetStates().end(); ++state)
                    state->second.Colour.A /= 2;
                CRGBColour colour = arrow->GetColour();
                colour.A /= 2;
                arrow->SetColour(colour);
            }
            CComponent* text = whole->FindFirstChildOfTypeNonRecursive(NUISystem::UI_TYPE_TEXT);
            if (text)
                text->MakeUnavailableDynamically();
        }
    }

    // the refresh rates read "<rate> Hz"
    CComponent* refreshSlider = list->FindChild("PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER");
    std::list<CComponent*> rates;
    refreshSlider->FindAllChildrenOfType(NUISystem::UI_TYPE_TEXT, rates);
    for (std::list<CComponent*>::iterator rate = rates.begin(); rate != rates.end(); ++rate) {
        CText* rateText = static_cast<CText*>(*rate);
        if (rateText)
            rateText->SetValue(rateText->GetValue() + L" " + CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_REFRESH_RATE_HZ"), true, true);
    }

    list->ChangeState(5);
}