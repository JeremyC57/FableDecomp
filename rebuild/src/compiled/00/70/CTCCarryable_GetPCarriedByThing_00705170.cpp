// CTCCarryable::GetPCarriedByThing() const -- the thing carrying this one: the carrying TC's parent thing, or null when not carried
// Leaf: ?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00705170, 14 bytes: mov eax,[ecx+10h] ; test eax,eax ; je ; mov eax,[eax+4] ; ret ; xor eax,eax ; ret
// Naming (agentNF31, 2026-11-12): was the __fastcall stand-in GetCurrentSelectedEntryData(S*) over local Entry / S models.
//   Ego_r.pdb 0x00725060 + FableWin.pdb 0x0180067E / 0x02603B60 ?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ; the non-const
//   twin (Ego_r 0x00725070, the next function) is the byte-identical retail 0x00705180, so this first copy is the const one;
//   engine/CTCCarryable.h declares it at 0x00705170 (PCarryingTC +0x10, CTCBase::ParentThing +4) and the landed caller
//   0x004BD3C0 binds this symbol.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCCarryable.h"
#include "engine/CTCCarrying.h"

const CThing* CTCCarryable::GetPCarriedByThing() const
{
    return PCarryingTC ? PCarryingTC->ParentThing : 0;
}
