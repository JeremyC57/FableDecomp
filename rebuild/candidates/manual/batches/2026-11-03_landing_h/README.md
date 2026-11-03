# Landing 2026-11-03h: chunk 737 (CPSCSingleSprite / CPSCLight, 4 rows), 5 inline-depth redos, 15 respells

## Result
- **Landed 4 chunk 737 rows** (`logs/land4.log`). On the merged tree, before landing, all four grade the same as on chunk 737's overlay and PASS (`logs/new4_regrade.log`):

  | Address | Name | Grade |
  |---|---|---|
  | 0x00B062A0 | `NParticleEngine::CPSCSingleSprite::InterpolateTo` | RELOCATION_MATCH, grade `retail_asm` |
  | 0x00B068F0 | `NParticleEngine::CPSCSingleSprite::ReadBinary` | RELOCATION_MATCH |
  | 0x00B18C60 | `NParticleEngine::CPSCLight::InterpolateTo` | RELOCATION_MATCH, grade `retail_asm` |
  | 0x00B193D0 | `NParticleEngine::CPSCLight::ReadBinary` | RELOCATION_MATCH |

  - The two `retail_asm` rows have no asm of their own. They call the shared `engine/GFFloatToLongNear.h` helper and each carries a `// Retail-asm:` line.
  - The cited instructions are in the Fable.exe bytes: 0x00B0650E `db 5c 24 14` = `fistp dword ptr [esp+14h]`, and 0x00B18D8E `db 5c 24 18` = `fistp dword ptr [esp+18h]`. Each body has 7 such roundings.
- **Not landed** (left in `pending/2026-11-03_linux_hand737/differ/`): 0x00B087C0 and 0x00B19C60 PreparePrimitives.
  - On the merged tree they grade exactly as parked: DIFFER(2951v2967) and DIFFER(2546v2516) (`logs/parked_merged.log`).
  - 0x00B087C0's own test does not compile under vland (TCC_FAIL). It was only ever graded with q.py.
  - Their labels (slot 13 = PreparePrimitives) are applied.
- **5 inline-depth rows re-landed pragma-free** (sources `../2026-11-03_inline_revisit3/out/`):
  - All 5 were regraded on the current tree first (`logs/b_regrade.log`): RELOCATION_MATCH + PASS.
  - Then `repair_remove.py` (`logs/repair_remove_b.log`) and `vland --land` (`logs/land_redo.log`: WINS 5, every test PASS).
  - The rows are 0x00E504C0, 0x00E6AF20, 0x00EDCB80 and 0x00D99A30 (loop-head macro, line-2 `// Inline-split:` note), and 0x00EF5200 (plain `return;`, marker dropped).
  - The four `#pragma inline_depth` lines each that the tests of 0x00E504C0 and 0x00E6AF20 had copied were dropped.
  - File names are unchanged. None lost its match, so no row kept its old landed form.
  - `// Revisit: inline-depth` 70 -> **65**. That is the line-start marker recounted in `rebuild/src/compiled`. The 65 marked rows are exactly the 65 sources that still hold a `#pragma inline_depth` directive. `docs/pipeline/INLINE_DEPTH_REVISIT.md` is updated (table moved to a new "landing 2026-11-03h" section).
- **Coverage** (`write_decomp_dashboard.py`, `update_readme_progress.py`):
  - 46,554 genuine, 93.927% of the 49,564-row catalog (was 46,552);
  - 54,089 / 57,609 known, 93.890%;
  - 5,729,660 bytes, 56.019% of function bytes (was 5,720,846); 38.562% of `.text`;
  - landed rows 54,098 -> 54,102; `retail_asm` 188 -> 190.

## Part A: header overlay merge (6 changed, 1 new)
- None of the 7 overlay files changed between the overlay's base 4ff4f9bdf and HEAD. Each overlay file holds every line of its repo copy (`diff` removes 0 lines). So copying them in reverts nothing from rounds e/f/g.
- Changed, each behind a retail macro:
  - `CDataInputStream.h` (FABLE_RETAIL_CDATAINPUTSTREAM): ReadFloat / ReadEBOOL inline, ReadColour declared;
  - `CParticleSystem.h` (new FABLE_RETAIL_CPARTICLESYSTEM branch, no landed users);
  - `CMatrix3x4.h` (FABLE_RETAIL_CMATRIX3X4: `__forceinline` TransformPoint);
  - `CIVCountedPointer.h` (FABLE_RETAIL_CIVCOUNTEDPOINTER);
  - `CEnginePrimitiveHandle.h` (FABLE_RETAIL_CENGINEPRIMITIVEHANDLE);
  - `CRGBFloatColour.h` (FABLE_RETAIL_CRGBFLOATCOLOUR).
- New: `CEnginePrimitiveLight.h`.
- **Evidence comments** (added in this landing). The probe output is in `logs/forceinline_probe.txt`. The parked sources were compiled on full mirrors with one helper at a time made plain `inline`, and the call relocations were read:
  - **CMatrix3x4 TransformPoint**:
    - The comment now cites the retail term order: 0x00B1A43A..0x00B1A4C0, `fld E3k ; fmul z ; fld E2k ; fmul y ; faddp ; fld E1k ; fmul x ; faddp ; fadd E4k`, matrix at esp+0x84.
    - Retail expands it in place, and Fable.exe has no out-of-line `?TransformPoint@CMatrix3x4@@QBEXAAVC3DVector@@@Z`. Only the debug PDBs carry one.
    - With plain inline, VC7.1 calls it out of line twice in 0x00B19C60.
    - The comment also records that the parked 0x00B087C0 orders its Z row differently.
  - **CRGBFloatColour Interpolate / GetRGBColourClamped**:
    - Retail does have out-of-line copies (0x00B09B90 / 0x00B09AA0). So per the owner rule of 2026-11-01, `__forceinline` is recorded as the form those rows need.
    - Retail calls neither copy from 0x00B19C60 / 0x00B087C0; the four `fistp dword` roundings start at 0x00B1A4DA.
    - With plain inline, VC7.1 calls Interpolate twice and GetRGBColourClamped once in each row.
- **hdrcheck2:** every file is `default=same` in all three runs:
  - the overlay against the pre-merge tree (`logs/hdrcheck2_overlay_pre.txt`);
  - the overlay against the merged tree (`logs/hdrcheck2_overlay_vs_tree.txt`, every macro `same`);
  - the merged headers against HEAD's `rebuild/include` (`logs/hdrcheck2_merged_vs_HEAD.txt`).

## Siblings (`sibling_grades.tsv`, `logs/final_recheck0*.log`)
- `includers.py` (landing d) finds 228 landed sources and tests that reach a changed header. `ppcmp.py` against HEAD's headers (`logs/ppcheck.txt`): 211 are token-identical and 17 differ.
  - 13 differ by the CDataInputStream additions: two inline members and one declaration.
    - 9 existing rows cannot call them, since they compiled without them.
    - The other 4 are this landing's users: the new 0x00B068F0 / 0x00B193D0 and the re-modelled 0x009945E0 / 0x00B1B1B0.
  - 4 differ through the FABLE_RETAIL_CMATRIX3X4 TransformPoint body: 0x0098ACF0, 0x00989CB0, 0x009DA9F0 and 0x00B26360. They reach it through CEngineLightingManager.h / CShaderRenderManager.h.
- **Final `--recheck-landed` on the final tree** (4 shards): 223 rows, **27 MATCH + 196 RELOCATION_MATCH, all PASS**. The 223 are the union of:
  - chunk 737's 188 SIBLINGS.tsv rows, which it had graded against its pre-round-g overlay; redone here;
  - the 17 pp-differing rows;
  - every 10th of the 200 pp-identical rows (20);
  - the 4 new rows;
  - the 15 respelled rows;
  - the 5 redos.
- **No regressions.** Every row matched before: SIBLINGS.tsv, `logs/nf_base.log`, or HEAD's headers for the 23 rows with no earlier grade (`logs/headbase_noprev.log`). There were 27 MATCH before and 27 after, and no MATCH became RELOCATION_MATCH. No header branch needed a fix.

## Names (`NAME_DECISIONS.tsv`, `BINDINGS.tsv`, `NAME_FIXES_c737.tsv`, `ghidra_out/labels_landing_2026-11-03h.tsv`)
Every decision cites two sources. The first is the FableWin / Ego_r name sets. The second is the retail vtable slot (rtti-vtables.tsv), the call site and `ret N` in chunk 737's retail disassembly, or the Fable.exe bytes. All 21 names of NAME_FIXES + LABELS are in the FableWin name set; 20 are also in Ego_r. The CEnginePrimitiveLight ctor is FableWin only (chunk 737's note said Ego_r).

**Applied and respelled:** 11 of the 12 NAME_FIXES. The 12th, 0x00AEAAA0, is applied as a label only. Every respelled row was graded before (`logs/nf_base.log`, 17 rows) and after (`logs/nf_respell.log`, `logs/nf_respell_00b1af50.log`). All grade the same and PASS, and `objdump -t` shows each definition emitting the PDB symbol.

| Row | Respell |
|---|---|
| 0x00AEA9F0 (definition) | extern "C" `GetGlobal` -> `long __fastcall NParticleEngine::GetTickRate()` (data symbol unchanged) |
| 0x00B1DB00, 0x00B18AF0, 0x00B10220 (+ tests) | three invented getters -> `NParticleEngine::GetTickRate` |
| 0x00B22310 (definition, + test) | `InterpolateTo(const CPSCBase*)` -> virtual `InterpolateTo(const CPSCBase&)` |
| 0x00B1AF50 (+ test stub) | callee respelled the same way; the qualified call passes the reference |
| 0x00B22B40 (definition, + test) | `ReadBinary(CDataInputStream*)` -> virtual reference form. Its nested readers 0x00B22100 / 0x00B21C90 now bind the FableWin reference spellings that their landed definitions already emit (lander-found). |
| 0x00B1B1B0 (definition, re-model) | the local `InputCalls` stand-in -> engine/CDataInputStream.h FABLE_RETAIL_CDATAINPUTSTREAM (`ReadEBOOL` / `ReadFloat`); new standalone test; "vfunc_15" relabelled |
| 0x009945E0 (definition, re-model) | the local `CDataInputStreamReader` -> `CRGBColour CDataInputStream::ReadColour()` on the shared header |
| 0x004595A3 (+ test) | `CRGBColour& ReadColour(CRGBColour&)` -> the hidden-result form. Its local CRGBColour gets a user ctor so the call uses the hidden pointer, as retail does. |
| 0x00642000 (definition) | interpolation set `long` -> `EEngineInterpolationSet`; MATCH kept |
| 0x00B18390 (definition, + test) | pointer, non-virtual -> virtual reference form. The `_base[0xc]` pad became the family's CPSCBase model. |
| 0x00B05B80 (definition) | `long&` -> `unsigned long&` |
| 0x00B05840, 0x00B05980 (definitions) | no namespace, pointer, non-const -> `NParticleEngine`, const, reference. 0x00B05980 is lander-found (FableWin). The 0x00AF3D80 callee now uses the namespace spelling that 0x00B201D0 binds. |

- **`reloc_bindings.tsv`** (`BINDINGS.tsv`): 10 rows retired and 4 added.
  - The landing itself added the two CPSCSplineBase rows from 0x00B062A0 / 0x00B068F0.
  - Re-landing 0x00EF5200 restored its one binding row.
- **Labels only:** 0x00AF8F00 (already right), 0x00B05E10, 0x00B08250, 0x00B219A0, 0x00A013E0, 0x00AEAAA0 and the two parked PreparePrimitives. `apply_labels.py`: 22 labels, 7 catalog and 11 manifest renames.
- **Not applied:** GFInterpolateUpdateParams. The FableWin symbol exists, but the select body has one source only. The rows keep it as a file-scope inline that emits no symbol; a NOTE is queued.
- **Rejected:** none.
- **Queued** (4 lines tagged `land0311h` in `naming/NAME_FIXES_QUEUE.tsv`):
  - **PRIORITY 0x00AEAAA0 GetGraphicBank.** Needs a re-model: the global 0x013D2E08 must be typed as the bank pointer, and it already has two data spellings.
  - **PRIORITY 0x00AEAA80** (lander-found). Probably SetGraphicBank, with binder 0x0042E98F. SetGraphicBank vs SetMeshGraphicBank is not yet proven.
  - **NOTE 0x00AF3D80.** Its bound `GetSystemTransform` name is a stand-in; FableWin's GetSystemTransform is 0x00AF2250.
  - **NOTE GFInterpolateUpdateParams.**
- **Header debt (not a name issue):** 0x00642000 still uses local CEnginePrimitive / CEnginePrimitiveLight models now that engine/CEnginePrimitiveLight.h exists. The verifier warns SHARED_HEADER_AVAILABLE.

## Final checks
- `bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` prints 0.
- Catalog rows 54,102 == `Address = '` entries 54,102.
- `sync_catalog_names.py`: 0 renames. `sync_manifest_from_catalog.py --labels` ran; `--labels --dry-run` reports 0 changes.
- `git diff | grep -c /tmp/claude` is 0, and no new file holds an `$S` path (logs stripped).
- No `__asm`, `_emit`, `naked`, `volatile` or `inline_depth` outside comments in any source or test this landing touched (39 files). The two `retail_asm` rows carry their evidence lines and reach the asm only through the shared helper header.
- GOTCHAS: 2 lines. The first: call relocations, not `objdump -t` COMDATs, prove a `__forceinline` need. The second: body-copy test stubs must follow a pointer-to-reference respell.

## Procedure
1. hdrcheck2 on the overlay against the current tree, `diff` per file (0 removed lines), copy in, add the two evidence comments (after the plain-inline probe), and run hdrcheck2 twice more.
2. Grade the 4 rows (and the 2 parked) on the merged tree. Then `fixmove.py`, `keypayload.py` (0 texts changed), the asm/volatile grep (clean) and `vland --land`.
3. Names: name-set lookups, rtti-vtables slots, call sites and `ret N` in chunk 737's `dis/`, and Fable.exe bytes. Then the baseline regrade, the respells, the regrade, `objdump -t` symbol check, `apply_bindings.py` and the queue.
4. Part B: regrade on the current tree, `repair_remove.py`, `vland --land`, `git reset` (nothing staged). Then the INLINE_DEPTH_REVISIT.md recount.
5. Labels, `apply_labels.py`, `sync_catalog_names.py`, `sync_manifest_from_catalog.py --labels`, the includer pp-compare and the 223-row final recheck (4 shards). Then the dashboard, the final checks and GOTCHAS.
