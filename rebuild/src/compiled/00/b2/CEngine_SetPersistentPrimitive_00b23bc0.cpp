// CEngine::SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags) -- forward to the
// primitive renderer component with nonPersistent = false
// Leaf: ?SetPersistentPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23BC0, `ret 0Ch`: re-pushes its three stack arguments plus `push 0` (the bool) and calls
//   0x00B324A0 on [0x01436E80] (NEngineInternal::GPPrimitiveRenderer); CEngine vtable 0x012A0F3C slot 23.
// Naming (NF172, 2026-11-20, q11904 / q11905): ported off the free __stdcall stand-in CEngine_SetPersistentPrimitive and its local
//   CGlobalTarget view onto engine/CEngine.h (NF172 opt-in FABLE_RETAIL_CENGINE_SETPERSISTENTPRIMITIVE, the CIEngine slot-23 override) and
//   engine/CEnginePrimitiveRenderer.h (NF150 opt-in SETPERSISTENTPRIMITIVE). Own leaf: Ego_r.pdb + FableWin.pdb
//   ?SetPersistentPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z (was ?CEngine_SetPersistentPrimitive@@YG...);
//   callee: ?SetPersistentPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K_N@Z (the self row
//   0x00B324A0's Leaf; was ?RealSet@CGlobalTarget@@QAEXPAVCEnginePrimitiveHandle@@PAVCEnginePrimitive@@KH@Z).
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_SETPERSISTENTPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_SETPERSISTENTPRIMITIVE
#include "engine/CEngine.h"
#include "engine/CEnginePrimitiveRenderer.h"
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }   // 0x01436E80

void CEngine::SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags)
{
    NEngineInternal::GPPrimitiveRenderer->SetPersistentPrimitive(handle, primitive, flags, false);
}
