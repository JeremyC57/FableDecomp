# NF309 notes (2026-11-23): naming batch, hand-back 1 (198 entries: virtual overrides 1-3, held 4-17, open 18-34, tracker T1 / T2 35-198)

## Summary
- **31 rows changed, all landing-ready** (`rows.txt`). Every one graded RELOCATION_MATCH PASS in vland `--recheck-landed`
  (`recheck_final.log`, payload work/rc1: WINS 31, no DIFFER / RELOC_CONFLICT / LINK_FAIL / SRC_FAIL). The warnings are pre-existing
  local-model / label warnings (SHARED_HEADER_AVAILABLE, RELOC_NAME on labels the catalog already carries).
- No shared-header, flag, catalog or test change; hdr/ is empty. Tests of the changed rows are standalone model copies and are
  unchanged (each prechecked before its lock). orig/ holds the HEAD .cpp and _test.cpp of every changed row (all still equal HEAD after
  the mid-run NF307 05:06 / NF308 05:15 landings).
- **TABLE_CHANGES.tsv: 32 lines** (22 delete, 7 replace, 2 rewitness, 1 add). Each was checked against the current table (0 problems) and,
  for its target, against every exe caller and every vtable / absolute reference (tools/absrows.py, xs_all). tblafter3 reports
  every target PDB_OK.
- **Entries: all 198 have a STATUS line.**
  - done 18 (5 of them are table-only stale lines: 24, 26, 47, 85, 166)
  - done-partial 10 (1, 2, 3, 10, 48, 80, 103, 129, 140, 189)
  - done-equivalent 1 (51, NF301 STL spelling)
  - skipped-port 73:
    - 63 from the census (tools/census309.py, NF299 Q4). Every stand-in binder binds through a class or scope found in neither PDB name
      set, the wrong class, or a free stand-in where the PDB has a member. 1 in 10 were sampled and every HDR_HIT entry was opened.
    - 5 that were opened and found to bind another class's member.
    - 35 (NF281 D2 duplicate), 38 (self defines QAE and needs `mutable`: T3), and 3 more whose binders DEFINE strong same-TU stand-ins.
  - skipped-held 56: 17 from the census (the binder is stay-off or in the T3 queue), 14 CPersistTraits family (T3_PORT_QUEUE unit
    nf300), 11 held entries (4-17) re-read against the new stay-off that still hit NF307 / NF308 entries, witnesses or locks, 169 (the target is an NF307 witness), and the rest because the binder is T3 / held.
  - open 40, not reached in this hand-back (18 19 20 21 22 23 25 27 28 29 30 31 32 34 36 41 72 74 76 77 78 82 96 98 101 107 108 110 111
    124 125 136 145 168 176 177 178 181 197 198). Their STATUS notes give the census class and the shape each needs.
- **Order of work**: a warm-up on unit 1 (35, 38, 40, 47-50, 67), then entries 1-3 (units 2 / 3), then the held entries 7 / 10 / 11 re-read
  after the NF304 / NF305 landings (unit 4), then the census over 35-198 (units 5, 6, 9 plus the table-only stale lines).

## Units (detail is in each STATUS note)
- **unit 1** (00694020, 00bf9aa0, 00b662f0, 00845900, 004d4a99): an access move (UpdateActionMovement under protected:) and pointer ->
  reference / const parameters in the PDB spelling. Call sites pass `*p`, or the object where the stand-in took its address.
- **unit 2** (entries 2 / 3; NF303 Q1 / NF299 Q3 shape):
  - 0078b410: Initialise is virtual in its local base and override.
  - 00694d10 is the owner self fix (NF304 Q6). The v12 placeholder became `virtual void Initialise();` at slot 12, and the trailing
    non-virtual declaration was removed. The only landed QAE binder, 0078b410, moves in the same hand-back.
  - 0082d810 / 00704150 / 00836260: CThing gets `virtual bool WorldUpdate();` (replacing the `void* __vftable` member or shrinking the
    pad by 4), and the base plus the own override are virtual.
  - Full own Leafs are written (RENAMES section 1). The self 0x00838BA0 is the EditWorldUpdate stand-in (NAME_FIXES).
- **unit 3** (entry 1, NF303 Q1 + NF300 Q2):
  - 005469e0 / 00547c90: virtual ChangeState.
  - 0053ebf0: the global struct was re-scoped into `namespace NUISystem` as a class with public bases (no vftable / RTTI change, objsec
    ALL_EQ).
  - 00543df0: ChangeState(long) -> (unsigned long) in its base and its override.
  - Not moved: 0054a990, whose HEAD object binds the global QAE and is rewitnessed (a new base virtual would shift its CSlider slots).
    Also not moved: the stand-in classes 0054c270 / 0053d3b0 / 00540e20 / 005419e0.
- **unit 4** (7, 10, 11):
  - 00f3afc0: CompressToBuffer -> the Compress overload; its own Leaf is written in full.
  - 009261f0: protected `CPlayerManager& GetPlayerManager() const`.
  - 009ba110: SkipUntilString(const CCharString&).
- **unit 5** (75, 80, 81): the NF284 CCountedPointer DeleteData hybrid (`FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` +
  `engine/CCountedPointer.h` + `&CCountedPointer<T>::DeleteData`) replaces the Destroy_xxx stand-ins in 0085a840 / 00ac3a40 / 00618660.
  The targets explicitly instantiate those exact symbols (evidence/name_census.txt).
- **unit 6** (103, 122, 130, 142, 182, 184): GetCamTools under protected:, WriteData -> WriteCompressed, PrioritiseBlock ->
  SetBlockToHighPriority (both callers), CalculateLODBand -> GetDesiredLODBand, PlaySound -> PlaySoundW, SetWillMorphAmount -> SetWill.
- **unit 9** (129, 140, 189):
  - RegisterEnable -> AddEnableConsoleVariable.
  - Init -> protected Initialise(), with the call qualified as `CInitBaseClass::Initialise()`.
  - IsScriptTaskRunning / IsTaskListEmpty -> `IsActionListEmpty() const`, in 3 binders (008326a0 was found by the census).
- **table-only stale lines** (tools/stale.py): 24 / 26 (witness 0x00B1B0C0), 47 (0x006BBC30), 85 (0x008D43C0), 166 (0x00899C90).
  In each, the witness was re-authored and its HEAD object references the PDB spelling, so the old line binds nothing
  (evidence/u1_006bbc30_head_binding.txt, u7_00899c90_head_binding.txt, u8_stale_lines.txt).
  - Entry 166: NF307 rows 00683590 / 0072c230 landed mid-run and call 0x00449970. Both name it GetMainPlayer, and no landed row source
    names Sub_ResolveA. Test 00899cf0 defines its own local cdecl mock under that name, which is not the fastcall table symbol.

## Proof
- **objsec** (tools/proof2.py, exact old=new pairs from symdiff.py): ALL_EQ for all 31 rows, every non-debug section and relocations
  included. There is no new ??_7 / ??_R symbol, and every new symbol is the entry's PDB spelling, 1:1. Files: evidence/proof_u1 / u2 /
  u2b / u3 / u4 / u5 / u6 / u9 / u9b.txt.
- **Links** (evidence/link_u*_*.txt): the HEAD set was linked against the new set. Nothing HEAD resolved becomes unresolved, and there are
  no new duplicates. Every moved reference resolves against its owner, or stays unresolved as at HEAD when the owner is a stand-in or
  uncompiled (NF302 Q2).
- **Name census** (evidence/name_census.txt, tools/namecensus.py): no landed row defines a same-named helper with a different body.
- **Two name sources** for every name: both PDB name sets, or one PDB plus a donor FableWin call (tools/rcalls.py), the self Leaf, or the
  retail address store (cited in each STATUS note and RENAMES line).

## Rule notes
- NF303 Q1 / NF299 Q3 (`virtual` on a local base), NF304 Q6 (owner self fix), NF302 Q2 (unresolved before and after), NF300 Q2 (re-scope
  into the PDB namespace), NF281 D2 (a strong same-TU copy blocks a self move), NF299 Q4 (census skipped-port), NF305 Q2 / NF300 D1 (cl
  /Zs when clang fails identically on HEAD and new), NF284 (DeleteData hybrid).
- An override is implicitly virtual when its base declares the function virtual, so those census binders already bind UAE and need no change
  (u3_census_objects.txt).

## Slips (own)
- **009ba110**: a comment-only edit was re-prechecked in the same call that launched its N grade. That grade was rerun in a later call
  (logged in STATUS).
- **Entry 169**: the target 0x00DC0830 is an NF307 witness. The stay-off check missed it at first, because it checked only the binder.
  The respell was graded, then reverted (work/reverted/00dc0300.cpp) and its lock released. It is ready for a later batch (Q1).
- **00ac3a40 / 00618660**: `>>` in nested template arguments passed the clang precheck and failed VC7.1 (SRC_FAIL). Fixed to `> >`
  and regraded.
- **Entry 7**: the table line was first written as a delete, which would have left the target with no line. It is now a replace onto
  ?Compress@CRangeCompressor@@QAEJPBXJJPAEJ@Z.

## Questions
- **Q1 (entry 169)**: DepopulateLevel (Ego_r + FableWin; donor PopulateLevel calls it) is ready in work/reverted/00dc0300.cpp. Land it
  after NF307 releases 0x00DC0830?
- **Q2 (entry 41)**: AddTriangleList is declared public in the shared engine/C3DPrimitive2.h with a comment that says "private in Ego_r".
  The fix is an opt-in access change plus `friend` for the caller. Is an opt-in that changes the access of an existing shared-header line
  acceptable (equiv_check + planted control), or is it T3?
- **Q3 (entry 36)**: the self fix QAE -> QBE forces const onto the same-TU noinline stand-ins CFactionMap::Find / LowerBound, renaming them.
  T3, or is renaming a strong stand-in in the self's own TU acceptable?
- **Q4**: are table-only deletes of stale lines (found with stale.py and proved by reading the witness's HEAD object) acceptable as
  "done" in a naming batch? Five entries depend on it.
- **Q5 (entries 78 / 82)**: the DeleteData hybrid needs the nested T declared (CTCCreatureModeManager::CModeBase /
  NParticleEngine::CPSCSplineBase::CSplinePosParamData), which needs shared-header nested-type opt-ins. Is that a naming-batch task, or a
  header lane?
