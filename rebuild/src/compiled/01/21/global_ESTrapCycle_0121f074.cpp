// $E_STrapCycle() -- dynamic initializer of the anonymous-namespace CCharString STrapCycle ("TRAP_CONTINUOUS"): construct it from the literal (length -1), then register its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F074 (unaligned: an /Os-packed TU), 29 bytes, in `.text$yc`, reached
//   from the .CRT$XCU table entry 0x013745F0, plain `ret`; registers the atexit destructor 0x01229BA8
//   (`mov ecx,0x013BAE00 ; jmp ~CCharString`).
// Identity (proven): Ego_r.pdb 0x0101196D..0x010119C4 and FableWin.pdb 03FF46FB..03FF4776 run, in one TU after
//   REACTION_MANAGER_DEF_INSTANTIATION_NAME, the anonymous-namespace initializers STrapUntriggeredAnimName, STrapTrigger,
//   STrapReset, STrapCycle (Ego_r packs them 0x1D apart, as retail does); retail runs 0x0121F000 ("OPINION_REACTION_
//   MANAGER_INSTANCE") -> 0x0121F01D "TRAP_UNTRIGGERED" -> 0x0121F03A "TRAP_TRIGGER" -> 0x0121F057 "TRAP_RESET" ->
//   this 0x0121F074 "TRAP_CONTINUOUS" (0x1D apart): the fourth, STrapCycle, the trap's continuous-cycle animation name.
//   Its one reader is 0x0078819C. The preceding BOASTING_CROWD_* names match retail's Hook_BoastingCrowd /
//   BoastingCrowdMember / MK_BoastingCrowd as well.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, _atexit 0x004012BC.
// Shape: the compiler-generated initializer of a namespace-scope object with a constructor and destructor.
#include "fable_string.h"      // CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0

namespace {
CCharString STrapCycle("TRAP_CONTINUOUS", -1);
}