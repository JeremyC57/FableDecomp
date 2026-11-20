# NF144 notes (2026-11-20, round 10: q6408, q5199 / q5220, q4802)

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries (4): done 2 (q5199, q5220: one unit); done-already 0; rejected 0; blocked 2 (q6408: 8 binders + the self row left; q4802: 7 rows left);
  deferred 0; open 0.
- Rows (STATUS.tsv, 32 lines): ported 17 (all RELOCATION_MATCH + test PASS: 16 for q6408, 0x006D41F0 for q5199 / q5220); not-a-binder 3
  (0x009E2590, 0x0075B8D0, 0x008C1900); blocked 12 (q6408: 0x0073ACC0, 0x008C1C30, 0x00848DB0, 0x008C0C00, 0x008C0D10, 0x008C48A0,
  0x008676A0, 0x008A08A0, 0x008A1A10; q4802: 0x00B21E90, 0x00762E10, 0x00764E90).
- Header opt-ins: 0 (hdr/README.md). RENAMES: section 1 empty (no self rename); section 2 has 33 caller respellings.
- Order taken: the brief's specifics order (q6408 pass, then q5199 / q5220, then q4802), then back to q6408's 16-row group.

## q5199 / q5220 (done)
- 0x006D41F0 (NF136's last binder): the free stand-in N006d41f0::ReadInteger -> CPersistTraitsSLONG::TransferIn through NF136's partial mode
  FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY (the declared 3-argument static into a long local; the context through the reference cast the row
  already uses). First form RELOCATION_MATCH; only the 0x009E2590 binding changes; defined-symbol diff empty.
- Census: every other REFS row calls 0x009E2590 through CPersistTraitsSLONG in code (source grep of all 60 REFS rows with comments stripped:
  the old stand-in names survive only in NF136 comment lines; 0x009BB110 goes through the shared CPersistTraits<long>); NF136 / NF133 probes
  compiled them. No split: the self row defines the claim.
- Not done (outside the entry, optional): 0x006D41F0's self rename + its caller 0x006D4180 (NAME_FIXES, carried from NF136). Lock 0x006D4180
  claimed then released (`released`).

## q6408 (blocked, 8 binders + self left)
- 16 binders ported, every one by the same two moves: drop the row's local CModeBase stand-in and local CTCCreatureModeManager for
  engine/CTCCreatureModeManager.h (ADDMODEPOINTER), and re-base the row's HEADER-LESS local mode class on the shared nested CModeBase (the narrow
  rule; landed sibling 0x0096DBC0 has the same shape: NCreatureMode::CModeScreamerAggressive on CTCCreatureModeManager::CModeBase), declaring
  GetType() (pure in the base) and resizing its pad so sizeof stays the retail allocation. The mode-ctor declaration (class token, namespace,
  parameters) is untouched, so every mode-ctor binding (0x008E1150, 0x0084B7C0, 0x008C35D0, ...) is as at HEAD. Scripts: work/xmode.py
  (CCountedModePointer group), work/xmode2.py (CCountedPointer<global CModeBase> group).
- Header check for each mode class: grep `(class|struct) <Mode>\b` over rebuild/include: none for all 16 (CModeJackOfBladesScream is only
  forward-declared in CCreatureAction_JackScream.h). CModeBriarRoseConcentrate HAS one -> 0x008C1C30 blocked (full port).
- Binding moves (pdiff of HEAD vs new object per row): 0x0073ACC0 -> the claim; 0x00718A00 -> ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z
  (FableWin.pdb 0x01DE57D0; already bound by landed 0x008C1900); 0x008ED950 also 0x007180B0 Destroy_007180b0 stand-in -> its self leaf
  ?DeleteData@?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@SIXPAX@Z (FableWin.pdb). No address gains a spelling; 0x0073ACC0 keeps 7
  spellings until the last members of each group move (moves are onto existing spellings, partial moves are neutral).
- Census: 26 of 34 REFS rows now bind the claim (10 before: NF135 census + NF138 0x00848FD0).
- One definition (objdump -t, new vs HEAD object, tools/defdiff.sh): each new object defines its own leaf plus pick-any COMDATs only. New:
  ??1?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@XZ (all 16), and for the 12-row group the ??0 / DeleteData instances that replace
  the HEAD objects' ??0 / ??1 / DeleteData of CCountedPointer<global CModeBase>; 0x008ED950 also ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z. Landed
  `// Leaf:` owners of those: only DeleteData (0x007180B0, an explicit instantiation, compiled here: LINK_ONCE_DISCARD). No strong clash.
- Four rows also replaced an empty local `enum ETCInterfaceType { };` with engine/ETCInterfaceType.h (0x008F47E0 by hand after v1 SRC_FAIL C2065;
  0x00848730 / 0x008C1F70 / 0x008CCD70 by the script up front). 0x00969CB0's local NON-template `struct CCountedPointer` (CBrainOutput::PAction) became
  the shared CCountedPointer<CCreatureActionBase> (v1 SRC_FAIL C2990); its body was the header default dtor; HEAD bound nothing at 0x00668D90.
  0x008ED950's local CCountedPointer<T> / SCountBlock became the shared header (v1 SRC_FAIL C2953).
- Blocked rows and what must move first: STATUS.tsv (0x008C1C30: CCharString, CDefPointer<T> + its GetSubDef / GetEffectFromString same-TU
  stand-ins, CThing, CCombatActionBase; 0x00848DB0: the 0x0073EAC0 CModeBase ctor and 0x01275760 vtable spellings, CAnimFilterOfGroup;
  0x008C0C00 / 0x008C48A0 / 0x008C0D10: local CCountedPointer<T> templates with per-type deleter / ctor stand-ins: the 0x008C4990 shape re-spells
  those addresses and the mode class namespace; the three singletons per NF141's closure map).

## q4802 (blocked, 7 rows left)
- The seven q4802 sources are unchanged since 05:00 today (find -newermt over their files; NF138's census ran after that). The one untried mechanical step: 0x00B21E90 (NF126 draft on engine/CCardinalSpline.h, binds the
  claim) inline-budget sweep, 3 forms, all DIFFER (STATUS). Blocked after 3 forms. Next idea: bisect /d2inlS between 60 and 150.

## Local classes (brief rule: header-less local model on a shared base)
- Kept tokens, now on CTCCreatureModeManager::CModeBase (none has a header anywhere under rebuild/include; landed 0x0096DBC0 uses the same shape;
  base from the shared header): CModeDragonFlying (0x008F4B00, 0x008F47E0, 0x00950C30), NCreatureMode::CModeSleepBed (0x00969CB0),
  CModeCarryingGoodsStand (0x00848730), CModeBriarRoseTeleportInvulnerable (0x008C1E80), CModeBriarRoseTeleportIntangible (0x008C1F70),
  CModeIceTrollGroundSmashIcicleAssault (0x008C7600), CModeSummonerSummon (0x008CB060), CModeSummonerLightningOrb (0x008CB1A0), CModeIntangible
  (0x008CCD70), CModeDazed (0x008ED950), CModeDragonRearUp (0x008F39A0), CModeDragonFireBeam (0x008F41D0), CModeDragonStrafeBurninate
  (0x008F4320), CModeDragonStrafe (0x008F4480). No class token was added to any row (all were defined at HEAD).
- Removed local models: CModeBase, CCountedModePointer, CTCCreatureModeManager (all 16), CCountedPointer (0x00969CB0, 0x008ED950), SCountBlock
  (0x008ED950, 0x00969CB0's forward), the empty ETCInterfaceType (4 rows). Every other HEAD local model is untouched (vland WARN
  SHARED_HEADER_AVAILABLE as at HEAD).

## One definition: reverse prefix check
- defdiff.sh greps full `// Leaf: <symbol>` lines; many landed Leaf lines are prefixes, so each new COMDAT was also tested against every landed
  Leaf line that is a prefix of it (work/leafs.txt): the CCountedPointer<CModeBase> ctor / dtor and the CCPPointerInfo ctor have none; DeleteData
  matches only the generic prefixes `?DeleteData@` / `?DeleteData@?$CCountedPointer@` (other T's owners) and its own owner 0x007180B0
  (LINK_ONCE_DISCARD, compiled here).

## Locks
- Claimed (plain mkdir; all were free): 0x008C1C30, 0x008F47E0, 0x008F4B00, 0x00969CB0, 0x006D4180, 0x00848730, 0x00848DB0, 0x008C0C00,
  0x008C0D10, 0x008C1F70, 0x008C48A0, 0x008C7600, 0x008CB060, 0x008CB1A0, 0x008CCD70, 0x008ED950, 0x008F39A0, 0x008F41D0, 0x008F4320,
  0x008F4480; 0x00B21E90 was `released` and re-claimed. 0x008C1E80 / 0x00950C30 / 0x006D41F0 were NF144's from the start (rows.txt).
- Ported rows outside rows.txt (14): 0x008F47E0, 0x008F4B00, 0x00969CB0, 0x00848730, 0x008C1F70, 0x008C7600, 0x008CB060, 0x008CB1A0,
  0x008CCD70, 0x008ED950, 0x008F39A0, 0x008F41D0, 0x008F4320, 0x008F4480 (locks held as NF144 for the landing).
- Written back `released` (not handed back): 0x006D4180, 0x008C1C30, 0x00848DB0, 0x008C0C00, 0x008C0D10, 0x008C48A0, 0x00B21E90.
- Account B: none of the 17 ported addresses appears in UNMATCHED_ROWS.tsv; in COORDINATION_CLAIMS.tsv only Account A naming lines mention
  them (substring grep).

## Forms spent on the blocked q6408 rows (for reassignment, not budget-exhausted)
- 0 forms (structural / not attempted): 0x008C1C30, 0x00848DB0, 0x008C0C00, 0x008C48A0, 0x008676A0, 0x008A08A0, 0x008A1A10 (and the self
  row 0x0073ACC0, which moves last). 1 form: 0x008C0D10 (v1 SRC_FAIL C2953). 0x00B21E90 (q4802): 3 forms, all DIFFER.

## EH funclets
- No row under rebuild/src/compiled includes any changed file (grep of each file name).

## Tests
- All 17 tests unchanged: standalone host copies of the old bodies (PASS). Rewriting them on the shared headers is a follow-up.

## Flags lines
- None added (0x00B21E90's flag drafts are blocked, not handed back).

## Descriptive names
- None new.

## Decisions for the coordinator
- None needed to land this hand-back. For the q6408 leftovers: 0x008C1C30's full port and the 0x008C0C00 / 0x008C48A0 / 0x008C0D10 deleter
  re-spells are authoring-size ports (each re-spells 2-3 more addresses).

## Rule slips
- Several read-only Bash calls (cat / grep / objdump) ran without the `source env.sh` preamble; every compile went through g.sh / defdiff.sh /
  pdiff.sh / vland, which source it.
- GOTCHAS.md and EXTRA_RULES.md were grepped for the relevant lines, not read in full.
- The vland batches ran in the background while I only read files (no second compile ran beside them).
