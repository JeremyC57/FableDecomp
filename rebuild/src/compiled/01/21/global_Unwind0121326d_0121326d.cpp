// EH unwind funclet @ 0x0121326D of CTimerHost::CTimerHost (parent @ 0x00C25B10): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0xb of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136CB38 lists 0x0121326D in its unwind map; its __ehhandler thunk
// 0x01213275 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C25B10.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121326D, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0xb) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c2/CTimerHost_CTimerHost_00c25b10.cpp"