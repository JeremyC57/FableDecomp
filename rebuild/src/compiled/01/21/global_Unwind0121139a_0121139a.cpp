// EH unwind funclet @ 0x0121139A of CItemDispatcher::RunAllItems (parent @ 0x00C10400): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x10 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136A160 lists 0x0121139A in its unwind map; its __ehhandler thunk
// 0x012113AA (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C10400.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121139A, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x10) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c1/CItemDispatcher_RunAllItems_00c10400.cpp"