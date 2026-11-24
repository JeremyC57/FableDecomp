# NF301 notes (2026-11-23): T1 "other" class / namespace / form, continuation of NF298, hand-back 1

Base: the repo working tree as of today. I never wrote to it and ran no git. "Entry n" = the n-th data line of ENTRIES.tsv (file line n+1).

## Summary (counts from STATUS.tsv)
- **59 entries with a verdict:** entries 1-60 in order, except 14, 15, 44, 45 and 46, plus 63 and 68 (in-place shaped, pulled ahead,
  E2) and 309 and 427 (stale-witness scan, E2). The prefix item is also done (NF298 entries 7 / 8, binder 0x00851860).
  - 14 / 15 / 44 / 45 / 46 were read and are in-scope fixes, not started. Each needs an add-only declaration opt-in in a shared header
    (see "Next"), so they have no STATUS line and stay open.
- **Status:** done 6, done-already 4, done-partial 8, skipped-port 29, skipped-held 12. No rejects, no vtable change, no header change
  (hdr/ empty).
- **6 rows changed** (rows.txt), all zero line delta, in place, in each row's own local model; tests unchanged:
  - 0x00B81BC0 (entry 8): member type `CPerlinNoise` -> `NFractal::CPerlinNoise`.
  - 0x00DC21F0 (entry 31): parent stand-in struct `CQ_BanditRaidScript` (no such PDB class) folded into its existing local
    `NScript::CQ_OakValeBanditRaidScript`; `ParentClass` retyped. Five vacated lines are comments.
  - 0x006778A0 (entry 38): free `CreateFromType` -> `static` member of the local `CReactionFrequencyTraits`, call qualified.
  - 0x006B84B0 (entry 48): member type `CScreenEffectPriorityStack` -> `NScreenEffect::CScreenEffectPriorityStack`.
  - 0x00717C50 (entry 63): free `GetMessageFromTextIndex` -> `static` member of the local `CSubtitledMessage`. The declaration shares
    the copy-ctor line, so no line moves; the freed free-declaration line is a comment.
  - 0x0092BCB0 (entry 68): view struct `CTCAvoidRegionExitView` renamed `CTCAvoidRegionExit` (no engine/CTCAvoidRegionExit.h exists).
- **Proof per row:**
  - HEAD and new objects both RELOCATION_MATCH (evidence/gr_h*.log, gr_n*.log).
  - objsec ALL_EQ under an explicit one-symbol rename map that covers exactly the DEF/UND symbol diff, so both function COMDAT sets
    are the same. Every non-debug section is equal, `.drectve` and the emitted vtables included (00DC21F0 3 tables, 00717C50 1,
    00B81BC0 2; evidence/proof_b1.txt, proof_b2.txt).
  - Link (evidence/link_A.txt), HEAD binders + 6 owners v new binders + the same owners: HEAD leaves the 6 stand-ins unresolved; the
    new set resolves all 6. 0 duplicates both sides. The remaining unresolved symbols are the owners' own callees, the same on both sides.
  - vland `--recheck-landed` on the new sources: 6/6 RELOCATION_MATCH, test PASS (recheck_final.log). Its SHARED_HEADER_AVAILABLE /
    STANDIN_LEAF / RELOC_NAME warnings come from local models and labels the rows already had at HEAD; no class was newly modelled.
  - No landed source or test includes any of the 6 changed rows by path. I grepped the 6 basenames over rebuild/src, rebuild/tests
    and rebuild/include and found none, so there are no funclet or folded-TU includers.
  - Table dry run: apply_table.py --dry on a scratch copy of reloc_bindings.tsv gave 21 / 21 hits, 0 misses, and no new
    multi-target symbol (evidence/apply_table_dry.txt).
  - Binder census: xsites + absrefs for every respelled target (evidence/xsites_nf301_*.tsv). Each changed row is the sole landed
    caller, or the other callers already bind the PDB spelling (0x00DC0300: 0x00DBF2D0 / 0x00DC08C0 use the NScript header; 0x00F36BE0:
    0x00AEBA90).
  - New-name census: each new symbol is defined only by its owner row (grep of `Class::Member` over rebuild/src and rebuild/tests; for CPerlinNoise the other hits are a declaration in 0x00AEBA90's model and comments);
    no same-TU helper under any of the names. Names: both PDB name sets for all six.
- **Table (TABLE_CHANGES.tsv, 21 lines):** 4 `replace` (the changed binders' spellings, where no PDB line existed yet) and 17 deletes.
  - 2 deletes are changed binders whose PDB line already exists (PopulateLevel, CPerlinNoise).
  - 15 are stale witnesses. Each witness HEAD object was compiled and read; it binds only the PDB spelling, or only another stand-in.
    - 0x00487520, 0x00449970, 0x00449960, 0x00CDD360, 0x00A821C0, 0x00961B90 (anon-ns hash normalised), 0x00D3AF50, 0x00DF2F90,
      0x0051CBB0;
    - the three NScript Init binders 0x00F09640 / 0x00D167C0 / 0x00D16BB0: the global copy of a doubled stand-in. A rider is the
      global WatchForContactDeath 0x00F09830 line: 0x00F09640's object defines only the NScript copy (evidence/objdump_readouts.txt).
      That target is in no agent's ENTRIES.
    - the prefix item: 0x00851860's CIMECalls lines (00404B50 / 00404C50).
  - The stale deletes are plain: for each stand-in, either the token appears in no other landed code (codegrep), or every landed caller
    was read (xsites / absrefs + source).

## Not done / left for others
- **Entry 19's PAV line** (`?GetPlayer@CPlayerManager@@QAEPAVCPlayer@@J@Z`, witness 0x0066FB80) is stale for its witness: the HEAD
  object binds only the PDB spelling. But about ten landed rows declare `CPlayer* GetPlayer(long)` on local CPlayerManager models and
  may bind it, so it is NOT deleted. It needs a rewitness from the gate scan.
- **0x00404A80 GFGetIME:** 0x00851860's HEAD object binds `?Get@CIME@NUISystem@@SIAAV12@XZ`, so the line
  `?GFGetIME@@YAPAUCIMECalls@@XZ 00404a80 00851860` is stale too. 0x00404A80 is an NF299 target, so I left it. Coordinator: merge it with
  NF299's hand-back.
- **Entry 3** (0x00AE8980): stale-shaped. 0x004174F1 carries both the stand-in and the PDB line, but it is an NF299 / NF300 witness, so
  it was not compiled.
- **Entry 32** (0x00DD1AC0): stale-shaped, but that source fails its own clang precheck (NF298), so it was not compiled.
- **0x0066FC90** was prechecked (test only, no lock) and then dropped from the set: it is an NF299 / NF300 witness. It was never locked or compiled.

## Decisions / questions
1. **Next: header batch (entries 14, 15, 44, 45, 46; also 75 further on).** They are the next work for this batch (NF301 continued or the next agent). Each binder
   derives a `_Calls` / `_Methods` / `ResourceMethods` class from the real class only to declare one PDB member that the shared
   header lacks:
   - CPixelFormat::GetInit() const;
   - CPlayer::PrepareForLoad;
   - CResource::SetResourceSize / GetUnusedFrames in the flattened branch (the existing opt-in is in the DEF_CLASS_BASE branch only);
   - CResourceList::UnloadNextResource;
   - CTCBuyableHouse::GetPrice() const.

   Each is an add-only declaration opt-in plus dropping the member from the derived stand-in class, done as one header batch with one
   equiv_check and a planted control. The binders' own self leaves stay T3 where they sit on the stand-in class.
2. **VC10-only PDB spellings (entries 6, 7).** The tracker's pdb_spelling for CParticleSystem::ClearParticle / ClearDecal names the VC10
   `std::_List_iterator<std::_List_val<..>>`. The repo STLport headers cannot emit that. 0x00AFCC30 / 0x00B1F200 already bind the STLport
   form, which is the attested spelling for STL instances (2026-11-20 ruling). Should the tracker map these entries to the STLport
   spelling, so that they count as converging instead of as splits?
3. **Polymorphic base nesting (entries 35, 51, 59).** I followed NF298's precedent (entries 37 / 39) and classed "the global base of
   the binder's own class must move into a namespace / class" as a port. For 0x0055C5B0 (entry 59) no vtable or RTTI is emitted in the
   object, so an in-place namespace move would probably be byte-equal. Do you want such no-vtable cases done in place?

## Rule notes
- **Locks.** Every compiled row was locked before its compile, and every landed test was prechecked before its lock (pre_tests1..5).
  - Each source was prechecked after its lock and before its grade (pre_src1..5, pre_rows1).
  - 0x00881370's test fails clang only on VC7.1 `_assert` (D1), so a VC7.1 `cl /Zs` passed in an earlier call
    (evidence/zs_t00881370.log).
  - All locks are released except the 6 changed rows.
- **Background runs.** Every grade, link, cl /Zs and vland run went to the background with `timeout 900` on its own launch line, and was
  checked with `kill -0` in a later call.
- **Not done:** no sleep, no rm, no git, nothing written under the repo, no `__asm` / `naked` / `_emit`.
- **Retail bytes:** none in hand-back files (hex-run grep). The vland oracle stays in work/pay1.
- **orig/** holds the HEAD copy of each changed row (cmp against the repo at hand-back: equal for all 6).
- **Reading list (disclosure).**
  - Read in full: my prefix, the standing brief, all of GOTCHAS.md, docs/COORDINATION.md, NF298's NOTES.md, LESSONS.md and
    NAME_FIXES.tsv.
  - Read only in part:
    - NF298's STATUS.tsv: lines cut to 400 characters.
    - NF298's TABLE_CHANGES / RENAMES / APPLIED: the head lines only. The rest was lost to a truncated output.
    - EXTRA_RULES.md: sections "names right the first time", "stand-in names", the 2026-11-20 pick-any / STLport / anonymous-namespace /
      relocation rulings, and the section index.
  - Not read: ASM_RULES.md (no asm in any change), and the nf281..nf285 landing READMEs.
- **Entry 29 callers:** the two local-model callers 0x00D3A1A0 and 0x00D3AE40 have NScript leaves
  (`?Main@CDF_Fireheart@CQ_FireHeartScript@NScript@@`, `?Main@CFH_Scythe@CQ_FireHeartScript@NScript@@`), so their parent class is in
  NScript.
- **Off-limits set** (work/offlimits.txt): NF299 / NF300 targets and witnesses, their locks, the unexpired codex-p28 reservations, the
  held rows and T3_PORT_QUEUE.
- **tools/:** NF298's set (paths set to this folder), plus:
  - fix301.py: the in-place edits, exact-match pairs, line count checked;
  - verdicts301.py + mkstatus301.py: the STATUS generator.
