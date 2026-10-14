// EH unwind funclet @ 0x01215820 of _unattributed::unattributed_CMailboxTake00c48860_00c48860 (parent @ 0x00C48860): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 25 bytes at +0x10 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01215820, oracle length 25 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x10) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c4/unattributed_CMailboxTake00c48860_00c48860.cpp"