// CTCInGameMenu::OpenInGameMenuPC(NUISystem::CComponent*, NUISystem::CComponent*) -- open the PC save / load screen inside the given options list: book the pause start, build the save and load sub-lists in their frames (scroll arrows, screenshot viewport, ring pictures, titles), one save and one load entry per save slot (description, screenshot picture, overwrite / confirm sub-lists with accept / back buttons, load-confirm sub-list), grey out the save entry while fishing or when there is no slot, then build the centred yes / no dialog and keep it in Components
// Leaf: ?OpenInGameMenuPC@CTCInGameMenu@@QAEXPAVCComponent@NUISystem@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004804DE, 11203 bytes, thiscall, `ret 8`; frame-pointer size TU (`push ebp ; lea ebp,[esp-70h] ;
//   sub esp,2C4h`), no EH frame. Next function 0x004830A1 OpenInGameMenu (landed; this row's console twin, same TU).
// Identity: Ego_r.pdb 0x0048CD2B + FableWin.pdb 0x01AB1A2F ?OpenInGameMenuPC@CTCInGameMenu@@QAEXPAVCComponent@NUISystem@@0@Z; the one
//   caller 0x0058B638 in the landed CTCInventory::OpenPCOptionsMenu passes its mouse pointer and its list.
// Twin: the landed console builder CTCInGameMenu::OpenInGameMenu 0x004830A1 (same TU): its defines, `#pragma optimize("s", on)` alone,
//   the local CTCFishing model, the save / load slot loop and signature-check spellings are copied (EVIDENCED by the twin); plus
//   VIDEODIALOGS (VideoChangesTimer = -1.0 at +0xC0, double 0x01236700), OPENPC, CYESNO and CACTIONPARAMULONG.
// Shape (bytes): the overwrite / confirm / load-confirm sub-lists each get accept / back containers and six actions; in the confirm
//   sub-list the actions sit in a nested block (released 0x0048213B..0x0048216F before the PC_CONFIRM_DESCRIPTION lookup) and its
//   containers, description and subtitle go to the LOAD frame (+0x30), its title to the save frame, as retail does.
// FITTED: `C2DVector position(...)` as a named function-scope local (retail keeps the C2DVector in its own frame slot at the bottom,
//   [ebp-254h]; a temporary or a block-scoped local shares a slot, frame 0x2BC) and the parenthesised `(GetBackBufferWidth() * 0.5f)`
//   (the PAREN tuple moves `fsubp` ahead of `lea ecx,[ebp-254h]` as at 0x00482FDE; MATCHING_PLAYBOOK "Scheduler").
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CLOSE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_MENUCOUNT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CTCINGAMEMENU_OPENMENU
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER_JOYSTICKDEVICE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVESLOTS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CJOYSTICK
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMEBOOL
#define FABLE_RETAIL_CACTIONPARAMSTATE
#define FABLE_RETAIL_CACTIONPARAMCCOUNTEDPOINTER
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
#define FABLE_RETAIL_CTCINGAMEMENU_OPENPC
#define FABLE_RETAIL_CYESNO
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CUserProfileManager.h"
#include "engine/CQuestManager.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#include "engine/CPlayerGui.h"
#include "engine/CInputManager.h"
#include "engine/CJoystick.h"
#include "engine/CASoundManager.h"
#include "engine/CAFile.h"
#include "engine/GFGetTime.h"
#include "engine/CDefPointer.h"
#include "engine/CTCInGameMenu.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamEBOOL.h"
#include "engine/NUISystem_CActionParamState.h"
#include "engine/NUISystem_CActionParamCCountedPointer.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NUISystem_CUILocaleGraphicsDef.h"
#include "engine/CCountedPointer.h"

#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"

using namespace NUISystem;

// CTCFishing (no shared header): Ego_r.pdb layout (CTCBase +0x00..+0x0B, PFish +0x0C, Stage +0x14), the inline static
// StatGetInterfaceType (TCI_FISHING = 177, the `push 0B1h` key) and ?IsActive@CTCFishing@@QBE_NXZ (FableWin.pdb, inline here:
// `cmp dword ptr [eax+14h],0 ; setne`).
class CTCFishing : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_FISHING; }
    bool IsActive() const { return Stage != 0; }
    unsigned char _pad_0x0c[0x14 - 0x0c];                // PFish (CIntelligentPointer<CThing>)
    long Stage;                                          // +0x14 EFishingStage
};


void CTCInGameMenu::OpenInGameMenuPC(CComponent* mousePointer, CComponent* list)
{
    Signatures.clear();
    CurStartPauseTime = GFGetTime();
    VideoChangesTimer = -1.0;
    InGameMenuOpen = true;
    PMousePointer = mousePointer;

    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CViewport* area = static_cast<CViewport*>(list->FindChild("PC_DESCRIPTION_VIEWPORT"));
    CCountedPointer<CComponent> saveFrame(manager->CreateComponent("PC_UI_FRAME_FOR_SAVE_LOAD_LISTS", false));
    CCountedPointer<CComponent> loadFrame(manager->CreateComponent("PC_UI_FRAME_FOR_SAVE_LOAD_LISTS", false));
    manager->SetPlayerThing(ParentThing);

    if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens()) {
        CText* text = static_cast<CText*>(list->FindChild("UI_SAVE_TEXT"));
        if (text)
            text->SetValue("TEXT_GUI_MENU_SAVE_HERO", true, true);
        text = static_cast<CText*>(list->FindChild("PC_SAVE_SUB"));
        if (text)
            text->SetValue("TEXT_GUI_MENU_SAVE_HERO", true, true);
        text = static_cast<CText*>(list->FindChild("PC_SAVE_DESC"));
        if (text)
            text->SetValue("TEXT_GUI_MENUGUIDE_PAUSEMENU_SAVE_HERO", true, true);
        CSprite* picture = static_cast<CSprite*>(list->FindChild("PC_RING_PIC_SAVE"));
        if (picture)
            picture->SetGraphicIndex(manager->GetMiscThingsDef()->SaveHeroGraphicIndex);
    }

    // Retail looks the save entry up here and drops the result (FindChild, `call [esi+0Ch]` 0x004806FE, eax never stored
    // before the PC_SUB_LIST string 0x00480701); it is fetched again at the end.
    list->FindChild("PC_MENU_ENTRY_SAVE");

    // the save and load sub-lists, each in its frame
    CCountedPointer<CComponent> saveSubList(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<CList> saveList;
    CCountedPointer<CComponent> loadSubList(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<CList> loadList;
    CCountedPointer<CComponent> loadDummy(manager->CreateComponent("PC_LIST_POSITION_DUMMY", false));
    CCountedPointer<CComponent> saveDummy(manager->CreateComponent("PC_LIST_POSITION_DUMMY", false));
    loadDummy->AddPositionChild(loadFrame.Get());
    saveDummy->AddPositionChild(saveFrame.Get());
    saveList.SetStaticCast(saveSubList);
    loadList.SetStaticCast(loadSubList);
    saveList->AddNonScrollingChild(saveFrame);
    loadList->AddNonScrollingChild(loadFrame);
    saveList->AddNonScrollingChild(saveDummy);
    loadList->AddNonScrollingChild(loadDummy);

    CComponent* scroll = saveFrame->FindChild("SCROLL_LIST_UP");
    if (scroll) {
        manager->AddObserver(scroll);
        manager->AddActionsListenIgnoreOnList(saveList.Get(), scroll, list->FindChild("SCROLL_LIST_UP"), UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = saveFrame->FindChild("SCROLL_DESCRIPTION_UP");
    if (scroll) {
        manager->AddObserver(scroll);
        manager->AddActionsListenIgnoreOnList(saveList.Get(), scroll, list->FindChild("SCROLL_DESCRIPTION_UP"), UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = saveFrame->FindChild("SCROLL_LIST_DOWN");
    if (scroll) {
        manager->AddObserver(scroll);
        manager->AddActionsListenIgnoreOnList(saveList.Get(), scroll, list->FindChild("SCROLL_LIST_DOWN"), UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = saveFrame->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (scroll) {
        manager->AddObserver(scroll);
        manager->AddActionsListenIgnoreOnList(saveList.Get(), scroll, list->FindChild("SCROLL_DESCRIPTION_DOWN"), UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = loadFrame->FindChild("SCROLL_LIST_UP");
    if (scroll) {
        manager->AddObserver(scroll);
        manager->AddActionsListenIgnoreOnList(loadList.Get(), scroll, list->FindChild("SCROLL_LIST_UP"), UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = loadFrame->FindChild("SCROLL_DESCRIPTION_UP");
    if (scroll) {
        manager->AddObserver(scroll);
        manager->AddActionsListenIgnoreOnList(loadList.Get(), scroll, list->FindChild("SCROLL_DESCRIPTION_UP"), UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = loadFrame->FindChild("SCROLL_LIST_DOWN");
    if (scroll) {
        manager->AddObserver(scroll);
        manager->AddActionsListenIgnoreOnList(loadList.Get(), scroll, list->FindChild("SCROLL_LIST_DOWN"), UI_EVENT_MOUSE_MOVEMENT);
    }
    scroll = loadFrame->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (scroll) {
        manager->AddObserver(scroll);
        manager->AddActionsListenIgnoreOnList(loadList.Get(), scroll, list->FindChild("SCROLL_DESCRIPTION_DOWN"), UI_EVENT_MOUSE_MOVEMENT);
    }

    CCountedPointer<CComponent> screenshot(manager->CreateComponent("PC_OPTIONS_MAP_VIEWPORT", false));
    CViewport* screenshotViewport = static_cast<CViewport*>(screenshot.Get());
    saveList->AddNonScrollingChild(screenshot);
    loadList->AddNonScrollingChild(screenshot);
    saveFrame->AddPositionChild(screenshot.Get());
    CCountedPointer<CComponent> savePicture(manager->CreateComponent("PC_RING_PIC_SAVE_FOR_LIST", false));
    saveList->AddNonScrollingChild(savePicture);
    saveFrame->AddPositionChild(savePicture.Get());
    if (savePicture.Get())
        static_cast<CSprite*>(savePicture.Get())->SetGraphicIndex(manager->GetMiscThingsDef()->SaveHeroGraphicIndex);
    CCountedPointer<CComponent> loadPicture(manager->CreateComponent("PC_RING_PIC_LOAD_FOR_LIST", false));
    loadList->AddNonScrollingChild(loadPicture);
    loadFrame->AddPositionChild(loadPicture.Get());
    CCountedPointer<CComponent> saveTitle(manager->CreateComponent("PC_TITLE_SAVE", false));
    CCountedPointer<CComponent> loadTitle(manager->CreateComponent("PC_TITLE_LOAD", false));
    saveList->AddNonScrollingChild(saveTitle);
    loadList->AddNonScrollingChild(loadTitle);
    saveFrame->AddPositionChild(saveTitle.Get());
    loadFrame->AddPositionChild(loadTitle.Get());

    GetFileNameList();
    if (CUserProfileManager::Get().IsDebugProfile())
        saveList->InitialiseOffsets(FileNames.size());
    else
        saveList->InitialiseOffsets(FileNames.size() - 1);
    long usedSlots = 0;
    for (unsigned long j = 0; j < FileNames.size(); ++j) {
        if (FileNames[j] != CUserProfileManager::Get().GetEmptySlotName())
            ++usedSlots;
    }
    loadList->InitialiseOffsets(usedSlots);

    bool noSaveEntries = true;
    for (unsigned long i = 0; i < FileNames.size(); ++i) {
        CCountedPointer<CComponent> fileDescription(manager->CreateComponent("PC_TEXT_FILE_DESCRIPTION", false));
        saveFrame->AddPositionChild(fileDescription.Get());
        CCountedPointer<CComponent> descriptionText(manager->CreateComponent("PC_DESCRIPTION", false));
        saveFrame->AddPositionChild(descriptionText.Get());
        CCountedPointer<CText> text;
        text.SetStaticCast(descriptionText);
        text->SetValue(L"INVALID FILE DESCRIPTION STRING", true, true);
        if (area)
            area->AddFakeChild(descriptionText.Get());
        CCountedPointer<CAction> descriptionAction(new CAction);
        descriptionAction->ActionType = UI_ACTION_LOAD_FILE_DESC;
        descriptionAction->UIActionParams.push_back(new CActionParamSLONG(i));
        descriptionAction->AddParam(new CActionParamCCountedPointer<CComponent>(descriptionText));
        CCountedPointer<CMenuEntry> menuEntry;
        CWideString name(FileNames[i]);
        bool hasSignature = false;
        CWideString signaturePath;
        signaturePath = CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false) + L".hs";
        if (CAFile::PathExists(signaturePath))
            hasSignature = true;
        CCountedPointer<CAction> minimapAction(new CAction);
        minimapAction->ActionType = UI_ACTION_LOAD_FILE_SAVEGAME_MINIMAP;
        CCountedPointer<CComponent> picture;
        if (VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false))
            && (!hasSignature || VerifySignatureCached(signaturePath))) {
            if (name != CUserProfileManager::Get().GetEmptySlotName()) {
                picture.Reset(manager->CreateComponent("PC_RING_PIC_DRAW_FROM_VIEWPORT", false));
                minimapAction->AddParam(new CActionParamEBOOL(true));
                minimapAction->AddParam(new CActionParamCCountedPointer<CComponent>(picture));
                minimapAction->AddParam(new CActionParamSLONG(i));
            }
        }
        if (picture.IsNull()) {
            picture.Reset(manager->CreateComponent("PC_RING_PIC_SAVE_VIEWPORT", false));
            if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens() && picture.Get()) {
                minimapAction->AddParam(new CActionParamEBOOL(false));
                minimapAction->AddParam(new CActionParamCCountedPointer<CComponent>(picture));
                minimapAction->AddParam(new CActionParamSLONG(i));
            }
        }
        screenshotViewport->AddFakeChild(picture.Get());
        saveFrame->AddPositionChild(picture.Get());
        if (!CUserProfileManager::Get().IsDebugProfile() && name != CUserProfileManager::Get().GetEmptySlotName()
            && name != CUserProfileManager::Get().GetAutoSaveFileName())
            name = CUserProfileManager::Get().GetManualSaveFileName(i);

        // the save entry
        if (name != CUserProfileManager::Get().GetAutoSaveFileName()) {
            noSaveEntries = false;
            if (name == L"")
                name = CUserProfileManager::Get().GetEmptySlotName();
            else
                name = CUserProfileManager::Get().GetSaveDisplayNameFromFileName(name, i);
            bool isEmpty = name == CUserProfileManager::Get().GetEmptySlotName();
            CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(name, 0.0f, 1.0f,
                (isEmpty && !CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens())
                    ? "PC_MENU_ENTRY_TEMPLATE" : "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CMenuEntry* saveEntry = static_cast<CMenuEntry*>(entry.Get());
            if (!isEmpty) {
                CCountedPointer<CComponent> subList(manager->CreateComponent("PC_SUB_LIST", false));
                subList->SetName("COMPONENT_SAVE");
                entry->AddChild(subList);
                CList* overwriteList = static_cast<CList*>(subList.Get());
                CCountedPointer<CComponent> overwrite(manager->CreateComponent("PC_UI_MENU_ENTRY_OVERWRITE_SAVE_GAME", false));
                overwriteList->AddScrollingChild(overwrite);
                CText* overwriteDescription = static_cast<CText*>(overwrite->FindChild("PC_OVERWRITE_DESCRIPTION"));
                if (overwriteDescription) {
                    if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens())
                        overwriteDescription->SetValue("TEXT_GUI_MENU_OVERWRITE_DESCRIPTION_HERO", true, true);
                    if (area)
                        area->AddFakeChild(overwriteDescription);
                    saveFrame->AddPositionChild(overwriteDescription);
                }
                CComponent* subtitle = overwrite->FindChild("PC_OVERWRITE_SUBTITLE");
                if (subtitle)
                    saveFrame->AddPositionChild(subtitle);
                CCountedPointer<CComponent> title(manager->CreateComponent("PC_TITLE", false));
                CText* titleText = static_cast<CText*>(title.Get());
                if (titleText) {
                    CWideString titleString = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_SAVE")
                        + manager->GetMiscThingsDef()->ColonSeparator + manager->GetMiscThingsDef()->SpaceSeparator + name;
                    titleText->SetValue(titleString, true, true);
                    overwriteList->AddNonScrollingChild(title);
                    saveFrame->AddPositionChild(title.Get());
                }
                CCountedPointer<CComponent> acceptContainer(manager->CreateComponent("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER_SAVE_LOAD", false));
                overwriteList->AddNonScrollingChild(acceptContainer);
                CCountedPointer<CComponent> backContainer(manager->CreateComponent("PC_OPTIONS_BACK_CONTAINER_SAVE_LOAD", false));
                overwriteList->AddNonScrollingChild(backContainer);
                saveFrame->AddPositionChild(acceptContainer.Get());
                saveFrame->AddPositionChild(backContainer.Get());
                CComponent* acceptParent = subList->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER_SAVE_LOAD");
                CComponent* backParent = subList->FindChild("PC_OPTIONS_BACK_CONTAINER_SAVE_LOAD");
                CComponent* accept = acceptParent->FindChild("PC_OPTIONS_ACCEPT_CHANGES_SAVE_LOAD");
                CComponent* back = backParent->FindChild("PC_OPTIONS_BACK_SAVE_LOAD");
                CCountedPointer<CAction> observeAccept(new CAction);
                observeAccept->ActionType = UI_ACTION_TYPE_OBSERVE_EVENT;
                observeAccept->AddParam(accept, observeAccept);
                observeAccept->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                subList->AddActionOnSelected(observeAccept);
                CCountedPointer<CAction> observeBack(new CAction);
                observeBack->ActionType = UI_ACTION_TYPE_OBSERVE_EVENT;
                observeBack->AddParam(back, observeBack);
                observeBack->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                subList->AddActionOnSelected(observeBack);
                CCountedPointer<CAction> ignoreAccept(new CAction);
                ignoreAccept->ActionType = UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreAccept->AddParam(accept, ignoreAccept);
                ignoreAccept->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                subList->AddActionOnInactive(ignoreAccept);
                CCountedPointer<CAction> ignoreBack(new CAction);
                ignoreBack->ActionType = UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreBack->AddParam(back, ignoreBack);
                ignoreBack->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                subList->AddActionOnInactive(ignoreBack);
                CCountedPointer<CAction> acceptAction(new CAction);
                acceptAction->ActionType = UI_ACTION_TYPE_SAVE;
                accept->AddActionOnLeftUnclicked(acceptAction);
                CCountedPointer<CAction> backAction(new CAction);
                backAction->ActionType = UI_ACTION_TYPE_SEND_EVENT;
                backAction->UIActionParams.push_back(new CActionParamSLONG(UI_EVENT_BACK));
                back->AddActionOnLeftUnclicked(backAction);
            } else if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens()) {
                CCountedPointer<CComponent> subList(manager->CreateComponent("PC_SUB_LIST", false));
                subList->SetName("COMPONENT_SAVE");
                entry->AddChild(subList);
                CList* confirmList = static_cast<CList*>(subList.Get());
                CCountedPointer<CComponent> confirmSave(manager->CreateComponent("PC_UI_MENU_ENTRY_CONFIRM_SAVE_GAME", false));
                confirmList->AddScrollingChild(confirmSave);
                CCountedPointer<CComponent> acceptContainer(manager->CreateComponent("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER_SAVE_LOAD", false));
                confirmList->AddNonScrollingChild(acceptContainer);
                CCountedPointer<CComponent> backContainer(manager->CreateComponent("PC_OPTIONS_BACK_CONTAINER_SAVE_LOAD", false));
                confirmList->AddNonScrollingChild(backContainer);
                loadFrame->AddPositionChild(acceptContainer.Get());
                loadFrame->AddPositionChild(backContainer.Get());
                {
                    CComponent* acceptParent = subList->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER_SAVE_LOAD");
                    CComponent* backParent = subList->FindChild("PC_OPTIONS_BACK_CONTAINER_SAVE_LOAD");
                    CComponent* accept = acceptParent->FindChild("PC_OPTIONS_ACCEPT_CHANGES_SAVE_LOAD");
                    CComponent* back = backParent->FindChild("PC_OPTIONS_BACK_SAVE_LOAD");
                    CCountedPointer<CAction> observeAccept(new CAction);
                    observeAccept->ActionType = UI_ACTION_TYPE_OBSERVE_EVENT;
                    observeAccept->AddParam(accept, observeAccept);
                    observeAccept->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                    subList->AddActionOnSelected(observeAccept);
                    CCountedPointer<CAction> observeBack(new CAction);
                    observeBack->ActionType = UI_ACTION_TYPE_OBSERVE_EVENT;
                    observeBack->AddParam(back, observeBack);
                    observeBack->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                    subList->AddActionOnSelected(observeBack);
                    CCountedPointer<CAction> ignoreAccept(new CAction);
                    ignoreAccept->ActionType = UI_ACTION_TYPE_IGNORE_EVENT;
                    ignoreAccept->AddParam(accept, ignoreAccept);
                    ignoreAccept->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                    subList->AddActionOnInactive(ignoreAccept);
                    CCountedPointer<CAction> ignoreBack(new CAction);
                    ignoreBack->ActionType = UI_ACTION_TYPE_IGNORE_EVENT;
                    ignoreBack->AddParam(back, ignoreBack);
                    ignoreBack->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                    subList->AddActionOnInactive(ignoreBack);
                    CCountedPointer<CAction> acceptAction(new CAction);
                    acceptAction->ActionType = UI_ACTION_TYPE_SAVE;
                    accept->AddActionOnLeftUnclicked(acceptAction);
                    CCountedPointer<CAction> backAction(new CAction);
                    backAction->ActionType = UI_ACTION_TYPE_SEND_EVENT;
                    backAction->UIActionParams.push_back(new CActionParamSLONG(UI_EVENT_BACK));
                    back->AddActionOnLeftUnclicked(backAction);
                }
                CComponent* confirmDescription = confirmSave->FindChild("PC_CONFIRM_DESCRIPTION");
                if (confirmDescription) {
                    if (area)
                        area->AddFakeChild(confirmDescription);
                    loadFrame->AddPositionChild(confirmDescription);
                }
                CComponent* subtitle = confirmSave->FindChild("PC_CONFIRM_SUBTITLE");
                if (subtitle)
                    loadFrame->AddPositionChild(subtitle);
                CCountedPointer<CComponent> title(manager->CreateComponent("PC_TITLE", false));
                CText* titleText = static_cast<CText*>(title.Get());
                if (titleText) {
                    titleText->SetValue("TEXT_GUI_MENU_SAVE", true, true);
                    confirmList->AddNonScrollingChild(title);
                    saveFrame->AddPositionChild(title.Get());
                }
            } else {
                CCountedPointer<CAction> saveAction(new CAction);
                entry->SetName("COMPONENT_SAVE");
                saveAction->ActionType = UI_ACTION_TYPE_SAVE;
                saveEntry->AddUIActionParam(entry.Get());
                entry->AddAction(saveAction);
            }
            saveEntry->AddChild(picture);
            saveEntry->AddActionOnSelected(minimapAction);
            entry->AddChild(fileDescription);
            saveList->AddScrollingChildAndSetOffsets(entry);
            if (name != CUserProfileManager::Get().GetEmptySlotName())
                entry->AddChild(descriptionText);
            menuEntry.SetStaticCast(entry);
            menuEntry->AddUIActionParam(new CActionParamSLONG(i));
            menuEntry->AddActionOnSelected(descriptionAction);
        }

        // the load entry
        name = CUserProfileManager::Get().GetSaveDisplayNameFromFileName(name, i);
        CCountedPointer<CComponent> loadEntry(manager->CreateMenuEntryComponent(name, 0.0f, 1.0f,
            "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        if (!(VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false))
              && (!hasSignature || VerifySignatureCached(signaturePath))))
            loadEntry->MakeUnavailable();
        CMenuEntry* loadMenuEntry = static_cast<CMenuEntry*>(loadEntry.Get());
        loadMenuEntry->AddUIActionParam(loadMenuEntry);
        loadMenuEntry->AddChild(picture);
        loadMenuEntry->AddActionOnSelected(minimapAction);
        // Retail evaluates the slot's signature check again here and never uses the result (as the console twin
        // 0x004830A1 does): `valid` is that unused value.
        bool valid = VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false))
            && (!hasSignature || VerifySignatureCached(signaturePath));
        loadEntry->AddChild(fileDescription);
        if (name != CUserProfileManager::Get().GetEmptySlotName()) {
            loadList->AddScrollingChildAndSetOffsets(loadEntry);
            loadEntry->AddChild(descriptionText);
            if (VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false))
                && (!hasSignature || VerifySignatureCached(signaturePath))) {
                CCountedPointer<CComponent> confirmLoad(manager->CreateComponent("PC_UI_CONFIRM_LOAD_LIST", false));
                CList* confirmLoadList = static_cast<CList*>(confirmLoad.Get());
                CCountedPointer<CComponent> title(manager->CreateComponent("PC_TITLE", false));
                CText* titleText = static_cast<CText*>(title.Get());
                if (titleText)
                    titleText->SetValue(CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_LOAD")
                        + manager->GetMiscThingsDef()->ColonSeparator + manager->GetMiscThingsDef()->SpaceSeparator + name, true, true);
                confirmLoadList->AddNonScrollingChild(title);
                loadFrame->AddPositionChild(title.Get());
                loadEntry->AddChild(confirmLoad);
                CComponent* confirmDescription = confirmLoad->FindChild("PC_LOAD_CONFIRM_DESC");
                if (confirmDescription) {
                    if (area)
                        area->AddFakeChild(confirmDescription);
                    loadFrame->AddPositionChild(confirmDescription);
                }
                CComponent* subtitle = confirmLoad->FindChild("PC_LOAD_CONFIRM_SUB");
                if (subtitle)
                    loadFrame->AddPositionChild(subtitle);
                CComponent* acceptContainer = confirmLoad->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER_SAVE_LOAD");
                CComponent* backContainer = confirmLoad->FindChild("PC_OPTIONS_BACK_CONTAINER_SAVE_LOAD");
                CComponent* accept = acceptContainer->FindChild("PC_OPTIONS_ACCEPT_CHANGES_SAVE_LOAD");
                CComponent* back = backContainer->FindChild("PC_OPTIONS_BACK_SAVE_LOAD");
                CCountedPointer<CAction> observeAccept(new CAction);
                observeAccept->ActionType = UI_ACTION_TYPE_OBSERVE_EVENT;
                observeAccept->AddParam(accept, observeAccept);
                observeAccept->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                confirmLoad->AddActionOnSelected(observeAccept);
                CCountedPointer<CAction> observeBack(new CAction);
                observeBack->ActionType = UI_ACTION_TYPE_OBSERVE_EVENT;
                observeBack->AddParam(back, observeBack);
                observeBack->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                confirmLoad->AddActionOnSelected(observeBack);
                CCountedPointer<CAction> ignoreAccept(new CAction);
                ignoreAccept->ActionType = UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreAccept->AddParam(accept, ignoreAccept);
                ignoreAccept->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                confirmLoad->AddActionOnInactive(ignoreAccept);
                CCountedPointer<CAction> ignoreBack(new CAction);
                ignoreBack->ActionType = UI_ACTION_TYPE_IGNORE_EVENT;
                ignoreBack->AddParam(back, ignoreBack);
                ignoreBack->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT));
                confirmLoad->AddActionOnInactive(ignoreBack);
                CCountedPointer<CAction> acceptAction(new CAction);
                acceptAction->ActionType = UI_ACTION_TYPE_LOAD;
                accept->AddActionOnLeftUnclicked(acceptAction);
                CCountedPointer<CAction> backAction(new CAction);
                backAction->ActionType = UI_ACTION_TYPE_SEND_EVENT;
                backAction->UIActionParams.push_back(new CActionParamSLONG(UI_EVENT_BACK));
                back->AddActionOnLeftUnclicked(backAction);
                loadFrame->AddPositionChild(acceptContainer);
                loadFrame->AddPositionChild(backContainer);
            }
        }
        menuEntry.SetStaticCast(loadEntry);
        menuEntry->AddUIActionParam(new CActionParamSLONG(i));
        menuEntry->AddActionOnSelected(descriptionAction);
    }

    // no saving while fishing
    bool fishing = false;
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    const CTCFishing* fishingTC;
    if (IS_THING_ALIVE(hero) && hero->PeekTC(&fishingTC))
        fishing = fishingTC->IsActive();
    CComponent* saveMenuEntry = list->FindChild("PC_MENU_ENTRY_SAVE");
    if (!noSaveEntries && !fishing)
        saveMenuEntry->AddChild(saveSubList);
    else
        saveMenuEntry->MakeUnavailable();
    list->FindChild("PC_MENU_ENTRY_LOAD")->AddChild(loadSubList);

    // the yes / no dialog, centred
    CComponent* yesNo = manager->CreateComponent("PC_DIALOG_YESNO_GAME", false);
    CComponent* button = yesNo->FindChild("PC_YESNO_GAME_YESBUTTON");
    if (button) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        action->UIActionParams.push_back(new CActionParamSLONG(UI_EVENT_SELECT));
        button->AddActionOnLeftUnclicked(action);
    }
    button = yesNo->FindChild("PC_YESNO_GAME_NOBUTTON");
    if (button) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        action->UIActionParams.push_back(new CActionParamSLONG(UI_EVENT_BACK));
        button->AddActionOnLeftUnclicked(action);
    }
    yesNo->Initialise();
    C2DVector position((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - NUISystem::CManager::GetInstance()->GetUIScale().X * 104.0f,
                       CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f);
    yesNo->ChangePosition(position, 0.0f, false);
    Components.push_back(yesNo);
    PYesNo = static_cast<CYesNo*>(yesNo);
}