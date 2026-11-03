// EH unwind funclet @ 0x01212D7A of CEngineVSConstantBlockSet::~CEngineVSConstantBlockSet (parent @ 0x00C22520): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 14 bytes at +0xce of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136C654 lists 0x01212D7A in its unwind map; its __ehhandler thunk
// 0x01212E2E (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C22520.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01212D7A, oracle length 14 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0xce) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c2/CEngineVSConstantBlockSet_CEngineVSConstantBlockSet_00c22520.cpp"