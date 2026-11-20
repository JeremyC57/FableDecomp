// CClass_005926c0::CClass_005926c0(const CAllocator&) -- constructor: initialises through the out-of-line helper @ 0x0099AED0; the allocator argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005926C0, oracle length 14 bytes.
// Leaf: ??0CClass_005926c0@@QAE@ABUCAllocator@@@Z
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x99aed0 ; mov eax, esi ; pop esi ; ret 4
// Tiny-body shape `CtorInitUnusedArg`: a constructor whose one argument (an empty allocator or a
// tag) is not read; `this` is kept in esi for the return while the out-of-line initialiser runs.
// The manifest label is propagated; the class is named from it when it is a plain class name.
struct CAllocator { };

// NF193 (q4546): the out-of-line base constructor 0x0099AED0 is the PDB ??0CWideString@@QAE@XZ (Ego_r.pdb / FableWin.pdb name sets):
//   the header-less local class keeps its token and now sits on the shared fable_string.h CWideString (narrow base rule) instead of the
//   ?Initialise@CClass_005926c0@@QAEXXZ stand-in.
#include "fable_string.h"

struct CClass_005926c0 : CWideString {
    CClass_005926c0(const CAllocator&);
};

CClass_005926c0::CClass_005926c0(const CAllocator&)
{
}