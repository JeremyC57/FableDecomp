# NF148 notes (2026-11-20, round 10: q11817 CMemoryDataInputStream and q11785 ThrowShotObject one-definition follow-ups)

Counts: see the "Counts" section at the end (from APPLIED.tsv / STATUS.tsv / recheck_final.log).

## q11817 (0x00994930 PBEJ -> PBXK): done
- Coordinator decision applied: 0x00BF3B60 re-ported without its strong local `CMemoryDataInputStream::CMemoryDataInputStream(const void*,
  unsigned long)` definition; the declaration in candidates/landscape_patch_load_foreground_compiled.h stays (retail calls 0x00994930 out of
  line at 0x00BF3C2E and 0x00BF3D69; HEAD expanded it inline).
- Read8 / Read16 / Read32 kept, a judgment call (advisor agreed): retail expands the stream's inline read and calls
  CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (Ego_r 0x00AFBD30 + FableWin 0x03080F80) on overflow; the Read8/16/32 names are in no
  name source, and no other row defines them, so removing them would only add undefined made-up externals. They clash with nothing.
- Grade: HEAD DIFFER(809v1480) (vland b0, q.py) -> DIFFER(825v1480) (vland b1, q.py), test PASS both. Not worse: exact instruction matches
  against retail 47 -> 55 of 502 (work/sim.py on the q.py listings), and the two retail `call 0x994930` sites now exist (2 relocations to
  ??0CMemoryDataInputStream@@QAE@PBXK@Z). q.py grades LoadForegroundPatch itself in both (0x329 / 0x339-byte section); a `// Leaf:` comment
  line was added so the grader no longer has to guess.
- Defined-symbol diff (objdump -t, HEAD vs new 0x00BF3B60 object): only ??0CMemoryDataInputStream@@QAE@PBXK@Z removed (now undefined).
  NF146's bf3b60_head / bf3b60_decl objects show the same.
- Test: rows/00bf3b60_test.cpp adds a fake of that constructor on the candidates model (the test links the row object); PassPattern kept.
- 0x00994930: NF141's draft (work/NF141_round1/00994930_draft.cpp) unchanged, regraded RELOCATION_MATCH on today's tree; defined-symbol diff vs
  HEAD: own leaf only. Its test is the old standalone model (generic local classes, no row object) and passes unchanged; not rewritten,
  because the row itself keeps its HEAD local models (in-place respell, NF141's accepted form).
- Link proof (LINK= cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED; tools/lnk.sh; work/link/):
  - ctl_q11817.log: HEAD 0x00BF3B60 + new 0x00994930 + tree 0x009FF590 -> 1 LNK2005 (the ctor), exit 145.
  - new_q11817.log: new 0x00BF3B60 + new 0x00994930 + tree 0x009FF590 -> 0 LNK2005, exit 0.
- Wider one-definition check: no shared header has an inline body of that constructor, and after this hand-back no built row defines it
  except 0x00994930 (grep of rebuild/src/compiled + rebuild/include for non-declaration mentions of `CMemoryDataInputStream(const void*`;
  the only other hit is the unbuilt rebuild/src/compiled/00/bf/00bf3b60.pre.cpp, see below). Every REFS row's ctor mention, including the
  candidates headers of 0x00BDC180 / 0x00BDC510 / 0x00BE8920 / 0x00BE7D70, is a declaration.
- Census (no new split): REFS 36 rows. NF141 compiled 31 (all PBXK); since then 0x009FF590 (nf141) and 0x009C8FB0 (changed 08:00) re-probed
  here: PBXK; 0x00BF3B60 binds PBXK (2 relocations). 33 of 36 on PBXK; 0x00BDC510 (PAXK), 0x00BE8920 (PAEK), 0x00BE7D70 (no ctor declared)
  are the old no-Leaf landscape rows NF141 left as found. The address's spelling set shrinks (PBEJ, the old self leaf, leaves).
- Note for the coordinator: rebuild/src/compiled/00/bf/00bf3b60.pre.cpp (2026-11-12, not in build_candidates.ps1) is an older copy of
  0x00BF3B60 with `#pragma optimize("y",off)` and the same strong ctor. It is not built, so it does not clash; probably root scratch to delete.
- Known gap (authoring, not done): the candidates model `CMemoryDataInputStream { Data; Size; Position; }` (12 bytes) is not the retail class
  (0x1C, CDataInputStream base, engine/CMemoryDataInputStream.h). A port of 0x00BF3B60 onto the shared header is authoring (Account B).

## q11785 (one ??_7CCreatureAction_ThrowShotObject@@6B@): done
- Evidence for the overrides (two sources each):
  - slot 11 (+0x2C) = 0x00956960: retail vftable read (work/vt_retail.txt) + rtti-vtables.tsv slot 11 + Ego_r 0x0097B4B0 / FableWin 0x02BF6900
    `?Clone@CCreatureAction_ThrowShotObject@@UBEPAVCCreatureActionBase@@XZ`.
  - slot 12 (+0x30) = 0x008B66D0: retail vftable read + rtti-vtables.tsv slot 12 + Ego_r 0x008EF1D0 / FableWin 0x029FFB90
    `?Initialise@CCreatureAction_ThrowShotObject@@UAEXXZ`; the landed 0x008B66D0 already defines that leaf (objdump of its tree object).
    NF145 called this slot weaker; the FableWin cross-check it lacked is now made, so both slots are in.
- Header opt-ins (hdr/README.md): ThrowShotObject `_CLONE` + `_INITIALISE`, CCombatActionBase MELEE `_MELEE_COPYCTOR` (the base copy constructor
  0x0062F3B0, both PDBs; without it the implicit copy would expand memberwise).
- Rows (rows/; vland work/b2.log and recheck_final.log: 3 x RELOCATION_MATCH + PASS):
  - 0x009551A0: + the two defines. Defined symbols identical to HEAD.
  - 0x009566C0: NF145's hand-back text + the two defines. Defined symbols identical to NF145's object.
  - 0x00956960: ported onto the shared headers (MELEE + _MELEE_COPYCTOR + _CLASS + _CLONE + _INITIALISE, NF145's include set); own leaf
    `QAEPAVCCombatActionBase@@XZ` stand-in -> the PDB `UBEPAVCCreatureActionBase@@XZ`; matched on the first form. New defined symbol beyond
    the leaf: ??_GCCreatureAction_ThrowShotObject@@UAEPAXI@Z, the header class's implicit pick-any COMDAT (0x009551A0 / 0x009566C0 emit it
    too). Exe scan (work/refs2.tsv): no landed row references 0x00956960 (nor 0x008B66D0, 0x009568E0); only the vtable does.
- One vtable: the three objects' ??_7CCreatureAction_ThrowShotObject@@6B@ sections are byte- and relocation-identical (tools/coffsec.py,
  md5 4a2f8c08951f3c2f47028bbda1c4a50f for all three; work/vt_*_v1.txt). HEAD had three different contents (0x009551A0 with the inherited
  slots 11 / 12, 0x009566C0 / 0x00956960 a 4-byte placeholder).
- Link (work/link/new_q11785.log): the three new objects + tree 0x008B66D0 -> 0 LNK2005, and the ThrowShotObject Clone / Initialise
  references resolve (control ctl_q11785.log: HEAD objects, also 0 LNK2005 - COMDAT vtables never raise it, so the section identity
  above is the real proof; the control's unresolved inherited Clone / Initialise show the old slot contents).
- Sequencing: the family was drafted on NF145's hand-back copy while nf145 was landing. nf145 landed at 08:35-08:41
  (batches/2026-11-20_landing_nf145, which hands q11785 to NF148); the landed 0x009566C0 text is byte-equal to work/d/009566c0_nf145.cpp, both
  overlaid headers are unchanged in the tree (cmp with orig/), the 0x009566C0 lock read `released` and is now NF148's. The whole family was
  then regraded on the new tree in recheck_final.log (vtable section md5 4a2f8c08951f3c2f47028bbda1c4a50f in all three wd objects).
- Not done (beyond the brief, follow-up): retail's vtable has 81 slots (0x144 bytes); the MELEE branch gives 70 (0x118). The 11 missing are
  CCombatActionBase's own virtuals (the other branch's `_OWN_VIRTUALS` opt-in). ThrowShotObject also overrides slot 73 IsToFaceTarget
  (0x00956720) and slot 78 GetIsToFaceTargetCone (0x00956730), and slot 0 is 0x009568E0 (??_G). Those three landed rows define made-up free
  functions (`?CCreatureAction_ThrowShotObject_IsToFaceTarget@@YI_NXZ`, `?CCreatureAction_ThrowShotObject__GetIsToFaceTargetCone@@YIMPAX@Z`,
  a free scalar-deleting-destructor stand-in), so declaring those overrides now would create name splits. A follow-up can port
  0x00956720 / 0x00956730 / 0x009568E0 onto the header (FableWin names `?IsToFaceTarget@CCreatureAction_ThrowShotObject@@UBE_NXZ`,
  `?GetIsToFaceTargetCone@CCreatureAction_ThrowShotObject@@UBEMXZ`, `??_GCCreatureAction_ThrowShotObject@@UAEPAXI@Z`; Ego_r lacks the first
  two) together with a MELEE `_OWN_VIRTUALS` opt-in.

## Tests
- 0x00BF3B60: changed (fake ctor), PASS.
- 0x00994930, 0x009551A0, 0x009566C0, 0x00956960: unchanged, PASS. 0x00956960's 10-line test models generic Base / X classes (no
  shared-header class name), like 0x009566C0's (NF145's reason: a test linking the row object would have to define all 70 vtable slots).

## Header users / needs header claim
- See hdr/README.md. No existing shared-header member renamed or changed: needs header claim: none.

## Local classes
- No row gained a class definition. 0x00956960 lost its local CCombatActionBase / CCreatureAction_ThrowShotObject models.
  0x00BF3B60 and 0x00994930 keep their HEAD local models (candidates header; in-place respell).

## Flags lines
- None added or changed.

## Descriptive names
- No new made-up names. Read8 / Read16 / Read32 (0x00BF3B60) are existing HEAD stand-ins, kept as found (above).

## EH funclets
- No source under rebuild/src or rebuild/tests includes the file of any changed row (grep of the five file names).

## Locks
- rows.txt: 0x009551A0, 0x00956960, 0x00994930, 0x00BF3B60; plus 0x009566C0 (owner `released` after nf145 landed -> NF148). 0x009FF590 (`released`) and 0x008B66D0 (no lock)
  were compiled read-only as link inputs, not changed.

## Decisions for the coordinator
1. q11785 needs nf145 (landed 08:41) in the tree: its 0x009566C0 row is NF145's landed text + two defines.
2. Rule on 0x00BF3B60's grade: DIFFER(809v1480) -> DIFFER(825v1480). I read it as "not worse" (the x moves toward y because the two retail
   out-of-line ctor calls now exist; exact-instruction matches 47 -> 55 of 502), but the brief's literal test is x/y: please confirm.
3. Optional follow-up above (slots 0 / 73 / 78 and CCombatActionBase's own virtuals in the MELEE branch); the three spellings are in
   NAME_FIXES.tsv.
4. rebuild/src/compiled/00/bf/00bf3b60.pre.cpp: unbuilt stale copy (above).
5. hdr/ serves only the q11785 family: land it with that family.

## Counts (APPLIED.tsv / STATUS.tsv / recheck_final.log / selfcheck.log)
- Entries (2): done 2 (q11817, q11785); done-already 0; rejected 0; blocked 0; deferred 0; open 0.
- Rows ported (rows/, recheck_final.log, one run): 5: 0x00BF3B60 DIFFER(825v1480) + PASS (one-definition re-port, grade not worse: decision
  2), 0x00994930 / 0x009551A0 / 0x009566C0 / 0x00956960 RELOCATION_MATCH + PASS. Blocked 0.
- Not-a-binder: 1 (0x008B66D0, link input). Header opt-ins: 3 (used by the q11785 family).
- NAME_FIXES: 3 (0x00956720, 0x00956730, 0x009568E0). selfcheck.log: 0 problems (rows/ now holds all five).

## Rule slips
- Two Bash calls exited non-zero: a `cat` of the candidates header under the wrong path (rebuild/candidates/ instead of
  rebuild/include/candidates/), and a loop whose last command was a `grep -m2` with no match.
- Most read-only Bash calls (cat / grep / objdump / file writes) ran without the full `source env.sh` preamble; every compile, link and
  vland run had it (g.sh / bindchk.sh / lnk.sh source it).
- GOTCHAS.md, EXTRA_RULES.md and ASM_RULES.md were grepped for the relevant lines (one-definition, pick-any, nf86 / nf88 / nf96a / nf140,
  GameCODA, unreachable end), not read in full.
- NF145's 0x009566C0 copy was compiled in my private pS (two q.py grades) while it was still held (lock NF145), to draft the family; the
  row was locked and moved to rows/ only after nf145 landed and its lock read `released`.
- The q11785 family was drafted before q11817's NOTES.md was written (STATUS / APPLIED / RENAMES for q11817 were written first).
