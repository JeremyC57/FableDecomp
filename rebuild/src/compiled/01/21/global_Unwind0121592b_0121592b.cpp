// EH unwind funclet @ 0x0121592B of CAsyncDecoder_00c48640::Run (parent @ 0x00C47F10): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 14 bytes at +0x1b of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136FC80 lists 0x0121592B in its unwind map; its __ehhandler thunk
// 0x01215939 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C47F10.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121592B, oracle length 14 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x1b) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c4/CAsyncDecoder00c48640_Run_00c47f10.cpp"