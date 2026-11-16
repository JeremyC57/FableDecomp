// CSpecialAbilitiesTurncoatSpellDef::Transfer(CPersistContext& context) -- persist the five per-level float vectors of the turncoat spell def
// Leaf: ?Transfer@CSpecialAbilitiesTurncoatSpellDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465662, 68 bytes, virtual `ret 4`; five calls of the unnamed
//   CPersistContext::TransferVector<float> 0x00464CBF on +0x3C .. +0x6C. Ego_r.pdb / FableWin.pdb
//   ?Transfer@CSpecialAbilitiesTurncoatSpellDef@@UAEXAAVCPersistContext@@@Z (RTTI vtable slot 18).
// Port (round 4, NF40): was a stand-in class named after the real class, with opaque 12-byte fields and a stand-in
//   Transfer_00464cbf(void*) member on a local CPersistContext; now engine/CSpecialAbilitiesTurncoatSpellDef.h (Ego_r.pdb
//   members StaminaCostPerSec, ResistanceDamagePerSec, TurncoatTime, TurncoatRange, TurncoatXPUpEveryNSecs; the override
//   through the FABLE_RETAIL_CSPECIALABILITIESTURNCOATSPELLDEF_TRANSFER opt-in) and engine/CPersistContext.h.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESTURNCOATSPELLDEF_TRANSFER
#include "engine/CSpecialAbilitiesTurncoatSpellDef.h"
#include "engine/CPersistContext.h"

void CSpecialAbilitiesTurncoatSpellDef::Transfer(CPersistContext& context)
{
    context.TransferVector(StaminaCostPerSec);
    context.TransferVector(ResistanceDamagePerSec);
    context.TransferVector(TurncoatTime);
    context.TransferVector(TurncoatRange);
    context.TransferVector(TurncoatXPUpEveryNSecs);
}
