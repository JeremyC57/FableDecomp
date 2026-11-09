// EH unwind funclet @ 0x01210AF0 of CSystem::LoadBank (parent @ 0x00C08D40): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 34 bytes at +0x5c of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x013696D4 lists 0x01210AF0 in its unwind map; its __ehhandler thunk
// 0x01210B12 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C08D40.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01210AF0, oracle length 34 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x5c) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c0/CSystem_LoadBank_00c08d40.cpp"