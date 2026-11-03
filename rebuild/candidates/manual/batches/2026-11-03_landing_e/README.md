# Landing 2026-11-03e: CWorld::UpdateFrame (chunk 734), 20 inline-depth rows redone pragma-free, chunk 734 name fixes

## Result
- **Landed 0x004A5A40 CWorld::UpdateFrame** (chunk 734) RELOCATION_MATCH + PASS (`logs/land_c734.log`).
- **20 inline-depth rows re-landed pragma-free** (`// Revisit: inline-depth` 90 -> 70). All 20 sources from `../2026-11-03_inline_revisit2/out/`
  were regraded read-only on the current tree first, after part A (`logs/c_regrade*.log`: 20 RELOCATION_MATCH + PASS). Then `repair_remove.py`
  (`logs/repair_remove_c.log`) and `vland --land` (`logs/land_redo.log`: WINS 20, every test PASS).
  - Marker dropped (10): 0x00715110, 0x00D79D50, 0x00D7A360, 0x00EE7A20, 0x00D86C30, 0x009F7890, 0x00EF6FE0, 0x0066BD00, 0x0066BDF0, 0x00BC9B80.
  - Per-exit masks with the line-2 `// Inline-split:` note (10): 0x00CE0920, 0x00CE0B00, 0x00CE0CE0, 0x00CF02A0, 0x00D151D0, 0x00DDE230,
    0x00E02E50, 0x00E6B750, 0x00EC0BA0, 0x00DD6440.
  - 0x009F7890 lands with `CompilerFlags = '/O2 /Oy /W3 /Od'` (precedent 0x00D1EB80). vland gave it a new file name; it was moved back to
    `00/9f/STL_Construct_009f7890.cpp` (+ `_test`).
  - The `#pragma inline_depth` lines that 7 tests had copied into their body copies were dropped (0x00715110, 0x0066BD00, 0x0066BDF0,
    0x00CE0920, 0x00CE0B00, 0x00CE0CE0, 0x00E6B750). Behaviour is unchanged and every test still PASSes.
  - None of the 20 lost its match, so no row kept its old landed form.
- **Not landed: chunk 733 (0x004075C0 RenderUIBackground, 0x0040BCA0 LoadGameState).** See "Chunk 733 access check" below.
- **Chunk 735:** not staged (`pending/2026-11-03_linux_hand735/` does not exist), so it was skipped.
- **Coverage** (`write_decomp_dashboard.py`, `update_readme_progress.py`):
  - 46,544 genuine, 93.907% of the 49,564-row catalog (was 46,543);
  - 54,077 / 57,609 known, 93.869%;
  - 5,708,049 bytes, 55.807% of function bytes (was 5,706,947); 38.416% of `.text`;
  - landed rows 54,089 -> 54,090.
- **Checks on the final tree:**
  - `bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` prints 0;
  - catalog rows 54,090 == `Address = '` entries 54,090;
  - `sync_manifest_from_catalog.py --labels --dry-run` reports 0 changes;
  - `git diff` and the new files hold no lane-scratch (`$S`) path (grep count 0);
  - hdrcheck2 of the HEAD copies against the final headers: 27 of 27 `default=same` (`logs/hdrcheck2_final.txt`).
  - Final `--recheck-landed` of every row this landing touched (31: UpdateFrame, the 20 redos, 9 respelled rows, 0x0047A9F0):
    31 RELOCATION_MATCH + PASS (`logs/final_recheck*.log`).

## Part A: chunk 734 headers (27 changed, 2 new)
- I applied the overlay `pending/2026-11-03_linux_hand734/hdr/` to `rebuild/include` as staged. Before applying, every overlay file contained
  every line of its repo copy, except the hunks inside the branches (CWorld.h, fable_string.h).
- New headers: engine/CTestQuest.h, engine/CEnginePrimitiveWeatherMask.h.
- `hdrcheck2.py` on the overlay (`logs/hdrcheck2_734_pre.txt`) prints `default=same` for all 27 changed headers.
  The `(cpp-err)` on CCombatManager.h and on CRegion.h's retail branch is the same on both sides.
- **Includer recheck** (`includer_recheck_grades.tsv`). 4,794 landed sources and tests reach a changed header (`scripts/` from landing d:
  includers.py + ppcmp.py).
  - 3,238 preprocess token-identically (`logs/ppcheck_a.txt`). 1,556 differ.
  - Of the 1,556, 1,512 differ only through the three declaration-only hunks (CGameScriptInterface::WorldUpdate, CTCBase::UpdateGlobalTCs,
    CCharString::EqualsCaseInsensitive's inline body; no landed row calls it). This was checked by preprocessing against a mirror with those
    three headers reverted (`scripts/ppcmp2.py`, `logs/ppcheck_a_nontrivial.txt`).
  - 44 rows change otherwise.
  - Regraded with `vland --recheck-landed` on the final tree (`logs/rc_a*.log`): the 44, chunk 734's 78 SIBLINGS.tsv rows, every 40th of the
    1,512 and 0x0047A9F0. That is 119 rows: 116 RELOCATION_MATCH + 3 MATCH, all PASS.
  - Every row that matched before still matches. Chunk 734's earlier-mirror test FAILs (0x004DA967 .. 0x007F0890) do not occur.

## Part A: chunk 734 names (`NAME_DECISIONS.tsv`, `NAME_FIXES_c734.tsv`)
- All 14 NAME_FIXES and 4 LABELS were accepted; none was rejected.
  - Source 1, the PDB name sets: 13 are exact in both Ego_r and FableWin. GetDef<CThingBuildingDef> is exact in FableWin, and Ego_r has the
    same `QBE ... CDefPointer<const T>*` template family.
  - Source 2, the retail bytes: every address is a call target in the chunk's retail disassembly (UpdateFrame or PreparePrimitivesForEngine),
    with the argument and `this` use that the notes describe.
- **Key fix:** 0x004A5E90 is `?AutoLoadRespawn@CWorld@@QAE_NXZ`, and AutoLoadRegionLeaving stays on 0x004A34F0 only.
  - Sizes: Ego_r 1643 / 539 B and FableWin 948 / 236 B against retail 1722 / 564 B, in the same address order.
  - Manifest 0x004A5E90 is now AutoLoadRespawn.
- **Respelled in this landing.** Each was regraded before (`logs/nf_base.log`) and after (`logs/nf_respell_recheck.log`,
  `logs/deletefilew_recheck.log`); all are RELOCATION_MATCH + PASS both times. Their `reloc_bindings.tsv` rows were rebound to the right symbol.

  | Binder | Respell |
  |---|---|
  | 0x006294A0 | AutoLoadRegionLeaving -> AutoLoadRespawn (source, test, line 1) |
  | 0x004A05C0, 0x0049E740 | DeleteFile -> DeleteFileW (EDriveReturn return, forward enum) |
  | 0x009E4BC0 (the definition) | DeleteFileW on engine/CDriveManager.h's FABLE_RETAIL_CDRIVEMANAGER branch; the API call is `::DeleteFileW`; `// Leaf:` added |
  | 0x004C9D20 | IsTimeForFullUpdate const |
  | 0x005BC5C6, 0x005EE99D | GetPlayerNumber -> GetJoystickDeviceNumber const |
  | 0x0083C690, 0x004C5EA0 | IsInitialised -> CInitBaseClass::IsValid (local CPlayer derives from engine/CInitBaseClass.h) |
- **Queued PRIORITY** (`RESPELL_QUEUE.tsv`, appended to `naming/NAME_FIXES_QUEUE.tsv` as `land0311e`, 9 lines). These binders are invented
  stand-ins that need a re-model, not a rename, and the definition rows are placeholders:
  - 0x00487510: 3 binders and the definition row;
  - 0x00AEAAF0: 7 binders and the definition row;
  - 0x00443F90: 9 binders and the definition row;
  - 0x004AB840: binder 0x0082D2D0 and the definition row;
  - definition rows only: 0x006B32A0, 0x00A14E20, 0x004C5E90, 0x004C6060, 0x004378C0.
- Labels: `ghidra_out/labels_landing_2026-11-03e.tsv` has 37 entries: UpdateFrame, the 20 re-landed rows and the 16 chunk 734 targets.
  The scripts ran in this order: `apply_labels.py` (7 catalog and 9 manifest renames), `sync_catalog_names.py`,
  `sync_manifest_from_catalog.py --labels` (`logs/`).

## Part D: 0x0047A9F0 CHeroLogBook::AddEntry
- Before part A: RELOCATION_MATCH + PASS (`logs/d_pre.log`). After part A: RELOCATION_MATCH + PASS (`logs/rc_a*.log`, `logs/d_post.log`,
  `logs/final_recheck*.log`).
- Chunk 734's DIFFER(1109v1105) is a grading artifact. Its sibling payloads (`sib/b16.json`, `sib/base_ae.json`) leave out the row's
  `CompilerFlags = '/O2 /Oy /W3 /Gr'`. Without `/Gr` the row grades exactly DIFFER(1109v1105) on the final tree (`logs/d_post_without_Gr.log`).
- No header broke the row, so there was nothing to bisect or fix (GOTCHAS).

## Chunk 733 access check (LANDING_CHECK.md): not landed
- **Searched:** `ghidra_out/egor_pdb_names.tsv`, `fablewin_pdb_names.tsv`, `egor_signatures.tsv`, `_fablewin_api.tsv`,
  `fablewin_editor_symbols.tsv`, `struct_layouts_egor.tsv`, `fable_types.h` and the naming-stragglers signatures.
- **Found:** only the accessors `?GetBackBufferWidth@CUIScreenUnitConverter@NUISystem@@SIMXZ` / `?GetBackBufferHeight@...@@SIMXZ`
  (Ego_r 0x0042D62C / 0x0042D64F, FableWin 0x01AAD177 / 0x01AAD1A9).
  - The function name sets hold no data symbols.
  - struct_layouts lists `CUIScreenUnitConverter` as size 1 with 0 members, because static members are not dumped.
  - fable_types.h has no entry.
- **No PDB on disk:** this machine has no Ego_r.pdb or FableWin.pdb (no `debug_build/`, none under the scratch dir) to read the TPI
  `LF_STMEMBER` attributes or the S_PUB32 mangled data names.
- **Hint only:** the static accessors suggest private statics, but that is not PDB evidence. The access of `BackBufferWidth` /
  `BackBufferHeight` (0x013B876C / 0x013B8770) is therefore **unproven**.
- **Decision:** per the check, neither row lands. Both remain staged in `pending/2026-11-03_linux_hand733/`.
- **For the coordinator:** neither row references the statics. Neither source includes `NUISystem_CUIScreenUnitConverter.h`, and no other
  overlay header does. Only the parked 0x0040A7F0 ApplyVideoValues uses it. The access question gates the two rows only if the coordinator
  ties them to that header.
- **Pending the PDB data-symbol dump** (owner decision 2026-11-01, `cvdump -s` of Ego_r.pdb). These landed binders spell the statics `@@2`
  (public) or as invented globals:
  - `?BackBufferWidth@CUIScreenUnitConverter@NUISystem@@2MA` / `?BackBufferHeight@...@@2MA` / `?IsUsingRelativeCoords@...@@2_NA`:
    0x004299A8 (SetIsUsingRelativeCoords). Chunk 733's NAME_FIXES also names 0x009BFB30 CDisplayManager::ResetDevice, but
    `reloc_bindings.tsv` has no binding from it to 0x013B876C / 0x013B8770 (its `BackBufferWidth` is the D3DPRESENT_PARAMETERS field);
  - invented globals: 0x0041CC14 (`g_HeadAngVelMaxXY_Override*`), 0x00543EE0 (`NUISystem::g_OverrideScreen*`), 0x0040F0E0 (`g_dimW/H`),
    0x0052E470 / 0x0052E4D0 (`g_gameSpeedMultiplier`, `g_lengthScale*`, `g_scaleEnabled`).
  - If the dump says private (`@@0`), these become a PRIORITY respell queue. They were not queued now because the access is unproven.

## Procedure
1. D pre-check, then part A: install the overlay, run hdrcheck2, then the includer pp-compare and 119 regrades.
2. Land UpdateFrame: `fixmove.py` and `keypayload.py` (0 texts changed), asm/volatile grep (clean), `vland --land`.
3. Name-fix respells: baseline regrade, edits, regrade, rebind `reloc_bindings.tsv`.
4. Part C: regrade, `repair_remove.py` (20), `vland --land`, `git reset` (no staged changes), file-name restore for 0x009F7890.
5. Labels, queue, `docs/pipeline/INLINE_DEPTH_REVISIT.md` (90 -> 70; 20-row table), GOTCHAS (2 lines), dashboard, final checks.
