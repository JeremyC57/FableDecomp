# Batch `2026-10-14_linux_eh28` — hand-authored small EH-framed bodies: 9 bodies (9 landed)

Twenty-eighth hand pass (Wine lane, `/EHsc`): two more `PKCAutoMap` entry factories (0x20- and
0x30-byte entries built from the key read through its pointer, "EMEM" warning), the `CVolume`
constructor matching the landed 0x00CA6370 destructor (four-argument base, three event holders,
three critical sections, `SetEvent` on the third holder; the side bases are `novtable` so only
the derived vtables are stored), the 0x28-byte red-black tree node constructor shared by the
module's containers (shared or owned sentinel, self-linked embedded link) and the 8-byte
morphable node holder (tree or flat node through a ternary over a common base), the mailbox
by-value `Take` (semaphore, profiling lock on `this`, "out of step" warning, the result popped
straight into the hidden pointer), the master-object receiver (`m_expected == ++m_received`
operand order), the combat block resolver (a 0x50-byte query record whose float zeros are
immediates and whose integer zeros share the zero register) and the async-decoder constructor
(an 8-byte thread parameter record stored flags-first).

Lessons kept: float members zeroed in an init list take immediate stores while integer zeros
reuse the zero register; a two-field record's store order follows the source statements; a
`novtable` side base keeps its own vtable store out of the derived constructor. Parked:
0x00CAF580 (an extra callee-saved register and a rotated second drain loop).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets33`.
