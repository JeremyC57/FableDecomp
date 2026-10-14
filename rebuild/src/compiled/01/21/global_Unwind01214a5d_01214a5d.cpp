// EH unwind funclet @ 0x01214A5D of CActivationReceptor::CActivationReceptor_CActivationReceptor00c38b20dtor_00c38b20 (parent @ 0x00C38B20): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x13 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01214A5D, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x13) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c3/CActivationReceptor_CActivationReceptor00c38b20dtor_00c38b20.cpp"