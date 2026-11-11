# 2026-11-11_acctB_r3004 -- acctB hand-back, near-miss retry chunk 3004 (2 previously parked rows: 1 matched, 1 still parked)

Authored by an Opus 5.5 agent starting from the parked sources of c2024 and c2014, re-verified by the acctB local
coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00B4FC40 CEngineShadowRenderer::SetupShadowFuncRenderStates(long, const CMatrix3x4&, const CColorBufferDepthPackParameters&) | RELOCATION_MATCH 1837v1837, test PASS (g++ and VC7.1) | **supersedes `2026-11-11_acctB_c2024/rows/parked/00b4fc40.cpp`** (was one x87 sum order). Keeps c2024's `// Flags: /d2inlT314` + `// Inline-budget:` line (PostMultiply scores 313). The fix: five accessor results held in named, forwarded locals (`shadow_buffer_texture`, `technique`, `depth_packing_texture`, `world_to_shadow`, `shadow_mode`) instead of calls in place -- no instruction and no frame slot of their own. 4 grades |
| 0x00A4E300 C3DAnimation2::GetDeltaMovement | still parked, DIFFER 1609v1619 | source unchanged from c2014 (1 regrade + 12 free traces; the agent's context went to the first row; passes 2 and 3 not used). Residue: product sums E21..E33 add `((t0+t1)+t2)` in retail, `((t2+t0)+t1)` in ours; one product's operands swapped (0x00A4E766, 0x00A4E7CE); translation term order (0x00A4E90A / 0D); start ToMatrix E11..E13 kept on the x87 stack in retail (0x00A4E567); frame 0x100 v 0x104. The symbol-count lever below does nothing here. Stays in the acctB near-miss queue |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean. I diffed the matched
source against c2024's parked one: the only code changes are the five named locals. Parked row regraded: DIFFER(1609v1619).

Review items for landing:

- **What the match rests on.** The agent measured (free c2 traces, tools/tord.sh) that VC7.1 keys the term order of this
  inlined sum on the outer function's own named-symbol count, period 8: 0 extra names give the parked order, 5-7 give
  retail's, 8 the parked one again; nothing else in the function moves. So the five local names are descriptive and are
  not byte evidence of the original source text -- any 5-7 forwarded names give the same bytes; "name every accessor
  result" is the natural reading chosen. Line 2 of the row says so.
- vland RELOC_NAME: 0x01396DC8 is labelled `g_textureUVModeToD3D` in the repo while the shared CRenderStateManager.h calls it
  `D3DTextureUVModeTable` (one address, one name: pick at landing). The 0x01436E98 / 0x01436E60 / 0x01436E9C spelling question
  from c2024 (`NEngineInternal::GP*` vs `g_p*`) is still open.
- NAME_FIXES.tsv / LABELS.tsv repeat c2024's entries for 0x00B4FC40, 0x00B50510, 0x00B50770, 0x00B50AE0.

To land: `hdr/` is the union of the c2024 and c2014 overlays, files byte-identical to those copies (no file name in
both); 0x00B4FC40 needs the c2024 part (which starts from c2018 / c2008 copies). Rebuild the oracle from Fable.exe (no
retail bytes are in this folder). `LESSONS.md` documents the symbol-count key for GOTCHAS.
