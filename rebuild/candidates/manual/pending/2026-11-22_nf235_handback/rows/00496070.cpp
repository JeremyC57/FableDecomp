// CFrontendGameComponent::CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit& init) -- construct the debug front end: game-component base, empty counted pointers / name lists / strings, the movement timer, then (with the default-profile flag) load the default profile and enter mode 1, else enter the profile list mode 12; reset the create flags, load the graphics and rebuild the key-to-number table
// Leaf: ??0CFrontendGameComponent
// Names (NF233, 2026-11-22, q12638, in place): the self signature is the PDB one, ??0CFrontendGameComponent@@QAE@AAVCGame@@ABVCFrontendGameComponentInit@@@Z
//   (Ego_r.pdb + FableWin.pdb), which its binders 0x0042EC7C / 0x0049DCC0 bind (and 0x00412F90 with this hand-back); was (CMainGameComponentParent*,
//   long unused). The init reference is unused, as the long was (`ret 8` either way). The base ctor 0x0051D8E0 is bound as
//   ??0CGameComponent@@QAE@AAVCGame@@@Z (was ...@@PAUCMainGameComponentParent@@@Z). Local vtable stand-ins unchanged (NAME_FIXES).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00496070, length 592 bytes, `ret 8` (the second argument is unused).
// Callees: CGameComponent::CGameComponent(CGame&) @ 0x0051D8E0, CWideString::CWideString() @ 0x0099AED0
//   (manifest "CCharString" -- byte-identical body), GFGetTime @ 0x009E1BC0, CTBaseSingleton<CUserProfileManager>::Get
//   @ 0x0040D2A0, CUserProfileManager::GetDefaultProfileName @ 0x004065D0, SetProfileName(const CWideString&, bool)
//   @ 0x0040D400, ~CWideString @ 0x0099B510, the mode-10 preparation @ 0x00495890 (manifest "CBrushLibraryDialog::
//   FillInListBox" is propagated), the profile-name list refresh @ 0x004959E0 (`ecx` = the vector), InitGraphics @ 0x00494840.
// Globals: the default-profile flag @ 0x013B8644, the key-to-number table @ 0x013B87B0 (122 longs; DIK scan codes of the top-row
//   digits and the numeric keypad map to 0..9, DIK_A to 11 and DIK_B to 12).
// Layout: the landed ~CFrontendGameComponent 0x00495620 (retail = Ego_r.pdb less 4 per STLport vector); the body fields from
//   the PDB: CreateGame +0x10, CreateGameJoystickDeviceNumber +0x14, CreateEditor +0x18, CreateReleaseFrontEnd +0x19, Mode +0x1c,
//   NoConnections +0x20, NoSessions +0x24, String +0x168, StringPos +0x1e8, First/LastSessionString +0x1df0/+0x1df4,
//   NoSessionStrings +0x1df8.
// Shape: SetMode (out of line @ 0x00495FE0) is inlined twice and folds per constant mode; no /EHsc (no unwind frame).
//   The profile load goes through a static inline (name, load) helper that fetches the singleton itself: retail builds the
//   GetDefaultProfileName() temporary BEFORE pushing `true` and calls Get() for SetProfileName after the pushes (a direct
//   `Get().SetProfileName(Get().GetDefaultProfileName(), true)` pushes `true` first; a member wrapper calls Get() early).
//   The name vectors use the real STLport _Vector_base / _STLP_alloc_proxy shape (the "24i" lesson) so the CWideString
//   ctor's `lea ecx` lands after two of the three zero stores. The key table is written keypad first, then the top row.
// Header port (NF235, 2026-11-22, q12656): the local CBase / CGameComponentInterface / CGameComponent / CWideString / CTexture /
//   CTBaseSingleton / CUserProfileManager / CFrontendGameComponent models replaced by the shared headers: engine/CGameComponent.h,
//   fable_string.h, engine/CTexture.h (FABLE_RETAIL_CTEXTURE: the inline default ctor), engine/CUserProfileManager.h (new
//   DEFAULTPROFILENAME opt-in), engine/GFGetTime.h and engine/CFrontendGameComponent.h (CTOR_DTOR: the typed layout and the private
//   members; SETQUIT: the slot-4 override; GETPROFILENAMES / GETSAVENAMES). The two vtables now carry their PDB / RTTI names
//   ??_7CFrontendGameComponent@@6BCBaseClassNonCopyable@@@ (0x01238768) / ??_7CFrontendGameComponent@@6BCDeviceResetCallback@@@
//   (0x0123875C) and their retail slots. The two helpers retail expands are the real private members, defined inline here as the
//   ctor's TU-mates (retail keeps their out-of-line copies 0x00494410 / 0x00495FE0 beside the ctor): ?LoadProfile@CFrontendGameComponent@@
//   AAEXABVCWideString@@_N@Z (Ego_r + FableWin; the singleton is fetched inside it, which is the push order retail shows) and
//   ?SetMode@CFrontendGameComponent@@AAEXW4EGameMode@@@Z (Ego_r + FableWin; the body of the landed owner 0x00495FE0, verbatim).
#include <string.h>
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_DEFAULTPROFILENAME
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_GETPROFILENAMES
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_GETSAVENAMES
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTexture.h"
#include "engine/GFGetTime.h"
#include "engine/CUserProfileManager.h"
#include "engine/CGameComponent.h"
#include "engine/CFrontendGameComponent.h"

extern bool g_bLoadDefaultProfile;                         // 0x013B8644
extern long g_KeyToNumber[122];                            // 0x013B87B0

inline void CFrontendGameComponent::LoadProfile(const CWideString& profileName, bool loadProfile)
{
    CUserProfileManager::Get().SetProfileName(profileName, loadProfile);
}

inline void CFrontendGameComponent::SetMode(EGameMode mode)
{
    Mode = mode;
    CursorHighlight = 1;
    memset(String, 0, sizeof(String));
    StringPos = 0;
    if (Mode == 10) {
        GetWorldNames();
        return;
    }
    if (mode == 11) {
        GetSaveNames(SaveNames);
        WorldToLoad = L"";
        SaveNamesOffset = 0;
        return;
    }
    if (mode == 12) {
        GetProfileNames(ProfileNames);
        ProfileNamesOffset = 0;
    }
}

CFrontendGameComponent::CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit& init)
    : CGameComponent(game), CursorHighlight(1), TimeLastMovementHappened(GFGetTime()), StartedRepeatingMovement(false),
      CurrentMovement(NGameComponentHelpers::ECursorMovement())
{
    Mode = EGameMode();
    Quit = false;
    NoConnections = 0;
    NoSessions = 0;
    FirstSessionString = 0;
    LastSessionString = 0;
    NoSessionStrings = 0;
    if (g_bLoadDefaultProfile) {
        LoadProfile(CUserProfileManager::Get().GetDefaultProfileName(), true);
        SetMode(EGameMode(1));
    } else {
        SetMode(EGameMode(12));
    }
    Quit = false;
    CreateGame = false;
    CreateGameJoystickDeviceNumber = 0;
    CreateEditor = false;
    CreateReleaseFrontEnd = false;
    InitGraphics();
    memset(g_KeyToNumber, 0, sizeof(g_KeyToNumber));
    g_KeyToNumber[82] = 0;
    g_KeyToNumber[79] = 1;
    g_KeyToNumber[80] = 2;
    g_KeyToNumber[81] = 3;
    g_KeyToNumber[75] = 4;
    g_KeyToNumber[76] = 5;
    g_KeyToNumber[77] = 6;
    g_KeyToNumber[71] = 7;
    g_KeyToNumber[72] = 8;
    g_KeyToNumber[73] = 9;
    g_KeyToNumber[11] = 0;
    g_KeyToNumber[2] = 1;
    g_KeyToNumber[3] = 2;
    g_KeyToNumber[4] = 3;
    g_KeyToNumber[5] = 4;
    g_KeyToNumber[6] = 5;
    g_KeyToNumber[7] = 6;
    g_KeyToNumber[8] = 7;
    g_KeyToNumber[9] = 8;
    g_KeyToNumber[10] = 9;
    g_KeyToNumber[30] = 11;
    g_KeyToNumber[48] = 12;
}