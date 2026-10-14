# Batch `2026-10-14_linux_eh31` — hand-authored EH-framed bodies: 10 bodies (9 landed here, the tenth in `_eh31_b`)

Thirty-first hand pass (Wine lane, `/EHsc`): the playback `LogStatus` (an MFC-style string
returned by value: a frame string assigned "CODA reports source IS playing" and appended
position/length, or the "NOT playing" literal constructed straight into the result), the
`CTCKraken` constructor mirroring the landed destructor (nine states, `__ehvec_ctor`, the sound
source's base assigning its fields in its body), the audio-bank `LoadAsync` (COM `QueryInterface`
into a one-word smart pointer, slot-3 accept test, synchronous fallback, engine queue), the
`~CFileManager` destructor (quit request with key -1 retried with `Sleep(100)` and a warning without
trace, event wait, thread handle closed) and its constructor (`CreateThread` over the 0x00C1AC30
procedure, request queue root/node with the root address homed for its unwind, "Could not create
thread"), the instance-manager `Assign` (two 0x180-byte slots scanned, "Cannot be!" warning in the
dead argument slot, the theme record copied through its implicit assignment), the
`CAbilitySoundComponent` constructor (sixteen slots reset in retail's store order, "G<i>" / "P<j>"
names built from temporaries), the `CSBAgents` constructor (two warnings homed in the dead owner
slot), the `~CCodaFrameStreamer` destructor (a profiling lock with no EH state because the CODA
helpers are `extern "C"`, frames popped and deleted, "Null frame") and the animation-group
initialiser (two class-allocated groups with virtual destructors, the second raw block homed in the
dead argument slot).

Lessons kept: a `__stdcall` helper called inside a lock scope that needs no EH state is
`extern "C"` (non-throwing under `/EHsc`); a record whose member owns an out-of-line assignment is
assigned whole (`m_theme = *theme`) and the DDK inlines it; a string temporary chained into
`AppendNumber` reuses the constructor's returned `this`; a base's field stores that precede its
derived vptr store come from the base constructor's body, not its init list. Parked with evidence
(`docs/pipeline/GOTCHAS.md`): 0x00CA6450, 0x00CA67B0, 0x00CA68F0, 0x00C131C0, 0x00C42DD0
(register colouring, lazy saves, tail-merged epilogues), 0x00CAB160 and 0x00C3BB50 (the `/GS` lane
gap), 0x00C3F800 (array-new zero-count special case).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets36`.
