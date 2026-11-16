# Landing gn follow-up (2026-11-16): the open items, done

Owner: "do all of these now. You can decide as the expert". Every row below is graded by `vland --recheck-landed` with
`LANDVERIFY_STRICT_HEADERS=1` and each row's own `// Flags:` (`vland_recheck.log`; non-strict `vland_recheck_nonstrict.log`), and
0x00BA50A0 by `--land` (`vland_land_00ba50a0.log`). Whole-tree equivalence against the previous commit
(`equivalence_changed_input_rows.tsv`) differs only in the rechecked rows and in `__FILE__` paths.

## 1. DoRender / Render (0x00435530 / 0x00435F70)
Ego_r.pdb: ?DoRender@CDisplayEngine@@IAEXPBVCInterpolationInfo@@_N1@Z 0x004490E0, then ?Render@...@@QAEXPBV...@Z 0x00449BC0 (0xAE0 later);
FableWin.pdb Render 0x01ADC6C0 (0x30 bytes) right before DoRender 0x01ADC6F0. Retail: DoRender = the 2,624-byte 0x00435530, Render =
the 5-byte `jmp` at 0x00435F70 (0xA40 later).
- `CDISPLAYENGINE_DORENDER` opt-in (chunk 1022) corrected: public Render at 0x00435F70, protected DoRender at 0x00435530, no friend.
- 0x00435F70 re-authored as `CDisplayEngine::Render` on engine/CDisplayEngine.h (was an `extern "C" Forward` placeholder spelled
  DoRender), with a real behaviour test. RELOCATION_MATCH.
- 0x00406CC0 respelled to call Render. RELOCATION_MATCH.

## 2. The three RELOC_CONFLICTs
- 0x00775B40 CTCShop::FrameUpdate calls 0x004376B0 = SetMoneyAsDisplaying (Ego_r Set*AsDisplaying order; reads PMoney +0x2CC), not
  SetCrimeAsDisplaying (0x00437770). New opt-in `FABLE_RETAIL_CPLAYERGUI_SETMONEYASDISPLAYING`; the CRIME opt-in's address
  corrected. 0x004376B0 itself re-authored as `CPlayerGui::SetMoneyAsDisplaying` on the shared headers (was a `__fastcall` free
  function on a local struct). MATCH.
- 0x00864620 respelled to call GetSightRadius (0x007448F0, SightRadius +0x40; GetExtendedSightRadius is 0x00744950).
- 0x004D2DDB re-authored as `CTCUndeadSoul::CTCUndeadSoul(CThing&) : CTCBase(thing)` on engine/CTCBase.h (FableWin.pdb
  ??0CTCUndeadSoul@@QAE@AAVCThing@@@Z; Ego_r ?Alloc@CTCUndeadSoul@@SIPAVCTCBase@@AAVCThing@@@Z). It was a generated body with a made-up
  CEngineSubPrimitive2DClockSprite base. RELOCATION_MATCH.
- The three conflicting bindings landing gn skipped are now recorded; every symbol involved has one address.
0x00406CC0, 0x00775B40 and 0x00864620 fail strict header policy because of local class models they already had at HEAD
(SHARED_HEADER_AVAILABLE CDisplayEngine, CGameTimeManager, CThing, ...), not because of these edits. Porting them is a separate job.

## 3. The four held name fixes
- 0x01436E24: `?GPEnvironmentDef@NEngineInternal@@3PBVCEnvironmentDef@@B` (Codex, PDB PUBLIC; the NEngineInternal::GP* pattern). acctC
  had found no PDB name. 0x00B26360, the only binding of the old descriptive spelling, respelled. RELOCATION_MATCH.
- 0x009FC3D0: `?AddToLoadingQueue@CResourceBank@@QAEXAAVCResource@@@Z` (acctC). Retail links into LoadingQueue +0x40; its twin
  0x009FC380 links into ResourceList +0x04 = AddResource. Catalog and landed row respelled. MATCH. The callers 0x00A42980 /
  0x00A25BD0 still bind AddResource spellings (queued).
- 0x007C2AB0: STLport `vector<C3DVector>(size_type)` (the queued spelling), not acctC's CArray(unsigned). The landed 0x00B149D0 /
  0x00B15BD0 match only through an inline CArray(n) forwarding to it, and acctC's "the only size ctor in either PDB" is wrong
  (FableWin has both). acctC's 0x007BFE30 respelled to bind it (RELOCATION_MATCH). Not done: re-authoring the 0x007C2AB0 row itself
  (still a local CSTLportVector placeholder; two tries on the shared headers gave 19 / 37 bytes, so stopped per the two-failure rule;
  docs/journal/FINDINGS_LOG.md) and 0x00B22B40's local CArray model.
- 0x00DCEC50: `?SetState@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAEXJ@Z`. The older queued line already said so. The row
  is re-authored on engine/CQ_OrchardFarmRaidScript.h (was CHandleBasedMemoryPool::GetPointerFromHandle on a local struct). MATCH.

## 4. 0x00BA50A0 SetupStaticBlocks landed
441 bytes (`ret 1Ch`), RELOCATION_MATCH + PASS. The source is the 0x00BA5260 TU's preamble and globals plus the stretch from
PeekGraphicInfoFrame to this function, verbatim (the whole TU grades the same); a new behaviour test models the blank / already-set /
per-block paths.
