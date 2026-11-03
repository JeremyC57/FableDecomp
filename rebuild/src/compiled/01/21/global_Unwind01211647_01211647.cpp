// EH unwind funclet @ 0x01211647 of CSoundEventManager::CSoundEventManager (parent @ 0x00C11D00): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x19 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136A624 lists 0x01211647 in its unwind map; its __ehhandler thunk
// 0x01211673 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C11D00.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01211647, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x19) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c1/CSoundEventManager_CSoundEventManager_00c11d00.cpp"