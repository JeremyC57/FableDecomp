// CCreatureAction_UnsheatheToShowToSheatheTrophy::Clone() const -- `return new CCreatureAction_UnsheatheToShowToSheatheTrophy(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_UnsheatheToShowToSheatheTrophy@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DFC20, 34 bytes: operator new(0xC4) 0x00BFEA1A, null test, then the copy constructor 0x006DFC50 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_UnsheatheToShowToSheatheTrophy@@UBEPAVCCreatureActionBase@@XZ: RTTI
//   placement + Ego_r/FableWin; callee ??0CCreatureAction_UnsheatheToShowToSheatheTrophy@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail
//   0x006DFC50 stores CCreatureAction_UnsheatheToShowToSheatheTrophy's vftable 0x0125F8D4 first; the landed row at 0x006DFC50 defines
//   it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_UnsheatheToShowToSheatheTrophy@@QAEPAVCCreatureActionBase@@XZ
//   (non-virtual, non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine/CCreatureAction_UnsheatheToShowToSheatheTrophy.h, its default branch: the polymorphic class there declares this Clone
//   and its copy constructor; prelude: the base class's opt-ins and header, then this header.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
#include "engine/CCreatureAction_UnsheatheToShowToSheatheTrophy.h"

CCreatureActionBase* CCreatureAction_UnsheatheToShowToSheatheTrophy::Clone() const
{
    return new CCreatureAction_UnsheatheToShowToSheatheTrophy(*this);
}
