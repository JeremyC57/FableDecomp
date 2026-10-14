// EH unwind funclet @ 0x0121112B of CCombatAbilityStrafeDef::CCombatAbilityStrafeDef_CCombatAbilityStrafeDef00c0edf0ctor_00c0edf0 (parent @ 0x00C0EDF0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This funclet is 11 bytes at +0x26 of the parent's
// block and is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121112B, oracle length 11 bytes.
// Grade: landed -- byte-exact (the parent's object, funclet block offset 0x26) and the parent's
//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).
#include "../src/compiled/00/c0/CCombatAbilityStrafeDef_CCombatAbilityStrafeDef00c0edf0ctor_00c0edf0.cpp"