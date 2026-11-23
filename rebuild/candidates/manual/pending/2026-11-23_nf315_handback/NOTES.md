# NF315 notes (2026-11-23): NF313's in-place leftovers + 15 uncovered T1 entries, hand-back 1

Base: the repo working tree as of today. All orig/ copies (18 here, 6 in held_t3q/orig) are cmp-equal to HEAD at hand-back (no staleness). Nothing was
written under the repo; no git command of any kind was run. "Line n" = the n-th data line of ENTRIES.tsv. STATUS.tsv has all 31
entries plus one rider line (0x006EBDD0, NF313's line-21 owner unit).

Reading done before the first compile: the prefix BRIEF.md, BRIEF_NF_BATCH.md, all of docs/pipeline/GOTCHAS.md, NF313's hand-back
(NOTES, LESSONS, STATUS, unreached, NAME_FIXES, REJECTED) and the landing nf313 README; CLAUDE.md as given; ASM_RULES.md,
COORDINATION.md and EXTRA_RULES.md (the naming sections, "names right the first time").

## Summary
- **18 rows changed, landing-ready** (rows.txt) + **6 proven rows held** in held_t3q/ (README there) because T3_PORT_QUEUE.tsv names
  them inside other ports' units (column 3 or 4; my off-limits script read only column 2 and missed them until the final screen).
  vland --recheck-landed: all 24 RELOCATION_MATCH + test PASS (recheck_final.log covers both sets; no DIFFER, no RELOC_CONFLICT; the
  WARN SHARED_HEADER_AVAILABLE / RELOC_NAME lines are pre-existing).
  - **B1** CMessageEventFilter_IsOriginatedByThing(const CThing&) 0x006AF2B0: 0x00919A70 (owner defines the PDB form). Its sibling
    0x00934920 is held, so the PAVCThing line stays (no delete).
  - **B2** CWorldMap::GetHeightZBelowPos(const C3DVector&) const 0x0063E480: 0x008C9F40 + 0x008CA4A0 + the QAEMPBV delete.
  - **B3** CTCFollowed::RemoveEmoteIcon(CThing&) protected 0x006D5050: 0x006D56E0 + its delete (entry line 28 fully done).
  - **B5** CAIStateGroupBase::AIAction_CombatPerformActionAndWait IAE (const CCombatActionBase&, bool) -> EAIActionReturn 0x0097DA40:
    0x0091E050 (NF313 Q5's rider) + 0x009621F0 (NF302 Q2: the owner still defines the CCreatureActionBase* stand-in; its unit is held
    on NF314 witness 0x0096FCD0; unresolved before and after; owner queued in NAME_FIXES). The third declarer 0x009624D0 is held, so
    no delete line.
  - **B6** CResourceBank::AddResource(CResource&) 0x009FC380: 0x00B2C460 + its delete (NF302 Q2: the owner self is on the made-up
    CResourceBank_Retail; owner queued in NAME_FIXES).
  - **G** GiveTutorialForCategory(ETutorialCategory, bool) 0x006C9FA0: the 7 free `long` binders 0x005BE028, 0x005EE418,
    0x006D77C0, 0x0072E940, 0x0077D4B0, 0x00784600, 0x007E00D0 (NF300 Q1: the existing opt-in FABLE_RETAIL_TUTORIAL_CATEGORIES_PASS26 +
    engine/ETutorialCategory.h, each literal -> its enumerator, which also reads right: BASIC_OBJECTS / TRADE_ITEM / ABILITY_CYCLING /
    LEVELLING_UP / BUILDING_OWNERSHIP / EXPERIENCE_SPENDING / BOASTING / TELEPORTING; 0x005BE028's `long category` local ->
    ETutorialCategory, the return type of the shared static CTCInventoryItem::GetTutorialCategory, NF304 Q2). The owner defines the
    PDB form (link: 7 unresolved -> 0). 0x0057C2E6 (T3_PORT_QUEUE) keeps the `J_N` form, so no delete line.
  - **Q** owner unit 0x00843F50 (NF304 Q6): the CCreatureAction_ActivateQuest ctor self `(CThingCreatureBase*, CThing*, ...)` -> the
    PDB `(CThingCreatureBase&, CThing*, ...)` (+ full Leaf on line 2), its one binder of the old self spelling 0x007F03D0, and
    0x007F01F0 off the `(CThingCreatureBase&, CThing&, ...)` stand-in; the self-spelling delete lands with them. The other
    reference / reference binder 0x007EF600 is held, so that line stays.
  - **W2** 0x006EBEE0 alone (NF302 Q2): CreatureUnsheathWeapon pointer -> reference, and its local enum with MADE-UP enumerator names
    (UNSHEATHE_INNER_RING = 4 / UNSHEATHE_OUTER_RING = 5) replaced by the shared engine/EUnsheatheWeaponType.h (4 =
    UNSHEATHE_MELEE_WEAPON, 5 = UNSHEATHE_RANGED_WEAPON); unresolved before and after against the HEAD owner (link_W2); its stand-in
    line deleted. The owner unit (0x006EBDD0 self + 0x0092B3E0 + 0x006ED510) is proven but held.
- **Proof per row:** HEAD and new RELOCATION_MATCH (evidence/gr_*.log); objsec ALL_EQ under exactly one rename per row, .drectve
  included, table counts unchanged (evidence/proof_B / proof_G / proof_Q / proof_W); COMDAT sets SAME under the rename (cmpfn_*);
  links (evidence/link_B1..B6, link_G, link_Q, link_W, link_W2): each stand-in unresolved at HEAD is resolved after (B2, B3, G, Q),
  or unresolved before and after on the PDB name (B5, B6, W2: NF302 Q2); nothing HEAD resolved becomes unresolved; 0 duplicates.
  Preprocessed diffs for the enum rows (evidence/ppdiff_G.txt; 0x0091E050 included): only the enum, the parameter type, the literal
  and (0x005BE028) the local's type change.
- **Table (TABLE_CHANGES.tsv): 5 deletes + 1 rewitness**, each delete LAND WITH its rows (named in its note). Every delete was checked
  against every exe call site of its target (tools/delchk.py -> evidence/xsites_<target>.tsv, every caller's declaration lines, and a
  grep of all of rebuild/include for the member: only engine/CWorldMap.h / CCombatManager.h / CAIStateGroupBase.h /
  CResourceBank.h / CMessageEventFilters.h declare them, all in the PDB form); 0 absolute references for every target and none is
  virtual (one is a constructor). `apply_table.py --dry`: 5 delete + 1 rewitness, 0 misses, no new multi-target symbol
  (evidence/apply_table_dry.txt).
- **SetPItem (NF313 Q4 / landing Q4):** the `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCThing@@@Z` line is REWITNESSED onto 0x007FDEB0,
  not deleted. tools/setpscan.py: 76 landed sources declare SetPItem(CThing*) (43 on a global CBaseIntelligentPointer model, 30 on the
  made-up CBaseIntelligentPointerCtor, 2 in made-up namespaces); the 43 intersected with the exe call sites of 0x00A01B90 gives three
  rows (0x007FDEB0, 0x007FDF60, 0x008C0900), and all three HEAD objects bind the spelling; three declarers without retail calls
  (0x0080F270, 0x008C8180, 0x0090C1A0) declare but emit nothing (evidence/setp_scan.txt).
- Counts: 1 done (line 28), 8 done-partial (lines 3, 5, 7 [table only], 9, 11, 13, 14 + the 0x006EBDD0 rider line), 13 skipped-held,
  10 skipped-port, 0 rejected entries (one rejected attempt in REJECTED.tsv: 0x00738F90).

## Not done / held (STATUS has the reason per line)
- **No-compile holds:** lines 1 (0x004BFFC0 still T3), 2 (only binder an NF314 witness), 10 and 22 (target an NF314 witness),
  23 (only binder T3), 25 (binder an NF314 witness).
- **Line 12, 0x007008C0 owner unit (NF313 Q5 rider): HELD.** The exe scan found SIX landed binders of the owner's stand-in self
  (0x00780C80, 0x00784260, 0x00784500, 0x007846B0, 0x007849C0, 0x00784BD0; one table witness); 0x007846B0 is in T3_PORT_QUEUE and
  0x007849C0 is an NF314 witness, so the owner cannot move (NF304 Q6).
- **Line 15, 0x00738F90 (rider): rejected attempt.** Byte-equal .text, but the compiler-emitted `/alternatename` .drectve line of
  the member-template declaration changes with the `const T**` respell: T3 (prefix; NF303 Q2). Reverted, locks released.
- **Q3-shaped (self virtual override retype), not censused here:** lines 4 (0x0090F0D0), 6 (0x0095A890), 31 (0x0082D080). Self units
  not done: 8 (0x0044C47B ctor parameter), 16 (0x007BB150). In place but unreached: line 18 (0x00999110: anonymous-namespace
  declaration-only stand-in of the static CAFile::ExpandPath, see STATUS).
- **Census ports (NF299 Q4):** lines 17, 19, 24, 26, 27, 29, 30: every stand-in is in neither PDB name set
  (evidence/pdbq_census.txt); sources opened for lines 18, 20, 21, 31 (more than 1 in 10). Lines 20 / 21 are ports on source read.
- 0x008C2240 (NF314 witness) and 0x007BDB66 (NF312 witness) still bind the non-const GetHeightZBelowPos reference form (line 3).

## LAND TOGETHER
- B2: 0x008C9F40 + 0x008CA4A0 + the QAEMPBV delete. B3: 0x006D56E0 + its delete. B6: 0x00B2C460 + its delete.
  W2: 0x006EBEE0 + its delete. B1 (0x00919A70) and B5 (0x0091E050, 0x009621F0) have no table line.
- G: the 7 binders may land together or one by one (no delete line).
- Q: 0x00843F50 + 0x007F03D0 + 0x007F01F0 + the self-spelling delete (the owner's RENAMES section-1 line).
- The SetPItem rewitness is independent. The held set's units and their lines: held_t3q/README.md.
- Includers by path: none of the 24 sources is #included by any other file (evidence/includers.txt; 0x006F2300 names 0x006ED510's
  file in a comment only). New-name census: every new spelling is the PDB name its owner row defines (or, B5 / B6, the shared
  header's declaration); no landed row defines another body under these names (0x0084C900 is the copy ctor, a different symbol).

## Rule notes
- Locks: lock.sh NF315 before any compile; every landed test prechecked BEFORE its lock (pre_testsB / testsOB / testsG / testsP /
  testsP2 / testsQ / testsW); every compiled source (HEAD and new) prechecked after its lock in an earlier call (pre_srcHB / rowsB,
  srcHG / rowsG, srcHP / srcHP2, srcHQ / rowsQ, srcHW / rowsW). D1 / NF300 D1 extension `cl /Zs` (background, timeout 900, earlier
  call): HEAD sources 0x009621F0 / 0x009624D0 and test 0x0092B3E0 fail clang only on MSVC address-of-temporary: ZS_OK
  (evidence/zs_srcHB.log, zs_testW.log); their new sources pass clang. Planted control: work/ctl_bad.cpp flags in pre.sh.
- Held now: exactly the 18 landing-ready rows (work/mylocks.txt = rows.txt). Released: every owner / probe lock used only for HEAD
  compiles, 0x00738F90 / 0x00738980, and the 6 held_t3q/ rows (evidence/locks.log).
- Background: every grade / link / /Zs / vland ran under nohup with `timeout 900` on its own launch line (tools/gr.sh, lk.sh,
  zsrun.sh, rf.sh) and was read with kill -0 / its DONE line in a later call. No sleep, no rm (clean.py only: the reverted 0x00738F90 and, at hand-back, the 6 held rows moved out of rows/ and orig/ into held_t3q/; the reverted
  rows/00738f90.cpp), no header / test / flag / catalog change, no __asm / naked / _emit, no payload or grade piped into head.
- Off-limits (tools/offl.py, retargeted to NF312 / NF314: their targets plus every reloc_bindings witness of their targets, live
  locks, codex-p28 reservations, held rows, T3_PORT_QUEUE), refreshed before each claim. Skipped for it: 0x008C2240, 0x007BDB66,
  0x007846B0, 0x007849C0, 0x0057C2E6, 0x009A6610, 0x0096FCD0, 0x009FE380 and the lines above.
- No retail or donor bytes in hand-back files (hex-run grep clean); oracles stay in work/.

## Questions
0. **T3_PORT_QUEUE membership (blocks the held set):** 0x00934920, 0x007EF600 and 0x006EBDD0 are named in column 4 (as binders of
   OTHER targets' ports: 0x008128F0 / 0x00870BB0 / 0x00934E50, 0x007B5680, 0x0085F0C0); 0x009624D0 and 0x0092B3E0 in column 3 of the
   0x0073A8A0 / 0x008D5110 lines. My respells touch other callees in those rows (0x006EBDD0's edit does rewrite the line that calls
   the 0x0085F0C0 IsIncapacitated stand-in, `IsIncapacitated(&creature)`). NF313's batch M changed 0x0092B3E0 and landed today. May
   such rows take an unrelated in-place respell (then held_t3q/ lands as four units), or are they off-limits until their ports land?
1. **B1, 0x00919A70's cast:** the row already passed `(CThing*)seller` (a CThingCreatureBase* parameter cast across its two
   forward-declared local types); the respell keeps it as `*(CThing*)seller` (GOTCHAS 459 debt, pre-existing, not added). The same
   TU also carries its pre-existing same-TU stand-in body of AIAction_WaitForMessage (it is not in T3_PORT_QUEUE). Land, or drop
   0x00919A70 alone (no table line depends on it)?
2. **W2, 0x006EBEE0's made-up enumerators:** replacing the local `enum EUnsheatheWeaponType { UNSHEATHE_INNER_RING = 4,
   UNSHEATHE_OUTER_RING = 5 }` by the shared header (same enum name, same values, PDB enumerator names) reads as NF300 Q1. If an
   enumerator rename needs its own ruling, drop 0x006EBEE0 with its delete line.
3. **Owner self fixes and their tests:** the landed standalone tests of 0x00843F50 (and of held 0x006EBDD0) still carry their own
   copy of the function under the OLD signature (pointer first parameter); they are unchanged (NF284 D2) and pass. Leave them, or
   respell the tests' copies too in the landing? (0x006EBDD0's body respell `creature->` -> `creature.` is in the held set.)
4. **Line 12 (0x007008C0)**: hold until 0x007846B0 leaves T3_PORT_QUEUE and NF314 releases 0x007849C0, then one 7-row owner unit?
5. **Next-batch in-place candidates:** line 18 (0x00999110 -> static CAFile::ExpandPath), and the Q3 censuses for lines 4 / 6 / 31
   plus the self units of lines 8 / 16.

## Observation (no change made)
- 0x006AF290's landed row defines `??0CMessageEventFilter_IsOriginatedByThing@@QAE@PBVCThing@@@Z`, while 0x00919A70's model and comments
  call 0x006AF290 the IsTargettedAtThing ctor (`??0CMessageEventFilter_IsTargettedAtThing@@QAE@ABVCThing@@@Z`): a possible identity /
  name issue on a byte-twin, one source only, so not queued in NAME_FIXES.
