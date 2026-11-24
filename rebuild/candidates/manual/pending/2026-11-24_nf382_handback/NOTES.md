# NF382 hand-back: q2 self renames, q6 identities, q9

Account A naming agent NF382. Work dir `$S/w22/agentNF382`. Nothing was written into the repo. Git: one read-only `git log` of two headers was run early in this session (no other git).

## Batches

| Batch | Rows | Verdict | Table |
|---|---|---|---|
| b1 | 00a4b7b0 004d15a0 004d1600 00a5a7d0 0071b2d0 007898a3 0081b300 0081b510 00826a00 006313a0 0046fece 004ea2ae 007691a0 00a52080 00a96350 00aaaf50 | 16 ACCEPT | replace ConvertToThisVersion @00825d40 |
| b2 | 00a13650 00a219e0 006bab80 006a7050 0091f990 | 5 ACCEPT | 3 deletes |
| b3 | units {00625350 + 006314c0}, {0067c490}, {00733020} | HOLD_RULING (QUEUE p1) | 2 deletes |
| b4 | 00e75ef0 | HOLD_RULING (QUEUE p2) | none |

Each run dir has `rows/`, `tests/`, `orig/`, `head/payload.json`, `RESULTS.tsv`, `TABLE_CHANGES.tsv`, `UNITS.tsv`,
`SELF_SYMBOLS.tsv` (proposed self_symbols lines, with the current line), `logs/` (vland grade lines) and `evidence/`.
There are no header edits, so there is no `hdr/` or `HEADERS.tsv`.

## Per target: verdict and its two sources
The full text is in each RESULTS reason (and in `tools/why382.py`).

- **0x00A4B7B0 -> `NBBBMaths::IsInRange`.**
  - The FableWin and Ego_r caller GetBestEnemyForCombatant 0x006EB700 is the table witness.
  - IsInRange is in lib_math_helpers.obj.
  - GFWorld3DDistanceSmallerThan is the landed self 0x006614F0.
- **0x004D15A0, 0x004D1600, 0x00A5A7D0 -> STLport `sort_heap`.**
  - Retail STL is STLport (NF301).
  - The landed __partial_sort witnesses are 004d1b40, 004d1bc0 and 00a5a8a0.
- **0x0071B2D0 -> `CTCThingOwner::SetAsDrawable`** (non-virtual).
  - The caller CThing::SetAsDrawable 0x004C8E50 is in FableWin and Ego_r.
  - No vtable holds 0x0071B2D0. OnSetInLimbo is vtable slot 16 at 0x0071B2A0.
- **0x007898A3 -> `CTCDPhysicalObstruction::SetThingToIgnoreObstruction`.**
  - The Ego_r tc_trap.obj body is 41 bytes, the same as retail.
  - Four retail callers match the PDB callers.
- **0x0081B300 / 0x0081B510 -> `CTextureMorphs` / `CParticleMorphs::TransferBinaryIn`.**
  - The Transfer<T> callers are 0x004E7822 / 0x004E78F2, and the FableWin and Ego_r traits agree.
  - The real CExpressionSet::TransferBinaryIn is 0x0086F680.
- **0x00826A00 -> `CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>::ReadFromFile`.**
  - The caller is CMap::LoadBrushesFromFile 0x00821010 (FableWin and Ego_r).
  - 0x49 bytes = the 73-byte CFileFormatBrush.
  - **0x00825D40** needs no row edit: the table line is replaced.
- **0x006313A0 -> `CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, ...>::operator=`.**
  - The caller is CCutsceneDef::operator= 0x00F2A000.
  - FableWin CParentDefClassBase::operator= calls it.
- **0x0046FECE -> `CAppearanceModifierGraphics::CAppearanceModifierGraphics`** (binder 0x0046FE40, G066 sibling argument).
- **0x004EA2AE -> `CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::CPersistTraits`.**
  - The FableWin Transfer<CReplaceableMeshes> calls it directly, and the binder is 0x004EA21E.
  - Wrapper-level question: QUEUE p6.
  - The old test used naked inline asm; the new test is plain C++.
- **fill rows:**
  - 0x007691A0 = `fill<CDefPointer<const CThingBaseDef>>`.
  - 0x00A52080 = `fill<C3DAnimationSequenceData::C3DVectorWord>`.
  - 0x00A96350 = `fill<CCountedPointer<C3DClothPrimitive>>`.
  - 0x00AAAF50 = `fill<CPlane>`.
  - Evidence for each: the retail caller (table witness), plus the FableWin _Insert_n of that element's object.
- **0x00A13650 -> `vector<CPlane>::reserve`.**
  - The caller is 0x00A12EE0.
  - Every caller of its copy callee 0x00A11210 is a vector<CPlane> body.
- **0x00A219E0 -> `list<CNavWayPoint>::pop_front`.**
  - The caller is CNavigatorAStar::CalculateAndMoveToPosProximity 0x00A1E5C0 (FableWin).
  - The shared engine/CNavWayPoint.h gives the 10-byte element.
- **q6, 0x006BAB80 -> `CPersistContext::GetDefaultVal<CFaction>`.**
  - FableWin faction_manager.obj order: GetDefaultVal<CFaction> sits directly before Transfer<CFaction> (0x006BABD0).
  - Its callees 0x006BA420 / 0x006BA6D0 are the CFaction ctor and dtor (witness 006bace0).
  - Ego_r CFaction is 0x1c bytes.
- **0x006A7050 -> `CThingFilter_RadiusIntersectsInfiniteLine::operator()`.**
  - Retail calls it from 0x007466D4 (landed 0x00746560, witness) and from 0x00745EF8.
  - Ego_r / FableWin define it, and the shared header on main declares it at this address.
- **0x0091F990 -> `CIntelligentPointer<const CThing>::CIntelligentPointer<CThingCreatureBase>(CIntelligentPointer<CThingCreatureBase>&)`.**
  - All 8 landed retail callers name it.
  - The FableWin callers are the same functions.
- **0x00625350 -> `NUISystem::CUIStateDef::operator=`** (held).
  - The binders 00632f80 (witness), 00632ff0, 006315e0 and 00625340 name it.
  - The FableWin ui_def1.obj helpers call it.
  - The real CConfigOptionsDefaultsDef::operator= is the landed 0x004345A0 (vtable slot 19).
- **0x006314C0 -> `std::__copy<const NUISystem::CUIStateDef*, NUISystem::CUIStateDef*, int>`** (unit binder, NF381 QUEUE p2).
  - The table already binds that spelling here (witness 006315e0).
- **0x0067C490 -> `CBlurredAttitudesDef::operator=`** (held).
  - The caller 0x0067D450 is reached from COpinionReactionManagerDef::operator= 0x0067CE50.
  - FableWin's COpinionReactionManagerDef::operator= calls vector<CBlurredAttitudesDef>::operator=.
  - The landed CBlurredAttitudesDef::Copy expands the same copy.
- **q6, 0x00733020 -> `NEntityEvents::CIsHitEventHitBy::operator()`** (held).
  - Its callee 0x00810FC0 is CEventHitBase::GetPHitBy (witness 007afb90). Every landed caller of 0x00810FC0 is a hit-event body.
  - Of the FableWin functors, only the tc_combat CIsHitEventHitBy body matches.
- **0x00E75EF0 -> `CV_ExposeMayorScript::PreventConflictWithTCE`** (held).
  - CV_ExposeMayorScript::Main stores its address at 0x00E75EA0, as Ego_r Main stores PreventConflictWithTCE.
  - WatchForEndOfScript is 0x00D013A0 (stored by CQ_BanditCampScript::Main at 0x00D00E4D), which is not landed.
- **Skipped / parked:**
  - 009e6530: joint unit (QUEUE p3).
  - 00a55500: closure (QUEUE p4).
  - 004382c0: Account B.
  - 00533cc0: T3 nf294.
- **q9, 0x0066C000 / 0x0066C150:** the CSmallVector<NAnimationEvents::CEvent,32> dtor / Clear pair. The order is unproven, so it is not respelled (QUEUE p5).

## Gates (all rows)
1. **vland.**
   - HEAD and new were graded at one LANDVERIFY_WORK path per run (r1 = 18 rows, r2 = 8, r3 = 006a7050 regrade after a comment-only fix, r4 = 004ea2ae regrade after a test-comment-only fix; both regraded objects equal the earlier ones).
   - All results are MATCH or RELOCATION_MATCH, and all tests PASS (PassPattern tokens kept).
   - Non-blocking warnings: STANDIN_LEAF (expected until the catalog labels change, see RENAMES.tsv) and SHARED_HEADER_AVAILABLE (QUEUE p7).
2. **objsec.**
   - objsec374 gives ALL_EQ for every row except:
     - 00a13650 (10 renames, beyond its search);
     - 00a219e0 (4 COMDAT sections emitted in another order).
   - `tools/objsecperm.py` (section-tied, order-independent) gives ALL_EQ_PERM for both, and for every multi-rename row.
   - objsec374 chose a swapped but valid bijection for 006bab80; the shipped renames file is the section-tied one.
   - 006314c0 `.drectve` gains `libcp` (shared STLport header).
3. **Pick-any checkers.**
   - pkdiff over both runs: no new NEQ line. Only HEAD lines are removed (wrong-name EQ / NEQ twins).
   - reachflip prints 1 flip for 00a13650 at 0x00A11210. It is a rename artifact: the HEAD C2DLineF copy and the new CPlane copy are both NEQ at reach y.
4. **Census and link.**
   - **Prefilter:** clang (MSVC mangling) over the 2,763 landed rows that name either spelling (`evidence/clcensus*.txt`).
   - **VC7.1 compile:** 83 binders and definers (`census_c1.txt`, `census_c2.txt`), including the clang failures that carry old-spelling tokens; `census377.py` over those objects for every old self spelling and every TABLE_CHANGES symbol is `census_vc71.txt`.
   - **No other binder moved:** the table scan of every changed address found exactly one old-spelling line at any of them (`??4CConfigOptionsDefaultsDef` @00625350, witness 006314c0, handled by unit b3 u1); every other old spelling is either unbound or bound at its real holder's address, and census_vc71 shows its other UND users (GFWorld3DDistanceSmallerThan, SetThingAsImmune, CExpressionSet::TransferBinaryIn, ??0CIndexBuffer) resolve to those landed holders after the rename.
   - **/FORCE /MAP link per batch over HEAD:** `link_bN.txt`, plus the symbol-level `dupdiff_bN.txt`.
   - **b1 / b2:** no new duplicate symbol, every new name resolves, and nothing HEAD resolved is lost. The b1 GFWorld3DDistanceSmallerThan NEW_DUP lines are HEAD duplicates among other rows, re-attributed; that symbol drops from 4 duplicates to 3.
   - **b3:** NoDuplicates self v Any inline copies of the same right-named function (`dupsel_*.txt`). Ruling QUEUE p1.
   - **b4:** 00d00af0 loses WatchForEndOfScript. Ruling QUEUE p2.
5. **apply_table --dry:** 0 misses per batch and combined, and no new multi-target symbol (`apply_table_dry_*.txt`).

No header edits and no `__asm` / `naked` / `_emit` / `volatile` in any row source or test (grep over every `rows/*.cpp`).

## Stay-off
`evidence/stayoff_sweep.txt`: Account B authored payload addresses and staged files, COORDINATION_CLAIMS, Codex RESERVED files, BRIEF_NF_BATCH, the NF378 lists and the NF381 / NF383 batches: no hit except NF381 notes deferring 00625350 / 006314c0, 006bab80 and 00826a00 to NF382. T3_PORT_QUEUE: no changed row in column 2 / 3. Line 3565 (nf345) holds 0x004345A0 with 0x00625350 as one unit; the b3 u1 rename removes that clash and leaves 0x004345A0 untouched, so u1 needs that hold released as well as the QUEUE p1 ruling. Line 1653 (nf311) names 0067c490 as a binder of the 0x00471E44 stand-in, which it keeps.

## Hand steps for the coordinator
- Land b1 and b2. Apply their TABLE_CHANGES and SELF_SYMBOLS lines, and the RENAMES.tsv catalog Module / name / file lines (NF382 renamed no files).
- b3 / b4: land after the QUEUE p1 / p2 rulings, as whole units (UNITS.tsv). b3 u1 also needs the T3 nf345 hold released and carries the ??4CConfigOptionsDefaultsDef table delete; u3 carries the GetHomeBuilding delete.
- NAME_FIXES.tsv lists the callee spellings kept to hold each closure to the self, and the stand-ins found on the way.

## Counts and locks
- **Compiles:** about 145 VC7.1 object compiles (pilot 4, r1 36, r2 16, r3 2, r4 2, census c1 71 + c2 12), plus free clang prefilter compiles.
- **Locks (owner NF382), kept until landing:**
  - b1–b4 rows: 0046fece 004d15a0 004d1600 004ea2ae 00625350 006313a0 006314c0 0067c490 006a7050 006bab80 0071b2d0 00733020 007691a0 007898a3 0081b300 0081b510 00826a00 0091f990 00a13650 00a219e0 00a4b7b0 00a52080 00a5a7d0 00a96350 00aaaf50 00e75ef0.
  - 006314c0 was claimed for the unit; its lock was missing, and NF381 had skipped it to this unit.
- **Released:** 00a55500 (parked). 0066c000 / 0066c150 already read `released`. 009e6530 and 00825d40 were never claimed.
