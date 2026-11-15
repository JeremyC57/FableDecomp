# 2026-11-15 landing gb -- Codex pass 22: 76 rows landed (80,158 bytes)

Source: the Codex pass 22 hand-back (`pending/2026-11-14_codex_pass22/return/`, from the owner's package). Its STATUS.tsv held
87 RELOCATION_MATCH, 6 PARKED and 7 EXTENT_WRONG rows. 76 landed here: 75 RELOCATION_MATCH + 1 MATCH, every behaviour test PASS
under the repo harness, `LANDVERIFY_STRICT_HEADERS=1` (`LANDED.tsv`, `logs/vland_land.log`). Then pass 22 was released
(`RESERVED.tsv` -> `RELEASED.tsv`).

## Rows that land at a different address than reserved
Codex proved six reservations start inside a function; the whole function lands at its real start (`LANDED.tsv` `reserved_as`):
0x0074BC20 (reserved 0x0074BC30), 0x00BA1140 (0x00BA115C), 0x00BC3F30 (0x00BC3F3D), 0x00BD71B0 (0x00BD71CC). Four rows had
over-long reservations and land at their true lengths (0x006C9C60 106, 0x007C36A0 606, 0x00C29DC0 8, 0x00C5E0C0 22).
The two compiler EH sections 0x01214FE0 / 0x01215000 land as funclet rows (`funclet_offset` 0, `/EHsc`), the established
`global_Unwind*` shape: 0x01215000 includes its landed parent 0x00C3DB80; 0x01214FE0 carries its parent 0x00C3DA60
CCodaContext::Create2DSource verbatim (that parent is not graded or landed yet).

## Not landed (11 rows, all RELOCATION_MATCH in Codex's grader; filed in `return/matched_test_port/`, back in the parked pool)
- Nine behaviour tests need Codex's per-test `test_include/` directory, which shadows repo headers (`rebuild_abi.h`, `fable_string.h`,
  `engine/CAFile.h`, `engine/CDefPointeeBase.h`, ...) with test doubles. The repo harness has no per-test include path, so under
  vland they fail to compile or link: 0x004A1840, 0x0054EF00, 0x00590235, 0x007C4430, 0x00813D00, 0x0090E5D0, 0x00927F30,
  0x00D29880 (reserved 0x00D2988B), and 0x00B8DCD0 (its test also needs `test_helpers_sol2.inc`, which the package lacks).
  Each needs its test ported to a self-contained form; the matched source is unchanged.
- 0x004A1C41 is a catch block inside 0x004A1840's extent; it lands with that parent.
- 0x00C298D0 (reserved 0x00C29A3C) instantiates the VC7.1 Dinkumware `vector::_Insert_n`; it needs the vendor `<vector>` ahead of
  the repo STL in the include order (SRC_FAIL here).

## Fixes made while landing
- Leaf: every payload entry passes its mangled `// Leaf:` symbol (the two destructors graded the deleting dtor without it).
- Retail-asm evidence lines (comments only): the purity gate needs a `0x` address and an instruction of the helper. Six rows had the
  address without `0x` or no line at all; each line now cites the retail site, checked by disassembly: 0x00750E77 / 0x00779F2C
  (GFFloatToLongCeil), 0x00B48841 / 0x00B48895 (GFFloatToLongFloor), 0x00BA160A (GFFloatToLongNear) and 0x00BA1709 / 0x00BD76A6
  (GFFastFloatToLong), 0x00BF7BE6 (GF_SSE_BOX_DISTANCE_SQUARED), 0x00C6B08E.. (FtoL), 0x00B98E5B (the new SIMD kernel header).
- `tools/decomp_pipeline/crawl/purity.py`: `_emit` is now matched as a token; 0x007C4430's local `ptc_emitter` read as an emit bake.
  No landed row changes grade.
- Inline-budget lines added under the Flags line where Codex gave the evidence only in its JSON: 0x0054AC90 (T256), 0x00B4BCC0 /
  0x00B4C550 (T81), 0x00BC3F30 (T512), 0x00BD71B0 (T256).
- 0x00B98E10 (retail_asm, the original hand-SSE clip kernel as a shared macro with a FABLE_PORTABLE twin): its test takes the
  trace file name as an optional argument (default `00b98e10_trace.txt`) and the fixture header is inlined. Dual-path driver
  `rebuild/tests/portable/rows/00b98e10_ClipSubSectionsSIMD_driver.cpp`: VC7.1 asm path, VC7.1 /DFABLE_PORTABLE and g++ -std=c++03
  portable all print the identical 7,680-case trace.

## Headers (`merge_tools/`)
Codex changed 170 headers against f7760866b and added 105. A 3-way merge onto the current tree gave 148 clean, 22 with conflicts
(30 hunks), 1 new-file clash. `resolve.py` resolves each hunk by rule: assert-pin-only hunks take ours (Codex pinned the same
lines); distinct opt-in blocks are concatenated; two opt-ins declaring the same inline body become `#if defined(A) || defined(B)`
(C4DVector, CEnginePrimitive2DViewportManager, CGraphicInfo); differing bodies become `#ifdef A .. #elif defined(B)`
(CMatrix3x4::Set, CRGBFloatColour ctors, C4DVector colour ctor, CEnginePrimitiveRenderer members); whole-class wraps nest
(CEnginePrimitiveManagerParticleLineGroup); CDateAndTime keeps the landed class line for the default path.
CEngineInternalPrimitiveMorphedStaticMesh.h (created by both landings) is `#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER` Codex's
version `#else` the landed one. Nine new bare `__forceinline` in CRenderManagerCore.h / CShaderRenderManager.h are spelled
FABLE_FORCEINLINE.

Two line-number effects found by the equivalence check and fixed:
- CTCInGameMenu.h: Codex pinned the first of two identical asserts as `AT(192)`; it is `AT(127)` (`pincheck.py` now checks every
  pin against the tree).
- fable_string.h: Codex's three new lines above line 431 moved the `__LINE__` in RunCutsceneMacro's StartsWith dead arm; two comment
  pairs are folded so it stays on line 431.

Gates: every header balances; `hdrcheck` flags 7 headers, each either a whitespace change or a sibling header read from the overlay
(with the full tree they preprocess the same); `pincheck.py` 0 mismatches; whole-tree `equiv_check.py` (exact): 56,291 landed rows,
all identical input except 0x00CBFB7D, whose only difference is the scratch tree's path inside `__FILE__`
(`logs/equivalence_summary.txt`).

## Names
`ghidra_out/labels_landing_2026-11-15gb.tsv` (90 labels at the real starts; EXTENT_WRONG rows and funclet labels left out).
217 NAME_FIXES rows queued as `land1115gb`; 121 were already queued with the same spelling. 22 give a different symbol than one
already queued for that address and are held for review in `return/NAME_FIXES_CONFLICTS_WITH_QUEUE.tsv`.

## Lander warnings (none blocking)
RELOC_CONFLICT `CQuestManager::RemoveAcceptedBoastsForScript` 0x004B0030 here v 0x004AFFA0 in landed 0x005E4740 (one binding is
wrong; audit). 71 RELOC_NAME, mostly catalog labels the queued fixes correct.
