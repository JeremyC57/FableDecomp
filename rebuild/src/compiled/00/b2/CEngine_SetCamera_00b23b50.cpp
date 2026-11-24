// CEngine::SetCamera(const CCamera*) -- hand the camera to the engine camera component, process that component once, and remember it as the current camera
// Leaf: ?SetCamera@CEngine@@UAEXPBVCCamera@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23B50, length 39 bytes.
// Callees: CEngineCamera::SetGameCamera(const CCamera*) @ 0x00B2FBF0 and CEngineCamera::Process(bool) @ 0x00B314E0, both on the engine camera
// component pointer @ 0x01436EA0 (re-read for the second call). Layout from engine/CEngine.h (PDB): CurrentCamera +0x14. Shape: the camera stays
// in edi for the push and the final member store.
// Body port (NF321, 2026-11-23): defined on the real class of engine/CEngine.h (FABLE_RETAIL_CENGINE + _OVERRIDES, the preamble of the landed
//   CEngine::OnActivate 0x00B23DE0; was ?SetCamera@CEngine_Methods@@QAEXPAVCCamera@@@Z) with the PDB parameter `const CCamera*`; the retail RTTI
//   vftable placement names this PDB member here. Callees as the donor FableWin.exe CEngine::SetCamera calls them (both PDB name sets),
//   declared by the add-only opt-in FABLE_RETAIL_CENGINECAMERA_SETGAMECAMERA_PROCESS_DECL: ?SetGameCamera@CEngineCamera@@QAEXPBVCCamera@@@Z
//   (was the made-up ?AddExclusiveObserver@CEngineCamera@@QAEXPAVCCamera@@@Z) and ?Process@CEngineCamera@@QAEX_N@Z (unchanged).
//   Ego_r.pdb types the member CurrentCamera as `CCamera*` (non-const), so the PDB `const CCamera*` parameter is stored through a const_cast
//   (both sides PDB types; C++ requires the conversion).
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_SETGAMECAMERA_PROCESS_DECL
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CEngine.h"
#include "engine/CEngineCamera.h"
namespace NEngineInternal { extern CEngineCamera* GPCamera; }   // 0x01436EA0

void CEngine::SetCamera(const CCamera* camera)
{
    NEngineInternal::GPCamera->SetGameCamera(camera);
    NEngineInternal::GPCamera->Process(true);
    CurrentCamera = const_cast<CCamera*>(camera);
}
