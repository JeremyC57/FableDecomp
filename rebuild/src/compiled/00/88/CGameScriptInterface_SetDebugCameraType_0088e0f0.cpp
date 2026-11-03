// CGameScriptInterface::SetDebugCameraType(long) const -- script call: pass the debug camera type to the world's current game camera
// Leaf: ?SetDebugCameraType@CGameScriptInterface@@UBEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E0F0, 11 bytes: `mov eax,[ecx+4] ; mov ecx,[eax+34h] ; mov edx,[ecx] ;
//   jmp [edx+54h]` (a virtual tail call that reuses the caller's stack argument).
// Identity: retail CGameScriptInterface vtable 0x01260F0C slot 4 (engine/CGameScriptInterface.h; Ego_r.pdb / FableWin.pdb
//   ?SetDebugCameraType@CGameScriptInterface@@UBEXJ@Z).
// Callee: CAIGameCameraBase slot 21 (+0x54) = FableWin.pdb ?SetDebugCameraType@CAIGameCameraBase@@UAEXJ@Z (body 0x00697160);
//   the camera is CWorld::PCurrentGameCamera (+0x34) read through FableWin.pdb's inline
//   ?GetGameCamera@CWorld@@QAEPAVCAIGameCameraBase@@XZ (Ego_r expands it; added to the FABLE_RETAIL_CWORLD branch).
// Layout: engine/CWorld.h (FABLE_RETAIL_CWORLD), engine/CAIGameCameraBase.h (FABLE_RETAIL_CAIGAMECAMERABASE, chunk 718: the
//   25-slot retail vtable 0x0125C1F4), engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT, World +0x04).
#define FABLE_RETAIL_CAIGAMECAMERABASE
#include "engine/CAIGameCameraBase.h"
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetDebugCameraType(long type) const
{
    World->GetGameCamera()->SetDebugCameraType(type);
}