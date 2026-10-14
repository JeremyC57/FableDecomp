# Batch `2026-10-14_linux_eh30` — hand-authored small EH-framed bodies: 15 bodies (15 landed)

Thirtieth hand pass (Wine lane, `/EHsc`): the two auto-delete-array `Resize` twins (0x00C169D0 /
0x00C31810: the "EMEM" warning, the copy count as `keep = count; if (m_count < count) keep =
m_count;`), the buffered-reader `Seek` (window test, look-behind pull), the two checked `At`
accessor twins (0x00C16B10 / 0x00C2EBC0: "Index <i> is out of range (<n>)" through the unsigned
`AppendNumber`), the mailbox `Dispatch` (event wait, "the queue is out of step" warning, the
message returned), the map-who `CreateInstance` (find key on the frame, 16-byte position with the
flag cleared first, the landed 0x00C37C90 entry constructor), the `__fastcall` bitset describer
returning an MFC-style string by value (hidden result in `ecx`, the loop index declared before the
string), the audio thread-owner `Attach` (`DuplicateHandle`, custom HRESULTs) and
`WaitForAsyncCompletion` (section released around `WaitForMultipleObjects`, a `switch` on the wait
result), the animation-table `GetMaxDamageInAnimation` (running maximum homed in the dead key slot,
the damage getter called twice), the `~CCreatureActionHeroJump` destructor (`Sleep(200)`, raw
`delete` of a trivial block, slot-0 and slot-1 virtual destructors, a container member destroyed
inline), the master-object `Finalize` (`delete this` through a non-virtual destructor), the
`~CTCKraken` destructor (ten EH states, `__ehvec_dtor` over four 0x14-byte elements, three
counted-reference member classes) and the hero `ReleaseTarget` sweep (four-byte `IsActive` result,
the key passed by address from the dead argument slot).

Lessons kept: the ternary `min` and the Dinkumware `min` helper both schedule the compare
differently from `keep = a; if (b < a) keep = b;`; an error-code chain of three values is a
`switch` when the fall-through case comes first in retail; the DDK out-lines an implicit or inline
`operator=` of a record that owns a member with its own assignment (write the memberwise copy in
the body); `wchar_t` is a typedef under the era defaults. Parked with evidence (see
`docs/pipeline/GOTCHAS.md`): 0x00C14E40 (owner argument loaded before `push esi`), 0x00C1A870 and
0x00C1A110 (the DDK does not share a frame slot between sibling-scope locals), 0x00C11AC0 and
0x00C88780 (register colouring), 0x00C11230 (return-0 exit laid out inline), 0x00CAF8F0 (64-bit
stamp loads interleaved), 0x00CAB370 / 0x00CAC070 (`fstp` of the double hoisted) and 0x00C48E00
(zero register for four uses).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets35`.
