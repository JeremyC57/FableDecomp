// CEngineResourceManager::GetVertexBufferUsage() -- the D3D usage flags for engine vertex buffers: write-only, plus software vertex processing while the device's vertex shader version is below 1.1 (24, else 8)
// Leaf: ?GetVertexBufferUsage@CEngineResourceManager@@QAEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B030, length 33 bytes.
// Name (NF152, 2026-11-20, q4561 / q4729): Ego_r.pdb 0x009D9680 / FableWin.pdb 0x02DD43A0 ?GetVertexBufferUsage@CEngineResourceManager@@QAEKXZ
//   (public, unsigned long, the first of the out-of-line run GetVertexBufferUsage / GetIndexBufferUsage / GetIndexBufferMemPool that retail
//   lays out at 0x00B3B030 / 0x00B3B060 / 0x00B3B0A0); every landed caller already binds it (0x00B51570, 0x00B6D940, 0x00BEAE50 ...).
//   Was CEngineResourceManager::GetIndexBufferUsage (static, a local model) calling the stand-in CResourceOwner_01436e1c.
// Callee: CDisplayManager::GetVertexShaderVersion @ 0x009BEDE0 (Ego_r / FableWin ?GetVertexShaderVersion@CDisplayManager@@QBEKXZ) on
//   NEngineInternal::GPDisplayManager @ 0x01436E1C (both PDBs ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A; queue land1011cr).
// Shape: the alternative constant 8 is hoisted into esi before the call, the unsigned `cmp eax,44Ch ; mov eax,18h ; jb` selects;
//   `this` is unused (24 = D3DUSAGE_WRITEONLY | D3DUSAGE_SOFTWAREPROCESSING, 8 = D3DUSAGE_WRITEONLY, 1100 = vs_1_1).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CDisplayManager.h"

namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A

unsigned long CEngineResourceManager::GetVertexBufferUsage()
{
    unsigned long usage = 8;                       // the alternative is held in esi across the call
    if (NEngineInternal::GPDisplayManager->GetVertexShaderVersion() < 1100)
        usage = 24;
    return usage;
}
