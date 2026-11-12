# Chunk 2045 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x00BB8EF0: a render pass whose body sits behind four exit tests (enable, `size() > 0`, a flag, a GetFrame failure) runs its scoped
  helpers at static frequency 0.0625; VC7.1's cost rule (score c1 * (1/f + 6) / 7) then refuses ~CScopedShadersEnable (c1 60, score 188),
  the CRenderStateScopedCaptureBlock ctor (119) and PeekGraphic (110) that retail expands. No if / return spelling raises the frequency
  (each test halves it); the smallest `// Flags: /d2inlT189` + `// Inline-budget:` line reproduces retail exactly. Read the trace first
  (tools/inl.sh): the needed N is the largest refused score + 1.
- 0x00BB8EF0: VC7.1 compiles a non-inline function AFTER the inline functions it calls, even when an inline body is defined below the
  function in the TU (probe e1: still the cost rule); only an inline caller (inline virtual emitted by a vtable) gets c2's budget rule, and
  that refused other sites retail expands -- not a substitute for the flag here.
- 0x00BB8EF0: CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode has c1 693 (six expanded SetRenderState blocks), over the
  450 hard cap: it must be FABLE_FORCEINLINE in every row that expands it; SetupBlendOpState (c1 154) likewise at low frequency.
- 0x00BB8EF0: `CGraphicFrame frame;` declared before the graphic lookup puts the FrameData zero store ahead of the bank read (retail
  0x00BB8F44, into the dead first-parameter home); declared after the lookup it sinks to the GetFrame argument pushes.
- 0x00BB8EF0: the decal TU's uncalled COMDAT run proves helper bodies for free: STLport `distance<list::const_iterator>` 0x00BB9770 is
  list::size() via FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE (MATCH), PeekGraphic's uncalled copy is 0x0041C1B0 (MATCH); the for-loop size()
  default gives the same instructions in this row, so the COMDAT is what decides the spelling.
- 0x00BB8EF0: a retail test of a group flag repeated right before a call with no else call (0x00BB951E `if (group.LandscapeDecal)
  RenderLandscapeDecalGroup(...)`) is a compiled-out else arm (FableWin RenderMeshDecalGroup is a 0x10-byte stub): write only the if.
- Retail has uncalled out-of-line copies of the scoped shader-enable helpers (??0CScopedShadersEnable 0x009D8AD0, ??1 0x009D8B40), so they
  stay plain `inline` in engine/CScopedShadersEnable.h; do not force them for one row.

Review at landing:
- The row needs `// Flags: /d2inlT189` (inline-budget device, owner decision 2026-11-08; evidence in the row's `// Inline-budget:` line).
- hdr/ overlay = chunk 2038's overlay + chunk 2045 additions (hdr/README.md); land both rows on one header set; chunk 2038's 0x00BB8750 was
  regraded under it (RELOCATION_MATCH).
- SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode bodies are row-local (as engine/CEnginePrimitiveRenderUtil.h directs); the parked
  0x00BBE090 has the same bodies -- one text if both land.
- NAME_FIXES.tsv: 8 callee addresses bound with invented class names by landed rows (0x00BCB800, 0x0041B550, 0x00B4D2C0, 0x00B86BB0,
  0x00B870A0, 0x00BE1E70, 0x00B849F0, 0x00B5E700, 0x00BAD2E0).
- vland WARN RELOC_NAME 0x01396DC8 (D3DTextureUVModeTable v label g_textureUVModeToD3D): the open data-symbol review.
- Catalog: 0x00BB8EF0 reservation 2659 v true 1941 (SPLITS.tsv); the bytes after it are the decal TU's COMDATs.
