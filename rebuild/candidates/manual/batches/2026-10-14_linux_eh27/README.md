# Batch `2026-10-14_linux_eh27` — hand-authored small EH-framed bodies: 10 bodies (10 landed)

Twenty-seventh hand pass (Wine lane, `/EHsc`): the `~CNavButton` destructor (`novtable`
most-derived class draining two containers; the virtual `First()` on `this` through a
non-virtual helper stays a dynamic dispatch), the combat hit-ratio calculator (`fild`/`fidiv`
under the module's profiling lock), the `PKRACBuffer` alignment check (unsigned `AppendNumber`
overload @ 0x00C0DF10), the resource-map owner constructor matching the landed 0x00C36590
destructor (a float argument re-read from its slot), the combat state lookup (a 0x10-byte key
whose `Set` lives on its +4 value part), MFC `CString::Right` (two inlined null-object checks,
the result built through `AllocCopy` and the hidden-result flag), the checked pointer-array
visitor walk, the `__cdecl` master-object factory (class `operator new`, a 0xa0-byte record
assignment on failure, `delete` through the virtual destructor), the `CCombatAbilityStrafeDef`
constructor (a `CNavButton` member, a three-word buffer, four containers with distinct roots
whose destructors are out of line) and the `PKCAutoMap` entry factory with the "EMEM" warning.

Lessons kept: a member pointer read twice around a store through it is one named local
(`CPart* part = m_pPart`); a float argument used twice is re-read from its slot where a `long`
would be kept in a register; `q.py` needs the mangled leaf (`"??1Class"`) when a helper name
contains the oracle's leaf; a behaviour test must initialise every pointer field the body reads
(VC7.1's heap is not zeroed, g++'s often is). Parked: 0x00CAB770 / 0x00CAC300 (twin constructors:
the DDK keeps the three derived vtable stores above the section-member push), 0x00C14F90 (the
shared holder vtable constant loaded before the final member clear), 0x00CAFE90 / 0x00C425E0
(register allocation), 0x00C101D0 (the empty-string temporary's data register).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets32`
and `_funclets32b`.
