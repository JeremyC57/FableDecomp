# Batch `2026-10-14_linux_eh29` — hand-authored small EH-framed bodies: 7 bodies (7 landed)

Twenty-ninth hand pass (Wine lane, `/EHsc`): the parry-block event factory (a map lookup on the
target with `this` = the target, a 0x4c-byte event homed in the dead target slot, two dispatcher
registrations), the creature-opinion state constructor (eight arguments, a 16-byte POD bounds
copy, a 0x12B5328 entry container, the two flags packed branchlessly), the `~C3DMesh2` destructor
(each owned part cleared inside its own `if (p) { delete p; p = 0; }`), the character-morph and
screen-effect action factories (`__stdcall` error hook, error recorded once in the global at
0x0143BB48, class `operator new`, inline derived constructors), the worker-thread constructor
(`CreateThread` on the landed 0x00C35510 procedure, "Problems starting thread" warning, optional
`SetThreadPriority`) and the `~CCombatAbilityStrafeDef` destructor mirroring the landed
constructor (four inline container destructors, the buffer freed, the `CNavButton` destroyed).

Lessons kept: an error hook called through a global function pointer without `add esp,4` is
`__stdcall`; `if (p) { delete p; p = 0; }` keeps the clear inside the test; a dead branch jumping
straight into a merged error block is retail tail-merging the DDK does not reproduce. Parked:
0x00C39130 (the DDK stores the base's EH state before a non-throwing member constructor),
0x00CAACA0 (second format store sunk past the state compare), 0x00C3DA60 (tail-merged error
block), 0x00C172F0 (key loaded early, dead `lea esi` before the attach call).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets34`
and `_funclets34b`.
