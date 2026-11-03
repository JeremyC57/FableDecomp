// EH unwind funclet @ 0x01216D28 of CBaseAllocator::~CBaseAllocator (parent @ 0x00CA9D00): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 39 bytes at +0x8 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0137143C lists 0x01216D28 in its unwind map; its __ehhandler thunk
// 0x01216D4F (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00CA9D00.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01216D28, oracle length 39 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x8) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/ca/CBaseAllocator_CBaseAllocator_00ca9d00.cpp"