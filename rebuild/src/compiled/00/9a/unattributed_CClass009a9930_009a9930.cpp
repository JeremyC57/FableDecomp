// CClass_009a9930::CClass_009a9930(const CAllocator&) -- constructor: initialises through the out-of-line helper @ 0x0099E4B0; the allocator argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A9930, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x99e4b0 ; mov eax, esi ; pop esi ; ret 4
// Tiny-body shape `CtorInitUnusedArg`: a constructor whose one argument (an empty allocator or a
// tag) is not read; `this` is kept in esi for the return while the out-of-line initialiser runs.
// The manifest label is propagated; the class is named from it when it is a plain class name.
struct CAllocator { };

// Name fix (NF199, 2026-11-20, q4456): the Initialise@CClass_009a9930 stand-in for the CCharString member is replaced by the shared fable_string.h
//   CCharString, so the constructor call binds ??0CCharString@@QAE@XZ (Ego_r.pdb; 0x0099E4B0).
#include "fable_string.h"
struct CClass_009a9930 {
    CCharString m_string;                  // +0x00 (0x0099E4B0 constructs it)
    CClass_009a9930(const CAllocator&);
};

CClass_009a9930::CClass_009a9930(const CAllocator&)
    : m_string()
{
}