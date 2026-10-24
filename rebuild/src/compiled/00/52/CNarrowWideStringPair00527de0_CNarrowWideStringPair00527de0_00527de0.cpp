// CNarrowWideStringPair_00527de0::CNarrowWideStringPair_00527de0(long unused) -- constructor of a narrow plus wide string pair: an empty CCharString (+0) and an empty CWideString (+4); the one argument is ignored
// Leaf: CNarrowWideStringPair_00527de0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00527DE0, 22 bytes (+ int3 padding), __fastcall, `ret 4`.
// Callees: CCharString::CCharString() 0x0099E4B0, CWideString::CWideString() 0x0099AED0.
// Evidence: no direct callers or data references; one stack argument the body never reads (as the landed
//   CTargetAndThing_0078e700(long unused) constructor). The manifest label "QuestData" is propagated: the PDB QuestData
//   is two CCharStrings, this one has a CWideString second, so the owner is named descriptively, suffixed by address.
// Shape: the two member constructors in declaration order, no EH frame (the default lane builds without /EHsc).
#include "fable_string.h"

struct CNarrowWideStringPair_00527de0 {
    CCharString m_narrow;                    // +0x00
    CWideString m_wide;                      // +0x04
    CNarrowWideStringPair_00527de0(long unused);
};

CNarrowWideStringPair_00527de0::CNarrowWideStringPair_00527de0(long unused)
{
}