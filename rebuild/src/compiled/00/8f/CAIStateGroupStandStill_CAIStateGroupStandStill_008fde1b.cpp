// CAIStateGroup_StandStill::CAIStateGroup_StandStill() -- base CAIStateGroupBase() then this class's vtable
// Leaf: ??0CAIStateGroup_StandStill@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FDE1B, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroupBase::CAIStateGroupBase ; mov dword ptr [esi],offset CAIStateGroup_StandStill::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base constructor @ 0x008FD255
// (??0CAIStateGroupBase@@QAE@XZ) runs, then the compiler stores this class's vtable (0x01289D88) and returns `this`.
// NF467 (r459a vftable port): the class and its base come from the shared headers (engine/CAIStateGroup_StandStill.h opt-in
//   FABLE_RETAIL_CAISTATEGROUP_STANDSTILL on engine/CAIStateGroupBase.h's FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS virtual list), so the
//   emitted ??_7CAIStateGroup_StandStill@@6B@ has retail's 14 slots (was a one-slot local model, 4 bytes).
// TU mode: size-optimised, as retail's TU was: the ctor sits at the unaligned 0x008FDE1B (a /O2 VC7.1 COMDAT is 16-byte aligned, an
//   optimize("s") one 1-byte aligned), and the companions the full table brings are then the retail copies: ??_GCAIStateGroup_StandStill
//   28 bytes = 0x00900484 and the implicit ??1CAIStateGroupBase 19 bytes = 0x008FD288 (relocations equal; under /O2 the ??_G is 30 bytes
//   and differs). The landed 0x00900484 sets the same pragma. The ctor's own bytes are the same in both modes.
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_STANDSTILL
#include "engine/CAIStateGroup_StandStill.h"

CAIStateGroup_StandStill::CAIStateGroup_StandStill()
    : CAIStateGroupBase()
{
}
