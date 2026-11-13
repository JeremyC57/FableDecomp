// NUISystem::CFrontEndManager::RefreshAvailableSavedGamesForProfile() -- rebuild the profile's saved-games screen: title "Load game" with the profile name, a fresh save list plus the text-area and screenshot viewports, then one button per save (skipping the empty slot) whose action loads the game when its signature (and its ".hs" header's) verifies
// Leaf: ?RefreshAvailableSavedGamesForProfile@CFrontEndManager@NUISystem@@IAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00598463, 1335 bytes, thiscall, plain `ret`, returns 0. Literals "TEXT_GUI_MENU_LOAD_GAME"
//   0x012524CC, "UI_FRONTEND_LIST_FOR_SAVES" 0x012524B0, "UI_SCROLLING_VIEWPORT_TEXT_AREA" 0x01237550, "UI_SCREENSHOT_VIEWPORT"
//   0x0123734C, "UI_BUTTON_FOR_SAVE_NAME" 0x01252498, "UI_FRONTEND_BUTTON_FOR_SAVE_LIST" 0x01252474, L".hs" 0x01236A74; 30.0f 0x01248018.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?RefreshAvailableSavedGamesForProfile@CFrontEndManager@NUISystem@@IAEJXZ (protected),
//   Ego_r donor order GetSavedGameNames (retail 0x00597DF7, called first here) -> this -> LoadProfile (retail 0x0059899A, right
//   after this row's `ret`); sole caller 0x0059A5B4 (CFrontEndManager::Action).
// Callees (reloc_bindings.tsv / PDB spellings): GetSavedGameNames 0x00597DF7, CManager::GetInstance 0x0041E5F2 / CreateComponent
//   0x0041DB1D, map<FRONTEND_SCREEN, CComponent*>::operator[] 0x0059B5D7, vector<CCountedPointer<CComponent> > copy 0x0059AE54 / ~
//   0x00535900, CCountedPointer<CComponent> copy 0x00428928 / (T*) 0x00429C15 / ReleaseData 0x004291DE, CCountedPointer<CText>
//   ShareData 0x00429460 / ReleaseData 0x0042925C, CCountedPointer<CAction> (T*) 0x0042AA29 / ReleaseData 0x00429160,
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0, NGameText::ConstructString 0x009C7980, CUserProfileManager::Get 0x0040D2A0 /
//   IsDebugProfile 0x00407030 / GetEmptySlotName 0x00406A80 / GetSaveDisplayNameFromFileName 0x00407E10 / GetPathNameFromFileName
//   0x00408120, CWideString () 0x0099AED0 / ~ 0x0099B510 / ToCharString 0x0099B2C0 / CreateFromCharString 0x0099B6A0 / operator=
//   0x0099B7D0 / operator+(const CWideString&, const unsigned short*) 0x0099BF30, STLport basic_string<unsigned short> operator== 0x0040F0A0
//   (inline CWideString::operator==), CDeletion () 0x0042BED4 / copy 0x0042CD84 (list dtor 0x0042ABCA), CreateComponent 0x00594F94,
//   CreateFrontEndButtonComponent 0x00596A68, VerifySignatureCached 0x005957D9, ConstructFileDescription 0x00595CC1, CAFile::PathExists
//   0x00999230, CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80 / ~ 0x00401630, CAction 0x0042BE50,
//   CActionParamString 0x0054DAE0, list<CActionParam*>::push_back 0x0042B646, operator new 0x00BFEA1A; CComponent slots 3 FindChild,
//   43 Initialise, 53 GetChildren, 59 AddChild, 60 AddChildDynamically, 62 SetDeletion, 66 AddAction, 71 AddActionOnLeftClicked;
//   CText slot 144 SetValue(const CWideString&, bool, bool).
// Opt: size TU (`optimize("s", on)` gives the biased `lea ebp,[esp-78h]` frame), as the landed AddRegionAndTimeInfo 0x00597228.
// Shape (each measured against retail, chunk 2003): shared headers only. FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ: the size TU expands
//   CWideString::operator== in place (0x0059866F..0x0059868E: data pointers, null tests, cdecl basic_string operator== 0x0040F0A0)
//   where plain inline is called out of line. FABLE_RETAIL_STLP_MAP: map::operator[]'s visible STLport body (still called out of
//   line) tells VC7.1 the FRONTEND_SCREEN key temporary is only read, so its word [ebp+68h] is shared with old_list and the
//   file-name string (frame 0xCC; declaration-only 0xD0). The empty-slot skip is one `!IsDebugProfile() && name == GetEmptySlotName()`
//   test (the GetEmptySlotName temporary is the conditional-temporary flag word [ebp+60h]).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_SAVEDGAMES
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamString.h"
#include "engine/CText.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CUserProfileManager.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/NGameText_CDataBank.h"

namespace NUISystem {
long CFrontEndManager::RefreshAvailableSavedGamesForProfile()
{
    std::vector<CWideString> save_names;
    NumberSaveGames = 0;
    GetSavedGameNames(save_names);
    CManager* manager = CManager::GetInstance();
    CComponent* menu = m_mapScreens[FRONTEND_SCREEN_PROFILE_SAVED_GAMES_MENU];
    std::vector<CCountedPointer<CComponent> > children = menu->GetChildren();
    CCountedPointer<CComponent> title = children[0];
    CCountedPointer<CText> title_text;
    title_text.SetStaticCast(title);
    CWideString title_string = CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_LOAD_GAME");
    NGameText::ConstructString(title_string, CUserProfileManager::Get().GetProfileName());
    title_text->SetValue(title_string, true, true);
    CComponent* old_list = menu->FindChild("UI_FRONTEND_LIST_FOR_SAVES");
    if (old_list) {
        CDeletion deletion;
        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        old_list->SetDeletion(deletion);
    }
    CCountedPointer<CComponent> list(manager->CreateComponent("UI_FRONTEND_LIST_FOR_SAVES", false));
    menu->AddChild(list);
    CCountedPointer<CComponent> text_area(CreateComponent("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    menu->AddChild(text_area);
    CCountedPointer<CComponent> screenshot(CreateComponent("UI_SCREENSHOT_VIEWPORT"));
    menu->AddChild(screenshot);
    float y = 0.0f;
    for (unsigned long i = 0; i < save_names.size(); ++i) {
        if (!CUserProfileManager::Get().IsDebugProfile() && save_names[i] == CUserProfileManager::Get().GetEmptySlotName())
            continue;
        CWideString display_name;
        CCharString file_name = save_names[i].ToCharString();
        display_name = CWideString::CreateFromCharString(file_name);
        display_name = CUserProfileManager::Get().GetSaveDisplayNameFromFileName(display_name, i);
        CCountedPointer<CComponent> button(CreateFrontEndButtonComponent(display_name, y, "UI_FRONTEND_BUTTON_FOR_SAVE_LIST", "UI_BUTTON_FOR_SAVE_NAME", true));
        CCountedPointer<CAction> action(new CAction);
        bool save_valid = VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(save_names[i], false));
        if (save_valid) {
            CDiskFileWin32 file(CUserProfileManager::Get().GetPathNameFromFileName(save_names[i], false), FILE_ACCESS_READ, 2);
        }
        CWideString header_name = CUserProfileManager::Get().GetPathNameFromFileName(save_names[i], false) + L".hs";
        bool header_exists = false;
        bool header_valid = VerifySignatureCached(header_name);
        if (CAFile::PathExists(header_name)) {
            header_exists = true;
            if (header_valid) {
                CDiskFileWin32 header_file(header_name, FILE_ACCESS_READ, 2);
            }
            NumberSaveGames++;
        }
        if (save_valid && (!header_exists || header_valid))
            action->ActionType = UI_ACTION_TYPE_LOAD_GAME;
        else
            action->ActionType = UI_ACTION_TYPE_NEXT_SCREEN_INVALID_SAVE;
        action->UIActionParams.push_back(new CActionParamString(save_names[i]));
        button->AddAction(action);
        button->AddActionOnLeftClicked(action);
        list->AddChildDynamically(button);
        y += 30.0f;
        ConstructFileDescription(i, button.Get(), menu, save_names[i]);
        NumberSaveGames++;
    }
    menu->Initialise();
    return 0;
}
}
