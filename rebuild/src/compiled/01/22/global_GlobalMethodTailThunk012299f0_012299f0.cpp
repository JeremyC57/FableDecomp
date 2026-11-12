// Leaf: ?GlobalMethodTailThunk_012299f0@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x012299F0, `mov ecx,offset 0x013BAD38 ; jmp ??1CCharString@@QAE@XZ`: the atexit destructor of the
//   `anonymous namespace'::SDoorCloseAnim (agentFAM09; was g_DoorCloseAnimName) (its dynamic initializer is landed at the matching 0x0121EDxx row). agentNF13: the object and the
//   destructor now bind their real spellings (was g_AutoTinyThunkObject_013bad38 / AutoTinyThunkTarget::Invoke); the thunk's own
//   name is the family's (a compiler-generated `??__F` function: no source name).
#include "fable_string.h"      // CCharString: ~CCharString @ 0x0099EAE0

namespace { extern const CCharString SDoorCloseAnim; }      // 0x013BAD38
void __fastcall GlobalMethodTailThunk_012299f0()
{
    SDoorCloseAnim.~CCharString();
}
