# Batch `2026-10-14_linux_eh18` — hand-authored small EH-framed bodies: 5 bodies (5 landed)

Eighteenth hand pass (Wine lane, `/EHsc`): two sequential scoped locks in one frame (the first
around a C-linkage flush that registers no EH state), a seven-argument event factory forward
under the lock, the resource-map destructor (`delete` on a non-virtual out-of-line destructor,
registry unregistration, inlined container member destructor), `HasAnimationTriggers` (esi
popped before the branch, ebx pushed only on the query path) and a bounded float element getter
whose array element is parked in the dead argument slot across the lock's destructor (the
out-of-range branch must come first in the source to keep retail's block order). Parked:
0x00C2CBD0 (script constructor: argument load scheduled after the `this` parking), 0x00CAF500
(first member constructor: `lea` / `push` order), 0x00C48430 (eight unexplained frame bytes
below the request record).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets22`.
