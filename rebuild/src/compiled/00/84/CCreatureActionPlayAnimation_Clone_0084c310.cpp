// CCreatureAction_PlayAnimation::Clone() const -- `return new CCreatureAction_PlayAnimation(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_PlayAnimation@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084C310, 34 bytes: operator new(0xB4) 0x00BFEA1A, null test, then the copy constructor 0x0084C340 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_PlayAnimation@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_PlayAnimation@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0084C340 stores
//   CCreatureAction_PlayAnimation's vftable 0x01273A0C first; the landed row at 0x0084C340 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_PlayAnimation@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const);
//   now the real virtual member, so the RTTI-placed name is defined.
// Header: engine/CCreatureAction_PlayAnimation.h, opt-in FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION: the polymorphic class there
//   declares this Clone and its copy constructor; prelude: the base class's opt-ins and header plus the opt-in, then this header.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_PlayAnimation.h"

CCreatureActionBase* CCreatureAction_PlayAnimation::Clone() const
{
    return new CCreatureAction_PlayAnimation(*this);
}
