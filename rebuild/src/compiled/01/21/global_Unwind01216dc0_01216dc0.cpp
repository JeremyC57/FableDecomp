// EH unwind funclet @ 0x01216DC0 of CVtable_012bf83c::~CVtable_012bf83c (parent @ 0x00CAA410): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x0 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x013714CC lists 0x01216DC0 in its unwind map; its __ehhandler thunk
// 0x01216DC8 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00CAA410.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01216DC0, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x0) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/ca/CVtable012bf83c_CVtable012bf83c_00caa410.cpp"