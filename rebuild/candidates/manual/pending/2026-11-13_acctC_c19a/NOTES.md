# agentC19 NOTES (w22 acctC lane; continues agentC17 -> see prev_C17/NOTES.md)
Tools: tools/ = C17's repointed; tools/g1.sh <srcs> (row-1 grade + diff count), tools/gam.py <base> <variants> (GAM body variants: /Od size + row-1
grade), tools/gamsib.py (same into cloth 0x00BB3E80), tools/respell2.sh (made-up globals -> PDB names), tools/sib.sh (SIBD=dir) cloth regrade.

## 0x00BA5260 RenderMeshPrimitive
- Global respelling (Anniversary PDB S_GDATA32 + the NEngineInternal::GP* siblings in reloc_bindings): 0x01436E78 g_pPrimitiveRenderer ->
  NEngineInternal::GPPrimitiveRenderer (Anniversary type CEnginePrimitiveRenderer* = the row's type), 0x01436E60 -> NEngineInternal::GPShadowRenderer,
  0x01436E84 -> NEngineInternal::GPPrimitiveTypeInfo, 0x0143B990 g_highestSetBitTable -> GTopBitInByteTable (Anniversary unsigned char[256];
  FableWin GFGetTopBitInByte). Code-neutral for row 1 (residue unchanged) and the cloth rows (RELOCATION_MATCH).
- GetAppropriateMipmap text (fwsize, FableWin 81i/269b): (float)graphic->GetWidth() (no (long) cast: +1i/+7b at /Od, /O2-neutral),
  GFLimit(size, 1L, 255L) (FableWin has GFLimit<long,long,long>, not <long,int,int>), and a named radius + distance in two steps
  (`float radius = sphere.GetRadius() * scale; float distance = (camera_position - sphere.GetCenter()).GetMagnitude();
  distance = (distance - radius) * mesh_primitive.GetAverageTextureStretch();`) -> /Od 81i/265b (FableWin 81i/269b; was 75i) and
  row 1 RELOCATION_MATCH (in the TU w/r2.cpp). Cloth 0x00BB3E80 with the same GAM text: RELOCATION_MATCH. Other matching texts:
  named `C3DVector offset = camera_position - sphere.GetCenter();` (79i), distance -= radius; distance *= stretch (81i/265b).
- CORRECTION to the coordinator's name hint: 0x01436E78 is NOT GPPrimitiveRenderer. Evidence: landed CEngine::Initialise 0x00B26360 does
  AddEngineComponent(new CEnginePrimitiveRenderer(), <0x01436E80>) and (new CEnginePrimitiveRenderer2D(), <0x01436E7C>); 0x01436E78 is re-stored at
  render time (0x00B25F2D CEngine::Uninitialise, 0x00B33596, 0x00B35DDB) and only CEnginePrimitiveRendererSharedData members are read through it
  (+0x04 BlockOnLoad, +0xB0 MipmapSizeMultiplier); the Anniversary PDB has exactly NEngineInternal::GPPrimitiveRenderer (CEnginePrimitiveRenderer*),
  GPPrimitiveRenderer2D and GPPrimitiveRendererSharedData (CEnginePrimitiveRendererSharedData*). So 0x01436E78 = NEngineInternal::GPPrimitiveRendererSharedData
  (type CEnginePrimitiveRendererSharedData*), 0x01436E80 = NEngineInternal::GPPrimitiveRenderer, 0x01436E7C = NEngineInternal::GPPrimitiveRenderer2D.
  The row TU used the E78 global for GetBlockOnLoad in GetDiffuse/GetBumpTexture (retail reads 0x01436E80): fixed (tools/respell3.sh).
- GetDiffuseTexture 0x00BA9360 / GetBumpTexture 0x00BA95D0 in the TU: RELOCATION_MATCH at their own addresses (were 382v613 / 373v597) with
  the SHARED-HEADER touch helpers instead of the in-row ones: opt-ins FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME + _TOUCHFRAME (FableWin TouchFrame 69i),
  FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET (FW 17i), FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE, FABLE_RETAIL_CGRAPHICFRAME(_GETTEXTURE), and the
  in-row CGraphicInfoFrame::GetTexture rewritten in the control flow of the landed out-of-line copy 0x00B66C10 (if Data { if FrameData { touch; if
  finer { BeginLoading; if !(flags&16)||!FileData return tex } else return tex } BeginLoading } else { if flags&4 fail; new CFrameData; BeginLoading }
  if !(flags&1)||!InternalGetFrameForcedLoading fail; return tex). Relationship: retail has ONE GetDiffuseTexture (0x00BA9360, 25 callers); the
  landed NoMorph 0x00BA48F0 file carries a visibility copy that was never graded at 0x00BA9360; the landed 0x00BA9360 file is a local-model text.
  The TU text (shared headers) is a candidate replacement for both. Also matches with the getters marked `inline` (header-inline emitted at TU end).
  With this TU (w/t3.cpp = differ/00ba5260.cpp): row 1, SetupStaticBlocks 0x00BA50A0, SetupFirstTextures, IndexBuffer, DegenerateTriangles,
  BlankTextures, GetDiffuseTexture, GetBumpTexture all RELOCATION_MATCH; only row 2 0x00BA2350 DIFFER(9516v9632).
- Row 1 REQUIRES the row-2 body in the TU (declaration only: DIFFER(4060v4056)): row 1 lands only once row 2 matches.
- Bonus rows re-handed back (regraded on today's headers + my overlay): src/00b9e5b0.cpp (C17's text, unchanged), cloth src/00bb30a0.cpp /
  src/00bb3e80.cpp = C17's sib3 replacement texts (repo-default operator-, SHADOWGUARD_DIRECT) + my global respellings (GPPrimitiveRendererSharedData,
  GPShadowRenderer, GPPrimitiveTypeInfo, GTopBitInByteTable) + the new GetAppropriateMipmap text (3e80); tests from 2026-11-13_acctC_c8a/test.
  All three RELOCATION_MATCH, vt PASS, addmatch done, read-only vland RELOCATION_MATCH PASS (no RELOC_IDENTITY).

## 0x00BA2350 SetupRenderModeShadersAndConstants (agentC19)
- c2prio (t3/t6): sub_primitive 59 (edi) v mesh_primitive 49 (ebp); this split (-447). --blocks: sub = 4*K1(16) + 6*K2(8) - 63 (live through blocks
  3..20 = stipple alpha + SetTextureFactor(CRGBFloatColour(tint)) + theme channel) + 2*K21(5) = 59; mesh = 2*K1 + 17 (non-combined passes only; mesh
  is not live in the combined path).
- The retail code of blocks 1..21 (0x00BA2350..0x00BA25F9) is structurally identical to ours (register-normalised diff: only register / slot names),
  so K / block structure there are the same as far as the final code shows.
- Forced what-if (tools/frc.sh, C2_FORCEID 19=60 = mesh above sub): retail's prologue exactly (ebp=this + [esp+14h], ebx=sub, edi=mesh after the
  setne, frame 0xB0) BUT the rest is further from retail than the unforced build (9412 bytes; block-2/3 temps take edx where retail takes edi:
  in retail the tint G byte stays in dl from the prologue to the CRGBFloatColour conversion, in the forced build it is reloaded). Forcing sub
  down instead (9=48) gives sub=ebp (worse). => the flip is necessary but NOT sufficient; more than one priority differs from retail.
- Callee body visibility: GetReflectionTexture 0x00BA9830 (never writes its bool& -- no byte store in its body) and Get2DAlphaMaskTexture 0x00BA9A70
  bodies added to the TU (w/t6.cpp; 556v570 / 561v563 at their own addresses): row 2 priorities unchanged (sub 59 / mesh 49). Not kept.
- Code-neutral variants with no effect on the prologue: dp3_bump local removed, block_on_load `& 0x80 != 0`, ERenderMode render_mode,
  no alpha_value local, `(flags & 8) != 0`. lights through primitive->PeekLights(): 9412, prologue changes (sub's last use moves).
- Forced 9=30 (sub far below mesh): see tmp; no better. Next ideas for row 2 (not done): IL-level K differences invisible in final code (dead IL temps
  in blocks 1..21), FableWin-size mismatches inside the SetTextureFactor expansion (SetTextureFactor 29i v FW 35i, CRGBFloatColour(const CRGBColour&)
  38i v 46i, C4DVector(const CRGBFloatColour&) 24i v 12i = a single one-argument call in FW, GetRGBColour 30i v 54i, CRGBFloatColour(MMMM) 20i v 22i):
  their texts change the candidate sets in exactly the blocks that decide sub (3..20) and mesh (Z_WRITE 350..359). Flipping the repo opt-ins
  INITIALISE_CTOR / GETRGBCOLOUR_NAMED_RESULT changes code (9504 / 9472) without fixing the prologue.
- Grades used (agentC19): row 1 ~45 (+GAM batches), support rows ~25, row 2 ~20 grades + 4 c2prio / forced runs, cloth/support regrades ~15.
