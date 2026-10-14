# Batch `2026-10-14_linux_hand22` — delegated hand chunk 22 (CConsoleCommandLine .. CCreatureActionBase): 39 bodies (39 landed)

Twenty-second delegated hand chunk of the Wine lane (`$S/w22/chunk22.tsv`, 40 small-module bodies
of 17–96 bytes authored by a background agent against the live VC7.1 compiler, re-proved through
the lander's own parity and behaviour checks): `NUISystem::CObservable`, `COk` and
`CControllerDisconnect` constructors, `CTCWobble(CThing*)`, four nested
`NScript::CPredicate_And<…CIsThingAlive…>` copy constructors (depth 2–5; the depth-3 level stays
out of line under the inliner's size budget), `CConversation` copy constructor and `operator=`
(retail 12-byte vectors), a critical-section vtable class pair, `CLockedQueue`, `CNamedRecord`
(STLport string), `CContainmentVolume` plane tests, `CCreatureActionBase` timing helpers and
several script-binding setters whose `this` is unused.

Label corrections and the globals identified (0x0122DEE0 = -1.0f, 0x01375550 = world fps,
0x0143E8F8 thing manager slots) are in the batch sources. Parked (register allocation only):
0x00C624C0.

All graded `RELOCATION_MATCH` / `MATCH` + `PASS` on 2026-10-14.
