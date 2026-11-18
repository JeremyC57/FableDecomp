// CFrontEndManager::Update(float dt) -- per-frame front-end update: unpause the file installer two seconds after a pause, pop the virtual-keyboard screen once it finishes, act on a finished profile-name entry (load an existing profile, create a new one, or go to the profile screen), update every screen component, and retire finished fade / message / load-game components
// Leaf: ?Update@CFrontEndManager@NUISystem@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00599E3F, 1017 bytes, ebp frame, `ret 4`.
// NF76 port (2026-11-17): self is Ego_r.pdb 0x0060680C ?Update@CFrontEndManager@NUISystem@@QAEXM@Z (queue q10441; the landed self was the
//   namespace-less ?Update@CFrontEndManager@@QAEXM@Z on a local model). On engine/CFrontEndManager.h's full-layout branch with the
//   0x00596CCE / 0x00597006 macro block (GetProfileNames 0x005955AB visible-noinline, the STLport push_back / overflow / erase bodies
//   visible-noinline), plus my overlay opt-ins: CFRONTENDMANAGER_UPDATE (also in this branch) and _VIRTUALKEYBOARD (LaunchVirtualKeyboard,
//   GotoNextScreen, CreateNewProfile), STLP_STACK_TOP_POP (stack top/pop over the out-of-line deque back 0x0059B039 / pop_back 0x0059AEE5),
//   CTBASESINGLETON_ISALLOCATED / _GET_DECL (CTBaseSingleton<CFileInstaller>), CVIRTUALKEYBOARD_INLINE_STATE (IsDone / WasCanceled),
//   CNEWFRONTENDGAMECOMPONENT_STARTGAME (StartGame inline, LoadGame). The made-up EnableInput is the header's
//   ?CloseVirtualKeyboard@CFrontEndManager@NUISystem@@QAEX_N@Z 0x00594FA9 (queue q10440); the component virtuals are the header's
//   ChangedState (slot 49), GetColour (slot 14), ChangeState (slot 48), Update (slot 1), CObserver::ObserveAllEvents; the UI manager's
//   AddObserver / GetMiscThingsDef / PlaySoundW (CManager vtable 0x01230134 slots 2 / 42 / 40) with CUIMiscThingsDef SoundForward / SoundBack.
//   Locals as Ego_d.pdb names them (fTime, cur_time, time_elapsed, time_for_installation, iter, previousProfile, strProfile,
//   profile_names, it, profileCreated).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_CFRONTENDMANAGER_GETPROFILENAMES_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_SAVEDGAMES
#define FABLE_RETAIL_CFRONTENDMANAGER_BUILDCOMPONENTS
#define FABLE_RETAIL_CFRONTENDMANAGER_REFRESHAVAILABLEPROFILES
#define FABLE_RETAIL_CFRONTENDMANAGER_UPDATE
#define FABLE_RETAIL_CFRONTENDMANAGER_VIRTUALKEYBOARD
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_GETPROFILEDIR
#define FABLE_RETAIL_CVIRTUALKEYBOARD
#define FABLE_RETAIL_CVIRTUALKEYBOARD_INLINE_STATE
#define FABLE_RETAIL_CTBASESINGLETON_ISALLOCATED
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_STARTGAME
#include <wchar.h>
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "stlport/fable_deque.h"
#include "fable_string.h"
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CDefPointer.h"
#include "engine/CRGBColour.h"
#include "engine/CUserProfileManager.h"
#include "engine/CVirtualKeyboard.h"
#include "engine/CTBaseSingleton.h"
#include "engine/CFileInstaller.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/GFGetTime.h"

namespace NUISystem {
void CFrontEndManager::Update(float fTime)
{
    double cur_time = GFGetTime();
    double time_elapsed = cur_time - InstallationPauseStartTime;
    double time_for_installation = 2.0;
    std::map<FRONTEND_SCREEN, CComponent*>::iterator iter;
    if (time_elapsed >= time_for_installation && InstallationPaused && CTBaseSingleton<CFileInstaller>::IsAllocated()) {
        CTBaseSingleton<CFileInstaller>::Get().Unpause();
        InstallationPaused = false;
    }
    if (GoToKeyboard && m_stackScreens.top()) {
        if (m_stackScreens.top()->ChangedState() || !m_stackScreens.top()->GetColour().A) {
            GoToKeyboard = false;
            LaunchVirtualKeyboard();
            if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_PROFILE_ALREADY_EXISTS_MENU])
                m_stackScreens.pop();
        }
    }
    if (m_pVirtualKeyboard && m_pVirtualKeyboard->IsDone()) {
        if (!m_pVirtualKeyboard->WasCanceled()) {
            CWideString previousProfile(CUserProfileManager::Get().GetProfileName());
            CWideString strProfile = m_pVirtualKeyboard->GetProfileName();
            std::vector<CWideString> profile_names;
            GetProfileNames(profile_names);
            std::vector<CWideString>::iterator it;
            for (it = profile_names.begin(); it != profile_names.end(); ++it)
                if (_wcsicmp((*it).GetAsCharArray(), strProfile.GetAsCharArray()) == 0)
                    break;
            if (it != profile_names.end()) {
                CloseVirtualKeyboard(false);
                LoadProfile(previousProfile);
                GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_PROFILE_ALREADY_EXISTS_MENU], false);
                CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
            } else {
                bool profileCreated = CreateNewProfile(strProfile);
                CloseVirtualKeyboard(false);
                if (profileCreated)
                    GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_MAIN_MENU], true);
                else {
                    LoadProfile(previousProfile);
                    GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_PROFILE_ERROR_PC], false);
                }
            }
        } else {
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1);
            CloseVirtualKeyboard(true);
        }
    }
    for (iter = m_mapScreens.begin(); iter != m_mapScreens.end(); ++iter)
        if ((*iter).second)
            (*iter).second->Update(fTime);
    if (PScreenToDeactivate && (PScreenToDeactivate->ChangedState() || !PScreenToDeactivate->GetColour().A)) {
        PScreenToActivate->ChangeState(5);
        CManager::GetInstance()->AddObserver(PScreenToActivate);
        PScreenToActivate->ObserveAllEvents();
        PScreenToDeactivate = 0;
        PScreenToActivate = 0;
    }
    if (PScreenToDeactivateForStartGame && (PScreenToDeactivateForStartGame->ChangedState() || !PScreenToDeactivateForStartGame->GetColour().A)
        && m_bStartNewGame) {
        PScreenToDeactivateForStartGame = 0;
        if (InstallationPaused && CTBaseSingleton<CFileInstaller>::IsAllocated()) {
            CTBaseSingleton<CFileInstaller>::Get().Unpause();
            InstallationPaused = false;
            FinalInstallationNeverPause = true;
        }
        PGameComponent->StartGame();
    }
    if (PScreenToDeactivateForLoadGame && (PScreenToDeactivateForLoadGame->ChangedState() || !PScreenToDeactivateForLoadGame->GetColour().A)) {
        PScreenToDeactivateForLoadGame = 0;
        if (InstallationPaused && CTBaseSingleton<CFileInstaller>::IsAllocated()) {
            CTBaseSingleton<CFileInstaller>::Get().Unpause();
            InstallationPaused = false;
            FinalInstallationNeverPause = true;
        }
        PGameComponent->LoadGame(GameToLoad);
    }
}
}
