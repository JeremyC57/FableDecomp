// _global::FUN_007b5590() -- construct, in place, the CCharString class name "CActivateQuestDef"; returns this
// Leaf: ??0CDefClassName_007b5590
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B5590, 19 bytes: `push -1 ; push offset "CActivateQuestDef" ; mov esi,ecx ;
//   call CCharString::CCharString(const char*, long) @ 0x0099EBF0 ; mov eax,esi`. Unreferenced; the string
//   construction that opens CDefClassBase::GetSubDef<CActivateQuestDef> (0x007B5680), compiled out of line as a
//   constructor (the target arrives in ecx, `this` is returned). Modelled as a CCharString-derived name holder.
#include "fable_string.h"

struct CDefClassName_007b5590 : CCharString {
    CDefClassName_007b5590();
};

CDefClassName_007b5590::CDefClassName_007b5590()
    : CCharString("CActivateQuestDef", -1)
{
}