## 0x00B8B780 CEngineBillboardGenerator::RenderBillboard -- author session 1 (bwf1)
- grades 1..30: v1 3795v3921 -> /GS flag (missing cookie) -> 3939 -> zbuffer as if/else AttachZBuffer + GetWidth(0) -> 3921v3921 (21 hunks).
- moved: /GS; FORCEINLINE PeekGraphic/PeekGraphicInfoFrame; sphere copy first (const C3DBoundingSphere& binding); stages/topBit order; unsigned char top-bit table; verts column order; E43 as `*= .5` then `+= .5` (one expression gets factored to (x+1)*.5 by VC).
- grades 31..~150 (most are probes): residue at 3921v3921 = (a) bookmark `test/or` operand order in setter blocks 3..5 (AlphaRef, AlphaTestFunc,
  FogEnable; retail `test esi,eax ; or eax,esi`), (b) Offset.Z product `fld E43 ; fmul R` (ours `fld R ; fmul E43`). Spelling-proof (header
  swaps, EARLY_RETURN, GetRadius, Offset forms, Peek/GetCurrentCamera accessors: no move). PROBE: k dead outer names `long dN = alphaRef;`
  after the sphere: k=21 fixes block 5, k=53..73 blocks 4..5, k=85..103 all blocks (period ~?); the Z order never moves with names.
  => c2 record-id tie (r3005): retail allocates far more records before the setters than ours (FableWin helper texts differ: SetIdentity,
  SetMultiplyNoTranslation (FW 444b v ours 557b), GetTranslation (54 v 45), C3DVector::operator*(float) (82 v 62), SetTranslate (56 v 48), CRGBColour ctor).
- KEY FINDING (probe): value sphere `C3DBoundingSphere sphere = mesh->GetBoundingSphere();` fixes the Offset.Z product order
  (const-ref binding gives `fld R ; fmul E43`). On that base (differ/ = src/v62.cpp) the only residue is the bookmark test/or tie in
  setter blocks; k dead outer names `long dN = alphaRef;` ANYWHERE in the body (position-independent, extra TU functions neutral) move a
  boundary across the setter blocks one block per ~32 names; k=108..112 gives RELOCATION_MATCH (sweep/s108.cpp; NOT deliverable).
  => the original function allocates ~110 more c1 records (named locals / expression temps) than ours. Natural accessors measured:
  GetCurrentCamera + static Apply*Shader + PeekNonHomogenousViewTransform shift it by ~+4 only.
- END OF SESSION 1 (~260 grades incl. probes): best = differ/00b8b780.cpp (= src/v62.cpp), DIFFER(3921v3921) 23 hunks, flags `/GS`,
  no pragma (the asm block + /O2 give the biased `lea ebp,[esp-64h]` frame by themselves). Residue = ONLY the c2 record-id tie of the
  CRenderStateManager bookmark `test/or` operand order: loop-1 setters AlphaRef 0x00B8BC75, AlphaTestFunc 0x00B8BCE2, FogEnable 0x00B8BD49
  (retail `test esi,eax ; or eax,esi`) and loop-2 TextureArgument1 0x00B8C325 / TextureOperation 0x00B8C393. Proven by probe: +108..112
  outer dead names (anywhere in the body) give RELOCATION_MATCH (sweep/s108.cpp, sweep/m*.cpp tables; ksweep.sh is the ruler).
  Tried, no effect on the window: header operand swaps, EARLY_RETURN, extra TU functions (neutral), GetCurrentCamera/static
  Apply*Shader/PeekNonHomogenous (+~4), C3DTextureStages FW ctor, CRGBColour Initialise ctor, SetIdentity separate, sphere accessors.
  Best next idea: find ~110 natural c1 records the original had (named locals / expression temps / inline layers) -- e.g. a dead debug
  path compiled out by a const flag, accessor layers for the managers, named intermediates in loop 1/2 (bank, frame, texture, width,
  size, half, positions) -- measuring each candidate with ksweep.sh (window should move from k=108 toward k=0).
- Headers: hdr/README.md (15 opt-ins). Local models (no repo header): CRenderTarget (Ego_r 0x24 : CBaseClass), CEngineBillboard (Ego_r 0x18).
- Name evidence: 0x00A10E90 is CRenderTarget::AttachZBuffer (FableWin ?AttachZBuffer@CRenderTarget@@QAEXABVCSurface@@@Z + body writes
  ZBufferSurface +0x14); landed spelling AttachTarget is wrong (NAME_FIXES.tsv). Test: test/00b8b780_test.cpp (vt.py PASS).
