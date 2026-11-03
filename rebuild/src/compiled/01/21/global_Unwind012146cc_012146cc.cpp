// EH unwind funclet @ 0x012146CC of _global::CTCActivationTrigger_DestroyPredicates (parent @ 0x00C37DA0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0xb of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136E6E8 lists 0x012146CC in its unwind map; its __ehhandler thunk
// 0x012146DF (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C37DA0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012146CC, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0xb) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c3/unattributed_CTCActivationTrigger00c37da0dtor_00c37da0.cpp"