# NF309 notes (2026-11-23): naming batch, hand-back 1 (198 entries: virtual overrides 1-3, held 4-17, open 18-34, tracker T1 / T2 35-198)

## Summary
- **28 rows changed, all landing-ready** (`rows.txt`). All 28 graded RELOCATION_MATCH PASS in vland `--recheck-landed`
  (`recheck_final.log`, payload work/rc3: WINS 28, no DIFFER / RELOC_CONFLICT / LINK_FAIL / SRC_FAIL). The warnings are pre-existing
  local-model / label warnings.
  - The final run follows comment-only respells of stale callee names in 7 rows (006a8560 006ba030 007e7ac0 008326a0 009f4910 00bad040
    00bf9aa0). They were prechecked in an earlier call; the pre-edit copies are in work/precomment/.
- **Unit 5 withdrawn** (entries 75 / 80 / 81: 0085a840, 00ac3a40, 00618660, and 4 table lines). Their DeleteData targets 0x006174F0 /
  0x0085A5A0 / 0x00AC3A10 are in the NF284 D4 "deferred-12" T3 unit (T3_PORT_QUEUE line 74). The final stay-off rerun caught this.
  The work, which was proven, is kept in work/reverted/unit5/ (see Slips and Q6). Their locks are released.
- No shared-header, flag, catalog or test change; hdr/ is empty. Tests of the changed rows are standalone model copies; none includes its row
  or a local header (grep). They are unchanged and were prechecked before their locks.
- orig/ holds the HEAD .cpp and _test.cpp of every changed row (56 files). All equal HEAD after the mid-run NF307 (05:06) / NF308 (05:15)
  landings. orig/tools_mkstayoff.py.nf306 is the tool as received.
- **TABLE_CHANGES.tsv: 28 lines** (22 delete, 5 replace, 1 rewitness).
  - Each was checked against the current table (0 problems); tblafter3 reports PDB_OK for every target.
  - **Delete audit** (evidence/delete_audit.txt, tools/delaudit2.py): every binder of every deleted or replaced symbol (the tracker
    spellings column, plus exe call / absolute / vtable references and the mid-run landed rows) is MOVED in this hand-back, or is a
    stale-line witness whose HEAD object no longer references the symbol.
- **Entries: all 198 have a STATUS line.**
  - done 16: 7 11 24 26 40 47 49 50 67 85 122 130 142 166 182 184. Of these, 24 / 26 / 47 / 85 / 166 are table-only stale lines.
  - done-partial 9: 1 2 3 10 48 103 129 140 189.
  - done-equivalent 1: 51 (NF301 STL spelling).
  - skipped-port 72:
    - 62 from the census (tools/census309.py, NF299 Q4). Every stand-in binder binds through a class or scope in neither PDB name set, the
      wrong class, or a free stand-in where the PDB has a member. 1 in 10 were sampled and every HDR_HIT entry was opened.
    - 5 were opened and bind another class's member.
    - 35: NF281 D2, the self fix duplicated a binder's strong same-TU copy.
    - 38: the self defines QAE; its const fix needs `mutable`, which is T3.
    - 44: the binder is a free stand-in function.
    - 45 / 46: the binders DEFINE strong same-TU stand-ins.
  - skipped-held 66:
    - 17 from the census (a binder is stay-off or T3);
    - 14 in the CPersistTraits family (T3 unit nf300);
    - 12 in the nf284 deferred-12 T3 unit (71-82);
    - 11 held entries (4-17) that still hit NF307 / NF308 entries, witnesses or locks after a re-read;
    - 169 (the target is an NF307 witness);
    - the rest are binders or targets that are stay-off, T3 or standing held.
  - open 34, not reached (18 19 20 21 22 23 25 27 28 29 30 31 32 34 36 41 96 98 101 107 108 110 111 124 125 136 145 168 176 177 178 181
    197 198). Each STATUS note gives the census class and the shape it needs.
- **Order of work**: a warm-up on unit 1 (35, 38, 40, 47-50, 67), then entries 1-3 (units 2 / 3), then the held entries 7 / 10 / 11 re-read
  after the NF304 / NF305 landings (unit 4), then the census over 35-198 (units 6 / 9 plus the table-only stale lines).

## Units (detail in each STATUS note)
- **unit 1** (00694020, 00bf9aa0, 00b662f0, 00845900, 004d4a99):
  - an access move: UpdateActionMovement under protected:;
  - pointer -> reference / const parameters in the PDB spelling, with the call sites passing `*p` or the object.
  - Entry 48: 0x00B99710 binds a different spelling (T3); its line stays.
- **unit 2** (entries 2 / 3; NF303 Q1 / NF299 Q3):
  - 0078b410: Initialise is virtual in its local base and override.
  - 00694d10: owner self fix (NF304 Q6). The v12 placeholder became `virtual void Initialise();` at slot 12, and the trailing non-virtual
    declaration was removed.
  - 0082d810 / 00704150 / 00836260: CThing gets `virtual bool WorldUpdate();`, and the base plus the own override are virtual.
  - Full own Leafs are in RENAMES section 1. The self 0x00838BA0 is the EditWorldUpdate stand-in (NAME_FIXES).
- **unit 3** (entry 1, NF303 Q1 + NF300 Q2):
  - 005469e0 / 00547c90: virtual ChangeState.
  - 0053ebf0: the global struct was re-scoped into `namespace NUISystem` as a class with public bases (objsec ALL_EQ).
  - 00543df0: ChangeState(long) -> (unsigned long) in its base and its override.
  - 0054a990 is rewitnessed (its HEAD object binds the global QAE). The stand-in classes 0054c270 / 0053d3b0 / 00540e20 / 005419e0 stay.
- **unit 4** (7, 10, 11):
  - 00f3afc0: CompressToBuffer -> the Compress overload, with its own Leaf written in full.
  - 009261f0: protected `CPlayerManager& GetPlayerManager() const`. evidence/u4_008fcf90_decl_census.txt shows it was the only pointer-form
    caller among 34 landed callers.
  - 009ba110: SkipUntilString(const CCharString&).
- **unit 6** (103, 122, 130, 142, 182, 184):
  - GetCamTools under protected:;
  - WriteData -> WriteCompressed;
  - PrioritiseBlock -> SetBlockToHighPriority, in both callers;
  - CalculateLODBand -> GetDesiredLODBand;
  - PlaySound -> PlaySoundW;
  - SetWillMorphAmount -> SetWill.
- **unit 9** (129, 140, 189):
  - RegisterEnable -> AddEnableConsoleVariable;
  - Init -> protected Initialise(), with the call qualified;
  - IsScriptTaskRunning / IsTaskListEmpty -> `IsActionListEmpty() const`, in 3 binders.
- **table-only stale lines** (tools/stale.py): 24 / 26 (witness 0x00B1B0C0), 47 (0x006BBC30), 85 (0x008D43C0), 166 (0x00899C90).
  - In each, the witness was re-authored and its HEAD object references the PDB spelling, so the old line binds nothing.
  - Entry 166: NF307 rows 00683590 / 0072c230 landed mid-run and call 0x00449970 as GetMainPlayer. Test 00899cf0's local cdecl mock
    named Sub_ResolveA is not the fastcall table symbol.
- **T3 mentions that remain** (rows or table addresses named in a T3 queue line as a binder of another target): 00b662f0 (nf287
  0x00B64550 / 0x00B66190), 00704150 (nf302 0x007C21A0), 00449970 (nf301 0x004498C0), 006bbc30 (nf300 0x006BB860 / 0x006BB940). My
  edits there are other callees on other lines.

## Proof
- **objsec** (tools/proof2.py with exact old=new pairs): ALL_EQ for all 28 rows, every non-debug section and relocations included. There is
  no new ??_7 / ??_R symbol, and every new symbol is the entry's PDB spelling, 1:1 (evidence/proof_u1 / u2 / u2b / u3 / u4 / u6 / u9 /
  u9b.txt; proof_u5.txt belongs to the withdrawn unit).
- **Links** (evidence/link_u*_*.txt): the HEAD set was linked against the new set. Nothing HEAD resolved becomes unresolved, and there are no
  new duplicates. The only new spellings still unresolved point at the stand-in selves 0x00838BA0 / 0x008FCF90 (NAME_FIXES; NF302 Q2).
- **Name census** (evidence/name_census.txt, tools/namecensus.py v2): it covers every new symbol (the TABLE_CHANGES targets plus RENAMES
  sections 1 and 2), and every OTHER hit was read. No landed row defines a same-named function with a different body. 00b31160's
  SetCameraState is a documented same-TU strong copy of the owner's body, verbatim, and it exists at HEAD.
- **Two name sources** for every name: both PDB name sets, or one PDB plus a donor FableWin call (tools/rcalls.py), the self Leaf, or the
  retail address store (cited in each STATUS note).

## Tools (tools/)
- New:
  - decl.py: caller declaration census;
  - silook.py: stand-in binder lines;
  - census309.py: the SAMECLS / NOTPDB / WRONGCLS / FREE classes;
  - hdrdecl.py: HDR_HIT;
  - stale.py: stale-line candidates;
  - absrows.py: absolute references -> rows;
  - namecensus.py: new-name census;
  - delaudit.py / delaudit2.py: the delete-line audit;
  - lku1.sh: links.
- Modified:
  - mkstayoff.py: NF307 / NF308, the t3mention set, and the T3 queue's unit lines (column 2 not an address) put their column-3 targets in
    t3rows. The original is in orig/tools_mkstayoff.py.nf306.
  - so_check.py: T3M / T3MW / T3MC flags.
  - pre.sh: CLANG_RC.

## Slips (own)
- **Unit 5 / T3 queue**: mkstayoff took only column 2 of T3_PORT_QUEUE.tsv. The nf284 "deferred-12" line lists its 12 targets in column 3,
  so entries 75 / 80 / 81 were worked on stay-off targets. The final rerun caught it and they were withdrawn. The tool is fixed, and t3rows
  grows by exactly those 12.
- **009ba110**: a comment-only edit was re-prechecked in the same call that launched its N grade. That grade was rerun in a later call.
- **Entry 169**: the target 0x00DC0830 is an NF307 witness. The stay-off check missed it because it covered only the binder. The respell
  was reverted (work/reverted/00dc0300.cpp) and its lock released.
- **00ac3a40 / 00618660**: `>>` in nested template arguments passed clang and failed VC7.1. Fixed to `> >` (that unit was later withdrawn).
- **Entry 7**: the table line was first written as a delete, which would have left the target with no line. It is now a replace.
- **Name census v1** read the RENAMES section-2 names from the wrong column and missed constructors. v2 fixes both (the v1 output is in
  work/name_census_v1.txt).

## Questions
- **Q1 (entry 169)**: DepopulateLevel (Ego_r + FableWin; the donor PopulateLevel calls it) is ready in work/reverted/00dc0300.cpp. Land it
  after NF307 releases 0x00DC0830?
- **Q2 (entry 41)**: AddTriangleList is public in the shared engine/C3DPrimitive2.h, with a "private in Ego_r" comment. The fix is an
  opt-in access change plus `friend` for the caller. Is an opt-in that changes the access of an existing shared-header line acceptable
  (equiv_check + planted control), or is it T3?
- **Q3 (entry 36)**: the self fix QAE -> QBE forces const onto the same-TU noinline stand-ins CFactionMap::Find / LowerBound. T3?
- **Q4**: are table-only deletes of stale lines (found with stale.py and proved by reading the witness HEAD object) acceptable as "done"?
  Five entries depend on it.
- **Q5 (entries 78 / 82, now held)**: the DeleteData hybrid needs the nested T declared (CTCCreatureModeManager::CModeBase /
  NParticleEngine::CPSCSplineBase::CSplinePosParamData), which needs shared-header nested-type opt-ins. Is that a naming-batch task, or a
  header lane?
- **Q6 (unit 5)**: three deferred-12 entries were in fact single-shape hybrids. RELOCATION_MATCH, objsec ALL_EQ and the link all passed
  (evidence/proof_u5.txt, link_u5_*.txt; 0085a5a0 keeps a rewitness onto its T3 binder 0x00905FD0). Should the T3 unit pick them up from
  work/reverted/unit5/ (rows + TABLE_LINES_unit5.tsv), or should a naming batch be cleared to land them?
