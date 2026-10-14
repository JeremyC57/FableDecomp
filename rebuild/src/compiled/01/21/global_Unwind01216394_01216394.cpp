// EH unwind funclet @ 0x01216394 of CWound::CWound_CWound00c5eca0dtor_00c5eca0 (parent @ 0x00C5ECA0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x34 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01216394, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x34) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c5/CWound_CWound00c5eca0dtor_00c5eca0.cpp"