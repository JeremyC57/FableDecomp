// EH unwind funclet @ 0x01211A74 of PKCDeferredBank::~PKCDeferredBank (parent @ 0x00C14F90): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x8 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136AD3C lists 0x01211A74 in its unwind map; its __ehhandler thunk
// 0x01211AC1 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C14F90.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01211A74, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x8) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c1/PKCDeferredBank_PKCDeferredBank_00c14f90.cpp"