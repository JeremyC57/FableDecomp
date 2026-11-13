# agentC13 NOTES (w22 acctC lane; continues agentC8 / C6 / C3 / C2). Tools: tools/ (C8's, repointed) + tools/v.sh (grade + L/O
# position diff vs retail), tools/prb.py (probe inserter), tools/rul.py, tools/trw.sh + tools/ids.py + tools/rawcmp.py (c2 tuple dump
# of the bookmark-test regions: temp ids), tools/sch.sh (scheduler DAG dump). Work sources in w/, probes in w/pr/, probe header overlay phdr/.

## 0x00B783F0 CEngineWaterRenderer::Render -- IN PROGRESS, DIFFER(9343v9343) (base = prev_C8/differ/00b783f0.cpp)
Residue (v.sh positions): L20..L23 quartet (exp2 enabled SBEM(1), retail C), L61/L62 exp3 Dst/AB (retail M), O31 0x00B79938.
Measurements (agentC13):
- Quartet + exp3 pair BOTH fixed (only O31 left, ndiff 108) by DEAD inline SetRenderState expansions placed ANYWHERE before exp2 (even
  right after block.Capture(), before exp1): 2x `if (0) GPRenderManager->SetFogEnable(false);` or 3x `if (0) rm->SetRenderState(0, rm->
  DeviceState.FogEnable);` (w/ps3.cpp). 1 dead fog = block 4 + L62 only; 3 dead fog = L60 breaks. Placed AFTER the quartet: no effect.
- NO effect on the quartet: dead FABLE_RETAIL_ASSERT arms in Render (49 positions), dead pure expressions (`if (0) x = time + time;` x1..32:
  exp3 only), dead ProbeW (store CBM through an inline) and dead AddStateToUpdateList (break stage-2 blocks instead), empty inline
  setters (SetStippleEnable etc., FW 13b/7i PC stubs) anywhere in SCRS, asserts in AttachD3DTextureToStage / SBEM, standalone
  CRenderStateScopedCaptureBlock model, Capture Open-first, early-test spellings (water/sea named bools change code).
- c2 tuple dump (trw.sh): the quartet's two loads' IL order follows the destination TEMP ids (ps3 shifts them +0x1b); not a single
  threshold on Cd (depends on a second counter: field/symbol records), so the lever must add SRS-like inline expansions.
- O31 (0x00B79938, exp2 disabled arm stage 2): register swap (retail CBM->esi, mask->edx; ours edx/esi), test/or text identical.
  Unmoved by every probe above, tail dead code (3+ dead SRS at the tail change code to 8947), dropping `ice` / component_params.
- 2-D probe grid (wk/grid1.txt; dead pure expressions `if (0) Settings.BumpMatFactor = time + time;` = A, dead inline stores
  `if (0) ProbeW(rm)` = B, both after CreateDynamicTextures): the quartet is moved ONLY by B (j=3: blocks 1-3 still M, 4: 1-2, 5: 1,
  6: none, 7: L60 breaks), and only when A >= 16 (with A < 8, B = 3..6 breaks the enabled stage-2 blocks L24..L30 instead); exp3 pair
  fixed for j >= 3 at A >= 16. Good region = A in [16, 28], B = 6 (probe/00b783f0_g16_6_PROBE.cpp, ndiff 108, O31 only).
  B counts only stores through an INLINE pointer formal: direct dead stores (`if (0) rm->CurrentBookmarkMask = 0;`, member bools),
  empty inline calls, read-only inline calls and the standalone scoped-capture model weigh 0. Three dead SetRenderState expansions
  supply A and B together (probe/00b783f0_ps3_PROBE.cpp). No natural source with that content found: FableWin's Render +83i is one
  assert (dead assert arms weigh 0 here), SCRS +1 call (13 b imm8 setter, later revision or an empty PC stub: weighs 0).
- O31 is the same IL operand-order phenomenon at another block (retail generated M first -> edx, C second -> esi, then scheduled C
  first); no probe in the grid, at the tail or inside SCRS (all x3 -> 8947-byte regime) moved it.
- Not tried: c2 hook at the operand canonicalisation (the brief discourages new hook tooling); the remaining natural candidates are
  helper texts with extra inline layers that store through a pointer formal (each such instance = 1 B unit) in the pre-exp2 code.

## 0x00BA5260 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive -- DIFFER(4060v4056), BLOCKED (callee body), ported
- Ported acctB's draft onto today's headers: differ/00ba5260.cpp (defines: _MESHRENDER -> _MESHPRIMITIVE, _SHAREDDATA_MIPMAP -> the
  overlay's _SHAREDDATA_CLOTH (MipmapSizeMultiplier / BlockOnLoad already modelled there), + _C3DPRIMITIVE2_INDEXBUFFER/_VERTEXBUFFER/
  _CINDEXBUFFER + new _C3DPRIMITIVE2_GETVERTEXBUFFER, _CENGINESUBPRIMITIVEMESHBASE_ACCESSORS). Headers: NEW engine/CEngineSubPrimitiveStaticMesh.h
  (derives from the repo's CEngineSubPrimitiveMeshBase in CEngineSubPrimitiveRepeatedStaticMesh.h; friend CEnginePrimitiveManagerStaticMeshesBase
  for the protected MeshPrimitive: FableWin has no accessor for it); stale opt-ins re-applied additively (tools/reapply.py) to C3DTextureStages.h
  (_CTOR, now inside the class), CEngineInternalPrimitiveStaticMesh.h (_PEEKMESH), CEnginePrimitiveManagerOcclusionBase.h (_PREPAREOCCLUSIONTEST),
  CEnginePrimitiveManagerStaticMeshesBase.h (_RENDERMESHPRIMITIVE), CEngineOcclusionManager.h (_OCCLUSIONTEST), C3DPrimitive2.h (_GETVERTEXBUFFER).
- Grade standalone DIFFER(4060v4056) (= acctB). With a non-capturing PROBE stand-in body of 0x00BA2350 in the TU (probe/00ba5260_standin_PROBE.cpp,
  never landable): DIFFER(4056v4056), 14 real instructions (tools/rh.py): the C3DTextureStages ctor store `mov byte [ebp-38h],0` scheduled
  at 0x00BA558B in retail (ours 0x00BA5579) with the [ebp-1Ch]/[ebp-20h] store pair swapped, and the cull SetRenderState bookmark loads
  0x00BA5EB3/0x00BA5EB9 (edx/esi). The real callee draft (acctB parked 0x00BA2350, 466 lines) does not compile against today's overlay (>100
  errors: its opt-in names differ from the cloth rows' overlay) -- porting it is the next step (it is also an acctC-reserved OPEN row).

## 0x00BD3070 CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive -- UNTOUCHED (length verified only)
- TRUE LENGTH = 11288 bytes (the extent scan is right; rows.tsv / oracle unchanged): code 0x00BD3070..0x00BD5C3C (`ret 8` at 0x00BD5C3A),
  3-byte `lea ecx,[ecx]` alignment pad 0x00BD5C3D, then the switch's inline jump table 0x00BD5C40..0x00BD5C87 (18 dword entries, entries
  4, 13 and 14 are 0 -- the same shape as sibling 0x00BA2350's 18-entry table with entry 14 = 0), then int3 padding 0x00BD5C88..0x00BD5C8F and
  the next function at 0x00BD5C90 (`push ebp; lea ebp,[esp-74h]`). The "nonsense last instruction" is table data disassembled.
  acctB's draft already compiled to 11288 bytes (its table included).
- Not ported: the draft needs 58 stale overlay headers (2 new: CEngineInternalPrimitiveMorphInfo.h, CEngineSubPrimitiveAnimatedMesh.h).

## Row 0x00B783F0, later finding (agentC13)
- Scheduler data (tools/sch16.sh + tools/s16.py, pre-RA DAG pass): the quartet blocks have the SAME pre-RA IL order in ours and retail
  (M-load first: M takes edx, CBM takes esi in both builds). The final order is decided in the post-RA scheduling pass, where the two
  loads carry node flags 8 / 4 (prio bonus 1<<15 for flag 4, sim.py): retail gives the CBM load the bonus there, ours does not. O31 is
  the other kind: there ours generated CBM first (edx) where retail generated M first. Next step for whoever continues: dump the
  post-RA regions with the load classification (needs the operand displacement field: the traw2 'kind' marker only exists pre-RA) and
  find what sets node flag 4 (c2.dll node +0x3c) -- then look for the source fact behind it.
