// CStringPair_009f0900::CStringPair_009f0900(long unused) -- constructor of a pair of narrow strings: two empty CCharStrings (+0 and +4); the one argument is ignored
// Leaf: CStringPair_009f0900
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F0900, 22 bytes, __fastcall, `ret 4`.
// Callees: CCharString::CCharString() 0x0099E4B0 twice.
// Evidence: no direct callers (the one "reference", 0x012AEAAC, is inside a packed data table, not a pointer); one stack
//   argument the body never reads (as the landed CTargetAndThing_0078e700(long unused) constructor). The manifest label
//   "QuestData" fits the layout (Ego_r.pdb NScript::CV_TalentlessBardScript::QuestData is {CCharString name, tag}) but
//   that constructor takes no argument and lives in the script module, while this one sits among the engine's
//   CCodeSectionManager functions, so the owner is named descriptively, suffixed by address.
#include "fable_string.h"

struct CStringPair_009f0900 {
    CCharString m_first;                     // +0x00
    CCharString m_second;                    // +0x04
    CStringPair_009f0900(long unused);
};

CStringPair_009f0900::CStringPair_009f0900(long unused)
{
}