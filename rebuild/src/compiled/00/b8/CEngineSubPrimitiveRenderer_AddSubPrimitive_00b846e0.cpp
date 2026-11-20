// CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* sub_primitive, float distance, unsigned char pass_flags,
//   unsigned long sort_key, CEnginePrimitiveManager* manager) -- add a sub-primitive with an empty chain: forward to the six-argument
//   overload with a fresh CEngineSubPrimitiveChain
// Leaf: ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B846E0, length 54 bytes.
// Callees: the six-argument AddSubPrimitive @ 0x00B84580 (on `this`, ecx passed straight through). Shape: the chain local (two words)
//   is zeroed from eax in the 8-byte frame by the inline CEngineSubPrimitiveChain constructor, its address pushed first, then the five
//   arguments re-read last-first in alternating edx/eax; `add esp,8 ; ret 14h`.
// Naming (NF172, 2026-11-20, q4824): ported off the local CEngineSubPrimitiveRenderer / SSortKey_00b846e0 / void* models onto
//   engine/CEngineSubPrimitiveRenderer.h (FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD + PASS22_STATIC_MESH_RENDER, which declares both
//   overloads) and engine/CEngineSubPrimitiveChain.h. The callee now binds Ego_r.pdb 0x00A2BA70 / FableWin.pdb 0x02DB22D0
//   ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@AAVCEngineSubPrimitiveChain@@@Z
//   (the self row 0x00B84580's Leaf; was ...@@QAEXPAX0000PAUSSortKey_00b846e0@@@Z) and the own leaf is Ego_r.pdb 0x00A2BB80 / FableWin.pdb
//   0x02DB24C0 ...@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@@Z, the spelling all 6 landed referencers already bind.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
#include "engine/CEngineSubPrimitiveChain.h"
#include "engine/CEngineSubPrimitiveRenderer.h"

void CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* sub_primitive, float distance, unsigned char pass_flags,
                                                  unsigned long sort_key, CEnginePrimitiveManager* manager)
{
    CEngineSubPrimitiveChain chain;
    AddSubPrimitive(sub_primitive, distance, pass_flags, sort_key, manager, chain);
}
