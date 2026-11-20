#pragma optimize("s",on)
// CClass_005b15c2::CClass_005b15c2() -- constructor: the base is built out of line (@ 0x0099AED0), then the member at +0x4 is zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B15C2, oracle length 16 bytes.
// Leaf: ??0CClass_005b15c2@@QAE@XZ
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x99aed0 ; and dword ptr [esi + 4], 0 ; mov eax, esi ; pop esi ; ret 
// Tiny-body shape `CtorBaseThenZero`: a derived constructor over an out-of-line base constructor
// followed by one zeroed member (`and [esi+N], 0`). The class comes from the manifest label.
// NF193 (q4546): the out-of-line base constructor 0x0099AED0 is the PDB ??0CWideString@@QAE@XZ (Ego_r.pdb / FableWin.pdb name sets):
//   the header-less local class keeps its token and now sits on the shared fable_string.h CWideString (narrow base rule) instead of the
//   CBase_005b15c2 stand-in.
#include "fable_string.h"

struct CClass_005b15c2 : CWideString {
    long m_value;                          // +0x4
    CClass_005b15c2();
};

CClass_005b15c2::CClass_005b15c2()
    : m_value(0)
{
}