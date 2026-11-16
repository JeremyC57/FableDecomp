// CFrontEndDef::Transfer(CPersistContext& context) -- transfer the attract-mode movie list and the three graphic indices
// Leaf: ?Transfer@CFrontEndDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe @ 0x004331C0, 57 bytes, `ret 4`; slot 18 of the CFrontEndDef vtable (reference at 0x01231004);
// Ego_r.pdb / FableWin.pdb ?Transfer@CFrontEndDef@@UAEXAAVCPersistContext@@@Z. Body: the unnamed
// CPersistContext::TransferVector<CCharString>(vAttractModeMovie) (0x004331F9, +0x3C), then Transfer<long> (0x00431102) on
// ErrorMessageBackgroundGraphic (+0x48), ButtonABigGraphic (+0x4C) and ButtonBBigGraphic (+0x50). No base-class Transfer call.
// Port (round 4, NF40): was a local CPersistContext / CFrontEndDef / std::vector model with a stand-in TransferVectorBinary
//   member (and the non-virtual QAE leaf); now engine/CFrontEndDef.h (its FABLE_RETAIL_CFRONTENDDEF_TRANSFER opt-in declares
//   the override) and engine/CPersistContext.h.
// Shape: /O2.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CFRONTENDDEF_TRANSFER
#include "engine/CFrontEndDef.h"
#include "engine/CPersistContext.h"

void CFrontEndDef::Transfer(CPersistContext& context)
{
    context.TransferVector(vAttractModeMovie);
    context.Transfer(ErrorMessageBackgroundGraphic);
    context.Transfer(ButtonABigGraphic);
    context.Transfer(ButtonBBigGraphic);
}
