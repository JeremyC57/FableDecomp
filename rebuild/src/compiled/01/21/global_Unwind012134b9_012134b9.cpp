// EH unwind funclet @ 0x012134B9 of _unattributed::unattributed_CStackOwner00c287e0dtor_00c287e0 (parent @ 0x00C287E0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 14 bytes at +0x19 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012134B9, oracle length 14 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x19) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c2/unattributed_CStackOwner00c287e0dtor_00c287e0.cpp"