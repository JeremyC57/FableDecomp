// EH unwind funclet @ 0x01215DE3 of PKCBufferedReader::PKCBufferedReader (parent @ 0x00C4B300): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0xb of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x013702F0 lists 0x01215DE3 in its unwind map; its __ehhandler thunk
// 0x01215E0E (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C4B300.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01215DE3, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0xb) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c4/PKCBufferedReader_PKCBufferedReader_00c4b300.cpp"