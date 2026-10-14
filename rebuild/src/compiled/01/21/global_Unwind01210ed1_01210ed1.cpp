// EH unwind funclet @ 0x01210ED1 of CAbilitySoundComponent::CAbilitySoundComponent_CAbilitySoundComponent00c0b200ctor_00c0b200 (parent @ 0x00C0B200): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 24 bytes at +0x18 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01210ED1, oracle length 24 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x18) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c0/CAbilitySoundComponent_CAbilitySoundComponent00c0b200ctor_00c0b200.cpp"