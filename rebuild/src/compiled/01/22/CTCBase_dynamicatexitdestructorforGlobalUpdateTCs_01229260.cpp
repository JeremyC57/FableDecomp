// CTCBase::`dynamic atexit destructor for 'GlobalUpdateTCs'' -- destroy the static STLport list<CTCBase*>: tail call ~list 0x004CC220 with ecx = 0x013BA8AC
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229260, 10 bytes.
// Leaf: _$E2
// Retail bytes:  mov ecx,0x013BA8AC ; jmp 0x004CC220. The same source as the initializer row 0x0121E010 (registers this function with atexit).
// Identity (NF96 q11449): the static at 0x013BA8AC is CTCBase::GlobalUpdateTCs, Ego_d.pdb PUB32
//   ?GlobalUpdateTCs@CTCBase@@1V?$list@PAVCTCBase@@V?$allocator@PAVCTCBase@@@std@@@std@@A (protected static list<CTCBase*>: the 0xC-byte node =
//   next, prev, CTCBase*); Ego_r.pdb 0x01010A70 / FableWin.pdb 0x03FE50A0 name `dynamic initializer for 'CTCBase::GlobalUpdateTCs'`; the
//   landed RemoveFromGlobalTCs 0x00686DF0 and UpdateGlobalTCs 0x00686DC0 bind the same symbol. The NF92 recipe of 0x0121A440 (the static defined
//   on the shared header, engine/CTCBase.h FABLE_RETAIL_CTCBASE_GLOBALUPDATETCS, NF95; the compiler emitting the initializer and its atexit
//   destructor; FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE: the destructor 0x01229260 tail-jumps to ~list<CTCBase*> 0x004CC220, FableWin.pdb
//   ??1?$list@PAVCTCBase@@V?$allocator@PAVCTCBase@@@std@@@std@@QAE@XZ, clear + free of the sentinel, out of line). Was the generated tail thunk GlobalMethodTailThunk_01229260 over AutoTinyThunkTarget g_AutoTinyThunkObject_013ba8ac (Invoke_004cc220).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_GLOBALUPDATETCS
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"

std::list<CTCBase*> CTCBase::GlobalUpdateTCs;   // 0x013BA8AC
