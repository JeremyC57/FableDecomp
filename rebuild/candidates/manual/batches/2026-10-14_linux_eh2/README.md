# Batch `2026-10-14_linux_eh2` — hand-authored small EH-framed constructors: 6 bodies (6 landed)

Second hand pass over the smallest open EH-framed bodies (Wine lane, `/EHsc`): the three
`CAIStateGroupBase` constructors (out-of-line root constructor, own vtable, one member built out of
line at +0x30 / +0x1f0 / +0x2f8), two intelligent-pointer constructors whose base zeroes its two
pointers inline before an out-of-line `Set` (one from a thing, one copy-constructing through `Get`),
and a non-polymorphic two-level constructor (`CThingMapManager`-labelled) whose member is built
from `(this, argument)`. Parked from the same pass: `CTargetPoint::~CTargetPoint` @ 0x00C202F0
(74 v 74 bytes: the retail body stores the derived vtable *after* the member destructor call,
where VC7.1 here stores it at entry -- see `docs/journal/FINDINGS_LOG.md`).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; their unwind funclets land through
`families/funclets.py` (batch `2026-10-14_linux_funclets7`).
