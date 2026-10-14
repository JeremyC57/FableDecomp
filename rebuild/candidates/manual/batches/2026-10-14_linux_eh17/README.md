# Batch `2026-10-14_linux_eh17` — hand-authored small EH-framed bodies: 7 bodies (7 landed)

Seventeenth hand pass (Wine lane, `/EHsc`): a theme setter whose 84-byte record assignment is the
intrinsic `rep movsd`, a registry walk (`element = FindNext(key)` peeled loop) updating each
element with `this`, the block/parry event dispatch to two agents followed by `delete event`
through the virtual destructor, the `WaypointData` constructor (`__ehvec_ctor` over two class
arrays, counterpart of the eh15 destructor), the outline-glow effect destructor (two inline block
holders released last to first, registry unregistration), the `CTCDiceGame` constructor (two
inline-constructed container members, root node pointer plus out-of-line node) and the
`CCombatAbilityStrafeDef` constructor (two container members at +0 and +0x10: the earlier
"missing EH state" DIFFER was a wrong hierarchy, not a compiler difference).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets21`.
