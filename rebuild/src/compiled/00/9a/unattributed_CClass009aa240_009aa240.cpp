// CClass_009aa240::CClass_009aa240() -- constructor: initialises through the out-of-line helper @ 0x0099E4B0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AA240, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x99e4b0 ; mov eax, esi ; pop esi ; ret 
// Tiny-body shape `CtorInit`: a default constructor whose whole body is one out-of-line
// initialiser call; `this` is kept in esi for the return. The class comes from the manifest label.
// Name fix (NF199, 2026-11-20, q4456): the Initialise@CClass_009aa240 stand-in for the CCharString member is replaced by the shared fable_string.h
//   CCharString, so the constructor call binds ??0CCharString@@QAE@XZ (Ego_r.pdb; 0x0099E4B0).
#include "fable_string.h"
struct CClass_009aa240 {
    CCharString m_string;                  // +0x00 (0x0099E4B0 constructs it)
    CClass_009aa240();
};

CClass_009aa240::CClass_009aa240()
    : m_string()
{
}