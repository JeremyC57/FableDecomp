// EH unwind funclet @ 0x01211D89 of HasBankSegments (parent @ 0x00C16C10): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 25 bytes at +0x19 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136B144 lists 0x01211D89 in its unwind map; its __ehhandler thunk
// 0x01211DA2 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C16C10.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01211D89, oracle length 25 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x19) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c1/global_HasBankSegments_00c16c10.cpp"