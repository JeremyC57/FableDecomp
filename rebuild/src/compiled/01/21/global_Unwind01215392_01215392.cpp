// EH unwind funclet @ 0x01215392 of CAmbientEmitter::CAmbientEmitter (parent @ 0x00C44EB0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 14 bytes at +0x29 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136F5F4 lists 0x01215392 in its unwind map; its __ehhandler thunk
// 0x012153A0 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C44EB0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01215392, oracle length 14 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x29) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c4/CAmbientEmitter_CAmbientEmitter_00c44eb0.cpp"