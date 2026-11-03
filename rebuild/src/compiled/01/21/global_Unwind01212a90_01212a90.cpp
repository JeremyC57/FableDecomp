// EH unwind funclet @ 0x01212A90 of PKCFrallocStatic<CUnprovenFrallocElement_00C22920>::Return (parent @ 0x00C22920): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x0 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136C330 lists 0x01212A90 in its unwind map; its __ehhandler thunk
// 0x01212A98 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C22920.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01212A90, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x0) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c2/PKCFrallocStaticCUnprovenFrallocElement00C22920_Return_00c22920.cpp"