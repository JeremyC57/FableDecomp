// $E_REACTION_MANAGER_DEF_INSTANTIATION_NAME() -- dynamic initializer of the anonymous-namespace CCharString REACTION_MANAGER_DEF_INSTANTIATION_NAME ("OPINION_REACTION_MANAGER_INSTANCE"): construct it from the literal (length -1), then register its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121AC08, 29 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x013743A8, plain `ret`; registers the atexit destructor 0x0122901F (`mov ecx,0x013B8BC4 ; jmp ~CCharString`).
// Identity (proven): Ego_r.pdb lists 33 and FableWin.pdb 33+ per-TU copies of `anonymous namespace'::`dynamic
//   initializer for 'REACTION_MANAGER_DEF_INSTANTIATION_NAME'' (a header constant in an unnamed namespace); retail's
//   .CRT$XCU holds 32 CCharString initializers of the literal "OPINION_REACTION_MANAGER_INSTANCE" (the def instantiation
//   name of the opinion reaction manager), one per including TU; in the trap TU the order REACTION_MANAGER_DEF_
//   INSTANTIATION_NAME -> STrapUntriggeredAnimName .. STrapCycle matches retail 0x0121F000 -> 0x0121F01D .. 0x0121F074
//   literal for literal. This copy (object 0x013B8BC4) opens the TU after the weapons/augmentations screen strings.
//   Nothing in retail reads this copy (only the initializer and its destructor reference 0x013B8BC4).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, _atexit 0x004012BC.
// Shape: the compiler-generated initializer of a namespace-scope object with a constructor and destructor.
#include "fable_string.h"      // CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0

namespace {
CCharString REACTION_MANAGER_DEF_INSTANTIATION_NAME("OPINION_REACTION_MANAGER_INSTANCE", -1);
}