# NF121 notes (round 10, PRIORITY reconciliation: the CTCCarrying family q4469..q4473, q4984)

Counts (from STATUS.tsv / APPLIED.tsv): 58 rows listed (57 in rows.txt + 0x006A1E00): 13 ported, 44 not-a-binder, 0 blocked,
1 deferred (0x006A1E00, held by NF118). 6 entries: 3 done (q4470, q4471, q4472), 2 done-already (q4473, q4984), 1 deferred (q4469,
held by NF118). Final recheck: recheck_final.log, one vland --recheck-landed run over the 13 ported rows: 13 RELOCATION_MATCH, all PASS.

## Method
- One HEAD recheck over all 58 rows (work/head.log), with each object's (symbol, target) pairs dumped by reloc_identity.check()
  (work/head_binds.txt, summary work/head_targets.txt). This gave the exact wrong-binder list; 0x004804DE turned out to be no binder
  at all (the exe-scan hit at 0x00482563 is the byte run 60 10 6A 00 inside `or dword ptr [ebp+60h],10h ; push 0`).
- The four HEAD self rows were graded once more to read their defined symbols (work/selfhead_binds.txt): these are the RENAMES old leaves.
- Claims re-verified: Ego_r.pdb and FableWin.pdb publics carry exactly the claimed spellings and no CThing* / non-const variants;
  the bodies compare GetPItem() of each carry slot with the thing's address (the const reference is the pushed pointer), so the bytes
  are the same; the callers on engine/CTCCarrying.h already bound the claims.
- Every change is an in-place respelling inside the row's existing local CTCCarrying model (round-9 rule: accepted). No new local
  model, no new cast token, no header change. Caveat: 0x006A1490's local key wrapper ctor now takes `const CThing*`, so its existing
  C cast `(CBaseObject*)item` (unchanged text) now also drops const. Self-row tests are standalone and unchanged (all PASS).
- work/respell.py is the transformer (declaration -> `const CThing&` const, call argument dereferenced); proven on the first rows and
  applied to 9 binders, all graded in one payload (work/b1.log). It wrongly dereferenced the (long) overload call in 0x006648D0; that
  one line was reverted by hand before any grade.

## Splits (before -> after, HEAD binds + after binds; no split created)
- 0x006A1060 IsCarryingThing: 3 spellings (QBE_NABV 9 rows, QAE_NPAV 9 rows, QBE_NPAV 1 row) -> 2 (QBE_NABV 18 rows incl. self;
  QAE_NPAV only 0x006A1E00, held by NF118). NAME_FIXES has the one-line recipe for that row.
  Choice recorded for the coordinator: the self leaf was renamed although 0x006A1E00 cannot move in this hand-back. The brief's
  specifics allow a split to shrink (the claim was already bound by 9 rows, so no spelling is new). If the round-9 "rename waits"
  rule is read strictly, pull 0x006A1060's self row out of the landing (its rows/ file stands alone); the 8 binder respells still
  shrink the split on their own.
- 0x006A1490: 3 -> 1. 0x006A1120: 2 -> 1. 0x006A19B0: 2 -> 1 (the old spelling was only the self row's own leaf).
- 0x006A11F0 / 0x006A1540: already 1 spelling (15 rows each incl. self).

## Seen, not touched (outside the six addresses)
- 0x006A10A0 IsCarryingThing(long): row 0x006648D0's local model binds ?IsCarryingThing@CTCCarrying@@QAE_NJ@Z; Ego_r / FableWin and
  engine/CTCCarrying.h say QBE_NJ. Not respelled (no REFS scan for that address; it could create a split). NAME_FIXES line.
- Pre-existing vland RELOC_NAME warnings in the ported rows (stand-in callee names, unchanged by this hand-back): 0x006647F0
  `GetWeapons_006a22d0` -> 0x006A22D0 (catalog RemoveAllThingsInPrimarySlot); 0x0069F9E0 / 0x0078E9C0 / 0x00793EC0 / 0x008BA3C0
  `CTC1A::OnNotCarried` / `CTCShowableItem::OnShowEnded` -> 0x005D8E50 (catalog CTCInventoryItem::ReturnToInventory); 0x0069F9E0
  PrepareWeaponSheathing -> 0x005C552C; 0x008BA3C0 IsPrimarySlotEmpty -> 0x006A1270 (IsPrimarySlotFree), UpdateFrontMode -> 0x0073A840,
  CTCEnflame::ResetState -> 0x007A44C0. Candidates for a later naming pass.

## Funclets
- No landed EH funclet row includes any of the 13 changed files (grep of rebuild/src/compiled for each file name and of 01/21 for the
  addresses). The four self renames change only the mangled leaf; their catalog names (module + name) are unchanged, so no file moves.

## Descriptive names
- None added. All names are PDB spellings.

## Flags lines
- None (no row needed new flags).

## Checks
- Full (target, symbol) relocation diff, HEAD probe vs work/recheck_final_binds.txt: the only binding changes are the ten RENAMES
  section-2 lines; the four self rows change only their own leaf and local-model helper symbols (FindThing / FindValue / begin / end /
  the key ctor). Before/after spelling sets recomputed from the final run: as listed under Splits.
- Account B: none of the 13 changed addresses appears in UNMATCHED_ROWS.tsv; COORDINATION_CLAIMS.tsv lists only Account A claims
  for q4469..q4473 / q4984 (including this agent's). NF118 has no rows/ copy of 0x006A1E00, so the 0x006A1060 split stays until that
  row is respelled (NAME_FIXES recipe).

## Rule slips
- One Bash call exited 1 (the first build_candidates.ps1 grep loop, case-sensitive addresses).
- STATUS.tsv / APPLIED.tsv were written once all rows were graded (one batch), not after every row.
- work/respell.py was applied to all nine binders at once without a hand proof on 2-3 rows first; its stray `*` on the (long)
  overload call in 0x006648D0 was caught by the diff and reverted before grading.
- 0x006A1E00 (held by NF118) was compiled read-only in the HEAD probe (my own work dir, repo source, no edit); STATUS counts that grade.
- Bash calls did not all start with the lane line verbatim (helper scripts work/vl.sh / probe.sh source it themselves; read-only
  calls cd'd directly). Every compile ran inside `timeout`, one at a time; no git; nothing written under /home/user/FableDecomp.
