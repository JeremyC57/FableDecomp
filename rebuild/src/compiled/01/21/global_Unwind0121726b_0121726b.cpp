// EH unwind funclet @ 0x0121726B of CCmdQueue::~CCmdQueue (parent @ 0x00CAF580): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0xb of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x01371A18 lists 0x0121726B in its unwind map; its __ehhandler thunk
// 0x0121728C (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00CAF580.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121726B, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0xb) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/ca/CCmdQueue_CCmdQueue_00caf580.cpp"