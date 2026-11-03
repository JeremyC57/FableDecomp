// EH unwind funclet @ 0x01213D93 of CUnprovenRecordList_00c2fc70::Transfer (parent @ 0x00C2FC70): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x13 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136DA20 lists 0x01213D93 in its unwind map; its __ehhandler thunk
// 0x01213D9B (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C2FC70.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01213D93, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x13) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c2/CUnprovenRecordList00c2fc70_Transfer_00c2fc70.cpp"