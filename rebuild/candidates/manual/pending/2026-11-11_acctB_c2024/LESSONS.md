# Chunk 2024 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00B891B0) A CRGBColour member set to a constant colour is `Colour.Initialise(r, g, b, a)` (R, G, B, A byte stores straight into the
  member, `mov al,0FFh` shared); `Colour = CRGBColour(r, g, b, a)` builds a frame temporary and copies a dword.
- (0x00B891B0) A screen-effect constructor's members that retail stores between scratch-allocator calls were assigned there in the
  source: a store cannot cross a call, so `OutlineTexture = 0` sits between the first and second AllocateTexture (0x00B892B6, inside the
  second call's pushes) and the two blur pointers after the third.
- (0x00B891B0) The capture block member's in-place `mov byte [x],0` is FableWin's inline `CRenderStateCaptureBlock() : Open(false)`
  (hdr opt-in FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR); dead parameter homes host later CCharString temporaries
  (the init parameter slot [esp+24h] after the flag test) with no extra device.
- (0x00B4FC40) Unreferenced COMDATs prove helper nesting: a COMDAT with the body and `mov eax,ecx` is the constructor, the same body without
  it is a void member -- CMatrix4x4(const CMatrix3x4&) { Initialise(m); } (0x00B50AE0 / 0x00B50770). Screen a candidate inline body by
  compiling it out of line against its retail COMDAT (free): only one of six PostMultiply spellings matched 0x00B507E0.
- (0x00B4FC40) VC7.1 emits an out-of-line COMDAT for an inline function even where every call site was expanded (plain inline and
  `__forceinline` alike), so a retail COMDAT does not tell which of the two the header used; /d2inlT<N> and FABLE_FORCEINLINE gave identical
  code here. Linker COMDAT de-duplication keeps one copy per name, so a missing accessor COMDAT in a TU's run is no evidence it was unused.
- (0x00B4FC40) The x87 term order of a 4-term sum of products in an expanded matrix loop is a per-column rotation of one of two cyclic
  orders (0231 / 2031 families) and moves with whole control-flow units before the loop (an inline accessor in an `if` condition, early
  returns, setters with internal ifs, a call inside an if), not with dead locals, symbol counts, inline levels in argument positions,
  statement order of the matrix construction, or other functions in the TU. A free fragment lab (k setters + the matrix code) reproduced
  the row's orders at one k but is not faithful to retail-side edits; measure on the row.
- (0x00B4FC40) In this row the CRenderStateManager bookmark test / or operand order matched retail in all 9 setter blocks with the shared
  header's SetRenderState as written (no clamp mix needed).

Review at landing:
- 0x00B4FC40 is parked at 1837v1837 with a single 4-term x87 sum-order residue (pass 3 unused); its differ source needs the overlay opt-ins
  listed in hdr/README.md and `// Flags: /d2inlT314`.
- Respell the landed 0x00B50510 (SetupShadowFuncRenderStates(long, const CColorBufferDepthPackParameters&) with the nested struct and the
  const-reference 3-argument callee) and 0x00B89080 (FinishWithDynamicTextures): NAME_FIXES.tsv.
- 0x01436E98 global spelling (g_pResourceManager here vs NEngineInternal::GPResourceManager in chunk 2018): pick one at landing.
- The landed OutlineGlow rows 0x00BEAAA0 / 0x00BEB4D0 ("CEngineScreenEffectOutlineGlow::~ / ScalarDeletingDestructor" with LandscapeLayerList
  models) carry a propagated wrong class: they store vtables 0x012A8134 / 0x0129B860, not 0x012A3B6C.
