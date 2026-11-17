// CEngine::PeekVideoOptionsDef(EEngineVideoOptionQuality quality) -- the video-options def of one quality level: *PEngineVideoOptionsDefs[quality].
// Leaf: ?PeekVideoOptionsDef@CEngine@@QAEABVCEngineVideoOptionsDef@@W4EEngineVideoOptionQuality@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B244F0, length 14 bytes (`mov eax,[esp+4] ; mov eax,[ecx+eax*4+0x120] ; ret 4`).
// Identity (NF56, round 7, queue q10417, a CLAIM settled from the body and the callers): Ego_r.pdb 0x009AEB40 / FableWin.pdb 0x02CA1700
//   ?PeekVideoOptionsDef@CEngine@@QAEABVCEngineVideoOptionsDef@@W4EEngineVideoOptionQuality@@@Z.
//   - `this`: every call site loads ecx from 0x01436EA4 = ?GPEngine@NEngineInternal@@3PAVCEngine@@A (reloc_bindings), so the owner is
//     CEngine, not CTextureManager.
//   - +0x120: the retail engine/CEngine.h puts PEngineVideoOptionsDefs (CDefPointer<const CEngineVideoOptionsDef>[4]) there (Ego_r +0x128
//     before the 12-byte STLport CArrays).
//   - the result: CEnginePrimitiveRenderer::FrameStart 0x00B32660 reads it as a record (`mov ecx,[eax+0x40]`, LODErrorTolerance), not as a size.
//   The catalog name CTextureManager::GetPoolTextureSize (Ego_r ?GetPoolTextureSize@CTextureManager@@QBEKK@Z, a const accessor) was a
//   propagated guess; it was the stand-in CTextureManager_Methods::GetPoolTextureSize reading the pointer as an unsigned long.
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF_TYPED
#include "engine/CEngine.h"
#include "engine/CEngineVideoOptionsDef.h"

const CEngineVideoOptionsDef& CEngine::PeekVideoOptionsDef(EEngineVideoOptionQuality quality)
{
    return *PEngineVideoOptionsDefs[quality];
}
