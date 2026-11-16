// CCardPositionsDef::Transfer(CPersistContext& context) -- persist the card-position def: its one member, the vector<C2DVector> Offset (+0x3C)
// Leaf: ?Transfer@CCardPositionsDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EBF22, 16 bytes: `add ecx,3Ch ; push ecx ; mov ecx,[esp+8] ; call` the unnamed
//   TransferVector<C2DVector> 0x00475D99 `; ret 4`; virtual slot 18 of vtable 0x012439DC.
// Port (round 4, NF40): was a local CPersistContext / CCardPositionsDef pad model calling the stand-in Xfer_00475d99 on
//   `this+0x3c`; now engine/CCardPositionsDef.h (Ego_r.pdb member Offset, std::vector<C2DVector> at retail +0x3C) and
//   engine/CPersistContext.h, binding ??$TransferVector@VC2DVector@@@CPersistContext@@.. at 0x00475D99 (that row's own name).
//   The PDB spelling of the override is the virtual ?Transfer@CCardPositionsDef@@UAEXAAVCPersistContext@@@Z (Ego_r / FableWin).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCardPositionsDef.h"
#include "engine/CPersistContext.h"

void CCardPositionsDef::Transfer(CPersistContext& context)
{
    context.TransferVector(Offset);
}
