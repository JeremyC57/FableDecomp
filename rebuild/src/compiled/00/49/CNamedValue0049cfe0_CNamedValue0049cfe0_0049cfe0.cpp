// CNamedValue_0049cfe0::CNamedValue_0049cfe0() -- default constructor of a name plus value pair: an empty CCharString name (+0) and a zero value (+4)
// Leaf: CNamedValue_0049cfe0
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049CFE0, 19 bytes (+ int3 padding), __fastcall, plain `ret`.
// Callees: CCharString::CCharString() 0x0099E4B0 (null data pointer, bumps the narrow-string count 0x013BD800).
// Evidence: no direct callers or data references; the body only says "a CCharString at +0 and a dword at +4 set to 0".
//   The manifest label "PerEffectInfo" is propagated and wrong (that PDB struct is a C3DVector plus an intelligent
//   pointer, see 0x0078E3A0), so the owner is named descriptively, suffixed by address.
// Shape: the member constructor, then the in-body store of 0 (`mov dword ptr [esi+4],0`).
#include "fable_string.h"

struct CNamedValue_0049cfe0 {
    CCharString m_name;                      // +0x00
    long m_value;                            // +0x04
    CNamedValue_0049cfe0();
};

CNamedValue_0049cfe0::CNamedValue_0049cfe0()
{
    m_value = 0;
}