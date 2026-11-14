// NUISystem::CFrontEndManager::BuildComponents(bool) -- build the front end's screens: the media-player error screen first when asked (its text = the error text, the error-code text and the player's error string), then every front-end screen component keyed by FRONTEND_SCREEN, the press-start click action, the about text, the resolution and anti-aliasing slider entries, the redefine-keys accept button, initialise every screen, grey out sliders with no range, append " Hz" to the refresh rates and push the first screen
// Leaf: ?BuildComponents@CFrontEndManager@NUISystem@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00598A1C, 4928 bytes (code 0x00598A1C..0x00599D17 + the anti-aliasing switch's 17-entry jump table
//   0x00599D18..0x00599D5B), thiscall, `ret 4`, biased ebp frame, no EH frame. Next function 0x00599D5C DoPressStart (landed).
// Identity: catalog label "CFrontEndManager::Init2" is wrong. FableWin.pdb ?BuildComponents@CFrontEndManager@NUISystem@@QAEX_N@Z (the only
//   CFrontEndManager member taking one bool besides two tiny setters; FableWin optimised copy 0x00605219) and the sole caller 0x0042EA62 in the
//   landed CNewFrontendGameComponent::Init2 0x0042E98F (`BuildComponents(!WindowsMediaPlayerInstalled)`). No Ego_r.pdb copy; the parameter
//   name `showMediaPlayerError` is descriptive.
// Twin: the matched CTCInventory::OpenPCOptionsMenu 0x0058AF10 (resolution / anti-aliasing / detail-slider / text-slider / refresh-rate blocks
//   copied in its spelling; same `#pragma optimize("s", on)`, no flags). Shape: the redefine-keys accept button and list are two named locals
//   (accept first, then the list: with the list named, the accept pointer lives in a frame word as in retail), and the first screen is `if (!m_bQuitFromGame) { if / else }
//   else { ... }` (three InitScreenStack calls cross-jumped into one tail, the quit arm laid out last).
// Callees: GetProfileNames 0x005955AB, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / copy 0x0099EC30,
//   map<FRONTEND_SCREEN, CComponent*>::operator[] 0x0059B5D7 (same TU, body visible: FABLE_RETAIL_STLP_MAP), NUISystem::CManager::GetInstance
//   0x0041E5F2 / CreateComponent 0x0041DB1D, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, the CWideString operators 0x0099BFF0 / BF30 /
//   BE70 / += 0x0099B8D0 / = 0x0099B7D0 / = (const wchar_t*) 0x0099B800 / () 0x0099AED0 / ~ 0x0099B510, GFIntToWideString 0x00405F00,
//   operator new 0x00BFEA1A, CAction() 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29 / ReleaseData 0x00429160,
//   CCountedPointer<CComponent>(CComponent*) 0x00429C15 / ReleaseData 0x004291DE, CUserProfileManager::Get 0x0040D2A0 / GetDisplayModes
//   0x00406880, GFGetSystemManager 0x009A4EC0, CDisplayManager::EnumerateAntiAliasingModes 0x009C05C0, GenerateAboutMessage 0x00594FDB,
//   CRedefinerList::SetAcceptButton 0x00556570, _Rb_global<bool>::_M_increment 0x004292C0, vector<CCharString>::push_back 0x0044BFF0 /
//   erase 0x0043336A / ~vector 0x00414EA0, list<CComponent*>() 0x0042ABAF / ~list 0x0042ABCA, ~vector<CWideString> 0x00412130,
//   LoadProfile 0x0059899A, InitScreenStack 0x0059672A, free 0x00BFEA14.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CREDEFINERLIST_SETACCEPTBUTTON
#define FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_ANTIALIASING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_BUILDCOMPONENTS
#include "stlport/fable_containers.h"
#include "engine/CDisplayManager.h"
#include "engine/CUserProfileManager.h"
#include "engine/CSystemManager.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CSlider.h"
#include "engine/NUISystem_CTextSlider.h"
#include "engine/NUISystem_CRedefinerList.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CAction.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "engine/CFrontEndManager.h"

using NUISystem::CComponent;
using NUISystem::CText;
using NUISystem::CSlider;
using NUISystem::CTextSlider;
using NUISystem::CRedefinerList;
using NUISystem::CUIState;
using NUISystem::CAction;

CWideString __fastcall GFIntToWideString(long value);               // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

typedef std::map<unsigned long, CUIState, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CUIState> > > CUIStateMap;

void NUISystem::CFrontEndManager::BuildComponents(bool showMediaPlayerError)
{
    std::vector<CWideString> profileNames;
    GetProfileNames(profileNames);

    if (showMediaPlayerError) {
        m_mapScreens[FRONTEND_SCREEN_MEDIA_PLAYER_ERROR] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_MEDIA_PLAYER_ERROR", false);
        CComponent* text = m_mapScreens[FRONTEND_SCREEN_MEDIA_PLAYER_ERROR]->FindChild("UI_MEDIA_PLAYER_ERROR_TEXT");
        if (text) {
            CWideString message = CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_VIDEO_PLAYER_ERROR");
            message += L"\n\n" + CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_VIDEO_PLAYER_ERROR_CODE") + L" " + MediaPlayerErrorString;
            static_cast<CText*>(text)->SetValue(message, true, true);
        }
    }

    m_mapScreens[FRONTEND_SCREEN_MAIN_MENU] = 0;
    m_mapScreens[FRONTEND_SCREEN_PRESS_START_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_PRESS_START_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_PROFILES_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_PROFILES_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_DELETE_PROFILE_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_DELETE_PROFILE_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_PROFILE_SAVED_GAMES_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_PROFILE_SAVED_GAMES_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_OPTIONS_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_OPTIONS_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_AUDIO_OPTIONS_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_AUDIO_OPTIONS_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_LIVE_AWARE_MENU] = 0;
    m_mapScreens[FRONTEND_SCREEN_EXTRAS_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_EXTRAS_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_CREDITS_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_CREDITS_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_PROFILE_ALREADY_EXISTS_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_PROFILE_ALREADY_EXISTS_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_NO_PROFILES_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_NO_PROFILES_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_INVALID_PROFILE_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_INVALID_PROFILE_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_INVALID_SAVE_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_INVALID_SAVE_MENU", false);

    CCountedPointer<CAction> action(new CAction);
    action->ActionType = NUISystem::UI_ACTION_TYPE_GO_TO_MAIN_MENU_FROM_START;
    m_mapScreens[FRONTEND_SCREEN_PRESS_START_MENU]->AddActionOnLeftClicked(action);

    m_mapScreens[FRONTEND_SCREEN_PROFILES_FOR_DELETE_PC] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_SCREEN_PROFILES_FOR_DELETE_PC", false);
    m_mapScreens[FRONTEND_SCREEN_REDEFINE_KEYS_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_SCREEN_REDEFINE_KEYS_PC", false);
    m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_SCREEN_VIDEO_OPTIONS_PC", false);
    m_mapScreens[FRONTEND_SCREEN_NEW_PROFILE_PC] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_NEW_PROFILE_SCREEN", false);
    m_mapScreens[FRONTEND_SCREEN_OPTIONS_SUB_MENU] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_OPTIONS_SUB_MENU", false);
    m_mapScreens[FRONTEND_SCREEN_OPTIONS_SCOREBOARD] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_OPTIONS_SCOREBOARD", false);
    m_mapScreens[FRONTEND_SCREEN_PROFILE_ERROR_PC] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_PROFILE_ERROR_PC", false);
    m_mapScreens[FRONTEND_SCREEN_QUIT] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_QUIT_PROMPT", false);
    m_mapScreens[FRONTEND_SCREEN_ABOUT] = NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_ABOUT_MENU", false);

    CComponent* about = m_mapScreens[FRONTEND_SCREEN_ABOUT]->FindChild("UI_FRONTEND_ABOUT_MESSAGE");
    if (about)
        static_cast<CText*>(about)->SetValue(GenerateAboutMessage(), false, true);

    // the resolution text slider: one "<width>X<height>X<bits>" entry per display mode
    CComponent* resolutionSlider = m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild("UI_OPTIONS_RESOLUTION_TEXT_SLIDER");
    const std::vector<CDisplayModeDesc>& displayModes = CUserProfileManager::Get().GetDisplayModes();
    for (std::vector<CDisplayModeDesc>::const_iterator mode = displayModes.begin(); mode != displayModes.end(); ++mode) {
        CCountedPointer<CComponent> entryPointer(NUISystem::CManager::GetInstance()->CreateComponent("UI_OPTIONS_TEXT_SLIDER_ITEM_TEMPLATE", false));
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
    CComponent* antialiasingSlider = m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild("UI_OPTIONS_ANTIALIASING_TEXT_SLIDER");
    CArray<ESurfaceMultisampleType> antialiasingModes;
    GFGetSystemManager()->GetDisplayManager()->EnumerateAntiAliasingModes(antialiasingModes);
    for (CArray<ESurfaceMultisampleType>::iterator level = antialiasingModes.begin(); level != antialiasingModes.end(); ++level) {
        CCountedPointer<CComponent> entryPointer(NUISystem::CManager::GetInstance()->CreateComponent("UI_OPTIONS_TEXT_SLIDER_ITEM_TEMPLATE", false));
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

    // the redefine-keys list's accept button
    CComponent* accept = m_mapScreens[FRONTEND_SCREEN_REDEFINE_KEYS_MENU]->FindChild("UI_ACCEPT");
    CRedefinerList* redefiner = static_cast<CRedefinerList*>(m_mapScreens[FRONTEND_SCREEN_REDEFINE_KEYS_MENU]->FindChild("UI_FRONTEND_LIST_REDEFINE_KEYS_MENU"));
    redefiner->SetAcceptButton(accept);

    for (std::map<FRONTEND_SCREEN, CComponent*>::iterator screen = m_mapScreens.begin(); screen != m_mapScreens.end(); ++screen)
        if (screen->second)
            screen->second->Initialise();

    // a detail slider with no range: its arrows stop listening and fade to half alpha, its text becomes unavailable
    std::vector<CCharString> names;
    names.push_back("UI_SLIDER_TEXTURE_DETAIL");
    names.push_back("UI_SLIDER_MESH_DETAIL");
    names.push_back("UI_SLIDER_EFFECTS_DETAIL");
    names.push_back("UI_SLIDER_SHADOW_DETAIL");
    for (std::vector<CCharString>::iterator name = names.begin(); name != names.end(); ++name) {
        CSlider* slider = static_cast<CSlider*>(m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild(*name));
        if (slider && slider->Min.X == slider->Max.X && slider->GetParent() && slider->GetParent()->GetParent()) {
            CComponent* whole = slider->GetParent()->GetParent();
            CComponent* arrow = whole->FindChild("UI_OPTIONS_LEFT_ARROW");
            if (arrow) {
                arrow->SetPreventObserving(true);
                for (CUIStateMap::iterator state = arrow->GetStates().begin(); state != arrow->GetStates().end(); ++state)
                    state->second.Colour.A /= 2;
                CRGBColour colour = arrow->GetColour();
                colour.A /= 2;
                arrow->SetColour(colour);
            }
            arrow = whole->FindChild("UI_OPTIONS_RIGHT_ARROW");
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
    names.push_back("UI_OPTIONS_RESOLUTION_TEXT_SLIDER");
    names.push_back("UI_OPTIONS_REFRESH_RATE_TEXT_SLIDER");
    names.push_back("UI_OPTIONS_ANTIALIASING_TEXT_SLIDER");
    for (std::vector<CCharString>::iterator name = names.begin(); name != names.end(); ++name) {
        CTextSlider* slider = static_cast<CTextSlider*>(m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild(*name));
        if (slider && slider->MaxChild == 0 && slider->GetParent()) {
            CComponent* whole = slider->GetParent();
            CComponent* arrow = whole->FindChild("UI_OPTIONS_LEFT_ARROW_TEXT");
            if (arrow) {
                arrow->SetPreventObserving(true);
                for (CUIStateMap::iterator state = arrow->GetStates().begin(); state != arrow->GetStates().end(); ++state)
                    state->second.Colour.A /= 2;
                CRGBColour colour = arrow->GetColour();
                colour.A /= 2;
                arrow->SetColour(colour);
            }
            arrow = whole->FindChild("UI_OPTIONS_RIGHT_ARROW_TEXT");
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
    CComponent* refreshSlider = m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild("UI_OPTIONS_REFRESH_RATE_TEXT_SLIDER");
    std::list<CComponent*> rates;
    refreshSlider->FindAllChildrenOfType(NUISystem::UI_TYPE_TEXT, rates);
    for (std::list<CComponent*>::iterator rate = rates.begin(); rate != rates.end(); ++rate) {
        CText* rateText = static_cast<CText*>(*rate);
        if (rateText) {
            CCharString symbol("TEXT_GUI_MENU_REFRESH_RATE_HZ");
            rateText->SetValue(rateText->GetValue() + L" " + CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol(symbol), true, true);
        }
    }

    if (!m_bQuitFromGame) {
        if (showMediaPlayerError)
            InitScreenStack(m_mapScreens[FRONTEND_SCREEN_MEDIA_PLAYER_ERROR]);
        else
            InitScreenStack(m_mapScreens[FRONTEND_SCREEN_PRESS_START_MENU]);
    } else {
        LoadProfile(CUserProfileManager::Get().GetProfileName());
        InitScreenStack(m_mapScreens[FRONTEND_SCREEN_MAIN_MENU]);
    }
}
