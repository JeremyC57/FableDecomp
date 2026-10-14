# Batch `2026-10-14_linux_eh14` — hand-authored small EH-framed bodies: 9 bodies (9 landed)

Fourteenth hand pass (Wine lane, `/EHsc`): a two-argument transfer and a one-argument setter of
`CEditControlCentre` / `CPersistContext`, the `CArtResourceSet` destructor (an out-of-line reset at
state 1 before the member destructors), the `CScriptModule` destructor (member cleared, an import
called with no state change), a sprite update reaching through two objects, the two-argument
factory, the `CClothingSuitPartMap` query (result kept in esi across the lock destructor), the
`TeleportData` constructor (four 0x14-byte members built out of line, EH state rising 0..3), and
`CFireballSpell::DispatchScaledDamage` (`fld [a] ; fmul [b]` is `b * a`). Parked: 0x00C0A9A0
(109 v 109 register choice), 0x00C2A3D0 (107 v 113), 0x00C15CD0 (127 v 114 inlining depth),
0x00CA9D00 (extra frame slot), see `docs/journal/FINDINGS_LOG.md`.

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets18`.
