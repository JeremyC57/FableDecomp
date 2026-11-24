# ESCALATE 0x00BD3070 CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive (AUTH21, 2026-11-22)

Leaf `?RenderMeshPrimitive@CEnginePrimitiveManagerAnimatedMeshesBase@@QAEXAAVCEngineSubPrimitiveAnimatedMesh@@W4ERenderPassType@@@Z`. The
row is 11288 B, graded at stock flags with overlay `hdr/` (12 headers; patch `hdr_00bd3070.patch`).

## Sources
- **Rule-clean best: `00bd3070.cpp` (= c6), DIFFER(11288v11288).** rc.py 151, built hash 506c5c674d66. No row-local member body is left that a
  shared header models. Every FABLE_FORCEINLINE / dependency body comes from a header opt-in (NOTES.md lists them).
- Bracket: `p2.cpp` (= AUTH20's b6 + C3DVECTOR_CTOR_FORCEINLINE), DIFFER(11288v11288), rc 129.
  - It keeps b6's row-local `PeekGraphic` spelled `&GraphicList.begin()[index]`.
  - The shared header spells it `&GraphicList[index]`: landed opt-in 0x00BB8EF0 and 13 landed in-row copies.
  - FableWin.pdb has the CArray<CGraphicInfo>::operator[] const COMDAT (CArray.h cites it, 0x01D2F450), so the debug PeekGraphic most
    likely indexed through operator[]. The begin() text is a fit; the evidence leans against it. Not adopted.

## What AUTH21 did (one move per grade, hash-compared with b6 ee83b86f3634)
- **Byte-neutral moves**, each to an existing opt-in or a new add-only one:
  - PeekGraphicInfoFrame
  - CRenderStateManager::SetTextureFactor
  - SelectVertexBuffer (plain in-class inline is enough)
  - FlushRender (in-class)
  - GetAlphaIncludingDistanceFade (the existing in-class _GETALPHAINCLUDINGDISTANCEFADE_FORCEINLINE)
  - GetTexture (the existing wfA5 _GETTEXTURE_INLINE text)
  - GetCurrentAlphaMask
  - ApplyPixelShader / SetTextureFactor / Set2DAlphaMaskTextureMode / DisableTextureFiltering / SetupAlphaBlendingStatesFromRenderFlags
    (in-class)
  - GetCurrent2DAlphaMaskInfo (after the class, because of an include cycle)
  - GFGetTopBitInByte
  - GetMipmapSizeMultiplier
  - GetAppropriateMipmap (after the class)
- **Order sensitivity measured.**
  - FlushRender as an out-of-class body after CRenderManagerCore gave DIFFER(11016v11288), rc 1317: the blocks were reshuffled from 0x00BD3075
    on, and 4 calls disappeared. The in-class body gives b6's bytes exactly.
  - GetAppropriateMipmap with the 5-row landed text gave 11272, rc 397. b6's text is kept (FableWin's locals match neither text).
- **Text that costs bytes.**
  - PeekGraphic through the header text (`&GraphicList[index]`) costs +30 rc. Plain `inline` there leaves 5 calls (11128).
  - The cost comes from FABLE_RETAIL_CARRAY_INDEX_ASSERT's dead-arm assert in operator[]. Dropping INDEX_ASSERT is worse: 11256, rc 201.
- **One pre-registered fix, kept.**
  - The only real unequal-count hunk at b6 was 0x00BD4D13: ours called C3DVector(float,float,float) out of line at one of the three
    SetUserVertexShaderConstant(0, C3DVector(...), w) sites, and retail expands it.
  - The existing C3DVector.h opt-in FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE (the 0x00CBFB7D budget case) closes it: c6, rc 158 -> 151.

## What is left in c6 (hcls.py: hunks merged across gaps of 2 or fewer; `c6_hunks.txt`)
- 2 UNEQUAL: 0x00BD5C40 / 0x00BD5C7B are the switch jump table, data decoded as code with our relocations zeroed. Not code residue.
- About 10 one-instruction SAMECOUNT hunks are relocation display only: retail absolute `[A]` against our unrelocated `[8]` / `[edx]` /
  `[eax*4]` (symbol + addend, e.g. retail 0x00BD43FE `[0x01396E6C]` v ours `[8]`). Inferred from the operand form: the relocation targets
  were not checked one by one.
- **13 real hunks, every one a commutative operand-order pick with identical instructions:**
  - 11 `test r1, r2 ; or r2, r1` pairs swapped (0x00BD3433, 0x00BD3734 .. 0x00BD38F5 (5), 0x00BD43B2, 0x00BD441F, 0x00BD448C, 0x00BD4C30): the
    or/test operand form the wfE6 id rule was measured on.
  - 1 x87 `fld [ebp+0x30] ; fmul [ebp+0x28]` swapped (0x00BD32F4).
  - 1 SIB base/index swap `[ecx+esi]` v `[esi+ecx]` (0x00BD5712, two instructions).
- No extra instruction, pad, frame or unexpanded call remains.

## Question for the advisor
The residue is now only commutative operand picks of the kind the wfE6 comparator readout (C2T1's tools task) is for. May the row wait for that
readout (a measured per-site id tie, then the 0x00CF4EC0 TERMINAL model)? Or should a TERMINAL proof be drafted now from this classification?
Separately: should the PeekGraphic `begin()[index]` bracket (rc 129 v 151) be recorded as an oracle (evidence leans against it) or dropped?
