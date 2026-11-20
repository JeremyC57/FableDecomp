// CEngineCamera::SetGameWindow(const C2DBoxF* window) -- remember the game window box the camera renders into
// Leaf: ?SetGameWindow@CEngineCamera@@QAEXPBVC2DBoxF@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2FC00, 11 bytes (`mov eax,[esp+4] ; mov [ecx+10h],eax ; ret 4`).
// Naming (NF172, 2026-11-20, q4582): Ego_r.pdb 0x009D0510 + FableWin.pdb 0x02DC15E0 ?SetGameWindow@CEngineCamera@@QAEXPBVC2DBoxF@@@Z;
//   the body stores the argument at +0x10 = Ego_r CEngineCamera::GameWindow (const C2DBoxF*, engine/CEngineCamera.h POSITION mode, NF172
//   opt-in FABLE_RETAIL_CENGINECAMERA_SETGAMEWINDOW); its one landed caller CEngine::SetWindow 0x00B23B80 tail-jumps here on the engine
//   camera 0x01436EA0 and already binds this spelling. Was the placeholder C_00b2fc00::M (file AddExclusiveObserver_..., a label of the
//   neighbouring 0x00B2FBF0; catalog _global::SetMember_dword0x10_00b2fc00).
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_SETGAMEWINDOW
#define FABLE_RETAIL_CMATRIX3X4
#include "engine/C2DVector.h"
#include "engine/CEngineCamera.h"

void CEngineCamera::SetGameWindow(const C2DBoxF* window)
{
    GameWindow = window;
}
