// std::pair<float, CWideString>::pair(const pair& p) -- STLport copy constructor: the float copied inline (through a register), then the
// CWideString copy constructor (0x0099B720) on +4; returns this
// Leaf: ??0?$pair@MVCWideString@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00564B20, 29 bytes, thiscall, `ret 4`.
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call 0x0099B720 ; mov eax,esi ; pop esi ; ret 4
// NF39 (2026-11-16): moved off its `SPairLongCSecond_0099b720` stand-in (a generated {unsigned long, CSecond} model) onto the shared
//   stlport/fable_containers.h pair and fable_string.h CWideString (copy ctor 0x0099B720). Identity: the one caller (0x00563CD9, already bound to
//   this spelling) copy-constructs its best record pair<float, CWideString> from element 0; FableWin.pdb lists ??1?$pair@MVCWideString@@@std@@QAE@XZ
//   in the same TU, so the pair type is from the PDB; the copy constructor's spelling is the compiler's for that type.
// Model: explicit instantiation emits the member as its own COMDAT, as retail kept every used inline member.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_containers.h"

template std::pair<float, CWideString>::pair(const std::pair<float, CWideString>&);
