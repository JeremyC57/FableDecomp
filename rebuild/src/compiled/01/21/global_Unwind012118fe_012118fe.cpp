// EH unwind funclet @ 0x012118FE of PKCWorkerQueue_00c14560::~PKCWorkerQueue_00c14560 (parent @ 0x00C14560): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x1e of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136AAD0 lists 0x012118FE in its unwind map; its __ehhandler thunk
// 0x01211906 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C14560.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012118FE, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x1e) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c1/PKCWorkerQueue00c14560_PKCWorkerQueue00c14560_00c14560.cpp"