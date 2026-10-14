// EH unwind funclet @ 0x012112C0 of CVtable_012b23a0::CVtable012b23a0_CVtable012b23a0_00c0fee0 (parent @ 0x00C0FEE0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x0 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012112C0, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x0) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c0/CVtable012b23a0_CVtable012b23a0_00c0fee0.cpp"