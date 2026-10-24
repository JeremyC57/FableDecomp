// CWideNamedValue_005b1a7b::CWideNamedValue_005b1a7b(long unused) -- constructor of a wide name plus value pair: an empty CWideString (+0) and a zero value (+4); the one argument is ignored
// Leaf: CWideNamedValue_005b1a7b
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B1A7B, 18 bytes, __fastcall, `ret 4`, unaligned (a size-optimised TU).
// Pragma: #pragma optimize("s", on) -- the zero store is `and dword ptr [esi+4],0` (the /O1 form of `= 0`).
// Callees: CWideString::CWideString() 0x0099AED0 (null data pointer, bumps the wide-string count 0x013BCA20).
// Evidence: no direct callers or data references; one stack argument that the body never reads (as the landed
//   CTargetAndThing_0078e700(long unused) constructor). The manifest label "PerEffectInfo" is propagated and wrong
//   (that PDB struct is a C3DVector plus an intelligent pointer), so the owner is named descriptively, suffixed by address.
#pragma optimize("s", on)
#include "fable_string.h"

struct CWideNamedValue_005b1a7b {
    CWideString m_name;                      // +0x00
    long m_value;                            // +0x04
    CWideNamedValue_005b1a7b(long unused);
};

CWideNamedValue_005b1a7b::CWideNamedValue_005b1a7b(long unused)
{
    m_value = 0;
}