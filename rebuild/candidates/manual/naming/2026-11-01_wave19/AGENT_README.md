# Naming pass wave 19 (2026-11-01): 136 landed rows respelled to proven names, all byte-identical

Inputs: brief `naming/briefs/BRIEF_W19.md`, the wave 18 README / AGENT_README / DEFERRED / scripts, QUEUE_W19_input.tsv (32 rows: 520 1,
c684 11, c685 20 -- the c685 rows were appended by the coordinator after chunk 685 landed, commit 70420de08; no c686 / c687 rows arrived)
and DEFERRED_W18_INPUT.tsv (the wave 18 leftovers incl. the 3 [recheck-fail] rows).

`scripts/compose19.py ov_final` (with `compose19_steps.py`) re-runs every stage, in order, on fresh copies of the current repo. hand19/ holds
the per-family hand stages (h19_stl, h19_c684, h19_c685, h19_own (+ h19_own_gif), h19_c680, h19_sfix); fam_stools19 / fam_gif19 / fam_xfer19 /
fam_cmp19 / fam_q18 are the drivers (`AGENT_README.md` has the commands). ov_final has 139 files: 136 sources plus 3 behaviour tests
(0x004E34B2 / 0x004DD782 rewritten self-contained for the STLport model, 0x0095AB50 for the filter rename).

**Recheck** against HEAD (commit 70420de08; the 8 chunk-684 rows and 8 chunk-685 rows landed during the wave are indexed in `idx/delta.jsonl`
/ `idx/delta2.jsonl` and merged into `idx/full.jsonl`, 53,144 rows) with vland --recheck-landed through scripts/vrun.py
(`logs/recheck_final.log`, `runs/final.pairs.jsonl`): **136 / 136 PASS (3 MATCH, 133 RELOCATION_MATCH).** No byte moved.
- The 0x007E5230 pgui respell (wave 18 [recheck-fail]) failed again in three models (DIFFER 817v817 / 811v817: VC7.1 swaps the two callee-saved
  reloads after the g_CurrentBet store under the class-static singleton read); it is reverted and stays DEFERRED with that finding.
- Nothing else failed its recheck: every family row of ov_final is in the final log. ov_final was then recomposed from scratch
  (`compose19.py ov_final`, so the ov_<fam>/ snapshots are current); 7 rows whose text had carried a duplicated comment / qualifier from a
  hand stage run twice before that (0x00405650 0x004051C0 0x0095AB50 0x00A89320 0x00A89350 0x00CAAEA0 0x00CAAF10) were rechecked on the
  recomposed text: 7 / 7 PASS, same symbols and pairs (`logs/recheck_fresh7.log`); every hand stage is now idempotent (proved by re-running all of them on ov_final: no file changed).

**BINDINGS.tsv:** 50 RETIRE / 37 ADD / 12 KEEP, no CONFLICT (the one CONFLICT mkbind19 first reported -- the same
`?GetPFirstThingInRoughArea@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_HasDefGlobalIndex@@PAPAVCThing@@@Z` bound to 0x007FCEF0 by
0x007FBC90 and resolved to 0x007FD110 by 0x00972830 -- was a wrong filter name in six callers of 0x007FD110, fixed in family stools, see below).
`runs/reloc_bindings.after.tsv` = the current repo table minus the RETIREs plus the ADDs; `bindings_multi_check.py` prints 0
(`logs/bindings_multi_check.log`). Index: `idx/full.jsonl` = HEAD (+ delta, delta2), `idx/after.jsonl` = HEAD + ov_final.

**LABELS.tsv:** 7 labels (bare catalog spelling). **RENAMES.tsv:** 9 rows, repo-rooted paths, bare new_name. Apply both after the content commits.

**QUEUE_W19.tsv** (32 rows): respelled 15, respelled-partial 3, already-correct 10, deferred 4 (0x00C28690 libpk second copy, owner decision;
0x00AB2BD0 / 0x00BEC680 / 0x00A36EC0 per-row re-models). **DEFERRED.tsv:** 440 rows (the queue leftovers + runs/deferred_extra.tsv: the
wave 18 input minus the 4 entries this wave resolved, plus runs/deferred19_manual.tsv).

## Families, in commit order (`runs/<fam>.rows.txt`, cumulative content in `ov_<fam>/`)
- **rfail 2** (wave 18 [recheck-fail], re-authored byte-identical on the STLport 4.6 text): 0x004E34B2
  `??1?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@QAE@XZ` (the `_Destroy` / `__destroy`
  forwarders `__forceinline` so the `__false_type` temporary becomes retail's `[esp+7]` byte; `__false_type` with a user ctor) and
  0x004DD782 `??$__destroy_aux@PAU?$pair@JVCCharString@@@std@@@std@@YIXPAU?$pair@JVCCharString@@@0@0ABU__false_type@0@@Z` (`__fastcall`
  template); both tests rewritten self-contained (`AUTO_TINY_004e34b2_TEST PASS`, `DESTROY_RANGE_FLASH_OK`). Files renamed (RENAMES).
- **stools 27** (scope 1, 0x008FD000 `?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ`): 25 rows take the protected
  const reference getter (`->` to `.`, the row's CThingSearchTools model members made const: the QAE->QBE respells of those members are the
  ADD / RETIRE pairs of the bindings), 31 rows driven; 6 skipped (DEFERRED: 2 pass the now-const tools to a free helper taking the non-const
  pointer, 3 stand-in classes on the flat engine headers, 1 derived-class getter). Plus h19_sfix: the filter of the 0x007FD110
  GetPFirstThingInRoughArea instantiation is `CThingFilter_HasScriptName` (the landed 0x007FD110 row: operator() 0x006A7E30 compares the
  ScriptName; 0x007FCEF0 is the HasDefGlobalIndex instance) in 0x0095B020 / 0x00972830 (called it HasDefGlobalIndex) and 0x007FBC90 /
  0x0095AB50 / 0x008C2240 / 0x0095B3E0 (called it HasTrackName) -- class rename, inline ctor, no bytes; 0x0095AB50's test renamed too.
- **c684 8** (scope 2): `?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ` in the three Allocate* callers 0x00BDA360 / 0x00BDA3D0 /
  0x00BDA470; `?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z` in OnDie 0x00663F50; both `NVectorMath::GFInterpolateVectors` overloads in
  0x00AE1260 / 0x00AE1600; the bool sixth parameter of `?ApplyRamps@CDspGain@@QAEXPBM0PAM01_NI@Z` (and the callers' own pass-through
  parameter) in Apply 0x00C74CE0 / Crossfade 0x00C74D30.
- **gif 10** (scope 2, c684): `class CGraphicInfoFrame::CFrameData : public CResource` nested, ctor `(const CGraphicInfoFrame*)`, the private
  const `InternalGetFrameForcedLoading(unsigned long)` in 9 rows (14 driven; 5 skipped: no CFrameData definition / a CFrameData_00a00d90
  model / the NUISystem-wrapped 0x0041B550), and the own row 0x009FF6B0 (private const, unsigned long: MATCH).
- **c685 20** (scope 2): ImeUi_Initialize / ImeUiCallback_DrawFans in CIME::Shutdown 0x004051C0 / Initialise 0x00405650; the C-linkage
  `_AMovieSetupRegisterFilter@12` in CBaseFilter::Register 0x00CAAEA0 / Unregister 0x00CAAF10; `CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData`
  in Reset 0x00AB36B0; `CEaxContext::SetSlotLoadEffect(unsigned long, SGuid)` in the dispatcher 0x00C689B0, itself respelled to
  `CEaxContext::SetSlotProperty` (the catalog name; file renamed); `?AddSaleItemPlacesFromThing@CTCShop@@QAEXPBVCThing@@@Z` in 0x00774B40;
  `?GetEntryDataSize@CBankFile@@QBEKK@Z` in 8 rows (fam_q18; 4 skipped). Own rows re-spelled (h19_own): 0x00AB3640 (the generic `OnDie` delete stub ->
  the DeleteData static member template, callee `??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ` 0x00AB34C0), 0x0099F560 (Dest_val ->
  `CCharString::RemoveWhiteSpaceBeginEnd` over RemoveWhiteSpaceBegin 0x0099F450 / RemoveWhiteSpaceEnd 0x0099F4D0), 0x00A89320 / 0x00A89350
  (`const`, MATCH), 0x00AB1480 (Move_CountedPointer -> the STLport `std::__copy<CCountedPointer<CBoneInfo>*, same, long>` template -- see
  the decision item). Files renamed (RENAMES).
- **c680 5** (scope 3, rows without a mangled final): 0x00C25860 `?Cross@C3DVector@@QBE?BV1@ABV1@@Z` (FableWin name set + body); 0x0063A390
  `?GetThemeSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z` (Ego_r donor order 0x90 apart GetGround / GetTheme / GetWater, callee
  `CMap::GetThemeSizeZAt` 0x008200B0) with the 0x00912700 view member; 0x009C4F20 `volatile bool ExitThread` (Retail-volatile line);
  0x00C73770 re-authored as `EnsureWindowsVersionDetected_00c73770` over the landed `?DetectWindowsVersion@@YAXXZ` 0x00C73660 (test rewritten).
- **xfer 26** (scope 4): the CPersistContext scalar Transfer overloads -> `template <class T> void Transfer(T& value);` (26 / 26).
- **cmp 38** (scope 4): `CBasicString<char>::Compare` as the protected static member of `template <class T> class CBasicString`, the row's
  string model deriving from it (80 driven; 42 skipped: free-helper shapes, CBasicString already defined, no declaration, compile errors).

## Not driven this wave (DEFERRED with reasons)
GetPWorldSeed 13 header-derived rows (the flat engine headers carry no CAIStateGroupBase base for the protected member: blocked on the engine
header migration), PeekTypeList 33, HasTC 11, the STLport set callers, 0x00CAF680, the destructor families, the CODA globals, 0x01238C8C,
the mega families AddEvent / GetInstance / GetPItem / ~CCharString (fam_q18 tried: 0 / 2 / 0 / 0 rows -- per-row class / namespace re-models),
GetSubDef<T>, cg10. Details in DEFERRED.tsv.

## Items for the coordinator
1. **0x007E5230** (pgui): the class-static singleton read cannot be reproduced (register allocation); the row keeps `?g_pPlayerGui@@3PAVCPlayerGui@@A`.
2. **0x00AB1480**: the queue's final spelling (chunk 685 NAME_FIXES, hand-mangled, in neither PDB) carries 4 parameters (`...PAV1@0ABU...`);
   VC7.1 mangles the 5-parameter STLport text as `...PAV1@00ABUrandom_access_iterator_tag@0@PAJ@Z` (probe: the 4-parameter shape would be a
   2-template-argument __copy, and retail pops 0Ch). The row's leaf and LABELS carry the compiler's spelling; NAME_FIXES should be corrected.
3. **0x008200B0 CMap::GetThemeSizeZAt** is IBE (protected const) in the PDB; the 0x0063A390 row keeps its public CMap model (LABELS adds the name).
4. **g_defStringInit at 0x0143CCC0** (0x00C73770's flag) kept its descriptive name (no PDB; the coordinator may respell with DetectWindowsVersion's own row).
5. **0x00AB34C0** is `??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ` (FableWin; LABELS): the catalog's CEntityBindingRecord_00ab34c0 is a propagated name.
6. **0x007FD110** six callers' filter was misnamed (now CThingFilter_HasScriptName); the vland RELOC_NAME warning on 0x00AB3640 names the stale 0x00AB34C0 label.
7. c685 targets left per-row: 0x00AB2BD0 (`__destroy_aux` through 0x00AB34C0's DestroyRange stand-in), 0x00BEC680 (std::map argument of
   CollectStats in 0x00BEC8B0 / 0x00B98DE0), 0x00A36EC0 (out-of-line ~CCountedPointer<const C3DMesh2> in 3 rows).
8. QUEUE c686 / c687 rows: none were appended to QUEUE_W19_input.tsv during the wave.
