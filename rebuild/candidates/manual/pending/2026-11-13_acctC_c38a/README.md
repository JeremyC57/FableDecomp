# acctC hand-back c38a (agentC38, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x006096E0 | CTCInventoryMap::ConstructLogbookListPC | 12,832 (code to `ret 4` 0x0060C8DA + pad + 8-entry switch table) | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`. The siblings
0x0061E6D0 (c28a) and 0x00601310 (c31a) were regraded by the coordinator on this overlay: both still RELOCATION_MATCH.
No compiler flag (the old draft's `/d2inlT71` is gone), no pragma device, no new header opt-in (the draft's seven dead
opt-ins map onto existing ones; list in NOTES.md).

REVIEW ITEM FOR THE OWNER — one FITTED spelling: the count loop is written
`CLogEntries::iterator it = entries.begin(); if (it != entries.end()) for (; it != entries.end(); ++it)`.
The guard is redundant; c2 folds it to no bytes, but it halves the loop's static frequency (5 -> 2.5), and only then does
VC7.1 call the tree `operator++` 0x0047B3E0 at 0x0060B0DB instead of expanding it (score 72 v threshold 73 without it:
DIFFER(12880v12832), one call missing). Six natural spellings still expand it. A guard of the same kind is already landed in
the console twin 0x00607330. Accept, or send back for a natural text.
Also: the STL layer set (`LIST_BASE_CONVERT_ALLOCATOR` + `MALLOC_ALLOC_ALLOCATE` + `CCOUNTEDPOINTER_RELEASEDATA`) is the
inferred one of sibling 0x00601310 (see c31a README); the test is the earlier chunk's model test updated, no mutants run.

`open/00652fa4.cpp`: CDrawQuickAccessIcons::Draw DIFFER(4734v4653), first compiling draft (successor agentC39).
