// CNewFrontendGameComponent::CNewFrontendGameComponent(CGame& game, const CNewFrontendGameComponentInit& init) -- construct the front-end game component: game-component base, empty counted pointers / vectors / strings, cursor timing from the clock, inputs allowed, three stop watches, a 1/3 s repeat delay and the media-player flag; the main-game init strings are cleared in the body
// Leaf: ??0CNewFrontendGameComponent@@QAE@AAVCGame@@ABVCNewFrontendGameComponentInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042EA8F, 295 bytes, `ret 8` (the init argument is unused here).
// Pragma: size TU (`fldz ; fstp` zero floats), as the destructor 0x0042F50B.
// Names (NF233, 2026-11-22, q12638): the self leaf is the PDB spelling (Ego_r.pdb, FableWin.pdb, Ego_d.pdb: CGame& and a const init
//   reference) that 0x00496CF0 / 0x0049DCC0 / 0x00412F90 bind; was ??0CNewFrontendGameComponent@@QAE@PAVCGame@@PAVCNewFrontendGameComponentInit@@@Z.
//   Base ctor 0x0051D8E0 is ??0CGameComponent@@QAE@AAVCGame@@@Z (Ego_r + FableWin; was PAVCGame). The vtables are the PDB names
//   ??_7CNewFrontendGameComponent@@6BCBaseClassNonCopyable@@@ 0x01230CA0 (was 6BCBase@@@) and ...@@6BCDeviceResetCallback@@@ 0x01230C94 (was
//   6BCGameComponentInterface@@@), every slot on its PDB spelling (see 0x0042F50B). L"" is the literal 0x0122D70C (was a local _g_emptyWide).
// Header port: engine/CNewFrontendGameComponent.h (RUN + FILENAMES_FILETIMES + the NF233 CTOR_DTOR opt-in), engine/CStopWatch.h (NF233 CTOR:
//   ??0CStopWatch@@QAE@XZ 0x0062F740), engine/CEngineInterpolationInfo.h (NF233 CENGINEINTERPOLATIONINFOSET_CTOR: m_LastInterp's inline
//   ctor, Ego_d / FableWin ??0CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAE@XZ, the three `fldz` stores). Every local model of
//   HEAD is gone (CBase, the CGameComponentInterface stand-in, CGameComponent, CDefPointer, CWideString, CMainGameComponentInit, the
//   CRetailVector / allocator-proxy stand-ins, CDateAndTime, C2DBoxI, CEngineInterpolationInfoSet, CStopWatch, the class itself).
// Callees: CGameComponent::CGameComponent(CGame&) 0x0051D8E0, GFGetTime 0x009E1BC0, CMainGameComponentInit() 0x004131A0,
//   CWideString(const wchar_t*) 0x0099B6B0 and operator=(const wchar_t*) 0x0099B800 (L"" 0x0122D70C), CStopWatch() 0x0062F740.
// Members: InitialWorldName (+0x2C) and SaveGameName (+0x38) are the two CMainGameComponentInit strings the body clears (HEAD: StartLevel /
//   StartPosition). FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE is not defined here: the ctor needs no ~CGameComponent, and with it the object
//   would emit CGameComponent's vtable and a size-mode ??_GCGameComponent (41 B) that differs from its owner 0x00415940 (43 B, speed).
// Pragma scaffolding: `#pragma optimize("", on)` after the body restores the TU default before the pick-any copies (as 0x0042F50B).
// Naming (NF234, 2026-11-22, q12646 / q12649): m_LastInterp's class is CEngineInterpolationInfo::CEngineInterpolationInfoSet (nested in all
//   three PDBs), so the inline set ctor's pick-any copy is the PDB ??0CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAE@XZ (was
//   ??0CEngineInterpolationInfoSet@@QAE@XZ): engine/CEngineInterpolationInfo.h FABLE_RETAIL_CENGINEINTERPOLATIONINFO_NESTED_SET.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CSTOPWATCH_CTOR
#define FABLE_RETAIL_CENGINEINTERPOLATIONINFOSET_CTOR
#define FABLE_RETAIL_CENGINEINTERPOLATIONINFO_NESTED_SET
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT
#include "engine/CGameComponent.h"
#include "engine/CStopWatch.h"
#include "engine/CEngineInterpolationInfo.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/GFGetTime.h"

CNewFrontendGameComponent::CNewFrontendGameComponent(CGame& game, const CNewFrontendGameComponentInit& init)
    : CGameComponent(game), CurrentMovement(NGameComponentHelpers::ECursorMovement()), StartedRepeatingMovement(false),
      TimeLastMovementHappened(GFGetTime()), m_bAllowInputs(true), m_bCreateGame(false),
      SaveGame(L""), PAVITextureShow(0), PAVITextureDecode(0), AVIPlaying(false), XMVCodeLoaded(false),
      m_pFrontEndManager(0), m_fLastRealUpdateTime(GFGetTime()), m_LastInterp(), m_fRepeatDelay(0.333f), m_dwDPadTriggerState(0),
      LastActiveJoystickDeviceNumber(0), WindowsMediaPlayerInstalled(true)
{
    m_bInitMainGame.InitialWorldName = L"";
    m_bInitMainGame.SaveGameName = L"";
    CreatedMarkerFile = false;
    CreateDevFrontEnd = false;
}
#pragma optimize("", on)
