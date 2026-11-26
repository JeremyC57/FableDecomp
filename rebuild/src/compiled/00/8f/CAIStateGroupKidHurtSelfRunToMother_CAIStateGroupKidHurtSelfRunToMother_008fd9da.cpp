// CAIStateGroup_KidHurtSelfRunToMother::CAIStateGroup_KidHurtSelfRunToMother() -- base CAIStateGroupBase() then this class's vtable
// Leaf: ??0CAIStateGroup_KidHurtSelfRunToMother@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD9DA, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroupBase::CAIStateGroupBase ; mov dword ptr [esi],offset CAIStateGroup_KidHurtSelfRunToMother::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base constructor @ 0x008FD255
// (??0CAIStateGroupBase@@QAE@XZ) runs, then the compiler stores this class's vtable (0x01289824) and returns `this`.
// NF474 (r467f rework of NF467 b3; r459a vftable port): the class is engine/CAIStateGroup_KidHurtSelfRunToMother.h's existing retail view FABLE_RETAIL_KID_HURT_SELF_PASS23
//   (defined here), with the constructor declared under the add-only sub-macro FABLE_RETAIL_CAISTATEGROUP_KIDHURTSELFRUNTOMOTHER_CTOR; so the emitted
//   ??_7CAIStateGroup_KidHurtSelfRunToMother@@6B@ has retail's 14 slots at retail's targets (was a one-slot local model, 4 bytes).
// TU mode: size-optimised, as retail's TU was (r467c): the ??_GCAIStateGroup_KidHurtSelfRunToMother@@UAEPAXI@Z the full table brings is then 28 bytes,
//   equal to retail 0x008FFC5C (under /O2 it is 30 bytes and differs), and the implicit ??1CAIStateGroupBase 19 bytes = 0x008FD288.
//   The ctor's own bytes are the same in both modes.
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_KID_HURT_SELF_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_KIDHURTSELFRUNTOMOTHER_CTOR
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"

CAIStateGroup_KidHurtSelfRunToMother::CAIStateGroup_KidHurtSelfRunToMother()
    : CAIStateGroupBase()
{
}
