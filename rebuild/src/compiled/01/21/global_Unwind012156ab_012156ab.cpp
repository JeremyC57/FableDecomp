// EH unwind funclet @ 0x012156AB of ~CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::~CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive> (parent @ 0x00C473F0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0xb of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Parent evidence: retail FuncInfo 0x0136F9D0 lists 0x012156AB in its unwind map; its __ehhandler thunk
// 0x012156C6 (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x00C473F0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012156AB, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0xb) and the parent's
//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).
#include "../src/compiled/00/c4/unattributed_CPredicateOwner00c473f0dtor_00c473f0.cpp"