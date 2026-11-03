// EH unwind funclet @ 0x0121439D of CItemGridTable_012b501c::Insert (parent @ 0x00C34F30): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x0 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136E23C lists 0x0121439D in its unwind map; its __ehhandler thunk
// 0x012143A8 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C34F30.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121439D, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x0) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c3/CItemGridTable012b501c_Insert_00c34f30.cpp"