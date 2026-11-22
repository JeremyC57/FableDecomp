// CGamePlayerInterface::Update() -- start a new input frame: clear HasCalledInputUpdateThisFrame (+0x894)
// Leaf: ?Update@CGamePlayerInterface@@QAEXXZ
// Retail: Fable.exe @ 0x004457F0, 8 bytes (`mov byte ptr [ecx+894h],0 ; ret`); one caller, CMainGameComponent::GetInputs 0x00416FB3,
//   once per frame before it polls GetGameEvent.
// Name: Ego_r.pdb / FableWin.pdb ?Update@CGamePlayerInterface@@QAEXXZ (public, void()). Ego_r keeps it out of line directly before
//   GetButtonPressureForEvent and GetMouseMovement (0x00442C50 / 0x00442C60 / 0x00442D40), retail's order here (0x004457F0 / 0x00445800 /
//   0x004458F0); FableWin.pdb's /Od body is 21 bytes, the size of one constant byte store through `this`. The class's other public void()
//   members do not fit: ReapplyControlScheme is 29 bytes at /Od and calls SetControlScheme, ResumeAllInputProcesses (FableWin only) clears
//   Suspended (+0x79C). Was the made-up free `CXMVPlaybackInfo(CXMVPlaybackInfoObj*)` (catalog) / `CGamePlayerInterface::BeginEventPoll`
//   (0x00416E78's binding).
// Layout: engine/CGamePlayerInterface.h (its FABLE_RETAIL_EXPRESSION_FRAME class: Ego_r.pdb members, HasCalledInputUpdateThisFrame +0x894).
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDISKFILEWIN32
class CEnginePrimitivePrepareInfo;                     // CInputProcessManager::PreparePrimitivesForEngine's parameter
#include "engine/CGamePlayerInterface.h"

void CGamePlayerInterface::Update()
{
    HasCalledInputUpdateThisFrame = false;
}
