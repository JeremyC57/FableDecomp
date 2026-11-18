// NUISystem::CFrontEndManager::RefreshAvailableProfiles() -- rebuild the profile menu's list: a "new profile" button and one button per saved profile; returns the profile count
// Retail 0x00596CCE, 824 bytes, plain `ret` (Ego_r.pdb ?RefreshAvailableProfiles@CFrontEndManager@NUISystem@@IAEJXZ).
// Body: profiles = GetProfileNames() (0x005955AB, local vector<CWideString>, dtor 0x00412130); on the profile menu screen
// (m_mapScreens[7], map::operator[] 0x0059B5D7) the old "UI_FRONTEND_LIST_FOR_PROFILES" child (FindChild, slot 3, by-value
// CCharString) gets SActionParamPair{1} through SetActionParameter (slot 62; pair ctor 0x0042BED4, copy 0x0042CD84, list
// dtor 0x0042ABCA); a fresh list of that definition is created (CManager::GetInstance 0x0041E5F2 -> CreateComponent
// 0x0041DB1D), viewed as a CList (CCountedPointer ShareData 0x0059AA86, dtor 0x0059A994) and added to the menu (AddChild,
// slot 59). A "new profile" button (text CWideString::CreateFromCharString("TEXT_GUI_MENU_NEW_PROFILE") 0x0099B6A0, the
// PDB's static __fastcall widening of the tag itself -- not the JustGetTheFrikkingText2 0x0057AD4C text lookup;
// CreateFrontEndButtonComponent 0x00596A68 with "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST" at y 0)
// carries a CAction of type 0x125 (operator new 0x00BFEA1A, CAction 0x0042BE50, CCountedPointer<CAction> 0x0042AA29 / dtor
// 0x00429160) as its action and left-click action (slots 66 / 71) and is added to the list (AddChildDynamically, slot
// 60). Each profile then gets a button at y = 50 + 30 * i whose action is 0x124 when the profile's save passes the
// signature check (VerifySignatureCached 0x005957D9 of CTBaseSingleton<CUserProfileManager>::Get() 0x0040D2A0
// .GetProfileDir(name, false) 0x004082D0) and 0xDB otherwise, with the profile name as a CActionParamString parameter
// (0x0054DAE0; UIActionParams push_back 0x0042B646). Finally the menu's slot 43 re-initialises it.
// Literals: 0x01252428, 0x0125240C, 0x012523E4, "" 0x0122D70E; 30.0f 0x01248018.
// Opt: size TU with a frame pointer (`optimize("s")` + `("y",off)`), as the landed CFrontEndManager rows.
// Shape: GetProfileNames 0x005955AB is compiled in this TU in retail and called out of line: its body is visible here
//   (engine/CFrontEndManager.h FABLE_RETAIL_CFRONTENDMANAGER_GETPROFILENAMES_BODY), with the STLport push_back / _M_insert_overflow /
//   erase(first, last) bodies it calls visible but kept out of line (PUSHBACK_NOINLINE, OVERFLOW_OUTOFLINE, ERASE_RANGE_NOINLINE). VC7.1
//   then knows the local vector's address does not escape, so the profile loop is strength-reduced onto an element pointer and a down
//   counter and the frame is retail's 0x64 (NF66's declaration-only port: DIFFER 838v824, frame 0x5C). Local names from Ego_d.pdb;
//   `fPosition` is declared before `count` for retail's store order.
// Leaf: ?RefreshAvailableProfiles@CFrontEndManager@NUISystem@@IAEJXZ
// NF73 (2026-11-17): protected per Ego_r.pdb / FableWin.pdb (IAEJXZ; the landed row was public QAEJXZ, its callers 0x00597B20 /
//   0x0059A238 go to NAME_FIXES.tsv); the local models replaced by the shared headers (NF51 / NF66 drafts).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes; likewise 0x00429160 = ReleaseData<CAction>, dtor jmp 0x00429B72). The local model's destructor is `{ ReleaseData(); }`
//   (engine/CCountedPointer.h FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY shape).
// NF51 (2026-11-17): the local CCharString / CWideString / vector / map / CCountedPointer / SActionParamPair / CUserProfileManager /
//   CTBaseSingleton / NUISystem::CActionParamString / CAction / CComponent / CList / CManager / CFrontEndManager models and the three
//   same-TU stand-ins replaced by the shared headers of the landed twin RefreshAvailableSavedGamesForProfile 0x00598463
//   (engine/CFrontEndManager.h FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR); the 8-byte "SActionParamPair" passed to slot 62 is
//   the NUISystem::CDeletion {Method, AssociatedParents} of SetDeletion (ctors 0x0042BED4 / 0x0042CD84, FableWin.pdb).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_CFRONTENDMANAGER_GETPROFILENAMES_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_SAVEDGAMES
#define FABLE_RETAIL_CFRONTENDMANAGER_BUILDCOMPONENTS
#define FABLE_RETAIL_CFRONTENDMANAGER_REFRESHAVAILABLEPROFILES
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_GETPROFILEDIR
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamString.h"
#include "engine/NUISystem_CList.h"
#include "engine/CUserProfileManager.h"

namespace NUISystem {

long CFrontEndManager::RefreshAvailableProfiles()
{
    std::vector<CWideString> vProfileNames;
    GetProfileNames(vProfileNames);
    CManager* pManager = CManager::GetInstance();
    CComponent* pProfileScreen = m_mapScreens[FRONTEND_SCREEN_PROFILES_MENU];
    CComponent* plist = pProfileScreen->FindChild("UI_FRONTEND_LIST_FOR_PROFILES");
    if (plist) {
        CDeletion deletion;
        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        plist->SetDeletion(deletion);
    }
    CCountedPointer<CComponent> pProfilesList(pManager->CreateComponent("UI_FRONTEND_LIST_FOR_PROFILES", false));
    CCountedPointer<CList> pProfileListReal;
    pProfileListReal.SetStaticCast(pProfilesList);
    pProfileScreen->AddChild(pProfilesList);

    CWideString newProfileText = CWideString::CreateFromCharString("TEXT_GUI_MENU_NEW_PROFILE");
    CCountedPointer<CComponent> pNewProfile(
        CreateFrontEndButtonComponent(newProfileText, 0.0f, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", true));
    CCountedPointer<CAction> pActionCreateNewProfile(new CAction);
    pActionCreateNewProfile->ActionType = UI_ACTION_TYPE_NEW_PROFILE;
    pNewProfile->AddAction(pActionCreateNewProfile);
    pNewProfile->AddActionOnLeftClicked(pActionCreateNewProfile);
    pProfileListReal->AddChildDynamically(pNewProfile);

    float fPosition = 50.0f;
    long count = vProfileNames.size();
    for (long i = 0; i != count; ++i) {
        CCountedPointer<CComponent> pProfile(
            CreateFrontEndButtonComponent(vProfileNames[i], fPosition, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", false));
        CCountedPointer<CAction> pActionSavedGame(new CAction);
        bool valid = VerifySignatureCached(CUserProfileManager::Get().GetProfileDir(vProfileNames[i], false));
        pActionSavedGame->ActionType = valid ? UI_ACTION_TYPE_GO_TO_MAIN_MENU_FROM_PROFILE_LIST : UI_ACTION_TYPE_NEXT_SCREEN_INVALID_PROFILE;
        pActionSavedGame->UIActionParams.push_back(new CActionParamString(vProfileNames[i]));
        pProfile->AddAction(pActionSavedGame);
        pProfile->AddActionOnLeftClicked(pActionSavedGame);
        pProfileListReal->AddChildDynamically(pProfile);
        fPosition += 30.0f;
    }
    pProfileScreen->Initialise();
    return count;
}
}
