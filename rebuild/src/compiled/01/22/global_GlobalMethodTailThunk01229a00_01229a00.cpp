// Leaf: ?GlobalMethodTailThunk_01229a00@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229A00, `mov ecx,offset 0x013BAD3C ; jmp ??1CCharString@@QAE@XZ`: the atexit destructor of the
//   static CCharString g_ClockHandTurnName (its dynamic initializer is landed at the matching 0x0121EDxx row). agentNF13: the object and the
//   destructor now bind their real spellings (was g_AutoTinyThunkObject_013bad3c / AutoTinyThunkTarget::Invoke); the thunk's own
//   name is the family's (a compiler-generated `??__F` function: no source name).
#include "fable_string.h"      // CCharString: ~CCharString @ 0x0099EAE0

extern CCharString g_ClockHandTurnName;      // 0x013BAD3C
void __fastcall GlobalMethodTailThunk_01229a00()
{
    g_ClockHandTurnName.~CCharString();
}
