// EH unwind funclet @ 0x01215FDE of PKRACMasterObject::PKRACMasterObject_CMasterObjectLoadScriptInfo00c4c8b0_00c4c8b0 (parent @ 0x00C4C8B0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 8 bytes at +0x1b of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01215FDE, oracle length 8 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x1b) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c4/PKRACMasterObject_CMasterObjectLoadScriptInfo00c4c8b0_00c4c8b0.cpp"