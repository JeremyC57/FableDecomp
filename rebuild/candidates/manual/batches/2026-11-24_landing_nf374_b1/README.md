# Landing nf374_b1 (2026-11-24): 28 binder rows call their callees by the exact PDB names (tracker T1, free slice)

Hand-back: `pending/2026-11-24_nf374_handback/` (NOTES.md, LESSONS.md, STATUS.tsv for all 306 targets: 14 done, 12 done-partial, 157
skipped-port, 121 skipped-held, 2 identity). Per row: HEAD and new MATCH / RELOCATION_MATCH, test PASS at one path; objsec ALL_EQ except
the intended renames, `.drectve` byte-equal; both pick-any checkers HEAD = new, 0 reach flips; /FORCE link 0 duplicates, nothing HEAD
resolved left unresolved; every new spelling in both PDB name sets (or the row's own Leaf). land_scripted rechecked all 28 on the
landed tree and applied the 21 table lines (0 misses, no new multi-target symbol).

Hand table lines (`TABLE_CHANGES_hand.tsv`, applied after a dry run against today's table: 0 misses): seven rewitness lines and the
table-only file. The coordinator compiled every witness from the repo first (all RELOCATION_MATCH PASS): the seven rewitness rows still
bind the old spelling, the two delete witnesses no longer reference it, and 0x00AE2CB0 binds both PDB ApplyDragging / ApplyForces forms.
self_symbols: 0x007BB150's own symbol moved to `?SeeIfAnyOccupantsKnowAboutDeed@CTCTrespasser@@AAE_NJW4EOpinionDeedType@@@Z`.

Held (`pending/2026-11-24_nf374_handback/b1_hold/`). Rulings (advisor decision, owner delegated):
- 0x006C7960: held only because it bound an NF373 symbol; nf373 has landed, so it is regraded at HEAD and landed separately.
- 0x00939E70: `.drectve` equal under the rename is acceptable when the line count and order are unchanged, every changed operand is in the
  row's rename map and the /FORCE link resolves each alias to the same address as at HEAD. 0x00BFC770 stays held until 0x00BFD0B0's self
  respell (NAME_FIXES nf374) lands.
- 0x00485B44 / 0x00800D30 / 0x00757F90: ports (queued).
- 0x00AD77F0: identity (one decorated name cannot sit on two addresses; diff the bodies' call targets first), queued.
- Stale-witness table lines: a line stays only while a landed object binds its spelling, and its witness must be a row seen binding it
  (compiled, never a text grep). FinishAction UAE / GiveTutorialForCategory: rewitness after compiling a declaring row; SetPItem /
  AIAction_DoNothing: their rewitness rows above were compiled and do bind the old form.
- 0x009425F0 / 0x00942C70 (binder 0x00942EE0): identity research, queued.
178 skip lines queued as nf374 in T3_PORT_QUEUE (none duplicates an existing line). Next T1 pass: 0x0046B0D6, 0x00786700, 0x00752640,
0x007FDF60, 0x0052AC10, 0x00950C30, 0x00A9B740 still bind old spellings (now the table witnesses for them).
