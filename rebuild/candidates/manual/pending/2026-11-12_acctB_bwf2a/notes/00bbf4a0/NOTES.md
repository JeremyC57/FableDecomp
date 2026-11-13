# 0x00BBF4A0 RenderSubPrimitive -- author notes (bwf2 run)
- grades 1..20: best DIFFER(4159v4169) 23 hunks. Moved it: Apply*Displacement FORCEINLINE, GFFastCos FORCEINLINE with the landed 0x00A14840 text (angle scaled in place: lets GFFastSin share angle*1024), `float angle = sprite.Angle` local, ConstantBlock[count++] references.
- Did not move: additive init/decl placement, group_manager before texture, loop as continue / nested if / do-while / while / iterator, /G7 /G5 flags.
- grades 21..56: RELOCATION_MATCH (fg.py, also with LANDVERIFY_STRICT_HEADERS=1; vt.py PASS; addmatch.py written: payload.json / oracle.tsv, module CEnginePrimitiveManagerParticleSpriteGroup, name RenderSubPrimitive, no flags).
## 0x00BBF4A0 matched -- what made it match
- Structure from OUTLINE + the 2059 ParticleMeshGroup idioms (constant-argument SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode, FORCEINLINE in the row) got 4119v4169 on grade 1.
- Then: Apply*DisplacementVertexShader FORCEINLINE (they were called); GFFastCos FORCEINLINE with the landed 0x00A14840 text + `float angle = sprite.Angle;` (GFFastSin reuses angle*1024); `ConstantBlock[count++]` references; camera constants through named refs c0/c1/c2 = ConstantBlock[0..2] (fixed the ebp/edi swap, the camera read order AND the missing aligned loop head in one edit: record-id / frequency lesson again); GetFloatTLX/TLY two-local text found with fwsize (FableWin 14i/38b) fixed the last `cmp`/`fild` order; the final byte was UpdateGlobalLightingStates called after BOTH light branches (point-light arm jumps to it), found by a masked-byte diff, not by fg.py's hunk list (which shows reloc noise only).
- Headers: hdr/ (README.md lists each opt-in). Flags: none. Globals: g_particleSpriteRenderEnabled_0139b6e0 is DESCRIPTIVE (no name anywhere; twin of 2059's g_particleMeshRenderEnabled_0139b7b0). No landed name proved wrong (dossier's note: file CShaderRenderManager_UpdateAmbient_00989700.cpp holds UpdateCameraPos -- filename only).
## Repair pass (verifier findings, 2026-11-12) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), vt.py PASS
- 1 (body justification): source header now names the copied sibling 0x00BC25F0 (agent2059/src/00bc25f0.cpp), the shared opt-in that
  says the including row defines the bodies, and the retail facts (FableWin-only debug copies; direct calls to 0x00988020; inline reads).
- 2 (FORCEINLINE): source carries an Inline-budget line: plain-inline Setup* grades DIFFER(3225v4169), plain-inline GFFastSin
  DIFFER(4005v4169) (repair/v3.cpp, repair/v2.cpp; ATTEMPTS.log), plus 0x00BC25F0's c2 cost numbers for the same helpers.
- 3 (GFDebugPDynamicCast): local copy removed; the row includes engine/CThing.h for the shared template (still matches).
- 4 (names): g_pDisplacementRenderer_01436e38 is DESCRIPTIVE (no PDB / string / FSE / EgoCore / sibling name; searched ghidra_out
  labels + naming_stragglers signatures + refs). Kept because it is the majority landed spelling (0x00B26360 Initialise stores
  `new CEngineScreenEffectDisplacementRenderer` there, 0x00B24500) and the type is proven. NAME_FIXES.tsv: g_pEngineSettings_01436e38
  (0x00BBBF70, 0x00BE0CD0) and g_pShadowRenderer (0x00B33010) are wrong spellings of 0x01436E38 -> g_pDisplacementRenderer_01436e38.
  This supersedes the earlier line "No landed name proved wrong".
