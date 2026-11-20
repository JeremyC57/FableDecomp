// `dynamic atexit destructor for 'BUILDING_BOUNDRY'' -- the destructor thunk the initializer 0x0121E940 registers with atexit:
//   runs the out-of-line ~CCharString on the global const CCharString BUILDING_BOUNDRY (0x013BAC98)
// Leaf: ?GlobalMethodTailThunk_012297f0@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x012297F0, 10 bytes: `mov ecx,0x013BAC98 ; jmp 0x0099EAE0` (??1CCharString@@QAE@XZ).
// Naming (NF183, 2026-11-20, q4256): the AutoTinyThunkTarget / g_AutoTinyThunkObject_013bac98 stand-in moved onto fable_string.h:
//   0x013BAC98 is ?BUILDING_BOUNDRY@@3VCCharString@@B (Ego_d.pdb public; Ego_r.pdb + FableWin.pdb `dynamic initializer for
//   'BUILDING_BOUNDRY'' / `dynamic atexit destructor for 'BUILDING_BOUNDRY''; the landed initializer 0x0121E940 defines it and pushes
//   this thunk to _atexit), and the jump target 0x0099EAE0 is CCharString's destructor. The thunk's own leaf (a free stand-in for
//   the compiler-generated `_$E` destructor, which only the TU defining the static emits) is unchanged. The qualified destructor call
//   keeps VC7.1 from emitting a ??_GCCharString scalar deleting destructor COMDAT (the unqualified spelling defines one; same bytes).
#include "fable_string.h"      // ~CCharString @ 0x0099EAE0

extern const CCharString BUILDING_BOUNDRY;               // 0x013BAC98 ?BUILDING_BOUNDRY@@3VCCharString@@B

void __fastcall GlobalMethodTailThunk_012297f0()
{
    BUILDING_BOUNDRY.CCharString::~CCharString();
}
