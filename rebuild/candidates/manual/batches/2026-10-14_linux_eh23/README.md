# Batch `2026-10-14_linux_eh23` — hand-authored small EH-framed bodies: 5 bodies (5 landed)

Twenty-third hand pass (Wine lane, `/EHsc`): the reaction-queue pop (`CReactionManager`, node
unlinked under a one-word critical-section holder and returned through the dead argument slot),
the `CVsyncCallbackTimer` constructor (two 0x12B1AB0-family containers plus a `CSectionLock`
member), the `CTCActivationTrigger` destructor (derived vtable store hoisted above the member
container destructors), a death handler reading its target through a re-read `m_pSource` (the
null-checked conversion stays separate from the unchecked member call) and a renderer
`Initialize` whose failure path is the "null-object warning" family: a nested block building an
MFC `CString` through `Append`/`AppendNumber` and handing it to the log sink, followed by an
empty-class temporary (`0x00C16750`) homed on a dead byte.

Lessons kept: the warning family is its OWN nested block followed by the empty temporary, never
one statement; `m_pSource->` re-read at each use keeps retail's two loads. Parked: 0x00C14560 /
0x00C14630 (retail materialises a shared state constant in a register, the DDK does not).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets28`.
