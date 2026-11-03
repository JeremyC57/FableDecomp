// EH unwind funclet @ 0x012129A0 of CFactory::Create (parent @ 0x00C1EFC0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x63 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136C214 lists 0x012129A0 in its unwind map; its __ehhandler thunk
// 0x012129C1 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C1EFC0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012129A0, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x63) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c1/CFactory_Create_00c1efc0.cpp"