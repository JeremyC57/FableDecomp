// CEngineLocalDetailLayerDef::Transfer(CPersistContext& context) -- persist the layer's spacing vector and its object sub-defs
// Leaf: ?Transfer@CEngineLocalDetailLayerDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00477CB1, 33 bytes, virtual `ret 4` (slot 18): the unnamed TransferVector<float>
//   0x00464CBF on SpacingFromLayer (+0x3C), then TransferVectorOfSubComponents<CEngineLocalDetailObjectDef> 0x00477CD2 on
//   Objects (+0x48). Ego_r.pdb / FableWin.pdb ?Transfer@CEngineLocalDetailLayerDef@@UAEXAAVCPersistContext@@@Z.
// Port (round 4, NF40): was a pad struct with stand-in PersistA / PersistB members on a local CPersistContext (and the
//   non-virtual QAE leaf); now engine/CEngineLocalDetailLayerDef.h (which declares the override) and engine/CPersistContext.h.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CEngineLocalDetailLayerDef.h"
#include "engine/CPersistContext.h"

void CEngineLocalDetailLayerDef::Transfer(CPersistContext& context)
{
    context.TransferVector(SpacingFromLayer);
    context.TransferVectorOfSubComponents(Objects);
}
