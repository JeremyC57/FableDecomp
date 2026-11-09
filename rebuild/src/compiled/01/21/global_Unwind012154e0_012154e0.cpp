// EH unwind funclet @ 0x012154E0 of CAmbientCube::Dump (parent @ 0x00C45990): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x0 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136F7A0 lists 0x012154E0 in its unwind map; its __ehhandler thunk
// 0x01215528 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C45990.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012154E0, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x0) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c4/CAmbientCube_Dump_00c45990.cpp"