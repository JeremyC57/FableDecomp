# Landing nf373 (b1, b2, b3; 2026-11-24): seven units, 22 rows, move NF370's held selves onto their PDB spellings with their binders

Hand-back: `pending/2026-11-24_nf373_handback/` (NOTES.md, STATUS.tsv, RENAMES.tsv, evidence/). Each unit lands whole (UNITS.tsv). Per
row: HEAD and new MATCH / RELOCATION_MATCH with the test passing at one path; objsec ALL_EQ under the row's renames; no `??_7` / `??_R`
symbol appears or changes; both pick-any checkers HEAD = new with 0 reach flips; a compile census of the 146 landed rows naming a unit
method (no member object keeps an old spelling); /FORCE link resolves the new spellings with no new duplicate; table `delete` lines for the
old spellings (a PDB line to the same target already exists), 0 misses.

| batch | unit (self first) | change |
|---|---|---|
| b1 | 0x006A4E90, 0x008374F0, 0x0083D070 | DrawGetEditDrawInfo QBE -> UBE (virtual, CThing slot 25; the two overrides move with it) |
| b2 | 0x00736590, 0x00736AD0 | UpdateUsableMeleeCombatAbilities QAE -> AAE |
| b2 | 0x006D5260, 0x006D5850 | CullNullFollowers QAE -> IAE |
| b2 | 0x006929E0, 0x0069FE60, 0x00692B30 | SetMoveToPos QAE -> IAE |
| b2 | 0x005BD4EA, 0x005F05E5 | OpenInventory QAE -> UAE (RTTI slot 30) |
| b3 | 0x00693030, 0x00693B30, 0x008430B0 | SetAnimation QAE -> IAE |
| b3 | 0x0099EAF0, 0x0099F0A0, 0x0099ED10, 0x0099EE20, 0x0099F100, 0x0099F180, 0x0099F400 | MakeOwnCopyOfData QAE -> IAE; 0x0099F0A0 ported from an extern "C" stub to the member `??YCCharString@@QAEABV0@ABV0@@Z` |

Coordinator checks before landing: every shared header already declares these methods with the PDB access (CTCCombat.h private,
CTCFollowed.h / CCreatureActionBase.h protected, CThing.h / CTCInventory.h virtual), so header-reached callers already bind the new
spellings; `operator+=(const CCharString&)` is declaration-only in fable_string.h and no landed row defines it (the port adds no second
definition); the four rows NF373 did not compile (0x0099B560, 0x00692F30, 0x0058578B, 0x005A04F7) were built from the repo: RELOCATION_MATCH
PASS, no old spelling in their objects. After landing: the graded symbol equals the `// Leaf:` for every renamed row (the non-self
members included); self_symbols updated for the 11 renamed rows; bindings_multi_check at baseline.

Rulings (advisor decision, owner delegated): (0) u8 lands in the slot-25 form (no object emits or changes a vtable; this supersedes NF290
entry 6's "table model change" objection; the appended-slot variant is dropped, retail has no virtual past slot 29 for it); (1) 0x00693B30
takes the access edit outside the nf293 family unit, which stays queued; (2) 0x0099ED10 lands and the nf294 line is re-added; the
0x0099F0A0 member port is confirmed by the checks above; (3) u5 / u9 / u2 / u7 land while their T3 lines stay queued for the other rows'
ports. T3 queue: nf290 0x006A4E90 and access1 0x006A4E90 closed (b1), nf370 0x006929E0 and nf288 0x006D5260 closed (b2), seven lines
re-added in place from T3_READD.tsv with only their NF373 clause updated, nf293 unchanged. Skipped as owner question 2 (a strong copy
of the PDB symbol in another row): 0x00B80AD0 (0x00B80F90 / 0x00B80FE0) and 0x0055FB80 (0x00560D00); queued as nf373.
