# Landing nf325 (Account A, 2026-11-24): 31 rows on generated class models, two class-model defects fixed (37 headers, 1 new)

Hand-back: `pending/2026-11-24_nf325_handback/`; helpers under `naming_batch/agent_tools/nf325/`.
- **Entries:** 89 (NF322's held and port rows, two class-model defects, three NF321 requeues).
- **Outcome:** 32 done (31 rows plus the header-only fix of CAIStateGroup_EnemyCreatureBase), 1 skipped-held, 55 skipped-port.
- Landed together with nf327 (one recheck).

## What landed
- **25 CCombatSequence rows:** 24 GetName plus 0x00908DE0 OnStart, on 25 generated opt-in models.
  - CCombatSequenceBase's `_PASS27` guard is widened with `|| defined(FABLE_RETAIL_CCOMBATSEQUENCEBASE)` (NF322 Q1).
  - Add-only `protected:` on slots 16-18.
  - 0x008B3810 and 0x008B42E0 also move their callee from the stand-in `?Init@CCharString@@QAEXPBDH@Z` to the PDB constructor
    `??0CCharString@@QAE@PBDJ@Z`. 0x008B3810's test is rewritten, because it linked against the old stand-in member.
- **0x00A85FB0:** on a new CPhysicsFacesSegmentBranch model. The base model is unconditional, so NF322's `_PASS24` reason did not apply.
- **0x00BBDDB0:** on the fixed primitive bases.
  - CEngineInternalPrimitiveBase's SetChildPrimitive overloads are now in retail order (slots 7/8) under an opt-in.
  - CEngineInternalPrimitiveParticleGroup gains GetRenderLayerMask (slot 28) and 9 more missing overrides under `_RTTI_SLOTS`.
  - Census: no landed row makes a virtual call through slots 7/8 of that model.
- **0x00A0D2B0:** add-only `FABLE_RETAIL_CAINPUTPROCESS_INIT` (slot 2), plus a friend opt-in on CInputProcessManager.
  - Its callee moves from Register to the PDB `?AddInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z`: a table replace.
  - ref_scan: no other landed row references Register (243 rows compiled).
  - The owner still defines a stand-in, so AddInputProcess is queued in NAME_FIXES.
- **0x0090080B, 0x0090086F, 0x009008A1:** on the NEW `engine/CAIStateGroup_MazeBase.h` (NF322 Q2): gen_class_headers.py's flat output
  as the default branch, with the opt-in model.
- **CAIStateGroup_EnemyCreatureBase:** override declarations for slots 1, 2, 9 and 11 (header only).

## Proof
- **Per row:**
  - precheck;
  - vland HEAD and new under one work path;
  - objsec ALL_EQ under the renames, with no COMDAT added or dropped and no `??_7` / `??_R`;
  - `.drectve` equal on 4 rows, HEAD plus exactly `/DEFAULTLIB:"libcp"` on 27 (NF318 Q1);
  - the 31-object HEAD and new link: 0 duplicates.
- **Slot probes:** the compiler-emitted vftables match retail in every declared slot. The only difference is slot 0, where VC7.1 emits
  `??_E` and retail has `??_G`. The primitive-base control without the fix fails.
- **equiv_check:**
  - NF325 ran it over the 365-row include closure only, because of load.
  - At landing, the coordinator ran it over ALL 57,146 rows and tests, comparing rebuild/include with the include tree plus NF325's
    headers. Result: 8 differing lines (7 rows), all path-only. The text differs only in `__FILE__` strings carrying the tree's location
    (`logs/equiv_full_pathonly.tsv`, each re-preprocessed and compared after normalising the path). Real differences: 0.
  - The planted control flags 213 lines, including the `_PASS27` definer 0x006EAE20.
- **At landing:**
  - all 101 orig/ copies equal HEAD (the new header excepted);
  - recheck of the 41 nf325 + nf327 rows on the landed tree: 41 of 41 PASS (6 MATCH, 35 RELOCATION_MATCH);
  - every row grades its RENAMES spelling.
  - The 1,773 includers of the changed headers are proven unchanged by the equiv_check (identical preprocessed input), so they are not
    recompiled.

## Rulings on NF325's questions
- **Q1:** `friend` under an add-only opt-in is allowed (NF309 Q2). NF327 did 0x00A86030 this way.
- **Q2:** base slots declared `= 0` from retail `_purecall` plus the derived classes' override spellings are allowed under the opt-in,
  with the probe. NF327 did ServeFood this way.
- **Q3:** the new MazeBase header lands now. The 20 files that model it locally get SHARED_HEADER_AVAILABLE warnings only.
- **Q4:** CCombatSequenceBase's pure slots 1, 2 and 15 go to a later unit, under the new macro with the probe.

## Queued
- 56 T3 lines `nf325`.
- NAME_FIXES `nf325`.

## Recheck
`logs/recheck.log` (landed tree, 41 rows of nf325 + nf327): 41 PASS.
