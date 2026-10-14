// EH unwind funclet @ 0x01211B70 of CVtable_012b29b0::CVtable012b29b0_CVtable012b29b0_00c15900 (parent @ 0x00C15900): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x0 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01211B70, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x0) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c1/CVtable012b29b0_CVtable012b29b0_00c15900.cpp"