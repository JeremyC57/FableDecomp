# agentC17 NOTES (w22 acctC lane; continues agentC16 / C13 / C8)
Tools: tools/ (C16's, repointed) + tools/gl.sh <addr> <key|leaf> <abs src> (grade any function of a TU at its own address;
keys sf ib dt bt sb rm sr), tools/mkt.py + tools/vb.sh (build the row-1 TU from w/00ba5260.cpp + w/inc/a.inc (SetupFirstTextures,
IndexBuffer, DegenerateTriangles before SetupRenderModeShadersAndConstants) + w/inc/bt*.inc + a SetupStaticBlocks variant (before
RenderMeshPrimitive)), tools/dd.py <addr> (relocation-insensitive diff of the last grade).
Oracle: rows.tsv + agentC17_oracle.tsv now also hold 0x00BA50A0 SetupStaticBlocks 441 (ret 1Ch at 0x00BA5256) and 0x00B9E5B0
RenderMeshPrimitiveBlocksIndexBuffer 402 (ret at 0x00B9E741); both extents verified.

## TU findings (2026-11-13)
- Real bodies on SHARED headers in the row-1 TU (w/inc/a.inc, bt.inc): SetupFirstTextures 0x00B9D670, DegenerateTriangles 0x00B9E750,
  SetupStaticBlocksBlankTextures 0x00BA4A40 (landed texts; landed globals/helpers respelled to the PDB/shared names:
  g_pRenderManagerCore -> NEngineInternal::GPRenderManager, CRenderManagerCore::SetTexture -> AttachTextureToStage, CBaseBlock fields ->
  FableWin accessors under new opt-in FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS_BLOCKDRAW) each grade RELOCATION_MATCH at their own
  address inside the TU.
- With those bodies + any non-capturing SetupStaticBlocks body, RESIDUE A (0x00BA558B) IS GONE in row 1 (real bodies, not stand-ins).
  Without a SetupStaticBlocks body it comes back. Row 2 (0x00BA2350) body is unchanged in the TU (C16's differ text); residue A did not
  need any change to it.
- SetupStaticBlocks 0x00BA50A0: RELOCATION_MATCH in the TU with text w/inc/c3.inc (`bool failed=false;` right after the blank-textures
  early return, before the bank load) ONLY when the bodies of CEnginePrimitiveManagerMeshBase::GetDiffuseTexture 0x00BA9360 /
  GetBumpTexture 0x00BA95D0 are visible in the TU (texts from the landed NoMorph 0x00BA48F0 file, w/inc/gt.inc): `failed`'s address
  then does not escape and its store is scheduled after the loads as in retail. Without them the store goes to the block top
  (hand762's residue). Retail places GetDiffuseTexture / GetBumpTexture AFTER this TU's functions (0x00BA9360 > RenderMeshPrimitive end):
  consistent with inline header functions emitted out of line at the TU end. BUT those NoMorph texts grade DIFFER(382v613) /
  DIFFER(373v597) at their own addresses -> they must be ported to real (matching) texts before SetupStaticBlocks is landable.
- RenderMeshPrimitiveBlocksIndexBuffer 0x00B9E5B0: RELOCATION_MATCH in the TU (w/ib/g1 text, in w/inc/a_cur.inc): no named locals for
  the draw arguments (FableWin accessor calls straight into DrawBuffersIndexed) and the vertex count through the FableWin inline
  C3DPrimitive2::GetVertexCount() (existing opt-in FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS; the plain field read gives DIFFER(402v402),
  the draft's named locals 403v402). Combined TU w/full1.cpp: sf/ib/dt/bt/sb all RELOCATION_MATCH, row 1 only residue B (6 lines).
- Residue B measured (c2prio, full2 = Initialise operator- v full2u = unnamed): the cull SetPolyCullMode expansion's two bookmark
  temps (CurrentBookmarkMask load / state BookmarkMask load) get candidate ids 259 / 303 in one order with Initialise and swapped
  with the unnamed form; `or` puts the higher id first. So it is the CREATION ORDER of two CSE temps (hash/record dependent), not
  priorities. Rulers (dead named locals after EndOcclusionTest) on the unnamed form: k 0..8 match, 10 breaks; on the Initialise form
  k 0..96 never match. => with Initialise, row 1 needs FEWER records/IL somewhere (or a different TEXT that moves those temps).
- New opt-in FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB (hdr/engine/C3DVector.h, FW 34i): cloth rows 0x00BB30A0 / 0x00BB3E80 stay
  RELOCATION_MATCH with it (sources sib2/src = c8a sources with the define swapped from NAMED_RESULT_SUB).
- Residue B form detector tools/cullform.py (A = CurrentBookmarkMask register is the `or` destination, retail's form): retail row 1 =
  BB then 16x A (cull 0x2890 = A); ours (Initialise) = BB, 15x A, cull B. Unnamed operator- gives retail's sequence.
  Levers tried on the Initialise TU (w/full2.cpp), all leave cull B: named-symbol levers (repo opt-ins SETRENDERSTATE_DESIRED /
  _BOOKMARK / ADDSTATE_DIRTY / EARLY_RETURN, every combination), declaration orders / const / PeekParent / occlusion constant
  spellings / capture-block position (w/v5.py), GetMagnitude texts (NAMED_SQUARED, new probe NAMED_XYZ, XY_THEN_Z, SQRTF, default),
  GAM texts (split distance, named radius, GetCenter (FW 12i accessor; breaks cloth), GFLimit<long,long,long>/<long,int,long>
  (FW has JJJ / JHJ, NOT our JHH: use 1L/255L; code-neutral)), dead-local rulers 0..96 at four positions (never back to A).
  Deletion calibration (hand892 lesson: the bookmark test/or key is an IL TUPLE count, periodic): removing one SetRenderState
  expansion before GAM flips cull to A but site 3 to B; removing one fail path (Restore + FlushRender) flips cull to A and sites
  3-4 to B. => with Initialise, row 1 has too many IL tuples between site 3 and the cull site (or the key is counted from the end).
- Same-TU evidence: the LANDED 0x00BA1B00 DisplayMirroredTextures (same retail TU) writes FlushRender with the inline
  CRenderManagerCore::SelectVertexBuffer(0, 0); row 1 uses the cloth rows' folded SetStreamSource(0,0,0,0). One TU, one text:
  the SelectVertexBuffer form is the evidenced one, but it adds IL: with it row 1 = DIFFER(4093v4056) (Initialise) /
  cull B (unnamed) -- i.e. it pushes the same key further. Both evidenced texts (Initialise operator-, SVB FlushRender) make row 1
  WORSE, so retail's row 1 must have markedly less IL somewhere before the late bookmark sites (not found yet).
- GetMagnitude: with the UNNAMED operator- row 1 matches with the plain default GetMagnitude (sqrt(GetSquaredMagnitude())) too: the
  X,Z,Y NAMED_SQUARED text is not needed by row 1.
- KEY MEASUREMENT (residue B): deleting IL AFTER the cull site flips it to retail's form: removing the shader-manager
  SelectVertexBuffer line (a_svb) or the final RestoreDefaultPrimitiveParameters expansion (x_rest) gives exactly retail's form
  sequence (BB + 16 A). Hand-expanding the final Restore and dropping one setter: dropping SetCurrentEnvironmentThemeChannel (44i)
  flips it, dropping SetEnableFadedShadowFactor (24i) or SetSelfIlluminationAmbient (19i) does not; with x_rest even +20 dead
  rulers stay A. Deleting plain CALLS after the site (PrepareOcclusionTest, EndOcclusionTest, RenderMeshPrimitiveBlocks) does not.
  => Initialise operator- leaves row 1 roughly 20..44 /Od-instruction units of INLINE IL too long, counted after the cull site.
- fwsize found two FableWin-DIFF helpers inside that final Restore: SetEnableFadedShadowFactor (ours 25i/81b, FW 24i/80b) and the
  CEnginePrimitiveThemeChannelDesc() ctor (ours 14i/34b, FW 13i/32b). New opt-in FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_
  SHADOWGUARD_DIRECT (guard reads ShadowMode directly, no PeekGlobalShadowMode call) is FW-SAME 24i/80b; row 1 still cull B with it
  (Initialise), the SVB-FlushRender variant improves from ...ABBB to ...AABB. ThemeChannelDesc ctor: no 13i text found (member
  inits, body assignments, chained assignment all 14i; +Pad 16i).

## 0x00BA2350 SetupRenderModeShadersAndConstants (agentC17)
- The 116 missing bytes located (tools/blk.py + pattern counts): retail has THREE copies of the outline tail
  `ApplyVertexShader(IsDP3Bump() ? VertexShaderDP3BumpOutline (+0x2D8) : VertexShaderOutline (+0x2D0))` (0x00BA42DE, 0x00BA44C1,
  0x00BA465C) and five inline AttachTextureToStage(0, 0) (SetupBlankTextures) / four `+0xDC` DiffuseOnly PS selects; ours has ONE
  VS select (0x2D0/0x2D8 once), four Attach(0,0), three 0xDC: c2 cross-jumped the identical outline tails. In retail the copies
  differ in where `this` lives (copy 1 and 3: [esp+14h], copy 2: ebp) -- a register-allocation consequence, so the missing bytes
  follow from the colouring residue (C16: mesh_primitive must be coloured before sub_primitive), not from absent source.
- 0x00B9E5B0 MATCHED (bonus support row): src/00b9e5b0.cpp (standalone TU, shared headers), test/00b9e5b0.cpp (host model; a mutant
  with the change-flag-1 test removed fails), addmatch + read-only vland RELOCATION_MATCH / PASS. Text: Lionhead-style locals
  (no_blocks / c_block / block_info as the landed SetupStaticBlocksBlankTextures), FableWin CBaseBlock accessors, GetVertexCount().

## Residue B resolved differently (late, agentC17) -- READ THIS FIRST for row 1
- Global names: the row TU used made-up names for 0x01436EA0 / 0x01436E9C / 0x01436E5C / 0x01436E98 (g_pEngineCamera, g_pEngineLightingManager,
  g_pOcclusionManager, g_pResourceManager); reloc_bindings has the PDB names NEngineInternal::GPCamera / GPLightingManager / GPOcclusionManager /
  GPResourceManager (tools/respell.py). THE NAMES CHANGE c2 RECORD ORDER: after respelling, the Initialise TU still has cull B, but the
  UNNAMED (repo default) operator- TU gets cull A and instead a TransformPoint x87 product order residue (0x00BA5434/0x00BA543D swapped).
- With the unnamed operator- + PDB names + SHADOWGUARD_DIRECT, dead rulers `const ERenderPassType r = pass;` (k = 3..12 at the function
  start, or 3+ before the GAM call or before SetupFirstTextures) give RELOCATION_MATCH (w/full2u.cpp + tools/rul.py). Unused locals
  (no initialiser) and single-use named int locals do NOT count. So row 1 needs ~3 'ruler units' more IL, anywhere before the cull site.
- Natural units found so far: GetCenter() in GetAppropriateMipmap (`(camera_position - sphere.GetCenter())`, FableWin
  ?GetCenter@C3DBoundingSphere@@QBE?BVC3DVector@@XZ 12i, existing repo opt-in FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE; GAM /Od 75i ->
  78i toward FableWin's 81i) = 2 units; cloth 0x00BB3E80 tolerates it (+0..2 rulers). PeekMesh_RENDERINFO and GetCurrentCamera() (cloth
  rows' spelling) are code-neutral and FW-backed but count 0. Remaining: ONE unit (differ/00ba5260.cpp = this configuration, 2 swapped
  x87 instructions). Next: the other 3 /Od instructions FableWin's GetAppropriateMipmap has (81i v 78i) -- find that text (it is shared
  with 0x00BB3E80, which tolerates +2 units), or one more FW-backed inline layer in row 1 before the cull site.
- Cloth rows: RELOCATION_MATCH with the repo-default operator- once SHADOWGUARD_DIRECT is on (sib3/src = proposed replacement texts:
  INITIALISE/NAMED_RESULT_SUB dropped, DIRECT added, PDB global names). So NO operator- opt-in is needed for any of the three rows:
  the overlay-only NAMED_RESULT_SUB (fitted) and my INITIALISE_SUB can both go.
- Grades (agentC17): ~330 full grades in ATTEMPTS.log across rows 1 / support rows / cloth checks, plus 2 c2prio runs and ~40 fwsize runs.
