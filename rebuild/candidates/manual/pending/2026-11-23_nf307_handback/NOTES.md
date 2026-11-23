# NF307 notes (2026-11-23): T1 signature, NF304's continuation (ENTRIES 1-4 = NF304's held 1, 2, 3, 8; 5-6 = NF304's 56 / 57; 7 = NF304's 55; 8.. = NF304's 61..)

Base: the repo working tree of today. No repo writes, no git. (in progress: this file is kept current as groups finish)

## Reading
Read in full: my BRIEF.md prefix, the standing brief BRIEF_NF_BATCH.md (with the NF299-NF304 rulings), docs/pipeline/GOTCHAS.md (all 513
lines), NF304's hand-back (NOTES, LESSONS, STATUS lines of my entries, held variants) and its landing README, CLAUDE.md, COORDINATION.md.
Read in part: EXTRA_RULES.md (the naming, stand-in, pick-any and relocation-target sections). Not read: ASM_RULES.md (no asm question),
the older landing READMEs nf281-285 / nf296's hand-back.

## Off-limits lists
work/offlimits.txt (brief's held rows, Codex p28 still-reserved rows, every T3_PORT_QUEUE address), work/other_agents_entries.txt
(NF305 / NF306 ENTRIES targets) and work/other_agents_witnesses.txt (every witness address in their spellings column), built by
tools/mkoff.py; locks checked per row with lock.sh.

## Result (entries 1-40 reached; 41-174 `open`, not started)
- **40 entries classified:** 15 done, 3 done-partial (5, 7, 36), 18 skipped-port, 4 skipped-held (10, 22, 29, 33: their only wrong
  binders are NF305 / NF306 witnesses).
- **24 rows changed** (rows.txt == the 24 locks I still hold; every control lock released). No test changed, no header changed (hdr/ empty,
  so no equiv_check). All 24 orig/ copies equal the repo at hand-back (cmp).
- **1 self rename** (RENAMES section 1): 0x00681AE0 KnowsOfDeed, `long deedType` -> EOpinionDeedType (MATCH both sides) with a new full
  Leaf line. No landed binder bound its old QBE_NJJ_N spelling; the five binders that already bound the PDB spelling were unresolved at
  HEAD and resolve now (evidence/link_g1.txt).
- **RENAMES section 2:** 26 caller lines. **TABLE_CHANGES:** 18 lines (5 delete, 10 replace, 3 rewitness). **NAME_FIXES:** 2 lines.
- **Proof per changed row:** HEAD + new VC7.1 objects RELOCATION_MATCH / MATCH (work/gr_*.log); objsec ALL_EQ for all 24
  (evidence/proof_g1, g2, g3, g4, g4b; .drectve included, no table emitted by any changed row); a `nf.py syms` HEAD v new diff per row
  (evidence/syms_g*.txt): only the respelled symbols change; DEF renames only in 0x00681AE0 (self) and in the two NF304-held re-scopes
  (entries 1-4, ruling NF304 Q1, below).
- **Links** (evidence/link_g1, g2, g3, g4; /FORCE, HEAD set v new set, both with every compiled control): no new duplicate (the 3 duplicates in
  link_g4 are identical in both sets, unrelated); nothing HEAD resolved is unresolved in the new set.
- **vland --recheck-landed** (recheck_final.log): 24 / 24 RELOCATION_MATCH (0x00681AE0 MATCH) + test PASS; no RELOC_CONFLICT; the
  RELOC_NAME warnings name other callees' labels (none is a spelling I moved).
- **Table lines v the full caller list:** every target touched was censused in the exe (nf.py census: calls, jumps, absolute dwords in
  landed code) and every census row compiled as a control and read with bind.py (evidence/bind_g*_head/new.txt), except rows of other
  agents (read from source, named in STATUS): 0x00713EE0 (entries 7 / 36), 0x006D50C0 / 0x006D5180 (selves of 13 / 14), 0x006D83B0 (19),
  0x007BFE30 (23), 0x00652FA4 (28), 0x00787E12 (36). No target is virtual, so no vtable references: every PDB spelling touched is a plain member (access / form letters QAE, QBE for
  members, QAE@ for the ctors; none is UAE / UBE / MAE), and the census finds no absolute-dword hit inside a vtable. Three lines are REWITNESS, not delete,
  because another binder still binds the old spelling: KnowsOfDeed QAE_NJJ_N -> 0x007BB150, GetNextNavPosOnRoute AU -> 0x00713EE0,
  LookAtPos PBV -> 0x00713EE0.
- **Untabled wrong binders found by the compiled census** (entry 36): 0x00714900, 0x00714CC0, 0x008D8EA0, 0x008DA610, 0x008DB240 bound the
  same PBV LookAtPos stand-in as the witness; fixed with it.
- **Name census** (evidence/name_census.txt): every new spelling is DEFINED only by its own self among the compiled objects. Source hits
  under the same Class::Method in other rows are other overloads (SetRadialBlurParams 0x00885070 / 0x008853A0, SetScreenFilterParams
  0x00885420) or pre-existing same-TU copies already in the PDB spelling (GetTotalStatLevel in 0x006D83B0, LookAtPos in 0x00787E12),
  plus one pre-existing debt: 0x007CD810 (the ??_GCTCMultiStrike row) defines `CTCMultiStrike::CTCMultiStrike(CThing&) { }` STRONGLY as a
  "TU emitter" next to the self 0x007CD390; entry 37's binder now binds that spelling (it bound an unresolved stand-in before).

- **NF300 Q1 evidence** (evidence/ppdiff_enum.txt): gcc -E HEAD v new of 0x00681AE0, 0x00683650, 0x00683590, 0x006D7960, 0x0072C230: only the
  included enum, the parameter / member types, the added `const` (0x00683650), the `*` at the reference call (0x00683590) and the literals
  -> enumerators change.
- **Path includers:** no landed source or test #includes any of the 24 changed files by path (grep of rebuild/src + rebuild/tests).
- **Off-limits re-check at hand-back:** tools/mkoff.py re-run; none of the 24 rows nor any self touched is an NF305 / NF306 entry or in
  T3_PORT_QUEUE / RESERVED; 0x006D50C0 / 0x006D5180 (selves of 13 / 14) are NF305 witnesses and were only read.

## Entries 1-4 (ruling NF304 Q1), per copy
- 0x00449E60 (entry 4): renamed pick-any ??0 / ??1CPlayerSave@NPlayerManager@@QAE@XZ are byte- and relocation-equal to the copies the
  landed 0x00449F90 ALREADY defines under those names (tools/cmpcomdat.py, evidence/pickany_g1.txt); no strong definition anywhere; the
  leaf does not call them (they are referenced only from .debug$F).
- 0x006B8640 (entries 1-3): the implicit copy ctor ??0CRadialBlurPositionParams@CEnvironmentSpellEffectInterface@@QAE@ABU01@@Z (a FableWin
  name) is byte-equal to the HEAD copy (55 bytes, no relocations) and defined by no other compiled landed object (21 extra controls: every
  landed row that names the three param types, `g1x`); no landed source defines it out of line. 0x00885680 (T3-queued, not compiled)
  has it as an in-class inline user copy ctor (pick-any, memberwise). The three local getters on the stand-in global
  CScreenEffectPriorityStack change only in their return type; no other landed source names them.

## Decisions / questions
- **D1 (entries 1-3):** the three renamed getters of 0x006B8640 are local inline getters on a STAND-IN class, and their new names are not
  PDB spellings (PDB: NScreenEffect::CScreenEffectPriorityStack::GetCurrentRadialBlurParams / GetCurrentRadialBlurPosParams /
  GetCurrentScreenFilterParam). I read the prefix's ruling ("each renamed pick-any copy's body equals the landed PDB definition, or no
  landed strong definition of it exists") as met: no landed definition of the new names exists. Moving them onto the PDB names would mean
  re-scoping CScreenEffectPriorityStack into NScreenEffect, which respells the FrameUpdate binding 0x00880E50 (a class / namespace fix,
  not this batch). Land as is, or hold 0x006B8640 for that re-scope?
- **D2 (entries 7 / 36, partial):** 0x00713EE0 is a witness of NF305's entry 46, so the CCreatureNavOutput re-scope (entry 7) and the
  LookAtPos reference (entry 36) were not applied there; both old table lines are rewitnessed to it. It needs the same two edits as
  0x0097E890 / 0x00714900 once NF305 is done with it.
- **D3 (entries 38 / 39):** HasCombinationOfAugmentations is called with 0xC (FLAME | LIGHTNING), which is no enumerator of
  engine/EObjectAugmentationType.h. May the binder pass `(EObjectAugmentationType)0xC` (or the OR of the two enumerators) under NF300 Q1,
  with the selves 0x00766050 / 0x007660C0 retyped to the enum (self + binder unit)?
- **D4 (entry 5):** 0x007BB150 has `typedef long EOpinionDeedType;`, which also spells its own self leaf (public, long; PDB private +
  enum). Replacing the typedef by the shared header fixes both, but renames its self: queued in NAME_FIXES rather than done here (its
  callers were not censused).

- **D5 (entry 37, census conflict):** the landed 0x007CD810 (the ??_GCTCMultiStrike row) defines `CTCMultiStrike::CTCMultiStrike(CThing&) { }`
  STRONGLY as a "TU emitter" (empty body) under ??0CTCMultiStrike@@QAE@AAVCThing@@@Z, beside the real self 0x007CD390. 0x004D5896 bound an
  unresolved stand-in (PAV) before and now binds that doubly defined name. Land 0x004D5896 with 0x007CD810 queued for the nf86 / nf88
  scaffolding (so the stub becomes pick-any), or hold 0x004D5896 until then?

## Method
- Groups: g1 = entries 1-6 (+ g1x: 21 pick-any census controls), g2 = 7, g3 = 9-19, g4 = 23, 28, 36, 37 (+ g4b: the five untabled
  LookAtPos binders). Per group: changed list + controls (every self + the full exe census of every entry touched, minus other agents'
  rows), pre_tests.sh on every landed test BEFORE the locks (NOWCHAR reruns; D1 cl /Zs for 0x00682810, 0x006C6040, 0x007993F0,
  0x0089FEB0, 0x0076C8A0, 0x004C9120, 0x004A3740, 0x0064AE80 tests), locks, medit.py / ins.py specs (work/spec_*.json, ins_*.json),
  clang prechecks of every new and HEAD source, grades in 4 background lanes (`timeout 900` per grade line), then bind / proof / syms
  diff / link / gen_ren / assemble.
- Edit classes: pointer -> reference (`*p` / dropped `&` at the call), `unsigned long` for `long`, a shared enum header included above the
  local model with literals -> enumerators (EHeroExperienceStatCategory, EHeroAbility) or a `long` member retyped to the enum
  (EOpinionDeedType), `const` on a declaration, a nested-class re-scope (CCreatureNavOutput into CTCCreatureNavigation, class key V).

## Rule notes
- No sleeps; every VC7.1 grade, cl /Zs, link and vland ran in the background with `timeout 900` on its own launch line and was checked
  with kill -0 in a later call; clang prechecks ran in the foreground under `timeout 120`. No rm (no removals at all). Every heredoc quoted.
- One slip in method, not in rules: the first lock.sh calls failed with "S not set" (S was not exported); no lock was taken until rerun.
- No retail or donor bytes in hand-back files (the vland oracle stays in work/).
