// CTCInGameMenu::OpenInGameMenu() -- open the (console-style) pause menu: stop rumble, fade the sound groups, freeze 3D rendering,
// (row 0x004830A1, 7896 bytes) -- RELOCATION_MATCH.
//   book the pause start, close the player GUI, build UI_LIST_PAUSE_MENU (descriptions, quest-time save texts, help sprites, the
//   change-state action of the top-level entries), the save and load sub-lists with one entry per save slot (description,
//   screenshot picture, overwrite / confirm sub-menus, load-confirm sub-menu), then grey out the save entry while fishing or when
//   there is no slot, initialise the menu, put it in state 5 and keep it in Components.
// Leaf: ?OpenInGameMenu@CTCInGameMenu@@QAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x004830A1, 7896 bytes, plain `ret`; frame-pointer size TU (`push ebp ; lea ebp,[esp-78h] ;
//   sub esp,230h`): `#pragma optimize("s", on)` ALONE (adding ("y",off) as the same-TU siblings do gives `mov ebp,esp` here).
// Callees: NUISystem::CManager::OnOpenMenus 0x0041D122 / GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D /
//   CreateMenuEntryComponent 0x00428494; map<CWideString,bool> tree clear 0x00485AEF; CTCBase::GetPlayerManager 0x00686D40;
//   CPlayerManager::GetMainPlayerJoystickDeviceNumber 0x00449980 / GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0;
//   IS_THING_ALIVE 0x0043DBD0; bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x004365B0 (inline PeekTC); GFGetTime 0x009E1BC0;
//   CPlayerGui::Close 0x00438400; CQuestManager::IsBetweenQuestStartAndFinishScreens 0x004AF290; CUserProfileManager::Get 0x0040D2A0
//   / IsDebugProfile 0x00407030 / GetEmptySlotName 0x00406A80 / GetAutoSaveFileName 0x00406690 / GetManualSaveFileName 0x00406610 /
//   GetPathNameFromFileName 0x00408120 / GetSaveDisplayNameFromFileName 0x00407E10; CAFile::PathExists 0x00999230;
//   CTCInGameMenu::GetFileNameList 0x0047FE83 / VerifySignatureCached 0x0047ED0A; NLocalisation::GetUIGraphics 0x00415640;
//   GetTextBySymbol 0x009C95E0; CAction() 0x0042BE50 / AddParam 0x0053FDB0; CCountedPointer ctors 0x0042AA29 / 0x00429C15, Reset
//   0x00429C6F, ShareData 0x00429486 / 0x00429460 / 0x0048524C, ReleaseData 0x004291DE / 0x00429160 / 0x00429288 / 0x0042925C /
//   0x004850F9; list<CActionParam*>::push_back 0x0042B646; CActionParamCCountedPointer<CComponent> 0x004854DF; CDeletion()
//   0x0042BED4 / copy 0x0042CD84 / list dtor 0x0042ABCA; vector<CComponent*>::push_back 0x00486024; CCharString / CWideString.
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

using namespace NUISystem;

extern CInputManager* GPInputManager;                   // 0x013B8388 ?GPInputManager@@3PAVCInputManager@@A

namespace NLocalisation {
void __fastcall GetUIGraphics(CDefPointer<const CUILocaleGraphicsDef>& graphics);   // 0x00415640
}

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

void CTCInGameMenu::OpenInGameMenu()
{
    NUISystem::CManager::OnOpenMenus();
    Signatures.clear();
    GPInputManager->GetJoystick(GetPlayerManager().GetMainPlayerJoystickDeviceNumber()).ClearAllRumbles();
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
    }
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
    CurStartPauseTime = GFGetTime();
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Close(true);
    InGameMenuOpen = true;

    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CComponent* menu = manager->CreateComponent("UI_LIST_PAUSE_MENU", false);
    CViewport* area = static_cast<CViewport*>(menu->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    area->AddFakeChild(menu->FindChild("UI_RETURN_TO_GAME_DESC"));
    area->AddFakeChild(menu->FindChild("UI_SAVE_DESC"));
    area->AddFakeChild(menu->FindChild("UI_LOAD_DESC"));
    area->AddFakeChild(menu->FindChild("UI_XLIVE_DESC"));
    area->AddFakeChild(menu->FindChild("UI_OPTIONS_DESC"));
    area->AddFakeChild(menu->FindChild("UI_AUDIO_OPTIONS_DESC"));
    area->AddFakeChild(menu->FindChild("UI_HELP_DESC"));
    area->AddFakeChild(menu->FindChild("UI_QUIT_DESC"));
    area->AddFakeChild(menu->FindChild("UI_QUIT_CONFIRM_DESC"));
    CComponent* description = menu->FindChild("UI_SIGN_IN_DESC");
    if (description)
        area->AddFakeChild(description);
    description = menu->FindChild("UI_FRIENDS_DESC");
    if (description)
        area->AddFakeChild(description);
    description = menu->FindChild("UI_APPEAR_OFFLINE_DESC");
    if (description)
        area->AddFakeChild(description);
    manager->SetPlayerThing(ParentThing);

    if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens()) {
        CText* text = static_cast<CText*>(menu->FindChild("UI_SAVE_TEXT"));
        if (text)
            text->SetValue("TEXT_GUI_MENU_SAVE_HERO", true, true);
        text = static_cast<CText*>(menu->FindChild("UI_SAVE_SUB"));
        if (text)
            text->SetValue("TEXT_GUI_MENU_SAVE_HERO", true, true);
        text = static_cast<CText*>(menu->FindChild("UI_SAVE_DESC"));
        if (text)
            text->SetValue("TEXT_GUI_MENUGUIDE_PAUSEMENU_SAVE_HERO", true, true);
        CSprite* picture = static_cast<CSprite*>(menu->FindChild("UI_RING_PIC_SAVE"));
        if (picture)
            picture->SetGraphicIndex(manager->GetMiscThingsDef()->SaveHeroGraphicIndex);
    }

    CComponent* topLevel = menu->FindChild("UI_TOP_LEVEL_PAUSE_SCREEN");
    CComponent* options = menu->FindChild("UI_MENU_ENTRY_OPTIONS");
    CComponent* audioOptions = menu->FindChild("UI_MENU_ENTRY_AUDIO_OPTIONS");
    // Retail looks the save entry up here and drops the result: CCharString("UI_MENU_ENTRY_SAVE") (push 0x012373EC, 0x00483516 /
    // 0x0048351E), FindChild (vtable slot 3, `call [edi+0Ch]` 0x00483525), then eax is never stored before the UI_MENU_ENTRY_HELP
    // lookup (`mov edi,[esi]` 0x00483528 .. 0x00483534). The entry is fetched again at the end (saveMenuEntry).
    menu->FindChild("UI_MENU_ENTRY_SAVE");
    CComponent* help = menu->FindChild("UI_MENU_ENTRY_HELP");
    CCountedPointer<CAction> action(new CAction);
    action->ActionType = UI_ACTION_TYPE_CHANGESTATE;
    action->UIActionParams.push_back(topLevel);
    action->UIActionParams.push_back(new CActionParamState(0));
    if (help) {
        CDefPointer<const CUILocaleGraphicsDef> graphics;
        NLocalisation::GetUIGraphics(graphics);
        if (!graphics.IsNull()) {
            CSprite* sprite = static_cast<CSprite*>(help->FindChild("UI_RING_PIC_HELP"));
            if (sprite)
                sprite->SetGraphicIndex(graphics->HelpRingPic);
            sprite = static_cast<CSprite*>(help->FindChild("UI_SPRITE_HELP1"));
            if (sprite)
                sprite->SetGraphicIndex(graphics->HelpScreenGraphics[0]);
            sprite = static_cast<CSprite*>(help->FindChild("UI_SPRITE_HELP2"));
            if (sprite)
                sprite->SetGraphicIndex(graphics->HelpScreenGraphics[1]);
            sprite = static_cast<CSprite*>(help->FindChild("UI_SPRITE_HELP3"));
            if (sprite)
                sprite->SetGraphicIndex(graphics->HelpScreenGraphics[2]);
            sprite = static_cast<CSprite*>(help->FindChild("UI_SPRITE_HELP4"));
            if (sprite)
                sprite->SetGraphicIndex(graphics->HelpScreenGraphics[3]);
            sprite = static_cast<CSprite*>(help->FindChild("UI_SPRITE_HELP5"));
            if (sprite)
                sprite->SetGraphicIndex(graphics->HelpScreenGraphics[4]);
            sprite = static_cast<CSprite*>(help->FindChild("UI_SPRITE_HELP6"));
            if (sprite)
                sprite->SetGraphicIndex(graphics->HelpScreenGraphics[5]);
        }
    }
    options->AddAction(action);
    audioOptions->AddAction(action);
    if (help)
        help->AddAction(action);

    // the save and load sub-lists
    CCountedPointer<CComponent> saveSubList(manager->CreateComponent("UI_SUB_LIST", false));
    CCountedPointer<CList> saveList;
    CCountedPointer<CComponent> loadSubList(manager->CreateComponent("UI_SUB_LIST", false));
    CCountedPointer<CList> loadList;
    saveList.SetStaticCast(saveSubList);
    loadList.SetStaticCast(loadSubList);
    CCountedPointer<CComponent> screenshot(manager->CreateComponent("UI_SCREENSHOT_VIEWPORT", false));
    CViewport* screenshotViewport = static_cast<CViewport*>(screenshot.Get());
    saveList->AddNonScrollingChild(screenshot);
    loadList->AddNonScrollingChild(screenshot);
    CCountedPointer<CComponent> savePicture(manager->CreateComponent("UI_RING_PIC_SAVE_FOR_LIST", false));
    saveList->AddNonScrollingChild(savePicture);
    if (savePicture.Get())
        static_cast<CSprite*>(savePicture.Get())->SetGraphicIndex(manager->GetMiscThingsDef()->SaveHeroGraphicIndex);
    CCountedPointer<CComponent> loadPicture(manager->CreateComponent("UI_RING_PIC_LOAD_FOR_LIST", false));
    loadList->AddNonScrollingChild(loadPicture);
    CCountedPointer<CComponent> back(manager->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<CComponent> select(manager->CreateComponent("UI_SELECT_FOR_LIST", false));
    saveList->AddNonScrollingChild(back);
    saveList->AddNonScrollingChild(select);
    loadList->AddNonScrollingChild(back);
    loadList->AddNonScrollingChild(select);
    CCountedPointer<CComponent> saveTitle(manager->CreateComponent("UI_TITLE_SAVE_NEW", false));
    CCountedPointer<CComponent> loadTitle(manager->CreateComponent("UI_TITLE_LOAD_NEW", false));
    saveList->AddNonScrollingChild(saveTitle);
    loadList->AddNonScrollingChild(loadTitle);

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
        CCountedPointer<CComponent> fileDescription(manager->CreateComponent("UI_TEXT_FILE_DESCRIPTION", false));
        CCountedPointer<CComponent> descriptionText(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
        CCountedPointer<CText> text;
        text.SetStaticCast(descriptionText);
        text->SetValue(L"INVALID FILE DESCRIPTION STRING", true, true);
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
                picture.Reset(manager->CreateComponent("UI_RING_PIC_DRAW_FROM_VIEWPORT", false));
                minimapAction->AddParam(new CActionParamEBOOL(true));
                minimapAction->AddParam(new CActionParamCCountedPointer<CComponent>(picture));
                minimapAction->AddParam(new CActionParamSLONG(i));
            }
        }
        if (picture.IsNull()) {
            picture.Reset(manager->CreateComponent("UI_RING_PIC_SAVE_VIEWPORT", false));
            if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens() && picture.Get()) {
                minimapAction->AddParam(new CActionParamEBOOL(false));
                minimapAction->AddParam(new CActionParamCCountedPointer<CComponent>(picture));
                minimapAction->AddParam(new CActionParamSLONG(i));
            }
        }
        screenshotViewport->AddFakeChild(picture.Get());
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
                    ? "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW" : "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CMenuEntry* saveEntry = static_cast<CMenuEntry*>(entry.Get());
            if (!isEmpty) {
                CCountedPointer<CComponent> subList(manager->CreateComponent("UI_SUB_LIST", false));
                subList->SetName("COMPONENT_SAVE");
                entry->AddChild(subList);
                CList* overwriteList = static_cast<CList*>(subList.Get());
                CCountedPointer<CComponent> confirm(manager->CreateComponent("UI_CONFIRM_FOR_LIST", false));
                if (confirm.Get())
                    overwriteList->AddNonScrollingChild(confirm);
                CCountedPointer<CComponent> cancel(manager->CreateComponent("UI_BACK_FOR_LIST", false));
                if (cancel.Get())
                    overwriteList->AddNonScrollingChild(cancel);
                CCountedPointer<CComponent> overwrite(manager->CreateComponent("UI_MENU_ENTRY_OVERWRITE_SAVE_GAME", false));
                overwriteList->AddScrollingChild(overwrite);
                CText* overwriteDescription = static_cast<CText*>(overwrite->FindChild("UI_OVERWRITE_DESCRIPTION"));
                if (overwriteDescription) {
                    if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens())
                        overwriteDescription->SetValue("TEXT_GUI_MENU_OVERWRITE_DESCRIPTION_HERO", true, true);
                    area->AddFakeChild(overwriteDescription);
                }
                CCountedPointer<CComponent> title(manager->CreateComponent("UI_TITLE_NEW", false));
                CText* titleText = static_cast<CText*>(title.Get());
                if (titleText) {
                    CWideString titleString = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_SAVE")
                        + manager->GetMiscThingsDef()->ColonSeparator + manager->GetMiscThingsDef()->SpaceSeparator + name;
                    titleText->SetValue(titleString, true, true);
                    overwriteList->AddNonScrollingChild(title);
                }
            } else if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsBetweenQuestStartAndFinishScreens()) {
                CCountedPointer<CComponent> subList(manager->CreateComponent("UI_SUB_LIST", false));
                subList->SetName("COMPONENT_SAVE");
                entry->AddChild(subList);
                CList* confirmList = static_cast<CList*>(subList.Get());
                CCountedPointer<CComponent> confirm(manager->CreateComponent("UI_CONFIRM_FOR_LIST", false));
                if (confirm.Get())
                    confirmList->AddNonScrollingChild(confirm);
                CCountedPointer<CComponent> cancel(manager->CreateComponent("UI_BACK_FOR_LIST", false));
                if (cancel.Get())
                    confirmList->AddNonScrollingChild(cancel);
                CCountedPointer<CComponent> confirmSave(manager->CreateComponent("UI_MENU_ENTRY_CONFIRM_SAVE_GAME", false));
                confirmList->AddScrollingChild(confirmSave);
                CComponent* confirmDescription = confirmSave->FindChild("UI_CONFIRM_DESCRIPTION");
                if (confirmDescription)
                    area->AddFakeChild(confirmDescription);
                CCountedPointer<CComponent> title(manager->CreateComponent("UI_TITLE_NEW", false));
                CText* titleText = static_cast<CText*>(title.Get());
                if (titleText) {
                    titleText->SetValue("TEXT_GUI_MENU_SAVE", true, true);
                    confirmList->AddNonScrollingChild(title);
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
            "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        if (!(VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false))
              && (!hasSignature || VerifySignatureCached(signaturePath)))) {
            loadEntry->MakeUnavailable();
            CComponent* arrow = loadEntry->FindChild("UI_SELECTION_ARROW_RIGHT");
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            if (arrow)
                arrow->SetDeletion(deletion);
        }
        CMenuEntry* loadMenuEntry = static_cast<CMenuEntry*>(loadEntry.Get());
        loadMenuEntry->AddUIActionParam(loadMenuEntry);
        loadMenuEntry->AddChild(picture);
        loadMenuEntry->AddActionOnSelected(minimapAction);
        // Retail evaluates the slot's signature check a third time here and never uses the result: CUserProfileManager::Get
        // 0x0040D2A0, GetPathNameFromFileName 0x00408120, VerifySignatureCached 0x0047ED0A, then `cmp byte ptr [ebp+5Bh],bl` (hasSignature)
        // and the second VerifySignatureCached on signaturePath 0x00484A33, with no store (0x00484A10..0x00484A38): eight calls to
        // 0x0047ED0A in all, as here. `valid` is that unused value.
        bool valid = VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false))
            && (!hasSignature || VerifySignatureCached(signaturePath));
        loadEntry->AddChild(fileDescription);
        if (name != CUserProfileManager::Get().GetEmptySlotName()) {
            loadList->AddScrollingChildAndSetOffsets(loadEntry);
            loadEntry->AddChild(descriptionText);
            if (VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false))
                && (!hasSignature || VerifySignatureCached(signaturePath))) {
                CCountedPointer<CComponent> confirmLoad(manager->CreateComponent("UI_CONFIRM_LOAD_LIST", false));
                CList* confirmLoadList = static_cast<CList*>(confirmLoad.Get());
                CCountedPointer<CComponent> title(manager->CreateComponent("UI_TITLE_NEW", false));
                CText* titleText = static_cast<CText*>(title.Get());
                if (titleText)
                    titleText->SetValue(CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_LOAD")
                        + manager->GetMiscThingsDef()->ColonSeparator + manager->GetMiscThingsDef()->SpaceSeparator + name, true, true);
                confirmLoadList->AddNonScrollingChild(title);
                loadEntry->AddChild(confirmLoad);
                CComponent* confirmDescription = confirmLoad->FindChild("UI_LOAD_CONFIRM_DESC");
                if (confirmDescription)
                    area->AddFakeChild(confirmDescription);
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
    CComponent* saveMenuEntry = menu->FindChild("UI_MENU_ENTRY_SAVE");
    if (!noSaveEntries && !fishing) {
        saveMenuEntry->AddChild(saveSubList);
    } else {
        CComponent* arrow = saveMenuEntry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
        saveMenuEntry->MakeUnavailable();
    }
    menu->FindChild("UI_MENU_ENTRY_LOAD")->AddChild(loadSubList);
    menu->Initialise();
    menu->ChangeState(5);
    Components.push_back(menu);
}