# Landing nf372_b1 (2026-11-24): three counted-pointer binders call their PDB DeleteData, plus one table-only fix

Hand-back: `pending/2026-11-24_nf372_handback/` (NOTES.md, STATUS.tsv, 169 targets: 3 done, 1 done-partial, 148 skipped-held, 17 port).
The three rows are NF309's parked unit-5 texts, re-measured at HEAD: RELOCATION_MATCH PASS before and after, objsec ALL_EQ under the one
rename, pick-any HEAD = new, link of binders plus selves resolves DeleteData with 0 duplicates.
- 0x00618660: `?Destroy_006174f0` -> `?DeleteData@?$CCountedPointer@V?$vector@_KV?$allocator@_K@std@@@std@@@@SIXPAX@Z`
- 0x0085A840: `?Destroy_0085a5a0` -> `?DeleteData@?$CCountedPointer@VCCombatActionBase@@@@SIXPAX@Z` (partial: 0x00905FD0 keeps the stand-in;
  compiled from the repo, its object leaves `?Destroy_0085a5a0@@YIXPAX@Z` undefined, so the table line is rewitnessed to it)
- 0x00AC3A40: `?Destroy_00ac3a10` -> `?DeleteData@?$CCountedPointer@V?$CArray@VC3DTriangle2@@@@@@SIXPAX@Z`
- `TABLE_CHANGES_hand.tsv` (applied by hand): the rewitness above, and 0x004D5ECA -> `??0CCarriedReadableDef@@QAE@XZ` (witness 0x004D92B0
  constructs the real class; no landed source names `CClass_004d5eca`).

T3 queue: the nf309 0x0085A840 line closed by `--release-t3 nf309` (NF284 D4 defers these twelve as a harder unit; it does not require the
binders to move together); 0x006174F0 and 0x00AC3A10 removed from the nf284 deferred-12 line, 0x0085A5A0 stays; the nf285 0x004D5ECA line
closed (`T3_CLOSED.tsv`). 28 skip lines queued as nf372. Rulings (advisor decision, owner delegated): entries 37 (vtable change), 121
(identity) and 164 (class-model switch) are queued, not ruled; the libpk / CODA entries are not respelled (propagated label
`??0C3DAnimationUtil@@QAE@XZ`, no PDB or RTTI).
