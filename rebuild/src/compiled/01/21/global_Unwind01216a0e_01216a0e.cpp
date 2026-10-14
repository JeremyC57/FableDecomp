// EH unwind funclet @ 0x01216A0E of CVolume::CVolume_CVolume00ca6370dtor_00ca6370 (parent @ 0x00CA6370): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x1e of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01216A0E, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x1e) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/ca/CVolume_CVolume00ca6370dtor_00ca6370.cpp"