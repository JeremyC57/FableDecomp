// EH unwind funclet @ 0x012135EA of CTCDPhysicalObstruction::CTCDPhysicalObstruction_CTCDPhysicalObstruction00c2a8c0ctor_00c2a8c0 (parent @ 0x00C2A8C0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x1b of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012135EA, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x1b) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c2/CTCDPhysicalObstruction_CTCDPhysicalObstruction00c2a8c0ctor_00c2a8c0.cpp"