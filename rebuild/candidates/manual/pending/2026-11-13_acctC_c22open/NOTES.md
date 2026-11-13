# agentC22 NOTES

## 0x00B89C30 CEngineScreenEffectOutlineGlow::ApplyOutline -- DIFFER(6431v6431), 1 hunk; parked (moved on per owner direction)
- Port: acctB draft graded unchanged once FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET was re-applied to a copy of today's
  CDisplayManager.h (hdr/; size assert pinned at 166). differ/00b89c30.cpp = the port.
- Residue (unchanged from acctB session 6): in the final-section SetRenderState run the bookmark-test load order (S = state.BookmarkMask
  first, M = CurrentBookmarkMask first; tool pat.py) is retail `??M*22 SSSSS M*11`, ours `??M*22 SSSSSS M*10`: the S-run ends one block
  late, at SetAlphaRef(1) (0x00B8AF59).
- New levels tried this session (all leave the pattern byte-identical):
  * FableWin size: with FW_EXTRA=/EHsc every helper is SAME (GetRenderTarget 20i, CRenderTarget ctor/dtor/operator=, setters,
    PrepareForRender); the row is 523i v FableWin 619i (+96i, about one ASSERT). Dead FABLE_RETAIL_ASSERT at 6 sites (A: before the
    capture block, B: before the blur loop, C: before SetRenderTargetAsBackScreen, D: before the last PrepareForRender, E: at the top,
    F: right before SetAlphaRef(1)) and 2/4/8 asserts at A: no change at all (dead asserts add no ids that matter in this row).
  * PDB global spelling NEngineInternal::GPCamera (0x01436EA0; g_pEngineCamera is made-up): no change -> differ uses GPCamera (code-identical).
    only because it is the draft; GPCamera is the better name (NAME_FIXES).
- acctB already tried ~900 grades: SetRenderState DESIRED/BOOKMARK/DIRTY/EARLY_RETURN/LOCAL_MASK opt-ins, alias locals, tex/array forms,
  loop/block shapes, statics placement, /Op, the c2 commutative-key analysis (ids wrap at 1024 x 64). No measured lead remains.
- Volatile: `static volatile float` blur offsets kept per the acctB owner ruling (retail x87 copies 0x00B8A913 / 0x00B8A927).

## 0x00B8B780 CEngineBillboardGenerator::RenderBillboard -- DIFFER(3921v3921), 3 operand-order sites; parked
- Port: 11 stale opt-ins re-applied to today's headers in hdr/ (reap2.py / fixelse.py; README lines); CRGBCOLOUR_CTOR_INITIALISE ->
  existing CTOR4_INITIALISE, RENDERUTIL_MIPMAPSIZEMULTIPLIER -> existing _MESHPRIMITIVE (same decl), +C3DPRIMITIVE2_INDEXBUFFER/CINDEXBUFFER
  (VERTEXBUFFER is now nested in INDEXBUFFER). Port = acctB state: 5 test/or sites (0x00B8BC75, BCE2, BD49, C325, C393), nd=15 (nd.py).
- FableWin-backed texts (keep): GETTRANSLATION_NAMED (existing opt-in; FW 22i, named 23i, plain 19i) closes C325; new nested
  FABLE_RETAIL_CMATRIX3X4_SETTRANSLATE_IDENTITY (FW SetTranslate(C3DVector&) 21i/56b = SetIdentity() call + 3 stores; now SAME) with the
  row's separate world.SetIdentity() dropped: code-identical, no hunk change.
- Count effect: dead FABLE_RETAIL_ASSERTs anywhere (position-independent, 1 = no change) close BD49 at 2, BC75/BCE2 at 4-5; C393 never.
  differ/ has 2 (FableWin +161i ~ 2 asserts). 5 asserts = nd 3 (only C393) but that is beyond the FableWin evidence (fitted).
- Measured not-text: FW SetIdentity 43i/106b and SetMultiplyNoTranslation 160i/444b are the later compiler's fldz/fld1 stores (12-store
  probe 31i/130b; 9-sum + translation clear = 157i/450b -> 160/444 with fldz). So FW's SetMultiplyNoTranslation is 9-sum + clear, but the
  row's retail bytes need the translation products: a 9-sum + clear + separate translation transform (w/rc1.cpp) gives new x87 order hunks at
  0x00B8B9CB. operator*(float) FW 31i/82b: no text found (plain 25i, named 29i but SCALE_NAMED_RESULT breaks the row: 3937 bytes).
- Next: find the record source behind C393 (loop 2, the second setter block) and the missing ~3 asserts' worth of records.

## 0x00BCC800 CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail -- DIFFER(3207v3205), unchanged from acctB; parked
- Port: 6 stale opt-ins re-applied (CVertexBufferBase ISBUSY_SIZEOF, CTVertexColTex1 SETTERS, RenderUtil DYNAMICVB, ShaderRenderManager
  DRAWNONINDEXED_TEXTURES, TreeFiler INLINES, VertexBufferPool INLINES). The repo's CEnginePrimitiveRenderer no longer models
  DynamicVB*: new nested opt-in FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_DYNAMICVB (Ego_r +0xC4/+0xC8/+0xCC) and the global respelled
  NEngineInternal::GPPrimitiveRendererSharedData (0x01436E78; was the made-up g_pPrimitiveRenderer typed CEnginePrimitiveRenderer).
- Residue (frame-masked, 39 hunks; nd.py 131): frame slot permutation + `lea edx` v `lea ecx` 0x00BCCDA9 + ReturnResource/FindNode tail
  0x00BCD32D..0x00BCD3DF (retail keeps the FindID node in the dead arg word [ebp+7Ch], so its leaf path stores it: `je; mov [ebp+7Ch],eax;
  jmp`; ours keeps it in a register and spills tree/key instead).
- Tried this session (no change at all): 1-3 dead asserts at the top; dead asserts inside FindID / ReturnResource (probe header); AddKey
  early-return form (FableWin 53b). Allocation residues do not move with record counts here.
- FableWin deltas for later: FindID +62i, GetResource +55i, AddNewResource +65i, GetBuffer +66i (a ~60-instruction assert form?),
  DrawBuffersNonIndexed(textures) +3i, TouchFrame +5i, CIVCountedPointer::Reset +6i, IsNull +12i.

## 0x00BAA870 CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList -- not graded (port incomplete; context budget)
- 10 stale opt-ins are missing from today's headers. Re-applied automatically (reap2.py / fixelse.py) they compile-fail: ZSPRITE in
  SharedData had to be merged with MIPMAPSIZEMULTIPLIER (#ifdef/#elif); CSCOPEDSHADERSENABLE_FORCEINLINE_DTOR -> existing
  FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE; still failing: CEngineInternalPrimitiveRepeatedZSprite.h (the TYPED branch vs pinned default
  asserts, offsetof __vftable), CEngineLocalDetailGenerator.h (_pad_0xc redefinition: ZSPRITE block not wrapped with the default),
  CEngineLightingManager FADEDSHADOW_ENABLE_FIRST and one C3DMeshInfo LODS block (no unique anchor), CPlane / CMeshDataBankEntry asserts.
  All row-4 header edits were reverted from hdr/ (copies in probe/row4_port_hdr/). Next agent: hand-merge those blocks, then grade.
