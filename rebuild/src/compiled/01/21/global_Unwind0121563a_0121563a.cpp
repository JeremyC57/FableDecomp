// EH unwind funclet @ 0x0121563A of CAmbientSource::UpdateChannels (parent @ 0x00C47210): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x0 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136F910 lists 0x0121563A in its unwind map; its __ehhandler thunk
// 0x01215642 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C47210.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121563A, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x0) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c4/CAmbientSource_UpdateChannels_00c47210.cpp"