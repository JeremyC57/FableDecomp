// LHNetworkLib::LSocket::`dynamic atexit destructor for 'GlobalList'' -- destroy the static socket list LSocket::GlobalList at exit
// (STLport 4.6 ~list -> out-of-line ~_List_base: every node freed, then the sentinel)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122A200, 10 bytes: `mov ecx,offset 0x013BB000 ; jmp 0x008F9F50`. Registered with atexit by the
//   dynamic initializer 0x0121F850 (`push 0x0122A200 ; ... ; call atexit`), whose TU emits this function as its `_$E2`.
// Identity (proven): Ego_r.pdb 0x0101D810 / FableWin.pdb 0x04011B20 `dynamic atexit destructor for 'LHNetworkLib::LSocket::GlobalList''
//   (the initializer is Ego_r 0x01017230 / FableWin 0x03FFFE40); the only reference to 0x0122A200 is that `push` in 0x0121F850.
//   0x013BB000 is ?GlobalList@LSocket@LHNetworkLib@@2V?$list@PAVLSocket@LHNetworkLib@@V?$allocator@PAVLSocket@LHNetworkLib@@@std@@@std@@A
//   (engine/LSocket.h, the spelling the landed initializer 0x0121F850 and LSocket::Think 0x008F84A0 bind).
// agentNF193 (q4133): the landed stand-in (GlobalMethodTailThunk_0122a200 on a local AutoTinyThunkTarget, g_AutoTinyThunkObject_013bb000 /
//   Invoke_008f9f50) is replaced by the definition of the static on the shared header, as the NF89 / NF164 initializer / atexit pairs
//   (0x0121F0E0 / 0x01229BE0, 0x0121E030 / 0x01229280): the compiler emits both functions from this one definition. The static is also
//   defined by the landed initializer row 0x0121F850 (the known initializer / atexit-destructor LNK2005, EXTRA_RULES 2026-11-20).
#define FABLE_RETAIL_LSOCKET_SEND_PASS23
#define FABLE_RETAIL_LSOCKET_THINK
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/LSocket.h"

std::list<LHNetworkLib::LSocket*> LHNetworkLib::LSocket::GlobalList;   // 0x013BB000
