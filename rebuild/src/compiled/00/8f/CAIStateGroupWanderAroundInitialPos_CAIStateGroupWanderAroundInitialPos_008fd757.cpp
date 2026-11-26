// CAIStateGroup_WanderAroundInitialPos::CAIStateGroup_WanderAroundInitialPos() -- base CAIStateGroupBase() then this class's vtable
// Leaf: ??0CAIStateGroup_WanderAroundInitialPos@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD757, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroupBase::CAIStateGroupBase ; mov dword ptr [esi],offset CAIStateGroup_WanderAroundInitialPos::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base constructor @ 0x008FD255
// (??0CAIStateGroupBase@@QAE@XZ) runs, then the compiler stores this class's vtable (0x01289460) and returns `this`.
// NF467 (r459a vftable port): the class and its base come from the shared headers (local class on engine/CAIStateGroupBase.h (r462a; no CAIStateGroup_WanderAroundInitialPos header) on
//   engine/CAIStateGroupBase.h's FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS virtual list), so the emitted ??_7CAIStateGroup_WanderAroundInitialPos@@6B@ has retail's
//   14 slots (was a one-slot local model, 4 bytes).
// TU mode: size-optimised, as retail's TU was: the ctor sits at the unaligned 0x008FD757 (a /O2 VC7.1 COMDAT is 16-byte aligned, an
//   optimize("s") one 1-byte aligned), and the ??_G / implicit ??1CAIStateGroupBase the full table brings are then the retail copies
//   (0x008FF704 and 0x008FD288; under /O2 the ??_G is 2 bytes longer). The ctor's own bytes are the same in both modes.
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"

// CAIStateGroup_WanderAroundInitialPos has no shared header: the class is declared here on its base's shared header (r462a).
class CAIStateGroup_WanderAroundInitialPos : public CAIStateGroupBase {
public:
    CAIStateGroup_WanderAroundInitialPos();                                 // 0x008FD757
    virtual bool IsRunnable();                                                     // slot 11 0x00911A00
    virtual void Update();                                                         // slot 12 0x00911A90
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_WanderAroundInitialPos) == 0x14);

CAIStateGroup_WanderAroundInitialPos::CAIStateGroup_WanderAroundInitialPos()
    : CAIStateGroupBase()
{
}
