// Leaf: ?GlobalMethodTailThunk_012299e0@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x012299E0, `mov ecx,offset 0x013BAD34 ; jmp ??1CCharString@@QAE@XZ`: the atexit destructor of the
//   static CCharString g_DoorOpenAnimName (its dynamic initializer is landed at the matching 0x0121EDxx row). agentNF13: the object and the
//   destructor now bind their real spellings (was g_AutoTinyThunkObject_013bad34 / AutoTinyThunkTarget::Invoke); the thunk's own
//   name is the family's (a compiler-generated `??__F` function: no source name).
#include "fable_string.h"      // CCharString: ~CCharString @ 0x0099EAE0

extern const CCharString g_DoorOpenAnimName;      // 0x013BAD34
void __fastcall GlobalMethodTailThunk_012299e0()
{
    g_DoorOpenAnimName.~CCharString();
}
