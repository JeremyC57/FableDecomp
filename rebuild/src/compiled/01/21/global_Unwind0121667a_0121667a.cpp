// EH unwind funclet @ 0x0121667A of CTCKraken::CTCKraken_CTCKraken00c6a780dtor_00c6a780 (parent @ 0x00C6A780): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 14 bytes at +0x3a of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121667A, oracle length 14 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x3a) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c6/CTCKraken_CTCKraken00c6a780dtor_00c6a780.cpp"